// Copyright 2025 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "app_cdc.h"

#define cdc_buff_len       64   // Length of CDC buffers
#define cdc_dict_len       4    // Number of commands in CDC dictionary

void app_cdc(
    client interface usb_cdc_interface cdc, 
    chanend cdc_vnr,
    chanend cdc_gpi)
{
    unsigned char buffer_in[cdc_buff_len] = {0};
    
    const uint32_t tx_vnr = 1;
    const uint32_t tx_gpi = 1;
    uint32_t rx_vnr = 0; 

    const unsigned char cdc_dict[cdc_dict_len][cdc_buff_len] = {
        "vnr\n",
        "vol\n",
        "empty1\n",
        "empty2\n"
    };

    while (1)
    {
        unsigned bytes_av = (unsigned)(cdc.available_bytes());
        int bytes_count = cdc.read(buffer_in, bytes_av);

        // Check received command against dictionary
        if (bytes_count > 0) {
            // Echo back received data
            unsigned char buffer_out[cdc_buff_len] = {0};
            unsigned buffer_out_len = 0;
            buffer_out_len = snprintf(buffer_out, sizeof(buffer_out), "%.*s\n", bytes_count - 1, buffer_in);
            cdc.write(buffer_out, buffer_out_len);
            for (unsigned i = 0; i < cdc_dict_len; i++){
                int cmp_len = strlen((const char *)&cdc_dict[i][0]);
                int cmp = strncmp((const char *)buffer_in, (const char *)&cdc_dict[i][0], cmp_len);
                if (cmp == 0){ // Command found
                    if (i==0){
                        cdc_vnr <: tx_vnr; // Trigger VNR level sending
                        cdc_vnr :> rx_vnr;
                        buffer_out_len = snprintf(buffer_out, sizeof(buffer_out), "VNR Level: %u\n", rx_vnr);
                        cdc.write(buffer_out, buffer_out_len);
                    }
                    else if (i==1){
                        cdc_gpi <: tx_gpi; // Emulate button press
                        buffer_out_len = snprintf(buffer_out, sizeof(buffer_out), "GPI Event Sent\n");
                        cdc.write(buffer_out, buffer_out_len);
                    }
                }
            }
        }
        delay_milliseconds(22); // Avoid busy waiting
    }
}
