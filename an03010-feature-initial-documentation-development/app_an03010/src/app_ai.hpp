// Copyright 2025 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#pragma once

#include <xccompat.h>

#include "app_gpio.h"

#if defined(__cplusplus) || defined(__XC__)
extern "C" {
#endif

/**
 * @brief AI Application main function. 
 * This function sets up parallel jobs for AI data collection, keyword spotting and VNR estimation.
 * 
 * @param c_ai  Channel end for AI data collection
 * @param c_gpo[GPO_NUM_CH] Channel ends array for GPO output (keyword spotting and VNR)
 * @param c_flash Channel end for flash server communication
 * @param c_cdc Channel end for CDC communication (VNR level reporting)
 */
void app_ai(chanend c_ai, chanend c_gpo[GPO_NUM_CH], chanend c_flash, chanend c_cdc);

#if defined(__cplusplus) || defined(__XC__)
}
#endif // End of extern "C" scope
