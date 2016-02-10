
#ifndef _NALU_H_
#define _NALU_H_

#include <stdio.h>
#include "global.h"

#define MAXRBSPSIZE 64000

#define NALU_TYPE_SLICE    1
#define NALU_TYPE_DPA      2
#define NALU_TYPE_DPB      3
#define NALU_TYPE_DPC      4
#define NALU_TYPE_IDR      5
#define NALU_TYPE_SEI      6
#define NALU_TYPE_SPS      7
#define NALU_TYPE_PPS      8
#define NALU_TYPE_AUD      9
#define NALU_TYPE_EOSEQ    10
#define NALU_TYPE_EOSTREAM 11
#define NALU_TYPE_FILL     12

#define NALU_PRIORITY_HIGHEST     3
#define NALU_PRIORITY_HIGH        2
#define NALU_PRIRITY_LOW          1
#define NALU_PRIORITY_DISPOSABLE  0
#define ZEROBYTES_SHORTSTARTCODE 2




int GetAnnexbNALU (NALU_t *nalu, FILE *bits);
int NALUtoRBSP (NALU_t *nalu);
int EBSPtoRBSP(unsigned char *streamBuffer, int end_bytepos, int begin_bytepos);
void RBSPtoSODB(NALU_t *nalu, int len);
unsigned char more_rbsp_data(NALU_t *nalu);


#endif
