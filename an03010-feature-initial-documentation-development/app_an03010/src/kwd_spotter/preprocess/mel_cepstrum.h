// Copyright 2025 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.
#pragma once

#include <stdint.h>

#ifndef DEBUG_LN_RANGE
#define DEBUG_LN_RANGE 0
#endif

#ifndef DEBUG_DCT_OVERFLOW
#define DEBUG_DCT_OVERFLOW 0
#endif

#define KEYWORD_LN_FRAC_BITS        16
#define KEYWORD_MEL_HEADROOM_BITS   8
#define KEYWORD_DCT_HEADROOM_BITS   8
#define DCT_COEFF_COUNT             MEL_SPECTRUM_BINS

#if defined(__cplusplus) || defined(__XC__)
extern "C" {
#endif

#include "mel_tables.h"

void compute_cepstrum(
    int32_t features[DCT_COEFF_COUNT],
    int32_t mel_spectrum[MEL_SPECTRUM_BINS]);

#if defined(__cplusplus) || defined(__XC__)
}
#endif
