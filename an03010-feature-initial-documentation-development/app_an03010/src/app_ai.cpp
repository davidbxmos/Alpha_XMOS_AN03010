// Copyright 2025 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>

#include <xs1.h>
#include <platform.h>

#include <xcore/chanend.h>
#include <xcore/channel.h>
#include <xcore/select.h>
#include <xcore/parallel.h>

#include "app_ai.hpp"
#include "config.h"

// kwd spotter includes
#include "kws_labels.h"
#include "kwd_spotter.hpp"

// vnr includes
#include "xmath/xmath.h"
#include "vnr_labels.h"
extern "C"{
    #include "vnr_features_api.h"
    #include "vnr_inference_api.h"
}

#define VNR_SAMPLES (240)

// AI defines
#define AI_SAMPLE_FREQUENCY (16000)
#define SAMPLE_COUNT        (2)
#define THRESHOLD           (80)

DECLARE_JOB(app_ai_collect, (chanend_t, chanend_t, chanend_t));
DECLARE_JOB(app_ai_kwd, (chanend_t, chanend_t));
DECLARE_JOB(app_ai_vnr, (chanend_t, chanend_t, chanend_t, chanend_t));

static float ma_filter_update(float new_sample) {
    const unsigned ma_window = 20;
    static float ma_buffer[ma_window] = {0};
    static unsigned ma_idx = 0;
    static float running_sum = 0.0f;

    // Update running sum and buffer
    float old_sample = ma_buffer[ma_idx];
    ma_buffer[ma_idx] = new_sample;
    running_sum += new_sample - old_sample;
    ma_idx = (ma_idx + 1) % ma_window;
    return running_sum / ma_window;
}

void app_ai_collect(chanend_t c_ai, chanend_t c_collect, chanend_t c_vnr) {
    // Define buffers and index variables
    int32_t samples[SAMPLE_COUNT][CHANS_PER_FRAME] = {{0}};
    unsigned samples_idx = 0;
    unsigned channel_idx = 0;

    int32_t frame[KWD_FRAME_SIZE] = {0};
    unsigned frame_idx = 0;

    int32_t frame_vnr[VNR_SAMPLES] = {0};
    unsigned frame_vnr_idx = 0;
    
    SELECT_RES(CASE_THEN(c_ai, event_ai_chanend))
    {
        event_ai_chanend:{
            samples[samples_idx][channel_idx] = chan_in_word(c_ai);
            channel_idx = (channel_idx + 1) % CHANS_PER_FRAME;
            if (channel_idx == 0) {
                samples_idx = (samples_idx + 1) % SAMPLE_COUNT;
                if (samples_idx == 0) {
                    // get decimated mono sample
                    int32_t sample = (samples[0][0] >> 1) + (samples[1][0] >> 1);

                    // sink keyword spotter frame
                    frame[frame_idx++] = sample;
                    if (N_SAMPLES <= frame_idx) { // frame collected
                        frame_idx = 0;
                        chan_out_buf_word(c_collect, (uint32_t *)frame, N_SAMPLES);
                    } // frame collected

                    // sink vnr frame                    
                    frame_vnr[frame_vnr_idx++] = sample;
                    if(VNR_SAMPLES <= frame_vnr_idx) { // vnr frame collected
                        frame_vnr_idx = 0;
                        chan_out_buf_word(c_vnr, (uint32_t *)frame_vnr, VNR_SAMPLES);
                    }
                }
            }
            continue;
        } // event_ai_chanend
    } // select
}

void app_ai_kwd(chanend_t c_process, chanend_t c_gpo) {

    kwd_spotter_state_t state = KWD_SPOTTER_NOT_READY;
    kwd_spotter_ctx_t ctx = {{0}};

    state = kwd_spotter_init(&ctx);
    assert(state == KWD_SPOTTER_INIT_DONE);

    uint32_t *fft_frame_ptr = ctx.kwd_frame_ptr;

    kwd_label_indices_t keyword_detected = UNKNOWN;

    SELECT_RES(CASE_THEN(c_process, event_frame_received))
    {
        event_frame_received:{
            chan_in_buf_word(c_process, fft_frame_ptr, N_SAMPLES);
            state = kwd_spotter_compute(&ctx);
            if (state == KWD_SPOTTER_DONE){
                if (ctx.kwd_output[LEFT] > THRESHOLD){
                    keyword_detected = LEFT;
                }
                else if (ctx.kwd_output[RIGHT] > THRESHOLD){
                    keyword_detected = RIGHT;
                }
                else{
                    keyword_detected = UNKNOWN;
                }
                chan_out_word(c_gpo, keyword_detected); // Notify GPO task
            }
            continue;
        } // event_frame_received
    } // select
}


void app_ai_vnr(chanend_t c_vnr, chanend_t c_gpo, chanend_t c_flash, chanend_t c_cdc) {
    // Defs
    complex_s32_t DWORD_ALIGNED input_frame[VNR_FD_FRAME_LENGTH];
    bfp_complex_s32_t X;
    bfp_s32_t feature_patch;
    
    int32_t feature_patch_data[VNR_PATCH_WIDTH*VNR_MEL_FILTERS];

    // Init the vnr
    vnr_input_state_t vnr_input_state;
    vnr_feature_state_t vnr_feature_state;
    vnr_input_state_init(&vnr_input_state);
    vnr_feature_state_init(&vnr_feature_state);
    int32_t err = vnr_inference_init(c_flash);
    assert(err == 0);

    // Output vars
    float vnr_output_raw = 0.0f;
    float vnr_output_ma = 0.0f;
    vnr_label_indices_t vnr_output_label = VOICE_00;
    uint32_t last_percent = 0;

    SELECT_RES(
        CASE_THEN(c_vnr, c_vnr_handler),
        CASE_THEN(c_cdc, c_cdc_handler)
    ){
        c_vnr_handler:{
            // add new frame
            int32_t new_frame[VNR_SAMPLES];
            chan_in_buf_word(c_vnr, (uint32_t*)&new_frame[0], VNR_SAMPLES);
            vnr_form_input_frame(&vnr_input_state, &X, input_frame, new_frame);
            
            // extract features
            vnr_extract_features(&vnr_feature_state, &feature_patch, feature_patch_data, &X);

            // run inference
            float_s32_t vnr_output_s32;
            vnr_inference(&vnr_output_s32, &feature_patch);
            vnr_output_raw = ldexpf((float)vnr_output_s32.mant, vnr_output_s32.exp);
            vnr_output_ma = ma_filter_update(vnr_output_raw);

            // decide label
            if(vnr_output_ma >= 0.9f) {
                vnr_output_label = VOICE_90;
            }
            else if(vnr_output_ma >= 0.5f) {
                vnr_output_label = VOICE_50;
            }
            else {
                vnr_output_label = VOICE_00;
            }
            chan_out_word(c_gpo, (uint32_t)vnr_output_label); // Notify GPO task

            // Send to CDC for monitoring (optional)
            last_percent = (uint32_t)(vnr_output_ma * 100.0f);
            continue;
        } // c_vnr_handler

        c_cdc_handler:{
            uint32_t rx = chan_in_word(c_cdc); // dummy read to trigger event
            chan_out_word(c_cdc, last_percent);
            (void)rx; // suppress unused variable warning
            continue;
        } // c_cdc_handler
    }
}

// C wrapper 
void app_ai(chanend_t c_ai,  chanend_t c_gpo[2], chanend_t c_flash, chanend_t c_cdc) {
    assert((SAMPLE_FREQUENCY / AI_SAMPLE_FREQUENCY == 2) && (SAMPLE_FREQUENCY % AI_SAMPLE_FREQUENCY == 0));
    channel_t c_kwd = chan_alloc();
    channel_t c_vnr = chan_alloc();
    PAR_JOBS(
        PJOB(app_ai_collect, (c_ai, c_kwd.end_a, c_vnr.end_a)),
        PJOB(app_ai_kwd, (c_kwd.end_b, c_gpo[0])),
        PJOB(app_ai_vnr, (c_vnr.end_b, c_gpo[1], c_flash, c_cdc))
    );
    chan_free(c_kwd);
    chan_free(c_vnr);
} // End of app_ai
