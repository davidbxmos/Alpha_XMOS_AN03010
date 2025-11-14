// Copyright 2019-2025 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include "dsp.h"

#include "mel_cepstrum.h"

//Take natural logs of the mel bins
void compute_logs(int32_t mel_spectrum[MEL_SPECTRUM_BINS]){

#if DEBUG_LN_RANGE
  int32_t max_log = INT_MIN;
  int32_t min_log = INT_MAX;
  int32_t max_mel = INT_MIN;
  int32_t min_mel = INT_MAX;
#endif
  for (int i = 0; i < MEL_SPECTRUM_BINS; i++){
#if DEBUG_LN_RANGE
    if (mel_spectrum[i] > max_mel) max_mel = mel_spectrum[i];
    if (mel_spectrum[i] < min_mel) min_mel = mel_spectrum[i];
#endif
    mel_spectrum[i] = dsp_math_log(mel_spectrum[i] + 10);
#if DEBUG_LN_RANGE
    if (mel_spectrum[i] > max_log) max_log = mel_spectrum[i];
    if (mel_spectrum[i] < min_log) min_log = mel_spectrum[i];
#endif
  }
#if DEBUG_LN_RANGE
  printf("mxl: %d, mnl: %d diff:%d\n", max_log, min_log, max_log - min_log);
  printf("mxm: %d, mnm: %d diff:%d\n", max_mel, min_mel, max_mel - min_mel);
#endif
}

void pre_dct_processing(int32_t log_mel[DCT_COEFF_COUNT], const int32_t log_offset, const int32_t pre_dct_scale){
  for(int i = 0; i < DCT_COEFF_COUNT; i++) {
    int32_t log_factored = log_mel[i] + log_offset;
    int32_t pre_dct_factored = log_factored / pre_dct_scale;
    log_mel[i] = pre_dct_factored;
  }
}

//Check for coefficient 0 overflow pre-dct. DCT coeff 0 is the sum of all of the inputs
int32_t check_dc_overflow(int32_t dct_input[DCT_COEFF_COUNT], int64_t *dct0){
    int64_t sum = 0;
    for (int i=0; i<DCT_COEFF_COUNT; i++) sum += dct_input[i];
    *dct0 = sum;

    if (sum < -0x7fffffff) return -1;
    if (sum > 0x7fffffff) return 1;
    return 0;
}

void compute_cepstrum(int32_t features[DCT_COEFF_COUNT], int32_t mel_spectrum[MEL_SPECTRUM_BINS]) {

  const int32_t log_offset    = log(1 << (KEYWORD_LN_FRAC_BITS + KEYWORD_MEL_HEADROOM_BITS)) * (1 << KEYWORD_LN_FRAC_BITS); // (1 << 16 + 8 * 1 << 16)
  const int32_t pre_dct_scale = (1 << KEYWORD_DCT_HEADROOM_BITS); // 1 << 8

  //In place natural log calculation
  compute_logs(mel_spectrum);

  int32_t *dct_input_ptr = &mel_spectrum[0];
  pre_dct_processing(dct_input_ptr, log_offset, pre_dct_scale);

  //Re-use the fft_mag_spectrum storage for the DCT output rather than declaring new array
  int32_t *dct_output_ptr = features;

  //Compute the 40pt DCT on the log-mels, which are the MFCCs
  dsp_dct_forward40(dct_output_ptr, dct_input_ptr);

#if DEBUG_DCT_OVERFLOW
  int64_t sum;
  if (check_dc_overflow(dct_input_ptr, &sum)) printf("DCT coeff 0 overflow: %lld\n", sum);
#endif
}
