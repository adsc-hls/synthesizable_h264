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


#ifndef _PARSET_H_
#define _PARSET_H_

#include "global.h"


int ProcessSPS(seq_parameter_set_rbsp_t *SPS, NALU_t* nalu);
int ProcessPPS(pic_parameter_set_rbsp_t *pps, NALU_t* nalu);
void ProcessSH(seq_parameter_set_rbsp_t *active_sps,pic_parameter_set_rbsp_t *active_pps,slice_header_rbsp_t* SH,ImageParameters *img,NALU_t* nalu);
//void process_pred_weight_table(pred_weight_table_rbsp_t* PWT, char B_slice,pic_parameter_set_rbsp_t *pps, NALU_t *nalu);
void intepretSLiceHeader(
    slice_header_rbsp_t *sliceheader,
    seq_parameter_set_rbsp_t *sps,
    pic_parameter_set_rbsp_t *pps,
    unsigned char nalu_idc,
    unsigned char nalu_ref_idc,
    ImageParameters *img,
    StorablePicture PicBuff[MAX_REFERENCE_PICTURES]);
#endif
