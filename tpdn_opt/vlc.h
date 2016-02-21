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


#ifndef _VLC_H_
#define _VLC_H_

#include "global.h"

unsigned char u_1(NALU_t* nalu);
unsigned char u_8(NALU_t* nalu);
unsigned int u_n(int n, NALU_t* nalu);
unsigned int u_e(NALU_t* nalu);
int s_e(NALU_t* nalu);
unsigned char m_e(int prediction_type, NALU_t* nalu);
unsigned char t_e(int n, NALU_t *nalu);
int showbits(int n, unsigned int, int);
unsigned int bytes_reverse_32(unsigned int num);
#endif
