Synthesizable H.264
===================

Introduction
------------

This source code is a synthesizable C code version of H264 decoder with certain specified format. The decoder accepts .264 format as input and yuv video file as output.
This source code also provides the shell script which automatically run the HLS and RTL cosimulation process for the RTL decoder.


Test Case Source
----------------
The input file is obtain from H.264/AVC JM Reference Software version 8.6. (See [JM Reference](http://iphome.hhi.de/suehring/tml/ for detail))

Please compile as instructed on the website and run the binary execution lencod.exe and ldecod.exe to generate test input file and golden files.

Use the .cfg files in directory "JM cfg" as the configuration for binary execution lencod.exe and ldecod.exe (copy them into the bin directory to replace the origin one).

Please modify the following options to make it fitful for the size of your raw yuv file.

```
InputFile   # Input file name, YUV 4:2:0
SourceWidth  # Image width in Pels, must be multiple of 16
SourceHeight  # Image height in Pels, must be multiple of 16
```


Following is an example of raw yuv input file "akiyo_qcif.yuv" with size as 176x144 
```
------example----------------------------------------
InputFile             = "akiyo_qcif.yuv"       # Input sequence, YUV 4:2:0
InputHeaderLength     = 0      # If the inputfile has a header, state it's length in byte here 
StartFrame            = 0      # Start frame for encoding. (0-N)
FramesToBeEncoded     = 3     # Number of frames to be coded
FrameRate             = 30	   # Frame Rate per second (1-100)
SourceWidth           = 176    # Image width in Pels, must be multiple of 16
SourceHeight          = 144    # Image height in Pels, must be multiple of 16
TraceFile             = "trace_enc.txt"
ReconFile             = "test_rec.yuv"
OutputFile            = "test.264"
-----------------------------------------------------
```

In the bin directory, run 
```
./lencod -f encoder.cfg
./ldecod decoder.cfg
```

The intput test file will be "test.264".
The golden file will be "test_dec.yuv".

Put the output file in directory "input".
Put golden file in directory "golden".

This source provides 2 groups of input and golden files with resolution of QCIF and 480P.

QCIF:
test_qcif.264
test_dec_qcif.yuv

640x480
test_480.264
test_dec_480.yuv

Note, the resolution size should be a multiple of 16.


HLS tool requirement
--------------------
This code is tested under and aimed for vivado High level synthesis tool 2014.4.  (see http://www.xilinx.com/support/download/index.html/content/xilinx/en/downloadNav/vivado-design-tools/2014-4.html for details)

Please make sure vivado HLS is installed and make sure command line "vivado_hls" take effect.

***make***

The binary file ldecode will be generated in the directory bin.
THe source code is located in directory tpdn_opt. 
The resolution size is set through the macro definition in global.h.


For QCIF resolution, set 
```
#define PicWidthInMBs 11
#define FrameHeightInMbs 9
```
For 640x480 resolution, set 
```
#define PicWidthInMBs 40
#define FrameHeightInMbs 30
```

Other sizes can be set as multiple of 16.
```
-----example--------

For 352 x 288 decoder, set 

#define PicWidthInMBs 22
#define FrameHeightInMbs 18
-------------------
```

use command "make clean && make" to compile the C source file.
***C testing***

run ./ctest.sh to check the correctness of decoder.
If the result is same as golden output file, the program will output as "PASS".

The decode yuv file is testresult.yuv.

***HLS Testing***
Run Vivado HLS (tested with Vivado HLS 2014.4)
```
vivado_hls script.tcl
```

It will go through 3 steps: C verification, Synthesis, and Co-simulation
(using Verilog with xsim as simulator). Simulation with SystemC is recommended
since it is faster and gives meaningful error messages if thing goes wrong.
To simulate with SystemC, replace "verilog" with "systemc" at the last line
of the file script.tcl.

The solution result is in tpdn_hls/solution_onboard
