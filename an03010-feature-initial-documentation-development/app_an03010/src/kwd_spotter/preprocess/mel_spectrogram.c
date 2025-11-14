// Copyright 2019-2025 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include <xcore/assert.h>
#include <xmath/xmath.h>

#include "mel_spectrogram.h"

#define MS_EXPONENT -22 // exponent for mel spectogram calculation

static bfp_s32_t mel_flt[2];
static bfp_s32_t wind;
static unsigned mel_spec_init_done = 0;
static int32_t* mel_idx = NULL;

void mel_spec_init_vid() {
    bfp_s32_init(&wind, (int32_t*)hamming_window_q30, -30, VID_WINDOW_LEN, 1);
    bfp_s32_init(&mel_flt[0], (int32_t*)&mel_q30_00_8k[1][0], -30, MELBANK_VID_W, 1);
    bfp_s32_init(&mel_flt[1], (int32_t*)&mel_q30_00_8k[2][0], -30, MELBANK_VID_W, 1);
    mel_spec_init_done = 1;
    mel_idx = (int32_t*)&mel_q30_00_8k[0][0];
}

void mel_spec_init_kwd() {
    bfp_s32_init(&wind, (int32_t*)hanning_window_q31, -31, KWD_WINDOW_LEN, 1);
    bfp_s32_init(&mel_flt[0], (int32_t*)&mel_q30_20_4k[1][0], -30, MELBANK_KWD_W, 1);
    bfp_s32_init(&mel_flt[1], (int32_t*)&mel_q30_20_4k[2][0], -30, MELBANK_KWD_W, 1);
    mel_spec_init_done = 1;
    mel_idx = (int32_t*)&mel_q30_20_4k[0][0];
}

void mel_spec_compute_kwd(
    int32_t mel_spectrum[MEL_SPECTRUM_BINS],
    int32_t in_frame[FFT_FRAME_SIZE]) {

    int32_t DWORD_ALIGNED spectrogram[128] = { 0 };

    // init
    xassert(mel_spec_init_done);
    bfp_s32_t input;
    bfp_s32_t mag;
    bfp_s32_init(&input, in_frame, -31, FFT_FRAME_SIZE, 1);
    bfp_s32_init(&mag, spectrogram, 0, 128, 0);

    // preparing chunks
    bfp_s32_t chunk_128[2];
    bfp_s32_init(&chunk_128[0], &in_frame[0], 0, 128, 0);
    bfp_s32_init(&chunk_128[1], &in_frame[128], 0, 128, 0);

    // window
    bfp_s32_mul(&input, &input, &wind);

    // fft, no unpacking as we'll get rid of dc and nq in mel anyway
    bfp_complex_s32_t* X = bfp_fft_forward_mono(&input);
    
    // we only use 0 - 4k Hz
    X->length = 128;
    bfp_complex_s32_headroom(X);
    
    bfp_complex_s32_mag(&mag, X);

    // mel spec
    bfp_s32_mul(&chunk_128[0], &mel_flt[0], &mag);
    bfp_s32_mul(&chunk_128[1], &mel_flt[1], &mag);
    bfp_s32_use_exponent(&chunk_128[0], MS_EXPONENT);
    bfp_s32_use_exponent(&chunk_128[1], MS_EXPONENT);
    
    // computing mel (output is q24)
    vect_s32_set(mel_spectrum, 0, MEL_SPECTRUM_BINS);
    
    for (unsigned i = 0; i < MELBANK_KWD_W; i++) {
        unsigned indx = mel_idx[i];
        mel_spectrum[indx] += chunk_128[0].data[i];
        if (indx != MEL_SPECTRUM_BINS - 1) {
            mel_spectrum[indx + 1] += chunk_128[1].data[i];
        }
    }
    // cleanup as the memory was reused in a weird way
    vect_s32_set(in_frame, 0, FFT_FRAME_SIZE);
}

void mel_spec_compute_vid(
    int32_t mel_spectrum[MEL_SPECTRUM_BINS],
    int32_t in_frame[FFT_FRAME_SIZE]) {
    
    int32_t DWORD_ALIGNED spectrogram[256] = { 0 };
    xassert(mel_spec_init_done);
    bfp_s32_t input;
    bfp_s32_t mag;
    // initialise for the windowing first
    bfp_s32_init(&input, &in_frame[VID_PADDING], -31, VID_WINDOW_LEN, 1);
    bfp_s32_init(&mag, spectrogram, 0, 256, 0);

    // preparing chunks
    bfp_s32_t chunk_256[2];
    bfp_s32_init(&chunk_256[0], &in_frame[0], 0, 256, 0);
    bfp_s32_init(&chunk_256[1], &in_frame[256], 0, 256, 0);

    // window
    bfp_s32_mul(&input, &input, &wind);
    // reinitialise for the whole array, keeping the headroom and exponent
    input.data = &in_frame[0];
    input.length = FFT_FRAME_SIZE;

    // fft, no unpacking as we'll get rid of dc and nq in mel anyway
    bfp_complex_s32_t* X = bfp_fft_forward_mono(&input);

    bfp_complex_s32_squared_mag(&mag, X);

    // mel spec
    bfp_s32_mul(&chunk_256[0], &mel_flt[0], &mag);
    bfp_s32_mul(&chunk_256[1], &mel_flt[1], &mag);
    bfp_s32_use_exponent(&chunk_256[0], MS_EXPONENT);
    bfp_s32_use_exponent(&chunk_256[1], MS_EXPONENT);

    // computing mel (output is q24)
    vect_s32_set(mel_spectrum, 0, MEL_SPECTRUM_BINS);

    for (unsigned i = 0; i < MELBANK_VID_W; i++) {
        unsigned indx = mel_idx[i];
        mel_spectrum[indx] += chunk_256[0].data[i];
        mel_spectrum[indx] = (mel_spectrum[indx] < 0) ? INT32_MAX : mel_spectrum[indx];
        if (indx != MEL_SPECTRUM_BINS - 1) {
            mel_spectrum[indx + 1] += chunk_256[1].data[i];
            mel_spectrum[indx + 1] = (mel_spectrum[indx + 1] < 0) ? INT32_MAX : mel_spectrum[indx + 1];
        }
    }
    // cleanup as the memory was reused in a weird way
    vect_s32_set(in_frame, 0, FFT_FRAME_SIZE);
}

void mel_spec_normalize_range(
    float* arr_out,
    const unsigned size,
    const float low,
    const float high,
    const char opt) {
    // normalize log-mel-spec given ranges and options
    // Find min and max values in the array
    const float range_tol = 1e-6;
    const float std_tol = 1e-6;
    float min = arr_out[0];
    float max = arr_out[0];
    float sum = 0.0; // For mean calculation
    float sum_sq = 0.0; // For standard deviation calculation
    for (unsigned i = 0; i < size; i++) {
        if (arr_out[i] < min) {
            min = arr_out[i];
        }
        if (arr_out[i] > max) {
            max = arr_out[i];
        }
        sum += arr_out[i]; // Add to sum
        sum_sq += arr_out[i] * arr_out[i]; // Add to sum of squares
    }

    // Calculate mean, std, range
    float mean = sum / size;
    float variance = sum_sq / size - mean * mean;
    float std_dev = sqrtf(variance);
    
    // Precompute normalization parameters
    float range = (max - min);

    // Compensate scale and range to avoid /0
    float scale = range < range_tol ? 0 : (high - low) / range;
    float adjusted_std_dev = std_dev < std_tol ? std_tol : std_dev;

    // Normalize the array
    for (unsigned i = 0; i < size; i++) {
        switch (opt) {
        case NORM_OPT_MINMAX:
        {
            arr_out[i] = (arr_out[i] - min) * scale + low;
            break;
        }
        case NORM_OPT_CLIP_LOW:
        {
            float tmp = (arr_out[i] - min + low);
            tmp = tmp > high ? high : tmp;
            arr_out[i] = tmp;
            break;
        }
        case NORM_OPT_CLIP_HIGH:
        {
            float tmp = (arr_out[i] - max + high);
            tmp = tmp < low ? low : tmp;
            arr_out[i] = tmp;
            break;
        }
        case NORM_OPT_MEANSTD:
        {
            arr_out[i] = (arr_out[i] - mean) / adjusted_std_dev;
            break;
        }
        case NORM_OPT_CLIP:
        {
            float tmp = arr_out[i];
            tmp = tmp < low ? low : tmp;
            tmp = tmp > high ? high : tmp;
            arr_out[i] = tmp;
            break;
        }
        default:
            xassert(0 && "Invalid normalization option");
        }
    }
}

// ----------  log functions  ----------

void mel_spec_to_log_float(float* dst, int32_t* src, const unsigned size) {
    // clip src values
    const unsigned min_clip = 10;
    for (unsigned i = 0; i < size; i++) {
        src[i] = src[i] < min_clip ? min_clip : src[i];
    }
    // src, dst DWORD aligned
    xassert(((uintptr_t)src % 8) == 0);
    xassert(((uintptr_t)dst % 8) == 0);
    vect_s32_log((q8_24 *)dst, src, MS_EXPONENT, size);
    vect_s32_to_vect_f32(dst, (int32_t*)dst, size, -24);
}
