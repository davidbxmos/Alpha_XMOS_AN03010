// Copyright 2025 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.
#include <stdint.h>

#include "kwd_spotter.hpp"
#include "mel_spectrogram.h"
#include "mel_cepstrum.h"

extern "C" {
    #include "xmath/util.h" // for xs3_memcpy
}

#define MFCC0_BASE          (80)
#define N_BLOCKS            (49)
#define N_MEL_SPEC_BINS     (MEL_SPECTRUM_BINS)
#define N_DCT_BINS          (DCT_COEFF_COUNT)


#define PREV_FRAME_LEN      (FFT_FRAME_SIZE - N_SAMPLES) // 512 - 320 = 192
#define IN_FRAME_INDX       (PREV_FRAME_LEN) // 192

// -------------------------
// Internal static buffers
// -------------------------
static int32_t mel_spec[N_MEL_SPEC_BINS] = {0};
static int32_t prev_frame[PREV_FRAME_LEN] = {0};
static int32_t features_int32_single[N_DCT_BINS] = {0};
static int32_t mfcc0_bins[N_BLOCKS] = {0};
static int8_t features_int8_block[N_BLOCKS][N_FEATURES] = {{0}};
static int8_t features_int8_single[N_FEATURES] = {0};
static unsigned block_count = 0;
static int is_initialized = 0;

static inline
void scale_input(int8_t * input_ptr, int32_t * mfcc0_bins) {
    // compute max of MFCC[0]
    int32_t max = INT32_MIN;
    for (unsigned i = 0; i < N_BLOCKS; i++) {
        int32_t val = mfcc0_bins[i];
        max = (val > max) ? val : max;
    }
    // scale input, offset can be [- a lot, 40]
    int32_t m_scale = 127 - max;
    m_scale = (m_scale > 40) ? 40 : m_scale;
    // adjust MFFC[0] by m_scale
    for (unsigned i = 0; i < N_BLOCKS; i++) {
        int32_t val = mfcc0_bins[i] + m_scale;
        input_ptr[i * N_FEATURES] = (int8_t)val;
    }   
}

static inline
void scale_cepstrum(int32_t * features, int8_t * features_int8) {
    const int32_t base[N_FEATURES] = {MFCC0_BASE,80,80,80,80,80,80,80,80,80};
    for(unsigned i = 0; i < N_FEATURES; i++) {
        int32_t v = features[i]/160000 + base[i];
        features[i] = v < 0 ? 0 : v;
        features_int8[i] = (int8_t)features[i];
    }
}

static inline
void dequantize_output(
    const int8_t *input,
    uint8_t *output,
    size_t input_size,
    const float scale,
    const int32_t zero_point)
{
    if (input == nullptr || output == nullptr) {
        assert(0 && "Null pointer");
    }
    for (unsigned i = 0; i < input_size; i++)
    {
        output[i] = (uint8_t)((((float)input[i]) - zero_point) * scale * 100);
    }
}

kwd_spotter_state_t kwd_spotter_init(kwd_spotter_ctx_t* ctx) {
    int ret = model_kwd_init(NULL);
    mel_spec_init_kwd();
    if (is_initialized || (ctx == NULL) || (ret != 0)) {
        return KWD_SPOTTER_INIT_NOK;
    }
    ctx->kwd_frame_ptr = (uint32_t*)&ctx->kwd_frame[IN_FRAME_INDX];
    is_initialized = 1;
    return KWD_SPOTTER_INIT_DONE;
}

kwd_spotter_state_t kwd_spotter_compute(kwd_spotter_ctx_t* ctx) {
    if (!is_initialized || !ctx) {
        return KWD_SPOTTER_INIT_NOK;
    }

    int8_t* input_ptr = (int8_t*)model_kwd_input_ptr(0);
    int8_t* outputs = (int8_t*)model_kwd_output_ptr(0);
    const int32_t zp = model_kwd_output_zeropoint(0);
    const float scale = model_kwd_output_scale(0);

    xs3_memcpy(ctx->kwd_frame, prev_frame, PREV_FRAME_LEN * sizeof(int32_t));
    xs3_memcpy(prev_frame, &ctx->kwd_frame[N_SAMPLES], PREV_FRAME_LEN * sizeof(int32_t));

    mel_spec_compute_kwd(mel_spec, ctx->kwd_frame);
    compute_cepstrum(features_int32_single, mel_spec);
    scale_cepstrum(features_int32_single, features_int8_single);

    memmove(mfcc0_bins, &mfcc0_bins[1], (N_BLOCKS - 1) * sizeof(int32_t));
    mfcc0_bins[N_BLOCKS - 1] = features_int32_single[0]; // scaled MFCC[0]

    // Move features, discard the first row and add new row at the end
    memmove(features_int8_block, features_int8_block + 1, (N_BLOCKS - 1) * N_FEATURES * sizeof(int8_t));
    xs3_memcpy(features_int8_block[N_BLOCKS - 1], features_int8_single, N_FEATURES * sizeof(int8_t));
    xs3_memcpy(input_ptr, features_int8_block, N_BLOCKS * N_FEATURES * sizeof(int8_t));

    // Wait to fill spectogram
    if (block_count < N_BLOCKS - 1) {
        ++block_count;
        return KWD_SPOTTER_NOT_READY;
    }

    // Scale input
    scale_input(input_ptr, mfcc0_bins);

    // Model Invoke
    model_kwd_invoke();

    // Dequantize output
    dequantize_output(outputs, ctx->kwd_output, KWD_DB_N, scale, zp);
    return KWD_SPOTTER_DONE;
}
