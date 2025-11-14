// Copyright 2025 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <xs1.h>
#include <platform.h>
#include <stdint.h>

void select_example_function(chanend c_dsp, chanend c_gpi)
{
    // Initialize the sample buffer
    int idx = 0;
    int32_t sample[512];

    // Create a place to sink the button-pressed data
    int button_pressed;

    // Main processing loop with event selection.
    // Each case statement starts an event hander due to an event on a channel end.
    // In this example, data arriving on the c_dsp channel end goes into the sample buffer,
    //   and data arriving on the c_gpi channel end goes into the button_pressed variable.
    // Each corresponding (elided) event handler runs as a consequence of receiving the data.
    while(1) {
        select {
            // Receive the sample from the DSP channel end and run the corresponding event handler
            case c_dsp :> sample[idx]:
                // .
                // .
                // .
                break; // end of event_dsp_chanend

            // Receive the button press state from the GPI channel end and
            //   run the corresponding event handler
            case c_gpi :> button_pressed:
                // .
                // .
                // .
                break; // end of event_button_pressed
        } // end of select
    } // end of while(1)
}
