// Copyright 2025 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#pragma once

#include <stdint.h>

#define N_LABELS 12

// Enum for keyword labels
typedef enum {
    DOWN = 0,
    GO,
    LEFT,
    NO,
    OFF,
    ON,
    RIGHT,
    STOP,
    UP,
    YES,
    SILENCE,
    UNKNOWN
} kwd_label_indices_t;

// Function to get the string representation of a keyword label
static inline const char* kws_get_label(unsigned label) {
    static const char* kws_labels[N_LABELS] = {
        [DOWN] = "Down",
        [GO] = "Go",
        [LEFT] = "Left",
        [NO] = "No",
        [OFF] = "Off",
        [ON] = "On",
        [RIGHT] = "Right",
        [STOP] = "Stop",
        [UP] = "Up",
        [YES] = "Yes",
        [SILENCE] = "Silence",
        [UNKNOWN] = "Unknown"
    };
    return (label < N_LABELS) ? kws_labels[label] : "Invalid";
}
