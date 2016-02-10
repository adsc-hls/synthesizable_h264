
#ifndef _DECODE_H
#define _DECODE_H
#include "global.h"
void decode_main(NALU_t* nalu, StorablePicture pic[MAX_REFERENCE_PICTURES], StorablePictureInfo pic_info[MAX_REFERENCE_PICTURES]);

seq_parameter_set_rbsp_t SPS_GLOBAL;
pic_parameter_set_rbsp_t PPS_GLOBAL;
ImageParameters img_inst;
slice_header_rbsp_t sliceHeader_inst;

#endif
