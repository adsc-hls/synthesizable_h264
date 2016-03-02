#ifndef _GLOBAL_H_
#define _GLOBAL_H_

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
//1. Number Macros

//test output control
#define _N_HLS_ 	0

//buffer sizes
#define MAXNALBUFFERSIZE 200000
#define MAXSPSSIZE 5
#define MAXPPSSIZE 32
#define MAX_REFERENCE_PICTURES 3

//slice type label
#define SLICE_TYPE_P 0
#define SLICE_TYPE_B 1
#define SLICE_TYPE_I 2

//NALU_type defines
#define NALU_TYPE_SLICE    1
#define NALU_TYPE_IDR      5
#define NALU_TYPE_SPS      7
#define NALU_TYPE_PPS      8

//nalu_ref_idc
#define NALU_PRIORITY_HIGHEST     3
#define NALU_PRIORITY_HIGH        2
#define NALU_PRIRITY_LOW          1
#define NALU_PRIORITY_DISPOSABLE  0

//Imode
#define INTRA4x4    0
#define INTRA16x16  1
#define INTER4x4    2
#define INTERSKIP   3
#define IPCM16x16   25


//Macro functions
#define MAX(a,b) ( (a )> (b)? (a):(b))
#define MIN(a,b) ( (a )< (b)? (a):(b))
#define ABSS(a)    ( (a) >0 ? (a): (-a) )
#define KTOX(a)    ((((a)&0x4) !=0)*2+(((a)&0x1) !=0)) // transfer block ID into its x position offset
#define KTOY(a)    ((((a)&0x8) !=0)*2+(((a)&0x2) !=0))  // transfer block ID into its y position offset

#define XYTOK(x,y)    (  (((y)&0x2)!=0)*8  +  (((x)&0x2)!=0)*4  +  (((y)&0x1)!=0)*2  +  (((x)&0x1)!=0)  )// transfer block ID into its x position offset


//size macros


#define SubWidth_C  2
#define SubHeight_C 2


#define MbWidth_C   8
#define MbHeight_C  8

#define PicWidthInMBs 40
#define PicWidthInSamplesL (PicWidthInMBs*16)
#define PicWidthInSamplesC	(PicWidthInMBs*MbWidth_C)

#define FrameHeightInMbs 30
#define FrameHeightInSampleL	(FrameHeightInMbs*16)
#define FrameHeightInSampleC	(FrameHeightInMbs*MbHeight_C)

#define BitDepthY 	8
#define QpBdOffsetY 0
#define BitDepthC	8
#define QpBdOffsetC	0

#define MAXnum_ref_frames_in_pic_order_cnt_cycle  256
#define MAXnum_slice_groups_minus1  8

#define BLOCK_SIZE 4




//2.structure definition
typedef unsigned char Boolean;
typedef unsigned char byte;
//structure of NAL unit

typedef struct
{
    int startcodeprefix_len;      //! 4 for parameter sets and first slice in picture, 3 for everything else (suggested)
    unsigned len;                 //! Length of the NAL unit (Excluding the start code, which does not belong to the NALU)
    int nal_unit_type;            //! NALU_TYPE_xxxx
    int nal_reference_idc;        //! NALU_PRIORITY_xxxx
    int forbidden_bit;            //! should be always FALSE
    unsigned long int bit_offset;
    unsigned long int bit_length;
    unsigned char buf[MAXNALBUFFERSIZE];        //! contains the first byte followed by the EBSP
} NALU_t;

//Structure for SPS
typedef struct
{
    Boolean   Valid;                  // indicates the parameter set is valid
    unsigned char profile_idc;                                      // u(8)
    Boolean   constrained_set0_flag;                            // u(1)
    Boolean   constrained_set1_flag;                            // u(1)
    Boolean   constrained_set2_flag;                            // u(1)
    unsigned char level_idc;                                        // u(8)
    unsigned  seq_parameter_set_id;                             // ue(v)
    unsigned  log2_max_frame_num_minus4;                        // ue(v)
    unsigned pic_order_cnt_type;
    // if( pic_order_cnt_type == 0 )
    unsigned log2_max_pic_order_cnt_lsb_minus4;                 // ue(v)
    // else if( pic_order_cnt_type == 1 )
    Boolean delta_pic_order_always_zero_flag;               // u(1)
    int     offset_for_non_ref_pic;                         // se(v)
    int     offset_for_top_to_bottom_field;                 // se(v)
    unsigned  num_ref_frames_in_pic_order_cnt_cycle;          // ue(v)
    // for( i = 0; i < num_ref_frames_in_pic_order_cnt_cycle; i++ )
    int   offset_for_ref_frame[MAXnum_ref_frames_in_pic_order_cnt_cycle];   // se(v)
    unsigned  max_num_ref_frames;                                   // ue(v)
    Boolean   gaps_in_frame_num_value_allowed_flag;             // u(1)
    unsigned  pic_width_in_mbs_minus1;                          // ue(v)
    unsigned  pic_height_in_map_units_minus1;                   // ue(v)
    Boolean   frame_mbs_only_flag;                              // u(1)
    // if( !frame_mbs_only_flag )
    Boolean   mb_adaptive_frame_field_flag;                   // u(1)
    Boolean   direct_8x8_inference_flag;                        // u(1)
    Boolean   frame_cropping_flag;                              // u(1)
    unsigned  frame_cropping_rect_left_offset;                // ue(v)
    unsigned  frame_cropping_rect_right_offset;               // ue(v)
    unsigned  frame_cropping_rect_top_offset;                 // ue(v)
    unsigned  frame_cropping_rect_bottom_offset;              // ue(v)
    Boolean   vui_parameters_present_flag;                      // u(1)
    //vui_seq_parameters_t vui_seq_parameters;                  // vui_seq_parameters_t
} seq_parameter_set_rbsp_t;

// structure for PPS
typedef struct
{
  Boolean   Valid;                  // indicates the parameter set is valid
  unsigned  pic_parameter_set_id;                             // ue(v)
  unsigned  seq_parameter_set_id;                             // ue(v)
  Boolean   entropy_coding_mode_flag;                         // u(1)
  // if( pic_order_cnt_type < 2 )  in the sequence parameter set
  Boolean      bottom_field_pic_order_in_frame_present_flag;                           // u(1)
  unsigned  num_slice_groups_minus1;
  unsigned  num_ref_idx_l0_active_minus1;                     // ue(v)
  unsigned  num_ref_idx_l1_active_minus1;                     // ue(v)
  Boolean   weighted_pred_flag;                               // u(1)
  Boolean   weighted_bipred_idc;                              // u(2)
  int       pic_init_qp_minus26;                              // se(v)
  int       pic_init_qs_minus26;                              // se(v)
  int       chroma_qp_index_offset;                           // se(v)
  Boolean   deblocking_filter_control_present_flag;           // u(1)
  Boolean   constrained_intra_pred_flag;                      // u(1)
  Boolean   redundant_pic_cnt_present_flag;                   // u(1)
} pic_parameter_set_rbsp_t;


// structure for slice header
typedef struct
{
  unsigned char no_output_of_prior_pics_flag;
  unsigned char long_term_reference_flag;
  unsigned char adaptive_ref_pic_marking_mode_flag;
  unsigned char Memory_control_operation[10];
  int operation_value[10];
}dec_ref_pic_marking_rbsp_t;


typedef struct
{
  unsigned int luma_log2_weight_denom;
  unsigned int chroma_log2_weight_denom;
  unsigned char luma_weight_l0_flag[MAX_REFERENCE_PICTURES];
  unsigned char luma_weight_l0[MAX_REFERENCE_PICTURES];
  unsigned char luma_offset_l0[MAX_REFERENCE_PICTURES];
  unsigned char chroma_weight_l0_flag[MAX_REFERENCE_PICTURES];
  unsigned char chroma_weight_l0[MAX_REFERENCE_PICTURES][2];
  unsigned char chroma_offset_l0[MAX_REFERENCE_PICTURES][2];
  unsigned char luma_weight_l1_flag[MAX_REFERENCE_PICTURES];
  unsigned char luma_weight_l1[MAX_REFERENCE_PICTURES];
  unsigned char luma_offset_l1[MAX_REFERENCE_PICTURES];
  unsigned char chroma_weight_l1_flag[MAX_REFERENCE_PICTURES];
  unsigned char chroma_weight_l1[MAX_REFERENCE_PICTURES][2];
  unsigned char chroma_offset_l1[MAX_REFERENCE_PICTURES][2];
}pred_weight_table_rbsp_t;



typedef struct
{
  unsigned int  first_mb_in_slice;
  unsigned char  slice_type;
  //unsigned char colour_plane_id;
  unsigned char pic_parameter_set_id;
  unsigned int  frame_num;
  unsigned char field_pic_flag;
  unsigned char bottom_field_flag;
  unsigned int  idr_pic_id;
  int  pic_order_cnt_lsb;
  int delta_pic_order_cnt_bottom;
  int delta_pic_order_cnt[2];
  unsigned int  redundant_pic_cnt;
  unsigned char direct_spatial_mv_pred_flag;
  unsigned char num_ref_idx_active_override_flag;
  unsigned char num_ref_idx_l0_active_minus1;
  unsigned char num_ref_idx_l1_active_minus1;


  unsigned char ref_pic_list_modification_flag_l0; // this one will also count as while loop number
  unsigned char ref_pic_list_modification_flag_l1;// this one will also count as while loop number
  unsigned char   modification_of_pic_nums_idc_l0[5];
  int modification_value_l0[5];
  unsigned char   modification_of_pic_nums_idc_l1[5];
  int modification_value_l1[5];

  pred_weight_table_rbsp_t pred_weight_table;

  dec_ref_pic_marking_rbsp_t  dec_ref_pic_marking;// may be need to be processed as function
  unsigned char cabac_init_idc;
  int   slice_qp_delta;
  unsigned char disable_deblocking_filter_idc;
  int slice_alpha_c0_offset_div2;
  int slice_beta_offset_div2;
}slice_header_rbsp_t;

// structure for image parameter
typedef struct
{
  unsigned int IdrPicFlag;
  unsigned int MaxFrameNum;
  int MaxPicOrderCntLsb;
  unsigned int ExpectedDeltaPerPicOrderCntCycle;
  unsigned char MbaffFrameFlag;
  unsigned char structure;

  int prevPicOrderCntLsb;
  int prevPicOrderCntMsb;
  int PictureOrderCount;
  int mem_idx;

  int IDR_idx;

  unsigned char buffer_idx;

  int list0[MAX_REFERENCE_PICTURES];
  int list1[MAX_REFERENCE_PICTURES];
  int ref_mem_idx;

  int sliceQPY;
  int Qsy;
  int chroma_offset;

}ImageParameters;



//structure for macroblock




//Picture
typedef struct
{
    int Picorder_num;
    unsigned char memoccupied;
    unsigned char frame_num;
    unsigned char type;
    unsigned char use_for_char;

    unsigned char Sluma[PicWidthInSamplesL][FrameHeightInSampleL];
    unsigned char SChroma[2][PicWidthInSamplesC][FrameHeightInSampleC];
} StorablePicture;

//Picture info
typedef struct
{
    char refIdxL0[PicWidthInMBs*2][FrameHeightInMbs*2];
    char refIdxL1[PicWidthInMBs*2][FrameHeightInMbs*2];
    int mvd_l0[PicWidthInMBs*4][FrameHeightInMbs*4][2];
    int mvd_l1[PicWidthInMBs*4][FrameHeightInMbs*4][2];
} StorablePictureInfo;




//3. global variables for current frame
char intra_pred_mode[PicWidthInMBs*4][FrameHeightInMbs*4];
unsigned char nz_coeff_luma[PicWidthInMBs*4][FrameHeightInMbs*4];
unsigned char nz_coeff_chroma[2][PicWidthInMBs*2][FrameHeightInMbs*2];
unsigned char Mb_prediction_type[PicWidthInMBs][FrameHeightInMbs];

//4. frame information as decoded frame buffer




const unsigned char QPc[52];
const unsigned char NCBP[48][2];
const unsigned char lentab[3][4][17];
const unsigned char codtab[3][4][17];
const unsigned char SNGL_SCAN[16][2];
const unsigned char FIELD_SCAN[16][2];
const unsigned char decode_block_scan[4][4];

//5. file declaration
#if _N_HLS_
FILE *prediction_test;
FILE* construction_test;
FILE *trace_bit;
FILE* debug_test;
#endif


#endif // _GLOBAL_H_
