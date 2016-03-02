#!/bin/bash

rm -rf hls_source_code
cp ../tpdn_opt hls_source_code -R
cp modified/decode.c hls_source_code
vivado_hls hls_script.tcl
vivado -mode batch -source vivado_script.tcl
