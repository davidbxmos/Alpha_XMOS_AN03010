// Copyright 2023-2025 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#pragma once

#include <stdint.h>

#define NORM_OPT_MINMAX     0  // min-max normalization
#define NORM_OPT_CLIP_LOW   1  // clip normalization (from bottom)
#define NORM_OPT_CLIP_HIGH  2  // clip norm (from high)
#define NORM_OPT_MEANSTD    3  // mean normalization
#define NORM_OPT_CLIP       4  // clip data (NORM_LOW, NORM_HIGH)

#define NORM_CHOICE         NORM_OPT_CLIP_HIGH

#define NORM_LOW        -13.8f
#define NORM_HIGH        4.0f

#if defined(__cplusplus) || defined(__XC__)
extern "C" {
#endif

#include "mel_tables.h"

// -------- mel functions --------
void mel_spec_init_kwd();
void mel_spec_init_vid();

void mel_spec_compute_kwd(
    int32_t mel_spectrum[MEL_SPECTRUM_BINS],
    int32_t in_frame[FFT_FRAME_SIZE]);

void mel_spec_compute_vid(
    int32_t mel_spectrum[MEL_SPECTRUM_BINS],
    int32_t in_frame[FFT_FRAME_SIZE]);

void mel_spec_normalize_range(
    float* arr_out,
    const unsigned size,
    const float low,
    const float high,
    char opt
);

// -------- log functions --------

// prefered way to convert q31 to float
void mel_spec_to_log_float(float* dst, int32_t* src, const unsigned size);

#if defined(__cplusplus) || defined(__XC__)
}
#endif
