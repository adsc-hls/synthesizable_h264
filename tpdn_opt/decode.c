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


#include "global.h"
#include "parset.h"
#include "decode.h"
#include "slice.h"



void decode_main(NALU_t* nalu, StorablePicture pic[MAX_REFERENCE_PICTURES], StorablePictureInfo pic_info[MAX_REFERENCE_PICTURES])
{
//#pragma HLS dataflow
	extern seq_parameter_set_rbsp_t SPS_GLOBAL;
	extern pic_parameter_set_rbsp_t PPS_GLOBAL;
	extern ImageParameters img_inst;
	extern slice_header_rbsp_t sliceHeader_inst;
	extern char intra_pred_mode[PicWidthInMBs*4][FrameHeightInMbs*4];
    extern unsigned char nz_coeff_luma[PicWidthInMBs*4][FrameHeightInMbs*4];
    extern unsigned char nz_coeff_chroma[2][PicWidthInMBs*2][FrameHeightInMbs*2];
    extern unsigned char Mb_prediction_type[PicWidthInMBs][FrameHeightInMbs];




    #if _N_HLS_
    fprintf(trace_bit,"reading NAL unit:length %d\t refidc %d\t type %d\n",nalu->len, nalu->nal_reference_idc,nalu->nal_unit_type);
    #endif // _N_HLS_

	switch(nalu->nal_unit_type)
	{
		case 7:
		ProcessSPS(&SPS_GLOBAL, nalu);
    #if _N_HLS_
    fprintf(trace_bit,"\n");
    #endif // _N_HLS_
		break;
		case 8:
        ProcessPPS(&PPS_GLOBAL,nalu);
    #if _N_HLS_
    fprintf(trace_bit,"\n");
    #endif // _N_HLS_
        break;
        case 5:
        case 1:
        ProcessSH(&SPS_GLOBAL,&PPS_GLOBAL,&sliceHeader_inst,&img_inst,nalu);
        intepretSLiceHeader(&sliceHeader_inst,&SPS_GLOBAL,&PPS_GLOBAL,nalu->nal_unit_type,nalu->nal_reference_idc,&img_inst,pic);

        ProcessSlice
        (
            nalu,
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
		#if _N_HLS_
		puts("NOOOOO");
		#endif
		break;
	}
}
