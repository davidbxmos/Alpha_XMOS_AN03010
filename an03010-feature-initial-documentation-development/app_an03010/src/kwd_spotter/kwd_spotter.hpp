// Copyright 2025 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.
#pragma once

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "mel_tables.h"
#include "kws.tflite.h"

#define N_SAMPLES      (320)
#define N_FEATURES     (10)
#define KWD_FRAME_SIZE (FFT_FRAME_SIZE) // 512
#define KWD_DB_N       (N_FEATURES)

/** Possible states of the keyword spotter. */
typedef enum {
    KWD_SPOTTER_INIT_DONE,   /**< Initialization completed successfully. */
    KWD_SPOTTER_INIT_NOK,    /**< Initialization failed. */
    KWD_SPOTTER_NOT_READY,   /**< More FFT frames required before inference. */
    KWD_SPOTTER_DONE         /**< Inference finished, output ready. */
} kwd_spotter_state_t;

/** Context for the keyword spotter. */
typedef struct {
    int32_t kwd_frame[KWD_FRAME_SIZE]; /**< Input frame buffer. */
    uint8_t kwd_output[N_FEATURES];    /**< Output scores (0–255). */
    uint32_t *kwd_frame_ptr;           /**< Pointer to active region of @p kwd_frame. */
} kwd_spotter_ctx_t;

/**
 * @brief Initialize the keyword spotter instance.
 *
 * Must be called once before any calls to kwd_spotter_compute().
 *
 * @param ctx Pointer to the spotter context.
 * @return KWD_SPOTTER_INIT_DONE on success, KWD_SPOTTER_INIT_NOK on failure.
 */
kwd_spotter_state_t kwd_spotter_init(kwd_spotter_ctx_t *ctx);

/**
 * @brief Process a single FFT frame.
 *
 * The user should fill the frame buffer (ctx->kwd_frame) or write via
 * ctx->kwd_frame_ptr before calling, with N_SAMPLES new samples.
 *
 * @param ctx Pointer to the spotter context.
 * @return Current processing state. When it returns KWD_SPOTTER_DONE,
 *         the output array is ready.
 */
kwd_spotter_state_t kwd_spotter_compute(kwd_spotter_ctx_t *ctx);
