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

/* this is a procedure of bubble sort*/

void init_listP(StorablePicture PicBff[MAX_REFERENCE_PICTURES], int listX[MAX_REFERENCE_PICTURES],int curFrame_NUM, int maxframNUM)
{
  int i,j;
  int temp;
  int tpvalue1,tpvalue2;

  for(i=0;i<MAX_REFERENCE_PICTURES;i++)
  {
    listX[i]=i;
  }

  for(i=0;i<MAX_REFERENCE_PICTURES;i++)
  {
    for(j=i;j<MAX_REFERENCE_PICTURES-1;j++)
    {
      if(PicBff[listX[j]].use_for_char )
      {
        tpvalue1=PicBff[listX[j]].frame_num;
      }
      else
        tpvalue1=-1;


      if(PicBff[listX[j+1]].use_for_char )
      {
        tpvalue2=PicBff[listX[j+1]].frame_num;
      }
      else
        tpvalue2=-1;

      if(tpvalue1<tpvalue2)
      {
        temp=listX[j];
        listX[j]=listX[j+1];
        listX[j+1]=temp;
      }

    }
  }

  for(i=0;i<MAX_REFERENCE_PICTURES;i++)
  {
    if((!PicBff[listX[i]].use_for_char)  )
      listX[i]=-1;
  }
}

void init_listB(StorablePicture PicBff[MAX_REFERENCE_PICTURES],int list0[MAX_REFERENCE_PICTURES], int list1[MAX_REFERENCE_PICTURES], int curPOC)
{
  if(PicBff[0].Picorder_num>PicBff[1].Picorder_num)
  {
    list1[0]=0;
    list1[1]=1;
    list0[0]=1;
    list0[1]=0;
  }
  else
  {
    list0[0]=0;
    list1[0]=1;
    list0[1]=1;
    list1[1]=0;
  }
}
