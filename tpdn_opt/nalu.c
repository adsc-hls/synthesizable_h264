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

#include <stdlib.h>
#include <string.h>


#include "global.h"
#include "nalu.h"

static int FindStartCode (unsigned char *Buf, int zeros_in_startcode);

int IsFirstByteStreamNALU=1;
int LastAccessUnitExists=0;
int NALUCount=0;

int GetAnnexbNALU (NALU_t *nalu,FILE* bits)
{
  int info2, info3, pos = 0;
  int StartCodeFound, rewind;
  unsigned char *Buf;
  int LeadingZero8BitsCount=0, TrailingZero8Bits=0;


  if ((Buf = (unsigned char*)calloc (MAXNALBUFFERSIZE , sizeof(char))) == NULL)
  {
      puts("not enough memory");
    exit(-1);
  }

  while(!feof(bits) && (Buf[pos++]=fgetc(bits))==0);
  if(feof(bits))
  {
    if(pos==0)
    return 0;
    else
    {
      printf( "GetAnnexbNALU can't read start code\n");
      free(Buf);
      return -1;
    }
  }

  if(Buf[pos-1]!=1)
  {
    printf ("GetAnnexbNALU: no Start Code at the begin of the NALU, return -1\n");
    free(Buf);
    return -1;
  }

  if(pos<3)
  {
    printf ("GetAnnexbNALU: no Start Code at the begin of the NALU, return -1\n");
    free(Buf);
    return -1;
  }
  else if(pos==3)
  {
    nalu->startcodeprefix_len = 3;
    LeadingZero8BitsCount = 0;
  }
  else
  {
    LeadingZero8BitsCount = pos-4;
    nalu->startcodeprefix_len = 4;
  }


  //the 1st byte stream NAL unit can has leading_zero_8bits, but subsequent ones are not
  //allowed to contain it since these zeros(if any) are considered trailing_zero_8bits
  //of the previous byte stream NAL unit.
  if(!IsFirstByteStreamNALU && LeadingZero8BitsCount>0)
  {
    printf ("GetAnnexbNALU: The leading_zero_8bits syntax can only be present in the first byte stream NAL unit, return -1\n");
    free(Buf);
    return -1;
  }
  IsFirstByteStreamNALU=0;

  StartCodeFound = 0;
  info2 = 0;
  info3 = 0;

  while (!StartCodeFound)
  {
    if (feof (bits))
    {
      //Count the trailing_zero_8bits
      while(Buf[pos-2-TrailingZero8Bits]==0)
        TrailingZero8Bits++;
      nalu->len = (pos-1)-nalu->startcodeprefix_len-LeadingZero8BitsCount-TrailingZero8Bits;
        if(nalu->len > MAXNALBUFFERSIZE)
        {
        #if _N_HLS_
          printf("lengh of NAL unit is larger than maxmum buffer size!");
        #endif
          exit(-1);
        }
      memcpy (nalu->buf, &Buf[LeadingZero8BitsCount+nalu->startcodeprefix_len], nalu->len);
      nalu->forbidden_bit = (nalu->buf[0]>>7) & 1;
      nalu->nal_reference_idc = (nalu->buf[0]>>5) & 3;
      nalu->nal_unit_type = (nalu->buf[0]) & 0x1f;

// printf ("GetAnnexbNALU, eof case: pos %d nalu->len %d, nalu->reference_idc %d, nal_unit_type %d \n", pos, nalu->len, nalu->nal_reference_idc, nalu->nal_unit_type);

#if TRACE
  fprintf (p_trace, "\n\nLast NALU in File\n\n");
  fprintf (p_trace, "Annex B NALU w/ %s startcode, len %d, forbidden_bit %d, nal_reference_idc %d, nal_unit_type %d\n\n",
    nalu->startcodeprefix_len == 4?"long":"short", nalu->len, nalu->forbidden_bit, nalu->nal_reference_idc, nalu->nal_unit_type);
  fflush (p_trace);
#endif
      free(Buf);
      return pos-1;
    }
    Buf[pos++] = fgetc (bits);
    info3 = FindStartCode(&Buf[pos-4], 3);
    if(info3 != 1)
      info2 = FindStartCode(&Buf[pos-3], 2);
    StartCodeFound = (info2 == 1 || info3 == 1);
  }


  //Count the trailing_zero_8bits
  if(info3==1)	//if the detected start code is 00 00 01, trailing_zero_8bits is sure not to be present
  {
    while(Buf[pos-5-TrailingZero8Bits]==0)
      TrailingZero8Bits++;
  }
  // Here, we have found another start code (and read length of startcode bytes more than we should
  // have.  Hence, go back in the file
  rewind = 0;
  if(info3 == 1)
    rewind = -4;
  else if (info2 == 1)
    rewind = -3;
  else
    printf(" Panic: Error in next start code search \n");

  if (0 != fseek (bits, rewind, SEEK_CUR))
  {
    fprintf (stderr, "GetAnnexbNALU: Cannot fseek %d in the bit stream file", rewind);
    free(Buf);
    exit(-2);
  }

  // Here the leading zeros(if any), Start code, the complete NALU, trailing zeros(if any)
  // and the next start code is in the Buf.
  // The size of Buf is pos, pos+rewind are the number of bytes excluding the next
  // start code, and (pos+rewind)-startcodeprefix_len-LeadingZero8BitsCount-TrailingZero8Bits
  // is the size of the NALU.

  nalu->len = (pos+rewind)-nalu->startcodeprefix_len-LeadingZero8BitsCount-TrailingZero8Bits;
  if(nalu->len > MAXNALBUFFERSIZE)
  {
  #if _N_HLS_
    printf("lengh of NAL unit is larger than maxmum buffer size!");
  #endif
    exit(-1);
  }
  memcpy (nalu->buf, &Buf[LeadingZero8BitsCount+nalu->startcodeprefix_len], nalu->len);
  nalu->forbidden_bit = (nalu->buf[0]>>7) & 1;
  nalu->nal_reference_idc = (nalu->buf[0]>>5) & 3;
  nalu->nal_unit_type = (nalu->buf[0]) & 0x1f;


//printf ("GetAnnexbNALU, regular case: pos %d nalu->len %d, nalu->reference_idc %d, nal_unit_type %d \n", pos, nalu->len, nalu->nal_reference_idc, nalu->nal_unit_type);
#if TRACE
  fprintf (p_trace, "\n\nAnnex B NALU w/ %s startcode, len %d, forbidden_bit %d, nal_reference_idc %d, nal_unit_type %d\n\n",
    nalu->startcodeprefix_len == 4?"long":"short", nalu->len, nalu->forbidden_bit, nalu->nal_reference_idc, nalu->nal_unit_type);
  fflush (p_trace);
#endif

  free(Buf);

  return (pos+rewind);
}



static int FindStartCode (unsigned char *Buf, int zeros_in_startcode)
{
  int info;
  int i;

  info = 1;
  for (i = 0; i < zeros_in_startcode; i++)
    if(Buf[i] != 0)
      info = 0;

  if(Buf[i] != 1)
    info = 0;
  return info;
}


int EBSPtoRBSP(unsigned char *streamBuffer, int end_bytepos, int begin_bytepos)
{
  int i, j, count;
  count = 0;

  if(end_bytepos < begin_bytepos)
    return end_bytepos;

  j = begin_bytepos;

  for(i = begin_bytepos; i < end_bytepos; i++)
  { //starting from begin_bytepos to avoid header information
    if(count == ZEROBYTES_SHORTSTARTCODE && streamBuffer[i] == 0x03)
    {
      i++;
      count = 0;
    }
    streamBuffer[j] = streamBuffer[i];
    if(streamBuffer[i] == 0x00)
      count++;
    else
      count = 0;
    j++;
  }

  return j;
}

void RBSPtoSODB(NALU_t *nalu, int len)
{
  unsigned char i;

  for(i=0;i<8;i++)
  {
    if( nalu->buf[len] & 0x01<<i)
    {
      break;
    }
  }

  nalu->bit_length=len*8+7-i;
}

unsigned char more_rbsp_data(NALU_t *nalu)
{
  return nalu->bit_offset < nalu->bit_length;
}
