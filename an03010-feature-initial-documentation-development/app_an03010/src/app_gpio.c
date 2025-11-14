// Copyright 2025 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.


#include <stdint.h>
#include <stdbool.h>
#include <assert.h>

#include <xs1.h>
#include <platform.h>
#include <xcore/channel.h>
#include <xcore/hwtimer.h>
#include <xcore/port.h>
#include <xcore/select.h>

#include "app_gpio.h"
#include "kws_labels.h"
#include "vnr_labels.h"

// Button definitions
#define BUTTON_0_MASK       (0x1)
#define PORT_BUTTON_BITS    (0x3)
#define BUTTON_0_PRESSED    (PORT_BUTTON_BITS & ~BUTTON_0_MASK)

// Led Definitions
#define LED_0_ON    (0x1)
#define LED_1_ON    (0x2)
#define LED_2_ON    (0x4)
#define LED_3_ON    (0x8)
#define LEDS_OFF    (0x0)

void app_gpio(
    // gpi resources
    chanend c_gpi, 
    port p_buttons, 
    chanend c_cdc_gpi,
    // gpo resources
    chanend c_gpo[GPO_NUM_CH], 
    port p_leds)
{
    assert(GPO_NUM_CH >= 2 && "VNR and KWD channels required");

    // Prepare the button port for input
    port_enable(p_buttons);
    int32_t button_state = port_in(p_buttons);
    port_set_trigger_in_equal(p_buttons, BUTTON_0_PRESSED);

    // Prepare the LED port for output
    // For simplicity, this example turns off all four LEDs initially,
    //  and only changes the state of LED 0
    port_enable(p_leds);
    uint32_t current_led_state = LEDS_OFF;
    uint32_t next_led_state    = LEDS_OFF;
    port_out(p_leds, current_led_state);
    chanend_t c_gpo_kwd = c_gpo[0];
    chanend_t c_gpo_vnr = c_gpo[1];

    // Prepare a timer for handling bouncing by the button input
    hwtimer_t t_settle = hwtimer_alloc();
    const uint32_t SETTLE_TIME = 5000000; // 50ms in 10ns ticks
    const bool BUTTON_PRESSED = true;
    bool stable = true;

    // Prepare a timer to sustain the LED output
    hwtimer_t t_sustain = hwtimer_alloc();
    const uint32_t SUSTAIN_DURATION = 85000000; // 850ms in 10ns ticks
    bool sustaining = false;

    // indices from both ai models
    kwd_label_indices_t led_signal = UNKNOWN;
    vnr_label_indices_t vnr_score = VOICE_00;

    SELECT_RES(
        // gpi events
        CASE_THEN(c_cdc_gpi, cdc_gpi_handler),
        CASE_GUARD_THEN(p_buttons, stable, event_button_port),
        CASE_NGUARD_THEN(t_settle, stable, event_button_settle_timer),
        // gpo events
        CASE_THEN(c_gpo_kwd, c_gpo_kwd_handler),
        CASE_THEN(c_gpo_vnr, c_gpo_vnr_handler),
        CASE_GUARD_THEN(t_sustain, sustaining, event_sustain_timer_trigger)) 
    { // SELECT_RES
        cdc_gpi_handler:{
            uint32_t cdc_rx = chan_in_word(c_cdc_gpi);
            if (cdc_rx){ // emulate button press
                chan_out_word(c_gpi, BUTTON_PRESSED);
            }
            continue; // end of cdc_gpi_handler
        } 
        event_button_port:{
            button_state = port_in(p_buttons);
            port_clear_trigger_in(p_buttons);
            chan_out_word(c_gpi, BUTTON_PRESSED);
            hwtimer_set_trigger_time(t_settle, hwtimer_get_time(t_settle) + SETTLE_TIME);
            stable = false;
            continue; // end of event_button_port
        }
        event_button_settle_timer:{
            button_state = port_in(p_buttons);

            // Active low; ignore values for buttons other than button 0
            if(0 == (button_state & BUTTON_0_MASK)) {
                hwtimer_change_trigger_time(t_settle, hwtimer_get_time(t_settle) + SETTLE_TIME);
            } else {
                hwtimer_clear_trigger_time(t_settle);
                stable = true;
                port_set_trigger_in_equal(p_buttons, BUTTON_0_PRESSED);
            }
            continue; // end of event_button_settle_timer
        }
        c_gpo_kwd_handler:{
            led_signal = chan_in_word(c_gpo_kwd);
            if(!sustaining) {
                switch (led_signal) {
                    case LEFT:
                        next_led_state = LED_3_ON;
                        break;
                    case RIGHT:
                        next_led_state = LED_0_ON;
                        break;
                    case UNKNOWN:
                        next_led_state = LEDS_OFF;
                        break;
                    default:
                        next_led_state = LEDS_OFF;
                        break;
                }

                if(next_led_state != current_led_state) {
                    port_out(p_leds, next_led_state);
                    current_led_state = next_led_state;

                    if(next_led_state != LEDS_OFF) {
                        sustaining = true;
                        hwtimer_set_trigger_time(t_sustain, hwtimer_get_time(t_sustain) + SUSTAIN_DURATION);
                    }
                }
            } // if (!sustaining)
            continue; // end of c_gpo_kwd_handler
        }
        c_gpo_vnr_handler:{
            vnr_score = chan_in_word(c_gpo_vnr);
            uint32_t port_value = port_peek(p_leds);

            if(!sustaining) {
                switch (vnr_score)
                {
                case VOICE_50:
                    port_value |= LED_1_ON;
                    port_out(p_leds, port_value);
                    break;
                case VOICE_90:
                    port_value |= LED_2_ON;
                    port_out(p_leds, port_value);
                    break;
                default:
                    port_value &= ~(LED_1_ON | LED_2_ON);
                    port_out(p_leds, port_value);
                    break;
                }
            }
            continue; // end of c_gpo_vnr_handler
        } // c_gpo_vnr_handler
        event_sustain_timer_trigger:{
            hwtimer_clear_trigger_time(t_sustain);
            sustaining = false;
            continue; // end of event_sustain_timer_trigger
        } // event_sustain_timer_trigger
    } // end of select
} // end of app_gpio
