# Copyright 2023-2025 XMOS LIMITED.
# This Software is subject to the terms of the XMOS Public Licence: Version 1.

import os
from pathlib import Path
from xmos_ai_tools import xformer as xf

print("Entering script")

cwd = Path(__file__).parent.absolute()
os.chdir(cwd)

model_in = "kws_ref_model.tflite"
model_out = "kws.tflite"
prefix = "model_kwd_"

params = {
    "xcore-thread-count": 1,
    "xcore-naming-prefix": prefix,
}

print("About to call convert")

xf.convert(
    model_in,
    model_out,
    params
)

print("About to call opt report")
xf.print_optimization_report()
