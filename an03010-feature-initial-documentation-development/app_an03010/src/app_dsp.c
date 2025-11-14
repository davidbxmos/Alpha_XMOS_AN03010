// Copyright 2025 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <xs1.h>
#include <platform.h>
#include <limits.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <xcore/channel.h>
#include <xcore/select.h>
#include "app_dsp.h"
#include "config.h"
#include "dsp/signal_chain.h"

static volume_control_t volume_control = {
    .target_gain = 0,
    .gain = 0,
    .slew_shift = 10, // 1024 samples to reach target gain
    .saved_gain = 0,
    .mute_state = 0
};

#define VOLUME_LEVEL_COUNT (4)
// Arrange violume levels in decreasing order of gain to allow an index equal to VOLUME_LEVEL_COUNT to signify a muted state.
static const int32_t VOLUME_LEVEL[VOLUME_LEVEL_COUNT] = {
    INT_MAX>>2,  // -6dB
    INT_MAX>>3,  // -9dB
    INT_MAX>>4,  // -12dB
    INT_MAX>>5,  // -15dB
};

#define DECREASE (false)
#define INCREASE (true)

typedef struct {
    size_t next_index;
    bool next_direction; // DECREASE or INCREASE
} volume_change_t;

static const volume_change_t DECREASE_VOLUME[VOLUME_LEVEL_COUNT] = {
    {1, DECREASE},  // Move to -9dB, continue decreasing
    {2, DECREASE},  // Move to -12dB, continue decreasing
    {3, DECREASE},  // Move to -15dB, continue decreasing
    {4, INCREASE}   // Mute, start increasing
}; // DECREASE_VOLUME

static const volume_change_t INCREASE_VOLUME[VOLUME_LEVEL_COUNT+1] = {
    {0, DECREASE},   // Should not happen, start decreasing
    {0, DECREASE},   // Move to -6dB, start decreasing
    {1, INCREASE},   // Move to -9dB, continue increasing
    {2, INCREASE},   // Move to -12dB, continue increasing
    {3, INCREASE}    // Unmute, move to -15dB, continue increasing
}; // INCREASE_VOLUME

void app_dsp(chanend c_dsp, chanend c_gpi)
{
    // Set initial volume control gain and make sure it's un-muted
    int volume_idx = 0; // Start at -6dB
    adsp_volume_control_set_gain(&volume_control, VOLUME_LEVEL[volume_idx]);
    adsp_volume_control_unmute(&volume_control);
    bool direction = DECREASE;

    // Initialize the audio sample buffer
    int idx = 0;
    int32_t sample[BUFFER_SIZE];

    // Create a place to sink the button-pressed data
    bool button_pressed;

    SELECT_RES(
        CASE_THEN(c_dsp, event_dsp_chanend),
        CASE_THEN(c_gpi, event_button_pressed)
    ) // SELECT_RES
    {
        event_dsp_chanend:
            sample[idx] = chan_in_word(c_dsp);

            if(BUFFER_SIZE <= ++idx) {
                for(size_t i = 0; i < BUFFER_SIZE; ++i) {
                    int32_t result = adsp_volume_control(&volume_control, sample[i]);
                    chan_out_word(c_dsp, result);
                }
                idx = 0;
            }

            continue; // end of event_dsp_chanend

        event_button_pressed:
            button_pressed = chan_in_word(c_gpi);

            if(direction == DECREASE) {
                direction  = DECREASE_VOLUME[volume_idx].next_direction;
                volume_idx = DECREASE_VOLUME[volume_idx].next_index;
                if(VOLUME_LEVEL_COUNT <= volume_idx) {
                    adsp_volume_control_mute(&volume_control);
                } else {
                    adsp_volume_control_set_gain(&volume_control, VOLUME_LEVEL[volume_idx]);
                }
            } else {
                direction  = INCREASE_VOLUME[volume_idx].next_direction;
                volume_idx = INCREASE_VOLUME[volume_idx].next_index;
                adsp_volume_control_set_gain(&volume_control, VOLUME_LEVEL[volume_idx]);
                if(volume_idx == VOLUME_LEVEL_COUNT-1) {
                    adsp_volume_control_unmute(&volume_control);
                }
            }

            continue; // end of event_button_pressed
    } // end of select
}
