//============================================================================//
//    H.264 High Level Synthesis Benchmark
//    Copyright (c) <2016>
//    <University of Illinois at Urbana-Champaign>
//    All rights reserved.
//
//    Developed by:
//
//    <ES CAD Group>
//        <University of Illinois at Urbana-Champaign>
//        <http://dchen.ece.illinois.edu/>
//
//    <Hardware Research Group>
//        <Advanced Digital Sciences Center>
//        <http://adsc.illinois.edu/>
//============================================================================//

#ifndef _INTERPRED_H_
#define _INTERPRED_H_

void processinterMbType(
unsigned char mb_type,
unsigned char slice_type,
NALU_t * nalu,
unsigned char num_ref_idx_active1,
unsigned char num_ref_idx_active0,
char refidx0[2][2],
char refidx1[2][2],
int mvd0[4][4][2],
int mvd1[4][4][2],
char globref0[PicWidthInMBs*2][FrameHeightInMbs*2],
char globref1[PicWidthInMBs*2][FrameHeightInMbs*2],
int globmvd0[PicWidthInMBs*4][FrameHeightInMbs*4][2],
int globmvd1[PicWidthInMBs*4][FrameHeightInMbs*4][2],
int list0[MAX_REFERENCE_PICTURES],
int list1[MAX_REFERENCE_PICTURES],
int Mbaddress,
int skipflag,
char refcol[2][2],
int mvcol[4][4][2],
unsigned char tmpmbtp
);


void inter_prediction_luma(
unsigned char predL[4][4],
char refid,
char refid1,
int mv[2],
int mv1[2],
unsigned char Sluma0[PicWidthInSamplesL][FrameHeightInSampleL],
unsigned char Sluma1[PicWidthInSamplesL][FrameHeightInSampleL],
int startblkx,
int startblky);



void inter_luma_single
(
unsigned char Sluma_cur[PicWidthInSamplesL][FrameHeightInSampleL],
int rMb[4][4],
int startx,
int starty,
unsigned char xfrac,
unsigned char yfrac,
unsigned char xoffset,
unsigned char yoffset,
unsigned char temp[9][9],
unsigned char rmbflag
);

void inter_luma_double_skip
(
unsigned char Sluma_cur[PicWidthInSamplesL][FrameHeightInSampleL],
int startx,
int starty,
unsigned char xfrac0,
unsigned char yfrac0,
unsigned char xfrac1,
unsigned char yfrac1,
unsigned char xoffset0,
unsigned char yoffset0,
unsigned char xoffset1,
unsigned char yoffset1,
unsigned char temp0[9][9],
unsigned char temp1[9][9],
int rMb[4][4],
unsigned char rmbflag

);
void inter_luma_double_bizero_skip
(
unsigned char Sluma0[PicWidthInSamplesL][FrameHeightInSampleL],
unsigned char Sluma1[PicWidthInSamplesL][FrameHeightInSampleL],
unsigned char Sluma_cur[PicWidthInSamplesL][FrameHeightInSampleL],
int startx,
int starty,
int xint0,
int yint0,
int xint1,
int yint1,
int rMb[4][4],
unsigned char rmbflag
);
void copy_comp(unsigned char Sluma[PicWidthInSamplesL][FrameHeightInSampleL],int xint, int yint,unsigned char xfrac, unsigned char yfrac, unsigned char temp[9][9]);


void inter_prediction_chroma_subblock_double(
int rMbC[4][4],
int mv0[2][2][2],
int mv1[2][2][2],
unsigned char Schroma0[PicWidthInSamplesC][FrameHeightInSampleC],
unsigned char Schroma1[PicWidthInSamplesC][FrameHeightInSampleC],
unsigned char Schroma_cur[PicWidthInSamplesC][FrameHeightInSampleC],
int startblkx,
int startblky,

unsigned char flag

);


void inter_prediction_chroma_subblock_single(
int rMbC[4][4],
int mv[2][2][2],
unsigned char Schroma[PicWidthInSamplesC][FrameHeightInSampleC],
unsigned char Schroma_cur[PicWidthInSamplesC][FrameHeightInSampleC],
int startblkx,
int startblky,

unsigned flag

);

#endif // _INTERPRED_H_
