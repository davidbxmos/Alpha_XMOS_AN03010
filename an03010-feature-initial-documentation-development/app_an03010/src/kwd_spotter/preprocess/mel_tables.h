// Copyright 2024-2025 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#pragma once

#include <stdint.h>

#define MELBANK_KWD_W     128
#define MELBANK_VID_W     256

#define FFT_FRAME_SIZE    512
#define MEL_SPECTRUM_BINS 40

#define KWD_WINDOW_LEN    512
#define VID_WINDOW_LEN    400

#define VID_PADDING       (FFT_FRAME_SIZE - VID_WINDOW_LEN) / 2

extern const int32_t mel_q30_20_4k[3][MELBANK_KWD_W];
extern const int32_t mel_q30_00_8k[3][MELBANK_VID_W];

extern const int32_t hanning_window_q31[KWD_WINDOW_LEN];
extern const int32_t hamming_window_q30[VID_WINDOW_LEN];
