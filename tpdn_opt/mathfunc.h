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


#ifndef _MATHFUNC_H_
#define _MATHFUNC_H_ v
int Clip1y(int x);
int Clip3(int x, int y, int z);
int InverseRasterScan(int a, int b, int c, int d, int e);
int Clip1c(int x);
int median(int x, int y, int z);
int minpositive(int x, int y);
#endif
