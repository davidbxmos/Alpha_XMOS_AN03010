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

void button_handler_example(
    // GPI resources
    chanend c_button, 
    port p_buttons)
{
    // Prepare the button port for input
    port_enable(p_buttons);
    int32_t button_state = port_in(p_buttons);
    port_set_trigger_in_equal(p_buttons, BUTTON_0_PRESSED);

    // Prepare a timer for handling bouncing by the button input
    hwtimer_t t_settle = hwtimer_alloc();
    const uint32_t SETTLE_TIME = 5000000; // 50ms in 10ns ticks
    const bool BUTTON_PRESSED = true;
    bool stable = true;

    SELECT_RES(
        // GPI events
        CASE_GUARD_THEN(p_buttons, stable, event_button_port),
        CASE_NGUARD_THEN(t_settle, stable, event_button_settle_timer))
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
    } // end of select
} // end of button_handler_example
