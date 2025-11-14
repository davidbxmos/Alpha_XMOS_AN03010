// Copyright 2025 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#pragma once

#include <xccompat.h>

#if defined(__cplusplus) || defined(__XC__)
extern "C" {
#endif

/**
 * @brief DSP Application main function. 
 * This function sets up the DSP processing task.
 * 
 * @param c_dsp  Channel end for DSP data communication
 * @param c_gpi  Channel end for GPI (button press) communication
 */
void app_dsp(chanend c_dsp, chanend c_gpi);

#if defined(__cplusplus) || defined(__XC__)
}
#endif // End of extern "C" scope
