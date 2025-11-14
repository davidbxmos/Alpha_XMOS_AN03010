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

void led_handler_example(
    // GPO resources
    chanend c_led, 
    port p_leds)
{
    // Prepare the LED port for output
    // For simplicity, this example turns off all four LEDs initially.
    port_enable(p_leds);
    uint32_t current_led_state = LEDS_OFF;
    uint32_t next_led_state    = LEDS_OFF;
    port_out(p_leds, current_led_state);

    // Index from GPO source
    int led_signal;

    SELECT_RES(
        // GPO events
        CASE_THEN(c_led, c_led_handler))
    { // SELECT_RES
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
} // end of led_handler_example
