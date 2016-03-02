############################################################
## This file is generated automatically by Vivado HLS.
## Please DO NOT edit it.
## Copyright (C) 2015 Xilinx Inc. All rights reserved.
############################################################
open_project hls_proj
set_top decode_main
add_files hls_source_code/cavlc.c
add_files hls_source_code/cavlc.h
add_files hls_source_code/decode.c
add_files hls_source_code/decode.h
add_files hls_source_code/framealloc.c
add_files hls_source_code/framealloc.h
add_files hls_source_code/global.h
add_files hls_source_code/interpred.c
add_files hls_source_code/interpred.h
add_files hls_source_code/intrapred.c
add_files hls_source_code/intrapred.h
add_files hls_source_code/ldecod.c
add_files hls_source_code/mathfunc.c
add_files hls_source_code/mathfunc.h
add_files hls_source_code/nalu.c
add_files hls_source_code/nalu.h
add_files hls_source_code/parset.c
add_files hls_source_code/parset.h
add_files hls_source_code/residual.c
add_files hls_source_code/residual.h
add_files hls_source_code/slice.c
add_files hls_source_code/slice.h
add_files hls_source_code/vlc.c
add_files hls_source_code/vlc.h
add_files -tb hls_source_code/ldecod.c
add_files -tb ../input/test_480.264
add_files -tb ../golden/test_dec_480.yuv


open_solution -reset "solution1"
#VC709
#set_part {xc7vx690tffg1761-2}
#Zynq 7045
set_part {xc7z045ffg900-1}

create_clock -period 10 -name default
csim_design -clean -argv {test_480.264 test_dec_480.yuv}
csynth_design
export_design
