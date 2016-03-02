#include <string.h>

#include "global.h"
#include "nalu.h"

#define HW

#include "xdecode_main.h"
#include "fpga_comm.h"

//Addresses of memory ports, must match
//the address values specified in Vivado
#define BASE_ADDRESS_11 0x11000000
#define BASE_ADDRESS_12 0x12000000
#define BASE_ADDRESS_13 0x13000000
#define BASE_ADDRESS_14 0x14000000
#define BASE_ADDRESS_15 0x15000000
#define BASE_ADDRESS_16 0x16000000
#define BASE_ADDRESS_17 0x17000000
#define BASE_ADDRESS_18 0x18000000
#define BASE_ADDRESS_19 0x19000000
#define BASE_ADDRESS_1a 0x1a000000
#define BASE_ADDRESS_1b 0x1b000000
#define BASE_ADDRESS_1c 0x1c000000
#define BASE_ADDRESS_1d 0x1d000000

int pingpong_read_index;
int pingpong_fetch_index;

#if _N_HLS_
extern FILE *prediction_test;
extern FILE* construction_test;
extern FILE *trace_bit;
extern FILE* debug_test;
#endif

FILE *bitstr;
FILE *p_out, *p_out_yuv;
NALU_t PINGPONGbuffer;

#include <sys/time.h>

struct timeval  tv1, tv2;
void runTest(int argc, char** argv);


void write_out_pic(StorablePicture *pic,FILE * p_out);
void write_out_pic1(unsigned char *sluma, unsigned char *chroma_0, unsigned char *chroma_1, int ref, FILE * p_out, FILE * p_out_yuv, int);

int main(int argc, char **argv)
{
  int mem_dev = open("/dev/mem", O_RDWR | O_SYNC);

  if(mem_dev == -1)
  {
    // Error
    printf("Error when opening /dev/mem\n");
    return 1;
  }

  //Decoder IP in the FPGA
  XDecode_main xcore;

  int Status;

  Status = XDecode_main_Initialize(&xcore, "decode_main");
  if (Status != XST_SUCCESS) {
    printf("Initialization failed %d\r\n", Status);
    return 1;
  }

  char AnnexbFileName[100];
  if(argc > 2)
  {
    puts("Too much argument!");
    exit(-1);
  }


  if(argc == 2)
  {
    strcpy(AnnexbFileName, argv[1]);
  }
  else
  {
    strcpy(AnnexbFileName,"test.264");
  }

  bitstr=fopen(AnnexbFileName,"rb");
  if(bitstr==NULL)
  {
    puts("cannot find the corresponding file.");
    exit(-1);
  }

#if _N_HLS_
  prediction_test=fopen("pred_test.txt","w");
  construction_test=fopen("construc_test.txt","w");
  trace_bit=fopen("trace.txt","w");
  debug_test=fopen("debug_test.txt","w");
#endif
  p_out_yuv=fopen("testresult.yuv","wb");
  p_out=fopen("testresult.txt","w");

  PINGPONGbuffer.nal_unit_type=0;

  //Allocate memory, need to do physical_to_virtual address translation
  //to allocate a specified amount of memory corresponding to a given physical address
  //Make sure the address values here match the assigned address values in Vivado
  unsigned char *nalu_buffer = (unsigned char*)mem_alloc(MAXNALBUFFERSIZE * sizeof(unsigned char), BASE_ADDRESS_11, mem_dev);
  int *pic_Picorder_num = (int*)mem_alloc(MAX_REFERENCE_PICTURES * sizeof(int), BASE_ADDRESS_12, mem_dev);
  unsigned char *pic_memoccupied = (unsigned char*)mem_alloc(MAX_REFERENCE_PICTURES * sizeof(unsigned char), BASE_ADDRESS_13, mem_dev);
  unsigned char *pic_framenum = (unsigned char*)mem_alloc(MAX_REFERENCE_PICTURES * sizeof(unsigned char), BASE_ADDRESS_14, mem_dev);
  unsigned char *pic_type = (unsigned char*)mem_alloc(MAX_REFERENCE_PICTURES * sizeof(unsigned char), BASE_ADDRESS_15, mem_dev);
  unsigned char *pic_use_for_char = (unsigned char*)mem_alloc(MAX_REFERENCE_PICTURES * sizeof(unsigned char), BASE_ADDRESS_16, mem_dev);
  unsigned char *pic_Sluma = (unsigned char*)mem_alloc(PicWidthInSamplesL * FrameHeightInSampleL * MAX_REFERENCE_PICTURES * sizeof(unsigned char), BASE_ADDRESS_17, mem_dev);
  unsigned char *pic_SChroma_0 = (unsigned char*)mem_alloc(PicWidthInSamplesC * FrameHeightInSampleC * MAX_REFERENCE_PICTURES * sizeof(unsigned char), BASE_ADDRESS_18, mem_dev);
  unsigned char *pic_SChroma_1 = (unsigned char*)mem_alloc(PicWidthInSamplesC * FrameHeightInSampleC * MAX_REFERENCE_PICTURES * sizeof(unsigned char), BASE_ADDRESS_1d, mem_dev);

  unsigned char *pic_info_refIdxL0 = (unsigned char*)mem_alloc(PicWidthInMBs * 2 * FrameHeightInMbs * 2 * MAX_REFERENCE_PICTURES * sizeof(unsigned char), BASE_ADDRESS_19, mem_dev);
  unsigned char *pic_info_refIdxL1 = (unsigned char*)mem_alloc(PicWidthInMBs * 2 * FrameHeightInMbs * 2 * MAX_REFERENCE_PICTURES * sizeof(unsigned char), BASE_ADDRESS_1a, mem_dev);
  int *pic_info_mvd_l0 = (int*)mem_alloc(PicWidthInMBs * 4 * FrameHeightInMbs * 4 * 2 * MAX_REFERENCE_PICTURES * sizeof(int), BASE_ADDRESS_1b, mem_dev);
  int *pic_info_mvd_l1 = (int*)mem_alloc(PicWidthInMBs * 4 * FrameHeightInMbs * 4 * 2 * MAX_REFERENCE_PICTURES * sizeof(int), BASE_ADDRESS_1c, mem_dev);

  //Init all data to 0
  memset(pic_type, 0, MAX_REFERENCE_PICTURES * sizeof(unsigned char));
  memset(pic_framenum, 0, MAX_REFERENCE_PICTURES * sizeof(unsigned char));
  memset(pic_memoccupied, 0, MAX_REFERENCE_PICTURES * sizeof(unsigned char));
  memset(pic_Picorder_num, 0, MAX_REFERENCE_PICTURES * sizeof(int));
  memset(pic_use_for_char, 0, MAX_REFERENCE_PICTURES * sizeof(unsigned char));
  memset(pic_Sluma, 0, PicWidthInSamplesL * FrameHeightInSampleL *MAX_REFERENCE_PICTURES * sizeof(unsigned char));
  memset(pic_SChroma_0, 0, 1 * PicWidthInSamplesC * FrameHeightInSampleC * MAX_REFERENCE_PICTURES * sizeof(unsigned char));
  memset(pic_SChroma_1, 0, 1 * PicWidthInSamplesC * FrameHeightInSampleC * MAX_REFERENCE_PICTURES * sizeof(unsigned char));

  memset(pic_info_refIdxL0, 0, PicWidthInMBs * 2 * FrameHeightInMbs * 2 * MAX_REFERENCE_PICTURES * sizeof(unsigned char));
  memset(pic_info_refIdxL1, 0, PicWidthInMBs * 2 * FrameHeightInMbs * 2 * MAX_REFERENCE_PICTURES * sizeof(unsigned char));
  memset(pic_info_mvd_l0, 0, PicWidthInMBs * 4 * FrameHeightInMbs * 4 * 2 * MAX_REFERENCE_PICTURES * sizeof(int));
  memset(pic_info_mvd_l1, 0, PicWidthInMBs * 4 * FrameHeightInMbs * 4 * 2 * MAX_REFERENCE_PICTURES * sizeof(int));

  int i=0;
  int j;
  int poc=0;

  while(1)
  {
    if(GetAnnexbNALU (&PINGPONGbuffer,bitstr)==0)
      break;

    PINGPONGbuffer.len = EBSPtoRBSP (PINGPONGbuffer.buf, PINGPONGbuffer.len, 1);
    RBSPtoSODB(&PINGPONGbuffer,PINGPONGbuffer.len-1);

    //decode_main(&PINGPONGbuffer, curPic_inst);

    memcpy(nalu_buffer, PINGPONGbuffer.buf, MAXNALBUFFERSIZE * sizeof(unsigned char));

    //Set static values of various nalu struct members
    XDecode_main_SetNalu_startcodeprefix_len(&xcore, PINGPONGbuffer.startcodeprefix_len);
    XDecode_main_SetNalu_len(&xcore, PINGPONGbuffer.len);
    XDecode_main_SetNalu_nal_unit_type(&xcore, PINGPONGbuffer.nal_unit_type);

    XDecode_main_SetNalu_nal_reference_idc(&xcore, PINGPONGbuffer.nal_reference_idc);
    XDecode_main_SetNalu_forbidden_bit(&xcore, PINGPONGbuffer.forbidden_bit);
    XDecode_main_SetNalu_bit_offset(&xcore, PINGPONGbuffer.bit_offset);
    XDecode_main_SetNalu_bit_length(&xcore, PINGPONGbuffer.bit_length);

    gettimeofday(&tv1, NULL);

    //Start the execution of the decoder IP in the FPGA
    XDecode_main_Start(&xcore);
    //Wait until it finishes
    while (!XDecode_main_IsDone(&xcore));

    gettimeofday(&tv2, NULL);

    printf ("Total time = %lf seconds\n",
        (double) (tv2.tv_usec - tv1.tv_usec) / 1000000 +
        (double) (tv2.tv_sec - tv1.tv_sec));

    //Write back the result to .txt file and .yuv file
    if(PINGPONGbuffer.nal_unit_type==5 || PINGPONGbuffer.nal_unit_type==1 )
      for(j=0;j<MAX_REFERENCE_PICTURES;j++)
        for(i=0;i<MAX_REFERENCE_PICTURES;i++)
        {
          if (pic_memoccupied[i] && pic_Picorder_num[i] == poc)
          {
            fprintf(p_out, "FRAME: %d\n", poc);
            write_out_pic1(pic_Sluma , pic_SChroma_0, pic_SChroma_1, i, p_out, p_out_yuv, poc);
            printf("writing %d\n",poc );
            poc++;
          }
        }
  }


#if _N_HLS_
  fclose(prediction_test);
  fclose(construction_test);
  fclose(trace_bit);
  fclose(debug_test);
#endif
  fclose(bitstr);
  fclose(p_out);
  fclose(p_out_yuv);

  //Free allocated memory
  mem_free((void*)nalu_buffer, MAXNALBUFFERSIZE * sizeof(unsigned char), BASE_ADDRESS_11);
  mem_free((void*)pic_Picorder_num, MAX_REFERENCE_PICTURES * sizeof(int), BASE_ADDRESS_12);
  mem_free((void*)pic_memoccupied, MAX_REFERENCE_PICTURES * sizeof(unsigned char), BASE_ADDRESS_13);
  mem_free((void*)pic_framenum, MAX_REFERENCE_PICTURES * sizeof(unsigned char), BASE_ADDRESS_14);
  mem_free((void*)pic_type, MAX_REFERENCE_PICTURES * sizeof(unsigned char), BASE_ADDRESS_15);
  mem_free((void*)pic_use_for_char, MAX_REFERENCE_PICTURES * sizeof(unsigned char), BASE_ADDRESS_16);
  mem_free((void*)pic_Sluma, PicWidthInSamplesL * FrameHeightInSampleL * MAX_REFERENCE_PICTURES * sizeof(unsigned char), BASE_ADDRESS_17);
  mem_free((void*)pic_SChroma_0, 1 * PicWidthInSamplesC * FrameHeightInSampleC * MAX_REFERENCE_PICTURES * sizeof(unsigned char), BASE_ADDRESS_18);
  mem_free((void*)pic_SChroma_1, 1 * PicWidthInSamplesC * FrameHeightInSampleC * MAX_REFERENCE_PICTURES * sizeof(unsigned char), BASE_ADDRESS_1d);

  mem_free((void*)pic_info_refIdxL0, PicWidthInMBs * 2 * FrameHeightInMbs * 2 * MAX_REFERENCE_PICTURES * sizeof(unsigned char), BASE_ADDRESS_19);
  mem_free((void*)pic_info_refIdxL1, PicWidthInMBs * 2 * FrameHeightInMbs * 2 * MAX_REFERENCE_PICTURES * sizeof(unsigned char), BASE_ADDRESS_1a);
  mem_free((void*)pic_info_mvd_l0, PicWidthInMBs * 4 * FrameHeightInMbs * 4 * 2 * MAX_REFERENCE_PICTURES * sizeof(int), BASE_ADDRESS_1b);
  mem_free((void*)pic_info_mvd_l1, PicWidthInMBs * 4 * FrameHeightInMbs * 4 * 2 * MAX_REFERENCE_PICTURES * sizeof(int), BASE_ADDRESS_1c);
  close(mem_dev);
  return 0;
}

void write_out_pic1(unsigned char *Sluma, unsigned char *SChroma_0, unsigned char *SChroma_1, int ref, FILE * p_out, FILE * p_out_yuv, int frame_no)
{
  int i,j;

  //TXT write
  fprintf(p_out, "LUMA\n");
  for(j=0; j<PicWidthInSamplesL; j++)
  for(i=0; i<FrameHeightInSampleL; i++)
  {
    fprintf(p_out, "frame %d, luma index %d: %d\n", frame_no, j * FrameHeightInSampleL + i, Sluma[ref * PicWidthInSamplesL * FrameHeightInSampleL + j * FrameHeightInSampleL + i]);

  }

  fprintf(p_out, "CHROMA0\n");
  for(j=0; j<PicWidthInSamplesC; j++)
  for(i=0; i<FrameHeightInSampleC; i++)
  {
    fprintf(p_out, "frame %d, chroma0 index %d: %d\n", frame_no, j * FrameHeightInSampleC + i, SChroma_0[ref * 1 * PicWidthInSamplesC * FrameHeightInSampleC + 0 * PicWidthInSamplesC * FrameHeightInSampleC + j * FrameHeightInSampleC + i]);

  }

  fprintf(p_out, "CHROMA1\n");
  for(j=0; j <PicWidthInSamplesC; j++)
  for(i=0; i<FrameHeightInSampleC; i++)
  {
    fprintf(p_out, "frame %d, chroma1 index %d: %d\n", frame_no, j * FrameHeightInSampleC + i, SChroma_1[ref * 1 * PicWidthInSamplesC * FrameHeightInSampleC + 0 * PicWidthInSamplesC * FrameHeightInSampleC + j * FrameHeightInSampleC + i]);
  }

  //YUV write
  for(i=0; i<FrameHeightInSampleL; i++)
  for(j=0; j<PicWidthInSamplesL; j++)
  {
    fputc(Sluma[ref * PicWidthInSamplesL * FrameHeightInSampleL + j * FrameHeightInSampleL + i],p_out_yuv);
  }

  for(i=0; i<FrameHeightInSampleC; i++)
  for(j=0; j<PicWidthInSamplesC; j++)
  {
    fputc(SChroma_0[ref * 1 * PicWidthInSamplesC * FrameHeightInSampleC + 0 * PicWidthInSamplesC * FrameHeightInSampleC + j * FrameHeightInSampleC + i],p_out_yuv);
  }

  for(i=0; i<FrameHeightInSampleC; i++)
  for(j=0; j <PicWidthInSamplesC; j++)
  {
    fputc(SChroma_1[ref * 1 * PicWidthInSamplesC * FrameHeightInSampleC + 0 * PicWidthInSamplesC * FrameHeightInSampleC + j * FrameHeightInSampleC + i],p_out_yuv);
  }
}


