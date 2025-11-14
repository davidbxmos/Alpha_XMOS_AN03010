// Copyright 2025 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <xs1.h>
#include <platform.h>
#include <stdbool.h>
#include <stdint.h>
#include <xccompat.h>
#include <xcore/channel.h>
#include <xcore/select.h>

void select_res_example_function(chanend c_dsp, chanend c_gpi)
{
    // Initialize the sample buffer
    int idx = 0;
    int32_t sample[512];

    // Create a place to sink the button-pressed data
    bool button_pressed;

    // Main processing loop with event selection.
    // The SELECT_RES macro sets up an infinite loop plus the logic to wait for events
    //   on multiple resources.
    // Each CASE_THEN macro associates an event on a resource, in this case a channel end,
    //  with an event handler.
    // In this example, data arriving on the c_dsp channel end triggers the (elided)
    //  event_dsp_chanend handler, and data arriving on the c_gpi channel end triggers
    //  the (elided) event_button_pressed handler.
    SELECT_RES(
        CASE_THEN(c_dsp, event_dsp_chanend),
        CASE_THEN(c_gpi, event_button_pressed)
    ) // SELECT_RES
    {
        // Handler for events on the DSP channel end
        event_dsp_chanend:
        // Receive the sample from the DSP channel end
        sample[idx] = chan_in_word(c_dsp);
            // .
            // .
            // .
            continue; // end of event_dsp_chanend

        // Handler for events on the button press channel end
        event_button_pressed:
        // Receive the button press state from the GPI channel end
        button_pressed = chan_in_word(c_gpi);
            // .
            // .
            // .
            continue; // end of event_button_pressed
    } // end of select
}
