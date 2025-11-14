cmake_minimum_required(VERSION 3.21)
include($ENV{XMOS_CMAKE_PATH}/xcommon.cmake)
project(app_an03010)

# setup Python venv
set(VENV_DIR ${CMAKE_CURRENT_LIST_DIR}/../.venv)
include(cfg_venv.cmake)

set(APP_HW_TARGET 
    XK-EVK-XU316
)

set(APP_DEPENDENT_MODULES       
    "lib_audio_dsp(1.4.0)"
    "lib_board_support(1.3.0)"
    "lib_i2s(6.0.1)"
    "lib_dsp(develop)"
    "lib_xud(3.0.0)"
)

set(APP_COMPILER_FLAGS       
    -DBOARD_SUPPORT_BOARD=XK_EVK_XU316    
    -mcmodel=large
    -Wno-xcore-fptrgroup
    -lquadflash 
    -mcmodel=large
    -report
    -Wall
    -O3
    -g
)

set(APP_INCLUDES 
    src 
    src/kwd_spotter src/kwd_spotter/model src/kwd_spotter/preprocess 
    src/vnr src/vnr/model
)

set(XMOS_SANDBOX_DIR            ${CMAKE_CURRENT_LIST_DIR}/../..)

XMOS_REGISTER_APP()

# Custom libs
include(cfg_aitools.cmake)
