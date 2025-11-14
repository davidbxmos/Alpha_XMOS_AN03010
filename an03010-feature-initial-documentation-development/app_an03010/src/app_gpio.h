// Copyright 2025 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#pragma once

#include <xccompat.h>

#define GPO_NUM_CH  (2)

#if defined(__cplusplus) || defined(__XC__)
extern "C" {
#endif

#if defined(__XC__)

/**
 * @brief GPIO Application main function.
 * 
 * @param c_gpi chanend for GPI (button press) communication
 * @param p_buttons port for button inputs
 * @param c_cdc_gpi chanend for CDC <-> GPI communication
 * @param c_gpo  chanend array for GPO (LED control) communication
 * @param p_leds port for LED outputs
 */
void app_gpio(
    chanend c_gpi, 
    in port p_buttons, 
    chanend c_cdc_gpi, 
    chanend c_gpo[GPO_NUM_CH], 
    out port p_leds
);

#else
void app_gpio(
    chanend c_gpi, 
    port p_buttons, 
    chanend c_cdc_gpi, 
    chanend c_gpo[GPO_NUM_CH], 
    port p_leds
);

#endif // defined(__XC__)
#if defined(__cplusplus) || defined(__XC__)
}
#endif // End of extern "C" scope
