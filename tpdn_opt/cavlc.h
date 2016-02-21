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

#ifndef _CAVLC_H_
#define _CAVLC_H_

unsigned char residual_block_cavlc_4(int coeffLevel[2][2], NALU_t *nalu,int startIdx, int endIdx);
unsigned char residual_block_cavlc_16(int coeffLevel[4][4], NALU_t *nalu,int startIdx, int endIdx,int nC);
unsigned char nc_Chroma(unsigned char residualmode[PicWidthInMBs][FrameHeightInMbs],unsigned char nzC[PicWidthInMBs*2][FrameHeightInMbs*2],int xoff,int yoff);
unsigned char nc_Luma(unsigned char residualmode[PicWidthInMBs][FrameHeightInMbs],unsigned char nz[PicWidthInMBs*4][FrameHeightInMbs*4],int xoff,int yoff);

#endif // _CAVLC_H_
