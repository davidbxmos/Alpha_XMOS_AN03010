// Copyright 2025 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "xud_cdc.h"

#if defined(__XC__) // only xc ccompatible
void app_cdc(
    client interface usb_cdc_interface cdc, 
    chanend cdc_vnr,
    chanend cdc_gpi);
#endif
