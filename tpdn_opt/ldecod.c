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

#include <string.h>

#include "global.h"
#include "nalu.h"
#include "decode.h"



#if _N_HLS_
extern FILE *prediction_test;
extern FILE* construction_test;
extern FILE *trace_bit;
extern FILE* debug_test;
#endif

FILE *bitstr;
FILE *p_out;
StorablePicture Pic[MAX_REFERENCE_PICTURES];
StorablePictureInfo Pic_info[MAX_REFERENCE_PICTURES];


NALU_t PINGPONGbuffer;

void write_out_pic(StorablePicture *pic,FILE * p_out)
{
  int i,j;

  for(i=0;i<FrameHeightInSampleL;i++)
    for(j=0; j<PicWidthInSamplesL; j++)
    {
      fputc(pic->Sluma[j][i],p_out);
    }
  for(i=0;i<FrameHeightInSampleC;i++)
    for(j=0; j<PicWidthInSamplesC; j++)
    {
      fputc(pic->SChroma[0][j][i],p_out);
    }
  for(i=0;i<FrameHeightInSampleC;i++)
    for(j=0 ;j <PicWidthInSamplesC ;  j++)
    {
      fputc(pic->SChroma[1][j][i],p_out);
    }

}


int main(int argc, char **argv)
{
  char AnnexbFileName[100];


  if(argc != 3)
  {
    puts("Too much argument!");
    exit(-1);
  }


  if(argc == 3)
  {
    sprintf(AnnexbFileName, "%s", argv[1]);
  }
  else
    strcpy(AnnexbFileName,"input/test.264");


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


  p_out=fopen("testresult.yuv","wb");

  PINGPONGbuffer.nal_unit_type=0;

  memset(Pic, 0, MAX_REFERENCE_PICTURES*sizeof(StorablePicture));
  memset(Pic_info, 0, MAX_REFERENCE_PICTURES*sizeof(StorablePictureInfo));


  int i=0;
  int j;
  int poc=0;


  while(1)
  {
    if(GetAnnexbNALU (&PINGPONGbuffer,bitstr)==0)
      break;

    PINGPONGbuffer.len = EBSPtoRBSP (PINGPONGbuffer.buf, PINGPONGbuffer.len, 1);
    RBSPtoSODB(&PINGPONGbuffer,PINGPONGbuffer.len-1);


    decode_main(&PINGPONGbuffer,Pic,Pic_info);


    if(PINGPONGbuffer.nal_unit_type==5 || PINGPONGbuffer.nal_unit_type==1 )
      for(j=0;j<MAX_REFERENCE_PICTURES;j++)
        for(i=0;i<MAX_REFERENCE_PICTURES;i++)
        {
          if(Pic[i].memoccupied && Pic[i].Picorder_num==poc)
          {
            write_out_pic(&Pic[i],p_out);
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

  sprintf(AnnexbFileName,"diff -q testresult.yuv %s",argv[2]);

  if (!system(AnnexbFileName))
    printf("PASSED\n");
  else
    printf("FAILED\n");

  return 0;
}
