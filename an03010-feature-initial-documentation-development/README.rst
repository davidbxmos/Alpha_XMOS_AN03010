:orphan:

##############################################################
AN03010: Adding functionality to an existing XCORE application
##############################################################

:vendor: XMOS
:version: 1.0.0
:scope: Tutorial
:description: How to incrementally extend an existing XCORE application
:category: General Purpose
:keywords: AI, IO, Serial Interface
:hardware: XK-EVK-XU316


*******
Summary
*******

This application note describes strategies for incrementally adding functionality to an existing XCORE application.
It includes an example application that illustrates some of these strategies.

********
Features
********

* Use of Control and Data planes
* Constraints when placing threads of processing
* Strategy for thread placement
* Thread combination

************
Known issues
************

* None

**************
Required tools
**************

* XMOS XTC Tools: 15.3.1

*********************************
Required libraries (dependencies)
*********************************

* lib_audio_dsp
* lib_board_support
* lib_dsp
* lib_i2s
* lib_xud

*************************
Related application notes
*************************

* `AN00124 - USB CDC Class as Virtual Serial Port <https://www.xmos.com/application-notes/an00124>`_
* `AN02050 - Extend I2S loopback application with DSP and AI <https://www.xmos.com/application-notes/an02050>`_
* `AN03009 - Placing interfaces on the XCORE <https://www.xmos.com/application-notes/an03009>`_
