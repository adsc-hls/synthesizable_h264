############################################################
## This file is generated automatically by Vivado HLS.
## Please DO NOT edit it.
## Copyright (C) 2015 Xilinx Inc. All rights reserved.
############################################################
open_project tpdn_hls
set_top decode_main
add_files tpdn_opt/cavlc.c
add_files tpdn_opt/cavlc.h
add_files tpdn_opt/decode.c
add_files tpdn_opt/decode.h
add_files tpdn_opt/framealloc.c
add_files tpdn_opt/framealloc.h
add_files tpdn_opt/global.h
add_files tpdn_opt/interpred.c
add_files tpdn_opt/interpred.h
add_files tpdn_opt/intrapred.c
add_files tpdn_opt/intrapred.h
add_files tpdn_opt/ldecod.c
add_files tpdn_opt/mathfunc.c
add_files tpdn_opt/mathfunc.h
add_files tpdn_opt/nalu.c
add_files tpdn_opt/nalu.h
add_files tpdn_opt/parset.c
add_files tpdn_opt/parset.h
add_files tpdn_opt/residual.c
add_files tpdn_opt/residual.h
add_files tpdn_opt/slice.c
add_files tpdn_opt/slice.h
add_files tpdn_opt/vlc.c
add_files tpdn_opt/vlc.h
add_files -tb tpdn_opt/ldecod.c
add_files -tb input/test.264
add_files -tb golden/test_dec.yuv


open_solution -reset "solution1"
set_part {xc7vx690tffg1761-2}
create_clock -period 10 -name default


csim_design -clean -argv {test.264 test_dec.yuv}

csynth_design

cosim_design -argv {test.264 test_dec.yuv} -trace_level none -rtl systemc -tool xsim

