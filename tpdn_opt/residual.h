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

#ifndef _RESIDUAL_H_
#define _RESIDUAL_H_
void scale_and_inv_trans_Intra16x16DC(int qP, int c[4][4], int qPm6, int scale1, int scale2, int scale3);
void scale_residual4x4(int qP, int qPm6, int temp1, int temp2, int temp3, int c[4][4], int d[4][4]);
void scale_and_inv_trans_chroma2x2(int c[2][2],int qP, int qPcm6);
void inv_trans_residual4x4(int d[4][4], int r[4][4]);
void scale_residual4x4_and_trans_inverse(int qP, int qPm6, int temp1, int temp2, int temp3, int c[4][4], int r[4][4], int DC_comp,unsigned char flag); /* 8.5.12.1 */
#endif
