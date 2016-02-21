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

#ifndef _FRAMEALLOC_H_
#define _FRAMEALLOC_H_

void init_listB(StorablePicture PicBff[MAX_REFERENCE_PICTURES],int list0[MAX_REFERENCE_PICTURES], int list1[MAX_REFERENCE_PICTURES], int curPOC);
void init_listP(StorablePicture PicBff[MAX_REFERENCE_PICTURES], int listX[MAX_REFERENCE_PICTURES],int curFrame_NUM, int maxframNUM);
#endif // _FRAMEALLOC_H_
