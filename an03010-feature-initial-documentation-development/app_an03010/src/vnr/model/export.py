# Copyright 2023-2025 XMOS LIMITED.
# This Software is subject to the terms of the XMOS Public Licence: Version 1.

import os
from pathlib import Path
from xmos_ai_tools import xformer as xf

cwd = Path(__file__).parent.absolute()
os.chdir(cwd)

model_in = "trained_model.tflite"
model_out = "trained_model_xcore.tflite"
params_file = "weights.flash"
flash_file = "weights.out"

params = {
    "xcore-thread-count": 1,
    "xcore-weights-file": params_file,
    "xcore-conv-err-threshold": 0.495,
}

xf.convert(
    model_in,
    model_out,
    params
)

xf.generate_flash(
    output_file=flash_file, 
    model_files=[model_out], 
    param_files=[params_file]
)

xf.print_optimization_report()
