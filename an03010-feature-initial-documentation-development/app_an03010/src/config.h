// Copyright 2025 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#pragma once

#ifndef CONFIG_H__
#define CONFIG_H__

#define CHANS_PER_FRAME         (2)
#define DATA_BITS               (32)
#define MASTER_CLOCK_FREQUENCY  (24576000)
#define NUM_I2S_LINES           (1)
#define SAMPLE_FREQUENCY        (32000)

#define BUFFER_SIZE     (NUM_I2S_LINES * CHANS_PER_FRAME)

#endif // CONFIG_H__
