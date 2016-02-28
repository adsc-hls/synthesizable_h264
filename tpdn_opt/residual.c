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

void matrix_mult4x4(int A[4][4], int B[4][4], int C[4][4])
{

  int i,j,k;
  int sum;

  for(i=0; i<4; i++)
  {
    for(j=0; j<4; j++)
    {
      sum = 0;
      for(k=0; k<4; k++)
      {
        sum += A[i][k]*B[k][j];
      }
      C[i][j] = sum;
    }
  }
}

void matrix_mult2x2(int A[2][2], int B[2][2], int C[2][2])
{
  int i, j, k;
  int sum;

  for(i=0; i<2; i++)
  {
    for(j=0; j<2; j++)
    {
      sum = 0;
      for(k=0; k<2; k++)
      {
        sum += A[i][k]*B[k][j];
      }
      C[i][j] = sum;
    }
  }
}

int get_LevelScale4x4(int m, int i, int j/*, int caller*/) /* 8.5.9 */
{
  const int v[6][2][2] ={
    {{160,208},{208,256}},
    {{208,224},{224,288}},
    {{208,256},{256,320}},
    {{224,288},{288,368}},
    {{256,320},{320,400}},
    {{288,368},{368,464}},
  };

  return v[m][i&0x01][j&0x01];
}


const int vt[6][2][2] ={
  {{160,208},{208,256}},
  {{208,224},{224,288}},
  {{208,256},{256,320}},
  {{224,288},{288,368}},
  {{256,320},{320,400}},
  {{288,368},{368,464}},
};

void scale_and_inv_trans_Intra16x16DC(int qP, int c[4][4], int qPm6, int scale1, int scale2, int scale3) /* 8.5.10 */
{
  int i,j;
  int inv1[4][4] = {{1,  1,  1,  1},
    {1,  1, -1, -1},
    {1, -1, -1,  1},
    {1, -1,  1, -1}};


  int	f[4][4], temp[4][4], LevelScale4x4;
  LevelScale4x4 = get_LevelScale4x4(qPm6, 0, 0);


  matrix_mult4x4(inv1, c, temp);
  matrix_mult4x4(temp, inv1, f);


  if(qP>=36)
    for(i=0; i<4; i++)
      for(j=0; j<4; j++)
        c[i][j] = (f[i][j] * LevelScale4x4) << scale1;
  else
    for(i=0; i<4; i++)
      for(j=0; j<4; j++)
        c[i][j] = (f[i][j] * LevelScale4x4 + scale3) >> scale2;

}

void scale_residual4x4_and_trans_inverse(int qP, int qPm6, int temp1, int temp2, int temp3, int c[4][4], int r[4][4], int DC_comp,unsigned char flag) /* 8.5.12.1 */
{
#pragma HLS PIPELINE

  int i, j;

  int temp[4][4];
  int tmp[4][4];

  //temp1=qP/6 - 4;
  //tmp=4 - qP/6;
  //temp3=1 << (3 - qP/6);
  if(qP >= 24)
    for(i=0; i<4; i++)
      for(j = 0;j<4; j++)
        temp[i][j] = (c[i][j] * vt[qPm6][i&0x01][j&0x01]) << temp1;
  else
    for(i=0; i<4; i++)
      for(j = 0;j<4; j++)
        temp[i][j] = (c[i][j] * vt[qPm6][i&0x01][j&0x01] + temp3) >> temp2;

  if(flag)
    temp[0][0]=DC_comp;

  int i1,j1, e[4], f[4];

  for (j=0;j<BLOCK_SIZE;j++)
  {

    f[0]=(temp[0][j]+temp[2][j]);
    f[1]=(temp[0][j]-temp[2][j]);
    f[2]=(temp[1][j]>>1)-temp[3][j];
    f[3]=temp[1][j]+(temp[3][j]>>1);

    for (i=0;i<2;i++)
    {
      i1=3-i;
      tmp[i][j]=f[i]+f[i1];
      tmp[i1][j]=f[i]-f[i1];
    }
  }
  // vertical
  for (i=0;i<BLOCK_SIZE;i++)
  {


    f[0]=(tmp[i][0]+tmp[i][2]);
    f[1]=(tmp[i][0]-tmp[i][2]);
    f[2]=(tmp[i][1]>>1)-tmp[i][3];
    f[3]=tmp[i][1]+(tmp[i][3]>>1);

    for (j=0;j<2;j++)
    {
      j1=3-j;
      r[i][j] =(f[j]+f[j1]+32)>>6;
      r[i][j1]=(f[j]-f[j1]+32)>>6;
    }
  }

}

void scale_and_inv_trans_chroma2x2(int c[2][2],int qP, int qPcm6) /* 8.5.11 */
{
#pragma HLS PIPELINE
  int tran[2][2];
  int temp;
  int i,j;

  temp=vt[qPcm6][0][0];

  tran[0][0]=c[0][0]+c[1][0]+c[0][1]+c[1][1];
  tran[1][0]=c[0][0]-c[1][0]+c[0][1]-c[1][1];
  tran[0][1]=c[0][0]+c[1][0]-c[0][1]-c[1][1];
  tran[1][1]=c[0][0]-c[1][0]-c[0][1]+c[1][1];


  for(i=0; i<2; i++)
  {
    for(j=0; j<2; j++)
    {
      c[i][j] = ((tran[i][j]*temp) << (qP/6)) >> 5;
    }
  }
}
