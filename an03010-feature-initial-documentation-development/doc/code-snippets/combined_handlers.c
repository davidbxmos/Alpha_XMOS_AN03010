// Copyright 2025 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.


#include <stdint.h>
#include <stdbool.h>

#include <xs1.h>
#include <platform.h>
#include <xccompat.h>
#include <xcore/channel.h>
#include <xcore/hwtimer.h>
#include <xcore/port.h>
#include <xcore/select.h>

// Button definitions
#define BUTTON_0_MASK       (0x1)
#define PORT_BUTTON_BITS    (0x3)
#define BUTTON_0_PRESSED    (PORT_BUTTON_BITS & ~BUTTON_0_MASK)

// LED Definitions
#define LEDS_OFF (0x0)

static uint32_t set_led_state(uint32_t current_state, int signal)
{
    uint32_t next_led_state = LEDS_OFF;
    // Determine next LED state based on current_state and signal
    // .
    // .
    // .
    return next_led_state;
}

void combined_handlers_example(
    // GPI resources
    chanend c_button, 
    port p_buttons, 
    // GPO resources
    chanend c_led, 
    port p_leds)
{
    // Prepare the button port for input
    port_enable(p_buttons);
    int32_t button_state = port_in(p_buttons);
    port_set_trigger_in_equal(p_buttons, BUTTON_0_PRESSED);

    // Prepare the LED port for output
    // For simplicity, this example turns off all four LEDs initially.
    port_enable(p_leds);
    uint32_t current_led_state = LEDS_OFF;
    uint32_t next_led_state    = LEDS_OFF;
    port_out(p_leds, current_led_state);

    // Prepare a timer for handling bouncing by the button input
    hwtimer_t t_settle = hwtimer_alloc();
    const uint32_t SETTLE_TIME = 5000000; // 50ms in 10ns ticks
    const bool BUTTON_PRESSED = true;
    bool stable = true;

    // Index from GPO source
    int led_signal;

    SELECT_RES(
        // GPI events
        CASE_GUARD_THEN(p_buttons, stable, event_button_port),
        CASE_NGUARD_THEN(t_settle, stable, event_button_settle_timer),
        // GPO events
        CASE_THEN(c_led, c_led_handler))
    { // SELECT_RES
        event_button_port:{
            button_state = port_in(p_buttons);
            port_clear_trigger_in(p_buttons);
            chan_out_word(c_button, BUTTON_PRESSED);
            hwtimer_set_trigger_time(t_settle, hwtimer_get_time(t_settle) + SETTLE_TIME);
            stable = false;
            continue; // end of event_button_port
        }
        event_button_settle_timer:{
            button_state = port_in(p_buttons);
            hwtimer_clear_trigger_time(t_settle);
            stable = true;
            port_set_trigger_in_equal(p_buttons, BUTTON_0_PRESSED);
            continue; // end of event_button_settle_timer
        }
        c_led_handler:{
            led_signal = chan_in_word(c_led);
            next_led_state = set_led_state(current_led_state, led_signal);
            if(next_led_state != current_led_state) {
                port_out(p_leds, next_led_state);
                current_led_state = next_led_state;
            }
            continue; // end of c_led_handler
        }
    } // end of select
} // end of combined_handlers_example
