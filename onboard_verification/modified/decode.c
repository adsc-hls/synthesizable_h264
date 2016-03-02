#include "global.h"
#include "parset.h"
#include "decode.h"
#include "slice.h"
#include <string.h>

void decode_main(NALU_t* nalu, StorablePicture pic[MAX_REFERENCE_PICTURES], StorablePictureInfo pic_info[MAX_REFERENCE_PICTURES])
{
#pragma HLS RESOURCE core=AXI4LiteS variable=return

#pragma HLS INTERFACE ap_none register port=nalu->startcodeprefix_len
#pragma HLS RESOURCE core=AXI4LiteS variable=nalu->startcodeprefix_len
#pragma HLS INTERFACE ap_none register port=nalu->len
#pragma HLS RESOURCE core=AXI4LiteS variable=nalu->len
#pragma HLS INTERFACE ap_none register port=nalu->nal_unit_type
#pragma HLS RESOURCE core=AXI4LiteS variable=nalu->nal_unit_type
#pragma HLS INTERFACE ap_none register port=nalu->nal_reference_idc
#pragma HLS RESOURCE core=AXI4LiteS variable=nalu->nal_reference_idc
#pragma HLS INTERFACE ap_none register port=nalu->forbidden_bit
#pragma HLS RESOURCE core=AXI4LiteS variable=nalu->forbidden_bit
#pragma HLS INTERFACE ap_none register port=nalu->bit_offset
#pragma HLS RESOURCE core=AXI4LiteS variable=nalu->bit_offset
#pragma HLS INTERFACE ap_none register port=nalu->bit_length
#pragma HLS RESOURCE core=AXI4LiteS variable=nalu->bit_length

#pragma HLS INTERFACE ap_bus port=nalu->buf
#pragma HLS RESOURCE core=AXI4M variable=nalu->buf

#pragma HLS INTERFACE ap_bus port=pic
#pragma HLS RESOURCE core=AXI4M variable=pic
#pragma HLS INTERFACE ap_bus port=pic_info
#pragma HLS RESOURCE core=AXI4M variable=pic_info

  extern seq_parameter_set_rbsp_t SPS_GLOBAL;
  extern pic_parameter_set_rbsp_t PPS_GLOBAL;
  extern ImageParameters img_inst;
  extern slice_header_rbsp_t sliceHeader_inst;
  extern char intra_pred_mode[PicWidthInMBs*4][FrameHeightInMbs*4];
  extern unsigned char nz_coeff_luma[PicWidthInMBs*4][FrameHeightInMbs*4];
  extern unsigned char nz_coeff_chroma[2][PicWidthInMBs*2][FrameHeightInMbs*2];
  extern unsigned char Mb_prediction_type[PicWidthInMBs][FrameHeightInMbs];

  NALU_t nalu1;
  nalu1.startcodeprefix_len = nalu->startcodeprefix_len;
  nalu1.len = nalu->len;
  nalu1.nal_unit_type = nalu->nal_unit_type;
  nalu1.nal_reference_idc = nalu->nal_reference_idc;
  nalu1.forbidden_bit = nalu->forbidden_bit;
  nalu1.bit_offset = nalu->bit_offset;
  nalu1.bit_length = nalu->bit_length;

  memcpy(nalu1.buf, nalu->buf, MAXNALBUFFERSIZE * sizeof(unsigned char));

#if _N_HLS_
  fprintf(trace_bit,"reading NAL unit:length %d\t refidc %d\t type %d\n",nalu->len, nalu->nal_reference_idc,nalu->nal_unit_type);
#endif // _N_HLS_

  switch(nalu1.nal_unit_type)
  {
    case 7:
      ProcessSPS(&SPS_GLOBAL, &nalu1);
#if _N_HLS_
      fprintf(trace_bit,"\n");
#endif // _N_HLS_
      break;
    case 8:
      ProcessPPS(&PPS_GLOBAL, &nalu1);
#if _N_HLS_
      fprintf(trace_bit,"\n");
#endif // _N_HLS_
      break;
    case 5:
    case 1:
      ProcessSH(&SPS_GLOBAL,&PPS_GLOBAL,&sliceHeader_inst,&img_inst,&nalu1);
      intepretSLiceHeader(&sliceHeader_inst,&SPS_GLOBAL,&PPS_GLOBAL,nalu1.nal_unit_type,nalu1.nal_reference_idc,&img_inst,pic);

      ProcessSlice
        (
         &nalu1,
         pic,
         pic_info,
         Mb_prediction_type,
         intra_pred_mode,
         nz_coeff_luma,
         nz_coeff_chroma,
         PPS_GLOBAL.constrained_intra_pred_flag,
         &sliceHeader_inst,
         &img_inst
        );
#if _N_HLS_
      fprintf(trace_bit,"\n");
#endif // _N_HLS_
      break;
    default:
      break;
  }
}
