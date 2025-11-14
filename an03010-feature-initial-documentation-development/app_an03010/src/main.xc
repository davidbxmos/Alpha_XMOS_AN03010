// Copyright 2025 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <xs1.h>
#include <platform.h>

#include <string.h>

#include "config.h"
#include "i2c.h"
#include "i2s.h"
#include "xk_evk_xu316/board.h"

#include "app_gpio.h"
#include "app_dsp.h"
#include "app_ai.hpp"

// Board configuration from lib_board_support
static const xk_evk_xu316_config_t hw_config = {
    MASTER_CLOCK_FREQUENCY
};

// GPIO resources
on tile[0]: out port p_leds = PORT_LEDS;
on tile[0]: in port p_buttons = PORT_BUTTONS;

// I2S resources
on tile[1]: in port p_mclk =                                PORT_MCLK_IN;
on tile[1]: buffered out port:32 p_lrclk =                  PORT_I2S_LRCLK;
on tile[1]: out port p_bclk =                               PORT_I2S_BCLK;
on tile[1]: buffered out port:32 p_dac[NUM_I2S_LINES] =     {PORT_I2S_DAC_DATA};
on tile[1]: buffered in port:32 p_adc[NUM_I2S_LINES] =      {PORT_I2S_ADC_DATA};
on tile[1]: clock bclk =                                    XS1_CLKBLK_1;

// Flash resources
#include <flash_server.h>
#define NFLASH_SPECS 1
fl_QuadDeviceSpec flash_spec[NFLASH_SPECS] = {FL_QUADDEVICE_DEFAULT};
on tile[0]: fl_QSPIPorts qspi = {
    PORT_SQI_CS,
    PORT_SQI_SCLK,
    PORT_SQI_SIO,
    XS1_CLKBLK_2
};

// USB CDC resources
#include "xud_device.h"
#include "xud_cdc.h"
#include "app_cdc.h"

#define XUD_EP_COUNT_OUT   2    //Includes EP0 (1 OUT EP0 + 1 BULK OUT EP)
#define XUD_EP_COUNT_IN    3    //Includes EP0 (1 IN EP0 + 1 INTERRUPT IN EP + 1 BULK IN EP)
XUD_EpType epTypeTableOut[XUD_EP_COUNT_OUT] = {XUD_EPTYPE_CTL | XUD_STATUS_ENABLE, XUD_EPTYPE_BUL};
XUD_EpType epTypeTableIn[XUD_EP_COUNT_IN] =   {XUD_EPTYPE_CTL | XUD_STATUS_ENABLE, XUD_EPTYPE_INT, XUD_EPTYPE_BUL};

void application(
    chanend c_ai,
    chanend c_dsp,
    server i2s_frame_callback_if i_i2s
)
{
    xk_evk_xu316_AudioHwInit(hw_config);

    // Array used for looping back samples
    int32_t samples[BUFFER_SIZE] = {0};
    size_t idx = 0;
    int32_t sample;

    while (1) {
    select {
        case i_i2s.init(i2s_config_t &?i2s_config, tdm_config_t &?tdm_config):
            i2s_config.mode = I2S_MODE_I2S;
            i2s_config.mclk_bclk_ratio = (MASTER_CLOCK_FREQUENCY / (SAMPLE_FREQUENCY * CHANS_PER_FRAME * DATA_BITS));
            xk_evk_xu316_AudioHwConfig(SAMPLE_FREQUENCY, MASTER_CLOCK_FREQUENCY, 0, DATA_BITS, DATA_BITS);
            break;

        case i_i2s.receive(size_t n_chans, int32_t in_samps[n_chans]):
            for (size_t i = 0; i < n_chans; ++i){
                // Send samples for AI processing
                c_ai <: in_samps[i];
                
                // Send samples to DSP processing
                c_dsp <: in_samps[i];
            }
            break;

        // Receive processed samples from DSP
        case c_dsp :> sample:
            samples[idx] = sample;
            if (BUFFER_SIZE <= ++idx){
                idx = 0;
            }
            break;

        case i_i2s.send(size_t n_chans, int32_t out_samps[n_chans]):
            for (size_t i = 0; i < n_chans; ++i){
                out_samps[i] = samples[i]; // loopback samples
            }
            break;

        case i_i2s.restart_check() -> i2s_restart_t restart:
            restart = I2S_NO_RESTART; // Keep on looping
            break;
        } // End select
    } // End while (1)
} // End application


int main(void)
{
    chan c_audio_hw_init;   // Channel for cross-tile communication
    chan c_ai;              // Channel for application <-> AI communication
    chan c_gpo[GPO_NUM_CH]; // Channel for LED events
    chan c_dsp;             // Channel for application <-> DSP communication
    chan c_gpi;             // Channel for button press events
    chan c_flash[1];        // Channel for flash server
    chan c_cdc_vnr;         // Channel for CDC <-> VNR communication
    chan c_cdc_gpi;        // Channel for CDC <-> GPI communication

    // usb cdc
    chan c_ep_out[XUD_EP_COUNT_OUT], c_ep_in[XUD_EP_COUNT_IN];
    interface usb_cdc_interface cdc_data;
    interface i2s_frame_callback_if i_i2s;

    par {
        // tile 0-1: USB and CDC
        on tile[0]: XUD_Main(
            c_ep_out, XUD_EP_COUNT_OUT, c_ep_in, XUD_EP_COUNT_IN,
            null, epTypeTableOut, epTypeTableIn,
            XUD_SPEED_HS, XUD_PWR_BUS
        );
        on tile[0]: Endpoint0(c_ep_out[0], c_ep_in[0]);
        on tile[0]: CdcEndpointsHandler(c_ep_in[1], c_ep_out[1], c_ep_in[2], cdc_data);
        on tile[1]: app_cdc(cdc_data, c_cdc_vnr, c_cdc_gpi);

        // tile 0: flash server
        on tile[0]: {
            flash_t headers[1];
            flash_server(c_flash, headers, 1, qspi, flash_spec, 1);
        }
        on tile[0]: xk_evk_xu316_AudioHwRemote(c_audio_hw_init);
        on tile[0]: app_gpio(c_gpi, p_buttons, c_cdc_gpi, c_gpo, p_leds);
        
        // tile 1: I2S, AI, DSP
        on tile[1]: xk_evk_xu316_AudioHwChanInit(c_audio_hw_init);
        on tile[1]: application(c_ai, c_dsp, i_i2s);
        on tile[1]: i2s_frame_master(i_i2s, p_dac,NUM_I2S_LINES,p_adc,NUM_I2S_LINES,DATA_BITS,p_bclk,p_lrclk,p_mclk,bclk);
        on tile[1]: app_ai(c_ai, c_gpo, c_flash[0], c_cdc_vnr);
        on tile[1]: app_dsp(c_dsp, c_gpi);
    } // Outer par
    return 0;
}
