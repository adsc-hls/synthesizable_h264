#include "global.h"
#include "framealloc.h"
#include <string.h>
#include "vlc.h"


#if _N_HLS_
extern FILE *trace_bit;
#endif


int ProcessSPS(seq_parameter_set_rbsp_t *SPS, NALU_t* nalu)
{
	unsigned char profile_idc_tmp;
	unsigned char c_set_flgs;
	unsigned char lvl_idc_tmp;
	unsigned int SPS_id;

	int i;

	nalu->bit_offset=8;
	profile_idc_tmp=u_8(nalu);

	c_set_flgs=u_8(nalu);
	lvl_idc_tmp=u_8(nalu);
	SPS_id=u_e(nalu);


	if(SPS_id>MAXSPSSIZE-1)
	{
		#if _N_HLS_
		printf("TOO MUCH SPS");
		#endif
		exit(-1);
	}


	SPS->profile_idc=profile_idc_tmp;
    #if _N_HLS_
    fprintf(trace_bit,"%s %*d\n","SPS->profile_idc",50-strlen("SPS->profile_idc"),SPS->profile_idc);
    fprintf(trace_bit,"%s %*d\n","SPS->c_set_flgs",50-strlen("SPS->c_set_flgs"),c_set_flgs);
    #endif // _N_HLS_
	SPS->constrained_set0_flag=(c_set_flgs>>7)&0x01;
	SPS->constrained_set1_flag=(c_set_flgs>>6)&0x01;
	SPS->constrained_set2_flag=(c_set_flgs>>5)&0x01;
	SPS->level_idc=lvl_idc_tmp;
    #if _N_HLS_
    fprintf(trace_bit,"%s %*d\n","SPS->level_idc",50-strlen("SPS->level_idc"),SPS->level_idc);
    #endif // _N_HLS_
	SPS->seq_parameter_set_id=SPS_id;
    #if _N_HLS_
    fprintf(trace_bit,"%s %*d\n","SPS->seq_parameter_set_id",50-strlen("SPS->seq_parameter_set_id"),SPS->seq_parameter_set_id);
    #endif // _N_HLS_
	SPS->Valid =1;

	SPS->log2_max_frame_num_minus4=u_e(nalu);
    #if _N_HLS_
    fprintf(trace_bit,"%s %*d\n","SPS->log2_max_frame_num_minus4",50-strlen("SPS->log2_max_frame_num_minus4"),SPS->log2_max_frame_num_minus4);
    #endif // _N_HLS_



    // this benchmark assums that picture order count type equals only 0
	SPS->pic_order_cnt_type=u_e(nalu);
    #if _N_HLS_
    fprintf(trace_bit,"%s %*d\n","SPS->pic_order_cnt_type",50-strlen("SPS->pic_order_cnt_type"),SPS->pic_order_cnt_type);
    #endif // _N_HLS_



	if(SPS->pic_order_cnt_type)
	{
		SPS->delta_pic_order_always_zero_flag=u_1(nalu);
    #if _N_HLS_
    fprintf(trace_bit,"%s %*d\n","SPS->delta_pic_order_always_zero_flag",50-strlen("SPS->delta_pic_order_always_zero_flag"),SPS->delta_pic_order_always_zero_flag);
    #endif // _N_HLS_
		SPS->offset_for_non_ref_pic=s_e(nalu);
    #if _N_HLS_
    fprintf(trace_bit,"%s %*d\n","SPS->offset_for_non_ref_pic",50-strlen("SPS->offset_for_non_ref_pic"),SPS->offset_for_non_ref_pic);
    #endif // _N_HLS_
		SPS->offset_for_top_to_bottom_field=s_e(nalu);
    #if _N_HLS_
    fprintf(trace_bit,"%s %*d\n","SPS->offset_for_top_to_bottom_field",50-strlen("SPS->offset_for_top_to_bottom_field"),SPS->offset_for_top_to_bottom_field);
    #endif // _N_HLS_
		SPS->num_ref_frames_in_pic_order_cnt_cycle=u_e(nalu);
    #if _N_HLS_
    fprintf(trace_bit,"%s %*d\n","SPS->num_ref_frames_in_pic_order_cnt_cycle",50-strlen("SPS->num_ref_frames_in_pic_order_cnt_cycle"),SPS->num_ref_frames_in_pic_order_cnt_cycle);
    #endif // _N_HLS_
		for(i=0;i<SPS->num_ref_frames_in_pic_order_cnt_cycle;i++)
        {

			SPS->offset_for_ref_frame[i]=s_e(nalu);
    #if _N_HLS_
    fprintf(trace_bit,"%s %*d\n","SPS->offset_for_ref_frame",50-strlen("SPS->offset_for_ref_frame"),SPS->offset_for_ref_frame[i]);
    #endif // _N_HLS_
        }
	}
	else
	{
		SPS->log2_max_pic_order_cnt_lsb_minus4=u_e(nalu);
    #if _N_HLS_
    fprintf(trace_bit,"%s %*d\n","SPS->log2_max_pic_order_cnt_lsb_minus4",50-strlen("SPS->log2_max_pic_order_cnt_lsb_minus4"),SPS->log2_max_pic_order_cnt_lsb_minus4);
    #endif // _N_HLS_
	}

	SPS->max_num_ref_frames=u_e(nalu);
    #if _N_HLS_
    fprintf(trace_bit,"%s %*d\n","SPS->max_num_ref_frames",50-strlen("SPS->max_num_ref_frames"),SPS->max_num_ref_frames);
    #endif // _N_HLS_
	SPS->gaps_in_frame_num_value_allowed_flag=u_1(nalu);
    #if _N_HLS_
    fprintf(trace_bit,"%s %*d\n","SPS->gaps_in_frame_num_value_allowed_flag",50-strlen("SPS->gaps_in_frame_num_value_allowed_flag"),SPS->gaps_in_frame_num_value_allowed_flag);
    #endif // _N_HLS_
	SPS->pic_width_in_mbs_minus1=u_e(nalu);
    #if _N_HLS_
    fprintf(trace_bit,"%s %*d\n","SPS->pic_width_in_mbs_minus1",50-strlen("SPS->pic_width_in_mbs_minus1"),SPS->pic_width_in_mbs_minus1);
    #endif // _N_HLS_
    SPS->pic_height_in_map_units_minus1=u_e(nalu);
    #if _N_HLS_
    fprintf(trace_bit,"%s %*d\n","SPS->pic_height_in_map_units_minus1",50-strlen("SPS->pic_height_in_map_units_minus1"),SPS->pic_height_in_map_units_minus1);
    #endif // _N_HLS_


    // this benchmark assumes that frame_mbs_only_flags equals 0
    SPS->frame_mbs_only_flag=u_1(nalu);
    #if _N_HLS_
    fprintf(trace_bit,"%s %*d\n","SPS->frame_mbs_only_flag",50-strlen("SPS->frame_mbs_only_flag"),SPS->frame_mbs_only_flag);
    #endif // _N_HLS_
	if(!SPS->frame_mbs_only_flag)
    {

		SPS->mb_adaptive_frame_field_flag=u_1(nalu);
    #if _N_HLS_
    fprintf(trace_bit,"%s %*d\n","SPS->mb_adaptive_frame_field_flag",50-strlen("SPS->mb_adaptive_frame_field_flag"),SPS->mb_adaptive_frame_field_flag);
    #endif // _N_HLS_
    }

    //following element is assumed to be zero
	SPS->direct_8x8_inference_flag=u_1(nalu);
    #if _N_HLS_
    fprintf(trace_bit,"%s %*d\n","SPS->direct_8x8_inference_flag",50-strlen("SPS->direct_8x8_inference_flag"),SPS->direct_8x8_inference_flag);
    #endif // _N_HLS_
    SPS->frame_cropping_flag=u_1(nalu);
    #if _N_HLS_
    fprintf(trace_bit,"%s %*d\n","SPS->frame_cropping_flag",50-strlen("SPS->frame_cropping_flag"),SPS->frame_cropping_flag);
    #endif // _N_HLS_
	if(SPS->frame_cropping_flag)
	{
		SPS->frame_cropping_rect_left_offset=u_e(nalu);
    #if _N_HLS_
    fprintf(trace_bit,"%s %*d\n","SPS->frame_cropping_rect_left_offset",50-strlen("SPS->frame_cropping_rect_left_offset"),SPS->frame_cropping_rect_left_offset);
    #endif // _N_HLS_
		SPS->frame_cropping_rect_right_offset=u_e(nalu);
    #if _N_HLS_
    fprintf(trace_bit,"%s %*d\n","SPS->frame_cropping_rect_right_offset",50-strlen("SPS->frame_cropping_rect_right_offset"),SPS->frame_cropping_rect_right_offset);
    #endif // _N_HLS_
		SPS->frame_cropping_rect_top_offset=u_e(nalu);
    #if _N_HLS_
    fprintf(trace_bit,"%s %*d\n","SPS->frame_cropping_rect_top_offset",50-strlen("SPS->frame_cropping_rect_top_offset"),SPS->frame_cropping_rect_top_offset);
    #endif // _N_HLS_

		SPS->frame_cropping_rect_bottom_offset=u_e(nalu);
    #if _N_HLS_
    fprintf(trace_bit,"%s %*d\n","SPS->frame_cropping_rect_bottom_offset",50-strlen("SPS->frame_cropping_rect_bottom_offset"),SPS->frame_cropping_rect_bottom_offset);
    #endif // _N_HLS_

	}
	SPS->vui_parameters_present_flag=u_1(nalu);
    #if _N_HLS_
    fprintf(trace_bit,"%s %*d\n","SPS->vui_parameters_present_flag",50-strlen("SPS->vui_parameters_present_flag"),SPS->vui_parameters_present_flag);
    #endif // _N_HLS_

	#if _N_HLS_
	if(SPS->vui_parameters_present_flag)
		puts("vui parameter set detected, but not supported");
	#endif



	//Warning: validate is not ready yet;
	//warning: slice loss not ready.
	return 0;

}



int ProcessPPS(pic_parameter_set_rbsp_t *pps, NALU_t* nalu)
{
	int pps_id;
	nalu->bit_offset=8;
	pps_id=u_e(nalu);

	if(pps_id>MAXSPSSIZE)
	{
		#if _N_HLS_
		printf("TOO MUCH PPS");
		#endif
		exit(-1);
	}


	pps->pic_parameter_set_id=pps_id;
    #if _N_HLS_
    fprintf(trace_bit,"%s %*d\n","pps->pic_parameter_set_id",50-strlen("pps->pic_parameter_set_id"),pps->pic_parameter_set_id);
    #endif // _N_HLS_
	pps->Valid=1;

	pps->seq_parameter_set_id=u_e(nalu);
    #if _N_HLS_
    fprintf(trace_bit,"%s %*d\n","pps->seq_parameter_set_id",50-strlen("pps->seq_parameter_set_id"),pps->seq_parameter_set_id);
    #endif // _N_HLS_




	pps->entropy_coding_mode_flag=u_1(nalu);
    #if _N_HLS_
    fprintf(trace_bit,"%s %*d\n","pps->entropy_coding_mode_flag",50-strlen("pps->entropy_coding_mode_flag"),pps->entropy_coding_mode_flag);
    #endif // _N_HLS_
	pps->bottom_field_pic_order_in_frame_present_flag=u_1(nalu);
    #if _N_HLS_
    fprintf(trace_bit,"%s %*d\n","pps->bottom_field_pic_order_in_frame_present_flag",50-strlen("pps->bottom_field_pic_order_in_frame_present_flag"),pps->bottom_field_pic_order_in_frame_present_flag);
    #endif // _N_HLS_
	pps->num_slice_groups_minus1=u_e(nalu);
    #if _N_HLS_
    fprintf(trace_bit,"%s %*d\n","pps->num_slice_groups_minus1",50-strlen("pps->num_slice_groups_minus1"),pps->num_slice_groups_minus1);
    #endif // _N_HLS_


	pps->num_ref_idx_l0_active_minus1=u_e(nalu);
    #if _N_HLS_
    fprintf(trace_bit,"%s %*d\n","pps->num_ref_idx_l0_active_minus1",50-strlen("pps->num_ref_idx_l0_active_minus1"),pps->num_ref_idx_l0_active_minus1);
    #endif // _N_HLS_
	pps->num_ref_idx_l1_active_minus1=u_e(nalu);
	#if _N_HLS_
    fprintf(trace_bit,"%s %*d\n","pps->num_ref_idx_l1_active_minus1",50-strlen("pps->num_ref_idx_l1_active_minus1"),pps->num_ref_idx_l1_active_minus1);
    #endif // _N_HLS_
	pps->weighted_pred_flag=u_1(nalu);
	#if _N_HLS_
    fprintf(trace_bit,"%s %*d\n","pps->weighted_pred_flag",50-strlen("pps->weighted_pred_flag"),pps->weighted_pred_flag);
    #endif // _N_HLS_
	pps->weighted_bipred_idc=u_n(2,nalu);
	#if _N_HLS_
    fprintf(trace_bit,"%s %*d\n","pps->weighted_bipred_idc",50-strlen("pps->weighted_bipred_idc"),pps->weighted_bipred_idc);
    #endif // _N_HLS_
	pps->pic_init_qp_minus26=s_e(nalu);
	#if _N_HLS_
    fprintf(trace_bit,"%s %*d\n","pps->pic_init_qp_minus26",50-strlen("pps->pic_init_qp_minus26"),pps->pic_init_qp_minus26);
    #endif // _N_HLS_
	pps->pic_init_qs_minus26=s_e(nalu);
	#if _N_HLS_
    fprintf(trace_bit,"%s %*d\n","pps->pic_init_qs_minus26",50-strlen("pps->pic_init_qs_minus26"),pps->pic_init_qs_minus26);
    #endif // _N_HLS_

	pps->chroma_qp_index_offset=s_e(nalu);
	#if _N_HLS_
    fprintf(trace_bit,"%s %*d\n","pps->chroma_qp_index_offset",50-strlen("pps->chroma_qp_index_offset"),pps->chroma_qp_index_offset);
    #endif // _N_HLS_
	pps->deblocking_filter_control_present_flag=u_1(nalu);
	#if _N_HLS_
    fprintf(trace_bit,"%s %*d\n","pps->deblocking_filter_control_present_flag",50-strlen("pps->deblocking_filter_control_present_flag"),pps->deblocking_filter_control_present_flag);
    #endif // _N_HLS_
	pps->constrained_intra_pred_flag=u_1(nalu);
	#if _N_HLS_
    fprintf(trace_bit,"%s %*d\n","pps->constrained_intra_pred_flag",50-strlen("pps->constrained_intra_pred_flag"),pps->constrained_intra_pred_flag);
    #endif // _N_HLS_
	pps->redundant_pic_cnt_present_flag=u_1(nalu);
	#if _N_HLS_
    fprintf(trace_bit,"%s %*d\n","pps->redundant_pic_cnt_present_flag",50-strlen("pps->redundant_pic_cnt_present_flag"),pps->redundant_pic_cnt_present_flag);
    #endif // _N_HLS_



	return 0;

}



 void process_pred_weight_table(pred_weight_table_rbsp_t* PWT, char B_slice, pic_parameter_set_rbsp_t* pps, NALU_t *nalu)
 {
 	PWT->luma_log2_weight_denom=u_e(nalu);
 	PWT->chroma_log2_weight_denom=u_e(nalu);
 	int i;
 	for(i=0;i<=pps->num_ref_idx_l0_active_minus1;i++)
 	{
 		PWT->luma_weight_l0_flag[i]=u_1(nalu);
 		if(PWT->luma_weight_l0_flag[i])
 		{
 			PWT->luma_weight_l0[i]=s_e(nalu);
 			PWT->luma_offset_l0[i]=s_e(nalu);
 		}
 		PWT->chroma_weight_l0_flag[i]=u_1(nalu);
 		if(PWT->chroma_weight_l0_flag[i])
 		{
 			PWT->chroma_weight_l0[i][0]=s_e(nalu);
 			PWT->chroma_offset_l0[i][0]=s_e(nalu);
 			PWT->chroma_weight_l0[i][1]=s_e(nalu);
 			PWT->chroma_offset_l0[i][1]=s_e(nalu);
 		}
 	}
 	if(B_slice)
 		return;
 	for(i=0;i<=pps->num_ref_idx_l1_active_minus1;i++)
 	{
 		PWT->luma_weight_l1_flag[i]=u_1(nalu);
 		if(PWT->luma_weight_l1_flag[i])
 		{
 			PWT->luma_weight_l1[i]=s_e(nalu);
 			PWT->luma_offset_l1[i]=s_e(nalu);
 		}
 		PWT->chroma_weight_l1_flag[i]=u_1(nalu);
 		if(PWT->chroma_weight_l1_flag[i])
 		{
 			PWT->chroma_weight_l1[i][0]=s_e(nalu);
 			PWT->chroma_offset_l1[i][0]=s_e(nalu);
 			PWT->chroma_weight_l1[i][1]=s_e(nalu);
 			PWT->chroma_offset_l1[i][1]=s_e(nalu);
 		}
 	}


 	return;
 }



void process_dec_ref_pic_marking(dec_ref_pic_marking_rbsp_t *drpm, ImageParameters *img, NALU_t* nalu /* some more argument may be involved*/ )
{

    int i;
	if(nalu->nal_unit_type==5)
	{
		drpm->no_output_of_prior_pics_flag=u_1(nalu);
    #if _N_HLS_
    fprintf(trace_bit,"%s %*d\n","drpm->no_output_of_prior_pics_flag",50-strlen("drpm->no_output_of_prior_pics_flag"),drpm->no_output_of_prior_pics_flag);
    #endif // _N_HLS_

		drpm->long_term_reference_flag=u_1(nalu);
	}
	else
	{

		drpm->adaptive_ref_pic_marking_mode_flag=u_1(nalu);
    #if _N_HLS_
    fprintf(trace_bit,"%s %*d\n","drpm->adaptive_ref_pic_marking_mode_flag",50-strlen("drpm->adaptive_ref_pic_marking_mode_flag"),drpm->adaptive_ref_pic_marking_mode_flag);
    #endif // _N_HLS_

		if(drpm->adaptive_ref_pic_marking_mode_flag)
        {
            for(i=0;i<10;i++)
            {
                drpm->Memory_control_operation[i]=u_e(nalu);
                if(drpm->Memory_control_operation[i])
                    drpm->operation_value[i]=u_e(nalu);
                else
                {
                    break;
                    drpm->adaptive_ref_pic_marking_mode_flag=i+1;
                }

            }
            drpm->adaptive_ref_pic_marking_mode_flag=i;
        }
		//need to implement when you know the operation of memory operation;
	}
}


// processing the syntax of slice header
void ProcessSH(
	seq_parameter_set_rbsp_t *active_sps,
	pic_parameter_set_rbsp_t *active_pps,
	slice_header_rbsp_t* SH,
	ImageParameters *img,
	NALU_t* nalu)
{
	nalu->bit_offset=8;
	int i;



	SH->first_mb_in_slice=u_e(nalu);
    #if _N_HLS_
    fprintf(trace_bit,"%s %*d\n","SH->first_mb_in_slice",50-strlen("SH->first_mb_in_slice"),SH->first_mb_in_slice);
    #endif // _N_HLS_

	SH->slice_type=u_e(nalu)%5;
    #if _N_HLS_
    fprintf(trace_bit,"%s %*d\n","SH->slice_type",50-strlen("SH->slice_type"),SH->slice_type);
    #endif // _N_HLS_



	SH->pic_parameter_set_id=u_e(nalu);
    #if _N_HLS_
    fprintf(trace_bit,"%s %*d\n","SH->pic_parameter_set_id",50-strlen("SH->pic_parameter_set_id"),SH->pic_parameter_set_id);
    #endif // _N_HLS_
	SH->frame_num=u_n(active_sps->log2_max_frame_num_minus4+4,nalu);
    #if _N_HLS_
    fprintf(trace_bit,"%s %*d\n","SH->frame_num",50-strlen("SH->frame_num"),SH->frame_num);
    #endif // _N_HLS_

	img->IdrPicFlag= (nalu->nal_unit_type==5);

	if(img->IdrPicFlag)
	{
		SH->idr_pic_id=u_e(nalu);
    #if _N_HLS_
    fprintf(trace_bit,"%s %*d\n","SH->idr_pic_id",50-strlen("SH->idr_pic_id"),SH->idr_pic_id);
    #endif // _N_HLS_
	}

//this benchmarks assumes that pic_order_cnt_type equals to 0

	if(active_sps->pic_order_cnt_type==0)
	{
		SH->pic_order_cnt_lsb=u_n(active_sps->log2_max_pic_order_cnt_lsb_minus4+4,nalu);
    #if _N_HLS_
    fprintf(trace_bit,"%s %*d\n","SH->pic_order_cnt_lsb",50-strlen("SH->pic_order_cnt_lsb"),SH->pic_order_cnt_lsb);
    #endif // _N_HLS_
	}

//redundant_pic_cnt_present_flag is inferred to be 0 for main profile
//	if(active_pps->redundant_pic_cnt_present_flag)
//	{
//		SH->redundant_pic_cnt=u_e(nalu);
//	}

    if(SH->slice_type==SLICE_TYPE_B)
    {
        SH->direct_spatial_mv_pred_flag=u_1(nalu);
    #if _N_HLS_
    fprintf(trace_bit,"%s %*d\n","SH->direct_spatial_mv_pred_flag",50-strlen("SH->direct_spatial_mv_pred_flag"),SH->direct_spatial_mv_pred_flag);
    #endif // _N_HLS_
    }

    if(SH->slice_type==SLICE_TYPE_P || SH->slice_type == SLICE_TYPE_B)
    {
	 	SH->num_ref_idx_active_override_flag=u_1(nalu);
    #if _N_HLS_
    fprintf(trace_bit,"%s %*d\n","SH->num_ref_idx_active_override_flag",50-strlen("SH->num_ref_idx_active_override_flag"),SH->num_ref_idx_active_override_flag);
    #endif // _N_HLS_
	 	if(SH->num_ref_idx_active_override_flag)
	 	{
	 		SH->num_ref_idx_l0_active_minus1=u_e(nalu);
    #if _N_HLS_
    fprintf(trace_bit,"%s %*d\n","SH->num_ref_idx_l0_active_minus1",50-strlen("SH->num_ref_idx_l0_active_minus1"),SH->num_ref_idx_l0_active_minus1);
    #endif // _N_HLS_
	 		if(SH->slice_type==SLICE_TYPE_B)
	 		{
	 			SH->num_ref_idx_l1_active_minus1=u_e(nalu);
    #if _N_HLS_
    fprintf(trace_bit,"%s %*d\n","SH->num_ref_idx_l1_active_minus1",50-strlen("SH->num_ref_idx_l1_active_minus1"),SH->num_ref_idx_l1_active_minus1);
    #endif // _N_HLS_
	 		}
	 	}
    }

// this benchmark assumes ref_pic_list_modification_flag_l0 and ref_pic_list_modification_flag_l0 to be 0
// the functionality of reference list reordering is left for further implementation
        if(SH->slice_type!=2)
        {
            SH->ref_pic_list_modification_flag_l0=u_1(nalu);
    #if _N_HLS_
    fprintf(trace_bit,"%s %*d\n","SH->ref_pic_list_modification_flag_l0",50-strlen("SH->ref_pic_list_modification_flag_l0"),SH->ref_pic_list_modification_flag_l0);
    #endif // _N_HLS_

            if(SH->ref_pic_list_modification_flag_l0)
            {
                for(i=0;i<MAX_REFERENCE_PICTURES;i++)
                {
                    SH->modification_of_pic_nums_idc_l0[i]=u_e(nalu);
    #if _N_HLS_
    fprintf(trace_bit,"%s %*d\n","SH->modification_of_pic_nums_idc_l0",50-strlen("SH->modification_of_pic_nums_idc_l0"),SH->modification_of_pic_nums_idc_l0[i]);
    #endif // _N_HLS_
                    if( SH->modification_of_pic_nums_idc_l0[i]!=3)
                    {
                        SH->modification_value_l0[i]=u_e(nalu);
    #if _N_HLS_
    fprintf(trace_bit,"%s %*d\n","SH->modification_value_l0",50-strlen("SH->modification_value_l0"),SH->modification_value_l0[i]);
    #endif // _N_HLS_

                    }
                    else
                    break;
                }
                SH->ref_pic_list_modification_flag_l0=i;
            }
        }

        if(SH->slice_type==1)
        {

            SH->ref_pic_list_modification_flag_l1=u_1(nalu);
    #if _N_HLS_
    fprintf(trace_bit,"%s %*d\n","SH->ref_pic_list_modification_flag_l1",50-strlen("SH->ref_pic_list_modification_flag_l1"),SH->ref_pic_list_modification_flag_l1);
    #endif // _N_HLS_
            if(SH->ref_pic_list_modification_flag_l1)
            {
                for(i=0;i<MAX_REFERENCE_PICTURES;i++)
                {
                    SH->modification_of_pic_nums_idc_l1[i]=u_e(nalu);
     #if _N_HLS_
    fprintf(trace_bit,"%s %*d\n","SH->modification_of_pic_nums_idc_l1",50-strlen("SH->modification_of_pic_nums_idc_l1"),SH->modification_of_pic_nums_idc_l1[i]);
    #endif // _N_HLS_
                   if( SH->modification_of_pic_nums_idc_l1[i]!=3)
                   {
                    SH->modification_value_l1[i]=u_e(nalu);
    #if _N_HLS_
    fprintf(trace_bit,"%s %*d\n","SH->modification_value_l1",50-strlen("SH->modification_value_l1"),SH->modification_value_l1[i]);
    #endif // _N_HLS_

                   }
                    else
                    break;
                }
                SH->ref_pic_list_modification_flag_l1=i;
            }
        }

// this benchmark assumes weighted_pred flags to be 0
// the functionality of weighted_pred is left for further implementation
	 if((active_pps->weighted_pred_flag && (SH->slice_type==SLICE_TYPE_P)) ||
	 	(active_pps->weighted_bipred_idc && (SH->slice_type==SLICE_TYPE_B)) )
	 {
	 	process_pred_weight_table(&(SH->pred_weight_table),  SH->slice_type-SLICE_TYPE_B,active_pps, nalu);
	 }

	if(nalu->nal_reference_idc !=0)
	{
		process_dec_ref_pic_marking(&(SH->dec_ref_pic_marking), img, nalu /* some more argument may be involved*/ );
	}

	// if(active_pps->entropy_coding_mode_flag && SH->slice_type != SLICE_TYPE_I)
	// {
	// 	SH->cabac_init_idc=u_e(nalu);
	// }


	SH->slice_qp_delta=s_e(nalu);
    #if _N_HLS_
    fprintf(trace_bit,"%s %*d\n","SH->slice_qp_delta",50-strlen("SH->slice_qp_delta"),SH->slice_qp_delta);
    #endif // _N_HLS_

// this benchmark assumes disable_deblocking_filter_idc to be 1
// the functionality of deblocking filter is left for further implementation
	if(active_pps->deblocking_filter_control_present_flag)
	{
	 	SH->disable_deblocking_filter_idc=u_e(nalu);
    #if _N_HLS_
    fprintf(trace_bit,"%s %*d\n","SH->disable_deblocking_filter_idc",50-strlen("SH->disable_deblocking_filter_idc"),SH->disable_deblocking_filter_idc);
    #endif // _N_HLS_
	 	if(SH->disable_deblocking_filter_idc!=1)
	 	{
			SH->slice_alpha_c0_offset_div2=s_e(nalu);
    #if _N_HLS_
    fprintf(trace_bit,"%s %*d\n","SH->slice_alpha_c0_offset_div2",50-strlen("SH->slice_alpha_c0_offset_div2"),SH->slice_alpha_c0_offset_div2);
    #endif // _N_HLS_
	 		SH->slice_beta_offset_div2=s_e(nalu);
    #if _N_HLS_
    fprintf(trace_bit,"%s %*d\n","SH->slice_beta_offset_div2",50-strlen("SH->slice_beta_offset_div2"),SH->slice_beta_offset_div2);
    #endif // _N_HLS_
	 	}
	}
}



void intepretSLiceHeader(
	slice_header_rbsp_t *sliceheader,
	seq_parameter_set_rbsp_t *sps,
	pic_parameter_set_rbsp_t *pps,
	unsigned char nalu_idc,
	unsigned char nalu_ref_idc,
	ImageParameters *img,
	StorablePicture PicBuff[MAX_REFERENCE_PICTURES])
{
	int PicOrderCntMsb;

	img->MaxFrameNum=1<<(sps->log2_max_frame_num_minus4+4);
	img->MaxPicOrderCntLsb=1<<(sps->log2_max_pic_order_cnt_lsb_minus4+4);
	img->ExpectedDeltaPerPicOrderCntCycle = 0;
	//for( i = 0; i < sps->num_ref_frames_in_pic_order_cnt_cycle; i++ )
	//img->ExpectedDeltaPerPicOrderCntCycle += sps->offset_for_ref_frame[i];

	img->MbaffFrameFlag=(sps->mb_adaptive_frame_field_flag && (sliceheader->field_pic_flag!=0));

	img->IdrPicFlag=(nalu_idc==5);

    if(nalu_idc==5)
    {
    	img->prevPicOrderCntMsb=0;
    	img->prevPicOrderCntLsb=0;
    }




    if( (sliceheader->pic_order_cnt_lsb < img->prevPicOrderCntLsb) && ((img->prevPicOrderCntLsb - sliceheader->pic_order_cnt_lsb) >= img->MaxPicOrderCntLsb/2) )
    PicOrderCntMsb = img->prevPicOrderCntMsb+img->MaxPicOrderCntLsb;

    else
    if ( (sliceheader->pic_order_cnt_lsb > img->prevPicOrderCntLsb) && ((sliceheader->pic_order_cnt_lsb - img->prevPicOrderCntLsb) > (img->MaxPicOrderCntLsb/2)) )
	{
		PicOrderCntMsb = img->prevPicOrderCntMsb - img->MaxPicOrderCntLsb;

	}
	else
	PicOrderCntMsb=img->prevPicOrderCntMsb;

	img->PictureOrderCount=PicOrderCntMsb+sliceheader->pic_order_cnt_lsb;

	if(nalu_ref_idc!=0)
	{
    img->prevPicOrderCntMsb=PicOrderCntMsb;
    img->prevPicOrderCntLsb=sliceheader->pic_order_cnt_lsb;
	}

	#if _N_HLS_
	printf("nalu type: %d, nal_ref: %d current POC: %d\n",nalu_idc, nalu_ref_idc, img->PictureOrderCount );
	fprintf(trace_bit,"nalu type: %d, nal_ref: %d current POC: %d\n",nalu_idc, nalu_ref_idc, img->PictureOrderCount );

	#endif



	if(nalu_ref_idc!=0)
	{
	img->mem_idx=(img->ref_mem_idx)%2;
	img->ref_mem_idx++;
	}
	else
	img->mem_idx=2;

	if(sliceheader->slice_type == SLICE_TYPE_P)
	{
		init_listP(PicBuff, img->list0,sliceheader->frame_num,img->MaxFrameNum);
	}

	if(sliceheader->slice_type == SLICE_TYPE_B)
	{
		init_listB(PicBuff, img->list0, img->list1, img->PictureOrderCount/2);
	}
#if _N_HLS_
    printf("%d %d %d %d memid %d\n",img->list0[0],img->list0[1],img->list1[0],img->list1[1], img->mem_idx);
#endif
	PicBuff[img->mem_idx].frame_num=sliceheader->frame_num;
	PicBuff[img->mem_idx].Picorder_num=img->PictureOrderCount/2;
	PicBuff[img->mem_idx].memoccupied=1;
	PicBuff[img->mem_idx].type=sliceheader->slice_type;





	if(nalu_ref_idc)
	PicBuff[img->mem_idx].use_for_char=1;
	else
	PicBuff[img->mem_idx].use_for_char=0;




	if(sliceheader->num_ref_idx_active_override_flag == 0)
	{
		sliceheader->num_ref_idx_l0_active_minus1=pps->num_ref_idx_l0_active_minus1;
		sliceheader->num_ref_idx_l1_active_minus1=pps->num_ref_idx_l1_active_minus1;
	}
	img->sliceQPY=26+pps->pic_init_qp_minus26+sliceheader->slice_qp_delta;

		img->chroma_offset=pps->chroma_qp_index_offset;;
	// not yet implementted
}




