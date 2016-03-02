Synthesizable H.264
===================

Introduction
------------

This software package contains source code of a synthesizable C code version of H264 decoder with certain specified format. The source code is largely modified and rewritten based on the H.264/AVC JM Reference Software version 8.6 to be synthesizable, i.e. High-level Synthesis tools are able to synthesize the source code to RTL. The decoder accepts **.264** format as input and **.yuv** video file as output.
This software package also provides the TCL script which automatically run the HLS and RTL cosimulation process for the RTL decoder.


Test Case Source
----------------
The input file is obtained from H.264/AVC JM Reference Software version 8.6. (See [JM Reference](http://iphome.hhi.de/suehring/tml/ for detail))

Please compile as instructed on the website and run the binary execution **lencod.exe** and **ldecod.exe** to generate test input file and golden files.

Use the **.cfg** files in directory *JM cfg* as the configuration for binary execution **lencod.exe** and **ldecod.exe** (copy them into the bin directory to replace the origin one).

Please modify the following options to make it fitful for the size of your raw yuv file.

```
InputFile   # Input file name, YUV 4:2:0
SourceWidth  # Image width in Pels, must be multiple of 16
SourceHeight  # Image height in Pels, must be multiple of 16
```


Following is an example of raw yuv input file **akiyo_qcif.yuv** with size as 176x144 
```
------Example----------------------------------------
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

The intput test file will be **test.264**.
The golden file will be **test_dec.yuv**.

Put the output file in directory **input**.
Put golden file in directory **golden**.

This source provides 2 groups of input and golden files with resolution of QCIF and 480P.

QCIF:
**test_qcif.264**
**test_dec_qcif.yuv**

640x480
**test_480.264**
**test_dec_480.yuv**

Please note, the resolution size should be a multiple of 16.


HLS tool requirement
--------------------
This code is tested under Vivado High-level Synthesis (HLS) tool version 2014.4.  

Please make sure Vivado HLS is installed and make sure command line `vivado_hls` takes effect by updating your environment variable PATH.


Usage
-----

***make***

The binary file **ldecode** will be generated in the directory *bin*.
THe source code is located in directory *tpdn_opt*. 
The resolution size is set through the macro definition in **global.h**.


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

use command `make clean && make` to compile the C source file.

***C testing***

run `./ctest.sh` to check the correctness of decoder.
If the result is same as golden output file, the program will output "PASS".

The decode yuv file is **testresult.yuv**.

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

The solution result is in *tpdn_hls/solution1*

Onboard Verification
--------------------

We have also verified our synthesizable decoder on a real FPGA platform which is
the Zynq 7045 (Omnitek board). The scripts and code are located under *onboard_verification*.
To use it, please go to this directory and run the following command

```
./gen_design.sh
```

It will copy the source code into this directory, overwrite the existing source file(s)
with file(s) located under the directory *modified*. The modification is largely about
adding Vivado HLS interface pragmas to generate an IP that is compatible to integrate
into Vivado IP Integrator later. Specifically, we specify the interface signals of the
decoder IP as bus (either AXI4M for pointers or AXI4LiteS for control and scalars). The
script **hls_script.tcl** will synthesize the code to RTL IP. Next, the script **vivado_script.tcl**
will take the generated IP to integrate it into a simple Zynq-Accelerator system where
the decoder IP connects to the Zynq PS via the Slave AXI High Performance Port by using
the AXI interconnect IP (provided by Xilinx). The script will run Synthesis, Place & Route,
and finally generate a bitstream to configure the FPGA.

In this directory we also provide the host code for running the application on ARM located
at *host_code*. Copy this directory into the board, boot the OS, compile the code by typing
`make`. Then, configure the FPGA with the generated bitstream, and run the application:

```
./ldecode.elf test_480.264
```

The output files **testresult.txt** and **testresult.yuv** will be generated. Compare these
files against the golden output files **golden/test_dec_480.txt** or **golden/test_dec_480.yuv** 
either using the command `diff` to check whether the execution is correct or not. If there is 
no mismatch, the decoder IP funtions correctly.

Please note that this code just serves as an "onboard verification" to make sure that our
source code is verified at silicon level. This is not meant to demonstrate a full system doing
real time video streaming, which is our ongoing work. The results we claimed in our paper
come from Vivado Cosimulation result which the interface signals are implemented as dual-port
BRAM.


Publication
-----------
X. Liu, Y. Chen, T. Nguyen, S. Gurumani, K. Rupnow, and D. Chen, “High Level Synthesis of Complex 
Applications: An H.264 Video Decoder”, Proceedings of ACM/SIGDA International Symposium on 
Field Programmable Gate Arrays, February 2016.

Contact
-------
For bug report, please email Xinheng Liu at <xliu79@illinois.edu>.

