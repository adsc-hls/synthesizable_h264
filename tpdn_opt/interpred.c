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
#include "vlc.h"
#include "mathfunc.h"
#include <string.h>


void findmv
(
 char refidx01[PicWidthInMBs*2][FrameHeightInMbs*2],
 int mvd01[PicWidthInMBs*4][FrameHeightInMbs*4][2],
 int startblkx,
 int startblky,
 int blk,
 int width,
 int blkx,
 int blky,
 int refid,
 int retmv[2],
 unsigned char dir,
 unsigned char skipflag
 );


void findCidx
(
 int startblkx,
 int startblky,
 int blk,
 int width,
 int blkx,
 int blky,
 int idx[2]
 );


void find_directmv_flag
(
 unsigned char dflag,
 char refcol[2][2],
 int mvcol[4][4][2],
 int blkx,
 int blky,
 int mv1[2],
 int mv0[2],
 char refidx0,
 char refidx1
 );

char find_directzeroflag
(
 char refidx0[PicWidthInMBs*2][FrameHeightInMbs*2],
 char refidx1[PicWidthInMBs*2][FrameHeightInMbs*2],
 char ref01[2],
 int startblkx,
 int startblky
 );


void processinterMbType(
    unsigned char mb_type,
    unsigned char slice_type,
    NALU_t * nalu,
    unsigned char num_ref_idx_active1,
    unsigned char num_ref_idx_active0,
    char refidx0[2][2],
    char refidx1[2][2],
    int mvd0[4][4][2],
    int mvd1[4][4][2],
    char globref0[PicWidthInMBs*2][FrameHeightInMbs*2],
    char globref1[PicWidthInMBs*2][FrameHeightInMbs*2],
    int globmvd0[PicWidthInMBs*4][FrameHeightInMbs*4][2],
    int globmvd1[PicWidthInMBs*4][FrameHeightInMbs*4][2],
    int list0[MAX_REFERENCE_PICTURES],
    int list1[MAX_REFERENCE_PICTURES],
    int Mbaddress,
    int skipflag,
    char refcol[2][2],
    int mvcol[4][4][2],
    unsigned char MbType
)
{
  unsigned char substepx[2][2];
  unsigned char substepy[2][2];
  unsigned char stepx;
  unsigned char stepy;
  unsigned char listuse[2][2];
  unsigned char preddir[2][2];
  unsigned char subtype;
  unsigned char dflag;
  int i,j;

  int x,y;
  int subx,suby;
  char refidx;
  int mvdx;
  int mvdy;
  int tpmmv[2];
  int tpmmv1[2];
  int foundmv0[2];
  int foundmv1[2];
  char tmpref01[2];

  int startblkx= (Mbaddress%PicWidthInMBs)*4;
  int startblky= (Mbaddress/PicWidthInMBs)*4;



  const unsigned char val_stepX[30]=  {4,4,4,2,2,2, // this part is for P slice
    2,2,4,4,4, // following is for B slice
    4,2,4,2,
    4,2,4,2,
    4,2,4,2,
    4,2,4,2,
    4,2,2};
  const unsigned char val_stepY[30]= {4,4,2,4,2,2,
    2,2,4,4,4,
    2,4,2,4,
    2,4,2,4,
    2,4,2,4,
    2,4,2,4,
    2,4,2};

  const unsigned char val_listuse[30][4]= {
    //following are P slice
    {1,1,1,1},//skip
    {1,1,1,1},//0
    {1,1,1,1},//1
    {1,1,1,1},//2
    {1,1,1,1},//3
    {1,1,1,1},//4
    // following are B slice
    {0,0,0,0},//skip
    {0,0,0,0},//direct
    {1,1,1,1},//1
    {2,2,2,2},//2
    {3,3,3,3},//3
    {1,1,1,1},//4
    {1,1,1,1},//5
    {2,2,2,2},//6
    {2,2,2,2},//7
    {1,1,2,2},//8
    {1,2,1,2},//9
    {2,2,1,1},//10
    {2,1,2,1},//11
    {1,1,3,3},//12
    {1,3,1,3},//13
    {2,2,3,3},//14
    {2,3,2,3},//15
    {3,3,1,1},//16
    {3,1,3,1},//17
    {3,3,2,2},//18
    {3,2,3,2},//19
    {3,3,3,3},//20
    {3,3,3,3},//21
    {0,0,0,0},//22
  };

  const unsigned char val_predir[30][4]=  {
    //following are P slice
    {0,0,0,0},//skip
    {0,0,0,0},//0
    {2,2,1,1},//1
    {1,3,1,3},//2
    {0,0,0,0},//3
    {0,0,0,0},//4
    //following are B slice
    {0,0,0,0},//skip
    {0,0,0,0},//direct
    {0,0,0,0},//1
    {0,0,0,0},//2
    {0,0,0,0},//3
    {2,2,1,1},//4
    {1,3,1,3},//5
    {2,2,1,1},//6
    {1,3,1,3},//7
    {2,2,1,1},//8
    {1,3,1,3},//9
    {2,2,1,1},//10
    {1,3,1,3},//11
    {2,2,1,1},//12
    {1,3,1,3},//13
    {2,2,1,1},//14
    {1,3,1,3},//15
    {2,2,1,1},//16
    {1,3,1,3},//17
    {2,2,1,1},//18
    {1,3,1,3},//19
    {2,2,1,1},//20
    {1,3,1,3},//21
    {0,0,0,0},//22
  };



  const unsigned char val_sublistuse[17]={ 1,1,1,1,
    0,1,2,3,1,1,2,2,3,3,1,2,3};


  const unsigned char val_subX[17]={
    2,2,1,1,// P slice
    2,2,2,2,
    2,1,2,1,
    2,1,1,1,1
  };
  const unsigned char val_subY[17]={
    2,1,2,1,// P slice
    2,2,2,2,
    1,2,1,2,
    1,2,1,1,1
  };

  listuse[0][0]=val_listuse[MbType][0];
  listuse[1][0]=val_listuse[MbType][1];
  listuse[0][1]=val_listuse[MbType][2];
  listuse[1][1]=val_listuse[MbType][3];
  stepx=val_stepX[MbType];
  stepy=val_stepY[MbType];
  substepx[0][0]=stepx;
  substepy[0][0]=stepy;
  substepx[0][1]=stepx;
  substepy[0][1]=stepy;
  substepx[1][0]=stepx;
  substepy[1][0]=stepy;
  substepx[1][1]=stepx;
  substepy[1][1]=stepy;
  preddir[0][0]=val_predir[MbType][0];
  preddir[1][0]=val_predir[MbType][1];
  preddir[0][1]=val_predir[MbType][2];
  preddir[1][1]=val_predir[MbType][3];

  if(MbType==5 || MbType==29)
  {

    for(i=0;i<2;i++)
      for(j=0;j<2;j++)
      {
        subtype=u_e(nalu)+slice_type*4;
        substepx[j][i]=val_subX[subtype];
        substepy[j][i]=val_subY[subtype];
        listuse[j][i]=val_sublistuse[subtype];
      }
  }

  skipflag= (MbType==6 || MbType==7)?0:skipflag;

  if(MbType==0 || MbType==5  )
  {

    for(i=0;i<2;i++)
      for(j=0;j<2;j++)
      {
        refidx0[i][j] = 0;
        refidx1[i][j] = -1;
        globref0[startblkx/2+j][startblky/2+i]=0;
        globref1[startblkx/2+j][startblky/2+i]=-1;
      }
  }
  else
  {
    if(num_ref_idx_active0>0 )
    {
      for(y=0;y<2;y+= (stepy/2) )
        for(x=0;x<2;x+= (stepx/2) )
        {
          if(listuse[x][y]== 0)
          {
            dflag= find_directzeroflag
              (
               globref0,
               globref1,
               tmpref01,
               startblkx,
               startblky
              );
            refidx0[x][y]=globref0[startblkx/2+x][startblky/2+y]=tmpref01[0];
            refidx1[x][y]=globref1[startblkx/2+x][startblky/2+y]=tmpref01[1];


          }
          else if(listuse[x][y] & 0x01)
          {
            refidx=t_e(num_ref_idx_active0,nalu);
#if _N_HLS_
            fprintf(trace_bit,"refIdxL0[i][j] %d %d \n" ,refidx, num_ref_idx_active0);
#endif // _N_HLS_
            for(i=0;i<stepy/2;i++)
              for(j=0;j<stepx/2;j++)
              {
                refidx0[x+j][y+i]=refidx;
                globref0[startblkx/2+x+j][startblky/2+y+i]=refidx;
              }
          }
          else
          {
            for(i=0;i<stepy/2;i++)
              for(j=0;j<stepx/2;j++)
              {
                refidx0[x+j][y+i]=-1;
                globref0[startblkx/2+x+j][startblky/2+y+i]=-1;
              }
          }
        }
    }
    else
    {
      for(i=0;i<2;i++)
        for(j=0;j<2;j++)
        {
          refidx0[j][i]=0;
          globref0[startblkx/2+j][startblky/2+i]=0;
        }
    }
    if(num_ref_idx_active1>0  )
    {
      for(y=0;y<2;y+= (stepy/2) )
        for(x=0;x<2;x+= (stepx/2) )
        {

          if(listuse[x][y]== 0)
          {
            //donotihing
          }
          else if( (listuse[x][y] & 0x02) !=0)
          {
            refidx=t_e(num_ref_idx_active1,nalu);
#if _N_HLS_
            fprintf(trace_bit,"refIdxL1[i][j] %d %d\n" , refidx, num_ref_idx_active1);
#endif // _N_HLS_
            for(i=0;i<stepy/2;i++)
              for(j=0;j<stepx/2;j++)
              {
                refidx1[x+j][y+i]=refidx;
                globref1[startblkx/2+x+j][startblky/2+y+i]=refidx;
#if _N_HLS_
                fprintf(trace_bit,"refIdx1[%d][%d] %d\n" ,x+j, y+i, refidx);
#endif // _N_HLS_
              }
          }
          else
          {
            for(i=0;i<stepy/2;i++)
              for(j=0;j<stepx/2;j++)
              {
                refidx1[x+j][y+i]=-1;
                globref1[startblkx/2+x+j][startblky/2+y+i]=-1;
              }
          }
        }
    }
    else
    {

      for(i=0;i<2;i++)
        for(j=0;j<2;j++)
        {
          refidx1[j][i]=0;
          globref1[startblkx/2+j][startblky/2+i]=0;
        }
    }
  }
  unsigned char stpw;
  unsigned char stph;

  for(y=0;y<4;y+=stepy)
    for(x=0;x<4;x+=stepx)
    {
      if(listuse[x>>1][y>>1]==0)
      {
        findmv( globref0,globmvd0,startblkx, startblky,0,4,0,0,refidx0[x/2][y/2],foundmv0,0,0);
        findmv( globref1,globmvd1,startblkx, startblky,0,4,0,0,refidx1[x/2][y/2],foundmv1,0,0);

        for(i=0;i<2;i++)
          for(j=0;j<2;j++)
          {
            tpmmv[0]=foundmv0[0];
            tpmmv[1]=foundmv0[1];
            tpmmv1[0]=foundmv1[0];
            tpmmv1[1]=foundmv1[1];

            find_directmv_flag(dflag,refcol,mvcol,x+i,y+j,tpmmv1,tpmmv,refidx0[x/2][y/2],refidx1[x/2][y/2]);
            mvd0[x+i][y+j][0]=globmvd0[startblkx+x+i][startblky+y+j][0]=tpmmv[0];
            mvd0[x+i][y+j][1]=globmvd0[startblkx+x+i][startblky+y+j][1]=tpmmv[1];
            mvd1[x+i][y+j][0]=globmvd1[startblkx+x+i][startblky+y+j][0]=tpmmv1[0];
            mvd1[x+i][y+j][1]=globmvd1[startblkx+x+i][startblky+y+j][1]=tpmmv1[1];
          }
      }
      else if( listuse[x>>1][y>>1] & 0x01 )
      {
        stpw=substepx[x>>1][y>>1];
        stph=substepy[x>>1][y>>1];

        for(suby=0;suby<stepy;suby+=stph)
          for(subx=0;subx<stepx;subx+=stpw)
          {
            //calculate mvbase
            if(skipflag!=1  )
            {
              mvdx=s_e(nalu);
              mvdy=s_e(nalu);
#if _N_HLS_
              fprintf(trace_bit," curMB->mvd_l0[%d][%d][0] %d\n" ,startblkx+x+subx,startblky+y+suby,mvdx);
#endif // _N_HLS_
#if _N_HLS_
              fprintf(trace_bit," curMB->mvd_l0[%d][%d][1] %d\n" ,startblkx+x+subx,startblky+y+suby,mvdy);

#endif // _N_HLS_
              // calculate and write x
            }
            else
            {
              mvdx=0;
              mvdy=0;
            }

            findmv
              (
               globref0,
               globmvd0,
               startblkx,
               startblky,
               XYTOK(x+subx,y+suby),
               stpw,
               x+subx,
               y+suby,
               refidx0[x/2][y/2],
               tpmmv,
               preddir[x/2][y/2],
               skipflag
              );

            for(i=0;i<stpw;i++)
              for(j=0;j<stph;j++)
              {
                mvd0[x+subx+i][y+suby+j][0]=globmvd0[startblkx+x+subx+i][startblky+y+suby+j][0]=mvdx+tpmmv[0];
                mvd0[x+subx+i][y+suby+j][1]=globmvd0[startblkx+x+subx+i][startblky+y+suby+j][1]=mvdy+tpmmv[1];
              }
          }
      }
    }

  for(y=0;y<4;y+=stepy)
    for(x=0;x<4;x+=stepx)
    {
      if(listuse[x>>1][y>>1]==0)
      {
        // this part left
      }
      else if( listuse[x>>1][y>>1] & 0x02 )
      {
        stpw=substepx[x>>1][y>>1];
        stph=substepy[x>>1][y>>1];

        for(suby=0;suby<stepy;suby+=stph)
          for(subx=0;subx<stepx;subx+=stpw)
          {
            //calculate mvbase
            if(skipflag!=1  )
            {
              mvdx=s_e(nalu);
              mvdy=s_e(nalu);
#if _N_HLS_
              fprintf(trace_bit," curMB->mvd_l0[%d][%d][0] %d\n" ,startblkx+x+subx,startblky+y+suby,mvdx);
#endif // _N_HLS_
#if _N_HLS_
              fprintf(trace_bit," curMB->mvd_l0[%d][%d][1] %d\n" ,startblkx+x+subx,startblky+y+suby,mvdy);

#endif // _N_HLS_
              // calculate and write x
            }
            else
            {
              mvdx=0;
              mvdy=0;
            }

            findmv
              (
               globref1,
               globmvd1,
               startblkx,
               startblky,
               XYTOK(x+subx,y+suby),
               stpw,
               x+subx,
               y+suby,
               refidx1[x/2][y/2],
               tpmmv,
               preddir[x/2][y/2],
               skipflag
              );

            for(i=0;i<stpw;i++)
              for(j=0;j<stph;j++)
              {
                mvd1[x+subx+i][y+suby+j][0]=globmvd1[startblkx+x+subx+i][startblky+y+suby+j][0]=mvdx+tpmmv[0];
                mvd1[x+subx+i][y+suby+j][1]=globmvd1[startblkx+x+subx+i][startblky+y+suby+j][1]=mvdy+tpmmv[1];
              }
            // calculate and write x
          }

      }
    }
}


  void findCidx
(
 int startblkx,
 int startblky,
 int blk,
 int width,
 int blkx,
 int blky,
 int idx[2]
 )
{
  int blkCy;
  char tempblkcx=blkx+width;
  blkCy=blky-1;
  if( ( tempblkcx>3 && blkCy >=0 ) || ( XYTOK(tempblkcx,blkCy)> blk && tempblkcx<=3 && blkCy>=0 ) || (startblkx+tempblkcx>=PicWidthInMBs*4) )
    idx[0]=startblkx+blkx-1;
  else idx[0]=startblkx+tempblkcx;
  idx[1]=startblky+blkCy;
}



  void findmv
(
 char refidx01[PicWidthInMBs*2][FrameHeightInMbs*2],
 int mvd01[PicWidthInMBs*4][FrameHeightInMbs*4][2],
 int startblkx,
 int startblky,
 int blk,
 int width,
 int blkx,
 int blky,
 int refid,
 int retmv[2],
 unsigned char dir,
 unsigned char skipflag
 )
{
  int tmpAmv[2];
  int tmpBmv[2];
  int tmpCmv[2];
  int idxC[2];
  char refA;
  char refB;
  char refC;

  if( (startblkx+blkx-1)>=0)
    refA=refidx01[ (startblkx+blkx-1) /2][ (startblky+blky)/2];
  else refA=-1;

  if( (startblky+blky-1)>=0)
    refB=refidx01[ (startblkx+blkx)/2][ (startblky+blky-1)/2];
  else refB=-1;

  findCidx(startblkx,startblky,blk,width,blkx,blky,idxC);

  if(idxC[0] >=0 && idxC[1]>=0 )
    refC=refidx01[idxC[0]/2][idxC[1]/2];
  else refC=-1;

  if(skipflag&&(refA<0 || refB<0))
  {
    retmv[0]=0;
    retmv[1]=0;
  }
  else if(dir==1 && refA==refid)
  {
    retmv[0]=mvd01[startblkx+blkx-1][startblky+blky][0];
    retmv[1]=mvd01[startblkx+blkx-1][startblky+blky][1];
  }
  else if(dir==2 && refB==refid)
  {
    retmv[0]=mvd01[startblkx+blkx][startblky+blky-1][0];
    retmv[1]=mvd01[startblkx+blkx][startblky+blky-1][1];
  }
  else if(dir==3 && refC==refid)
  {
    retmv[0]=mvd01[idxC[0]][idxC[1]][0];
    retmv[1]=mvd01[idxC[0]][idxC[1]][1];

  }
  else
  {
    if(refC<0 && refB<0 && refA>=0)
    {
      retmv[0]=mvd01[startblkx+blkx-1][startblky+blky][0];
      retmv[1]=mvd01[startblkx+blkx-1][startblky+blky][1];
    }
    else if(refA==refid && refB!=refid && refC!=refid)
    {
      retmv[0]=mvd01[startblkx+blkx-1][startblky+blky][0];
      retmv[1]=mvd01[startblkx+blkx-1][startblky+blky][1];
    }
    else if(refA!=refid && refB==refid && refC!=refid)
    {
      retmv[0]=mvd01[startblkx+blkx][startblky+blky-1][0];
      retmv[1]=mvd01[startblkx+blkx][startblky+blky-1][1];
    }
    else if(refA!=refid && refB!=refid && refC==refid)
    {
      retmv[0]=mvd01[idxC[0]][idxC[1]][0];
      retmv[1]=mvd01[idxC[0]][idxC[1]][1];
    }
    else
    {
      if(refA<0)
      {
        tmpAmv[0]=0;
        tmpAmv[1]=0;
      }
      else
      {
        tmpAmv[0]=mvd01[startblkx+blkx-1][startblky+blky][0];
        tmpAmv[1]=mvd01[startblkx+blkx-1][startblky+blky][1];
      }

      if(refB<0)
      {
        tmpBmv[0]=0;
        tmpBmv[1]=0;
      }
      else
      {
        tmpBmv[0]=mvd01[startblkx+blkx][startblky+blky-1][0];
        tmpBmv[1]=mvd01[startblkx+blkx][startblky+blky-1][1];
      }

      if(refC<0)
      {
        tmpCmv[0]=0;
        tmpCmv[1]=0;
      }
      else
      {
        tmpCmv[0]=mvd01[idxC[0]][idxC[1]][0];
        tmpCmv[1]=mvd01[idxC[0]][idxC[1]][1];
      }

      if(skipflag && ( (refA==0&&tmpAmv[0]==0 &&tmpAmv[1]==0)|| (refB==0 &&tmpBmv[0]==0 && tmpBmv[1]==0) )   )
      {
        retmv[0]=0;
        retmv[1]=0;
      }
      else
      {
        retmv[0]=median(tmpAmv[0],tmpBmv[0],tmpCmv[0]);
        retmv[1]=median(tmpAmv[1],tmpBmv[1],tmpCmv[1]);
      }
    }
  }
}

char find_directzeroflag
(
 char refidx0[PicWidthInMBs*2][FrameHeightInMbs*2],
 char refidx1[PicWidthInMBs*2][FrameHeightInMbs*2],
 char ref01[2],
 int startblkx,
 int startblky
 )
{
  int idxC[2];
  char refA0, refA1;
  char refB0, refB1;
  char refC0, refC1;

  if( (startblkx-1)>=0)
  {
    refA0=refidx0[ (startblkx-1) /2][ (startblky)/2];
    refA1=refidx1[ (startblkx-1) /2][ (startblky)/2];
  }
  else
  {
    refA0=-1;
    refA1=-1;
  }

  if( (startblky-1)>=0)
  {
    refB0=refidx0[ (startblkx)/2][ (startblky-1)/2];
    refB1=refidx1[ (startblkx)/2][ (startblky-1)/2];
  }
  else
  {
    refB0=-1;
    refB1=-1;
  }



  findCidx(startblkx,startblky,0,1,0,0,idxC);

  if(idxC[0] >=0 && idxC[1]>=0 )
  {
    refC0=refidx0[idxC[0]/2][idxC[1]/2];
    refC1=refidx1[idxC[0]/2][idxC[1]/2];
  }
  else
  {
    refC0=-1;
    refC1=-1;
  }

  ref01[0]=minpositive(refA0,minpositive(refB0,refC0));
  ref01[1]=minpositive(refA1,minpositive(refB1,refC1));

  if(ref01[0]<0 && ref01[1]<0)
  {
    ref01[0]=0;
    ref01[1]=0;
    return 1;
  }
  else
  {
    return 0;
  }
}

void find_directmv_flag
(
 unsigned char dflag,
 char refcol[2][2],
 int mvcol[4][4][2],
 int blkx,
 int blky,
 int mv1[2],
 int mv0[2],
 char refidx0,
 char refidx1
 )
{
  unsigned char colzero;
  if(refcol[blkx/2][blky/2]==0 && mvcol[blkx][blky][0]<=1 && mvcol[blkx][blky][0]>=-1 && mvcol[blkx][blky][1]<=1 && mvcol[blkx][blky][1]>=-1)
    colzero=1;
  else
    colzero=0;

  if(dflag || refidx0<0 || (refidx0==0 && colzero) )
  {

    mv0[0]=0;
    mv0[1]=0;
  }
  if(dflag || refidx1<0 || (refidx1==0 && colzero) )
  {

    mv1[0]=0;
    mv1[1]=0;
  }
}

void inter_prediction_chroma_subblock_single(
    int rMbC[4][4],
    int mv[2][2][2],
    unsigned char Schroma[PicWidthInSamplesC][FrameHeightInSampleC],
    unsigned char Schroma_cur[PicWidthInSamplesC][FrameHeightInSampleC],
    int startblkx,
    int startblky,
    unsigned flag)
{
#pragma HLS ARRAY_PARTITION variable=rMbC complete dim=1
#pragma HLS ARRAY_PARTITION variable=rMbC complete dim=2



#pragma HLS ARRAY_PARTITION variable=mv complete dim=1
#pragma HLS ARRAY_PARTITION variable=mv complete dim=2
#pragma HLS ARRAY_PARTITION variable=mv complete dim=3


//#pragma HLS PIPELINE
  int i,j;
  int x,y;
  int x0,y0;


  unsigned char temp[3][3];
#pragma HLS ARRAY_PARTITION variable=temp complete dim=1
#pragma HLS ARRAY_PARTITION variable=temp complete dim=2

  unsigned char xfrac,yfrac;
  int xint,yint;


  for(i=0;i<2;i++)
    for(j=0;j<2;j++)
    {
      xint=mv[i][j][0]>>3;
      yint=mv[i][j][1]>>3;
      xfrac=(mv[i][j][0]&0x07);
      yfrac=(mv[i][j][1]&0x07);

      for(x=0;x<3;x++)
      #pragma HLS UNROLL
        for(y=0;y<3;y++)
        {
          #pragma HLS UNROLL
          x0=Clip3(0,PicWidthInSamplesC-1,startblkx+x+xint+i*2);
          y0=Clip3(0,FrameHeightInSampleC-1,startblky+y+yint+j*2);
          temp[x][y]=Schroma[x0][y0];
        }

      for(x=0;x<2;x++)
      #pragma HLS UNROLL
        for(y=0;y<2;y++)
        {
          #pragma HLS UNROLL
          Schroma_cur[startblkx+x+i*2][startblky+y+j*2]=Clip1y(flag*rMbC[x+i*2][y+j*2]+(((8-xfrac)*(8-yfrac)*temp[x][y]+xfrac*(8-yfrac)*temp[x+1][y]+(8-xfrac)*yfrac*temp[x][y+1]+xfrac*yfrac*temp[x+1][y+1]+32)>>6 ) );

        }
    }

}

void inter_prediction_chroma_subblock_double(
    int rMbC[4][4],
    int mv0[2][2][2],
    int mv1[2][2][2],
    unsigned char Schroma0[PicWidthInSamplesC][FrameHeightInSampleC],
    unsigned char Schroma1[PicWidthInSamplesC][FrameHeightInSampleC],
    unsigned char Schroma_cur[PicWidthInSamplesC][FrameHeightInSampleC],
    int startblkx,
    int startblky,
    unsigned char flag)
{

#pragma HLS ARRAY_PARTITION variable=rMbC complete dim=1
#pragma HLS ARRAY_PARTITION variable=rMbC complete dim=2

#pragma HLS ARRAY_PARTITION variable=mv0 complete dim=1
#pragma HLS ARRAY_PARTITION variable=mv0 complete dim=2
#pragma HLS ARRAY_PARTITION variable=mv0 complete dim=3

#pragma HLS ARRAY_PARTITION variable=mv1 complete dim=1
#pragma HLS ARRAY_PARTITION variable=mv1 complete dim=2
#pragma HLS ARRAY_PARTITION variable=mv1 complete dim=3
//#pragma HLS PIPELINE
  int i,j;
  int x,y;
  int x0,y0;


  unsigned char temp0[3][3];
#pragma HLS ARRAY_PARTITION variable=temp0 complete dim=1
#pragma HLS ARRAY_PARTITION variable=temp0 complete dim=2
  unsigned char temp1[3][3];
#pragma HLS ARRAY_PARTITION variable=temp1 complete dim=1
#pragma HLS ARRAY_PARTITION variable=temp1 complete dim=2
  unsigned char xfrac0,yfrac0;
  int xint0,yint0;


  unsigned char xfrac1,yfrac1;
  int xint1,yint1;



  for(i=0;i<2;i++)
    for(j=0;j<2;j++)
    {

      xint0=mv0[i][j][0]>>3;
      yint0=mv0[i][j][1]>>3;
      xfrac0=(mv0[i][j][0]&0x07);
      yfrac0=(mv0[i][j][1]&0x07);



      xint1=mv1[i][j][0]>>3;
      yint1=mv1[i][j][1]>>3;
      xfrac1=(mv1[i][j][0]&0x07);
      yfrac1=(mv1[i][j][1]&0x07);

      for(x=0;x<3;x++)
      #pragma HLS UNROLL
        for(y=0;y<3;y++)
        {
          #pragma HLS UNROLL
          x0=Clip3(0,PicWidthInSamplesC-1,startblkx+x+xint0+i*2);
          y0=Clip3(0,FrameHeightInSampleC-1,startblky+y+yint0+j*2);
          temp0[x][y]=Schroma0[x0][y0];
        }

      for(x=0;x<3;x++)
      #pragma HLS UNROLL
        for(y=0;y<3;y++)
        {
          #pragma HLS UNROLL
          x0=Clip3(0,PicWidthInSamplesC-1,startblkx+x+xint1+i*2);
          y0=Clip3(0,FrameHeightInSampleC-1,startblky+y+yint1+j*2);
          temp1[x][y]=Schroma1[x0][y0];
        }


      for(x=0;x<2;x++)
      #pragma HLS UNROLL
        for(y=0;y<2;y++)
        {
          #pragma HLS UNROLL
          Schroma_cur[startblkx+x+i*2][startblky+y+j*2]=Clip1y(flag*rMbC[x+i*2][y+j*2]+
              ( ( (((8-xfrac0)*(8-yfrac0)*temp0[x][y]+xfrac0*(8-yfrac0)*temp0[x+1][y]+(8-xfrac0)*yfrac0*temp0[x][y+1]+xfrac0*yfrac0*temp0[x+1][y+1]+32)>>6 )
                  +(((8-xfrac1)*(8-yfrac1)*temp1[x][y]+xfrac1*(8-yfrac1)*temp1[x+1][y]+(8-xfrac1)*yfrac1*temp1[x][y+1]+xfrac1*yfrac1*temp1[x+1][y+1]+32)>>6 )
                  +1)>>1) );
        }
    }

}

const int inter_tab[6][6]=  { {1, -5, 20, 20, -5, 1, },
  {-5, 25, -100, -100, 25, -5, },
  {20, -100, 400, 400, -100, 20, },
  {20, -100, 400, 400, -100, 20, },
  {-5, 25, -100, -100, 25, -5, },
  {1, -5, 20, 20, -5, 1, },};

void copy_j(unsigned char Sluma[PicWidthInSamplesL][FrameHeightInSampleL], int xint, int yint, unsigned char temp[9][9])
{

#pragma HLS ARRAY_PARTITION variable=temp complete dim=1
#pragma HLS ARRAY_PARTITION variable=temp complete dim=2
//#pragma HLS PIPELINE
  int i,j;
  int x,y;

  for(i=0;i<9;i++)
  #pragma HLS UNROLL
    for(j=0;j<9;j++)
    {
      #pragma HLS UNROLL
      x=Clip3(0,PicWidthInSamplesL-1,xint-2+i);
      y=Clip3(0,FrameHeightInSampleL-1,yint-2+j);
      temp[i][j]=Sluma[x][y];
    }
}



void copy_H(unsigned char Sluma[PicWidthInSamplesL][FrameHeightInSampleL],int xint, int yint, int yoffset, unsigned char temp[9][9])
{
#pragma HLS ARRAY_PARTITION variable=temp complete dim=1
#pragma HLS ARRAY_PARTITION variable=temp complete dim=2
//#pragma HLS PIPELINE
  int i,j;
  int x,y;

  for(i=0;i<9;i++)
  #pragma HLS UNROLL
    for(j=2;j<6;j++)
    {
      #pragma HLS UNROLL
      x=Clip3(0,PicWidthInSamplesL-1,xint-2+i);
      y=Clip3(0,FrameHeightInSampleL-1,yint-2+j+yoffset);
      temp[i][j+yoffset]=Sluma[x][y];
    }
}

void copy_V(unsigned char Sluma[PicWidthInSamplesL][FrameHeightInSampleL],int xint, int yint, int xoffset, unsigned char temp[9][9])
{

#pragma HLS ARRAY_PARTITION variable=temp complete dim=1
#pragma HLS ARRAY_PARTITION variable=temp complete dim=2
//#pragma HLS PIPELINE
  int i,j;
  int x,y;

  for(i=2;i<6;i++)
  #pragma HLS UNROLL
    for(j=0;j<9;j++)
    {
      #pragma HLS UNROLL
      x=Clip3(0,PicWidthInSamplesL-1,xint-2+i+xoffset);
      y=Clip3(0,FrameHeightInSampleL-1,yint-2+j);
      temp[i+xoffset][j]=Sluma[x][y];
    }
}



void copy_Cross(unsigned char Sluma[PicWidthInSamplesL][FrameHeightInSampleL], int xint, int yint,int xoffset, int yoffset,unsigned char temp[9][9])
{

#pragma HLS ARRAY_PARTITION variable=temp complete dim=1
#pragma HLS ARRAY_PARTITION variable=temp complete dim=2
//#pragma HLS PIPELINE
  int i,j;
  int x,y;

  for(i=0;i<9;i++)
  #pragma HLS UNROLL
    for(j=0;j<9;j++)
    {
      #pragma HLS UNROLL
      if( (i>1+xoffset&&i<6+xoffset) || (j>1+yoffset&&j<6+yoffset))
      {
        x=Clip3(0,PicWidthInSamplesL-1,xint-2+i);
        y=Clip3(0,FrameHeightInSampleL-1,yint-2+j);
        temp[i][j]=Sluma[x][y];
      }
    }
}


void copy_G(unsigned char Sluma[PicWidthInSamplesL][FrameHeightInSampleL], int xint, int yint,  unsigned char temp[9][9])
{

#pragma HLS ARRAY_PARTITION variable=temp complete dim=1
#pragma HLS ARRAY_PARTITION variable=temp complete dim=2
//#pragma HLS PIPELINE
  int i,j;

  for(i=2;i<6;i++)
  #pragma HLS UNROLL
    for(j=2;j<6;j++)
    {
      #pragma HLS UNROLL
      temp[i][j]=Sluma[xint-2+i][yint-2+j];
    }
}


void copy_comp(unsigned char Sluma[PicWidthInSamplesL][FrameHeightInSampleL],int xint, int yint,unsigned char xfrac, unsigned char yfrac, unsigned char temp[9][9])
{
#pragma HLS ARRAY_PARTITION variable=temp complete dim=1
#pragma HLS ARRAY_PARTITION variable=temp complete dim=2
  if( xfrac==0)
  {
    copy_V(Sluma,xint,yint,(xfrac==3 && yfrac!=0),temp);
  }
  else if(yfrac==0)
  {
    copy_H(Sluma,xint,yint,(yfrac==3 && xfrac!=0),temp);
  }
  else if(xfrac%2 && yfrac%2)
  {
    copy_Cross(Sluma,xint,yint,(xfrac==3 && yfrac!=0),(yfrac==3 && xfrac!=0),temp);
  }
  else
  {
    copy_j(Sluma,xint,yint,temp);
  }
}

void inter_luma_double_bizero_skip
(
 unsigned char Sluma0[PicWidthInSamplesL][FrameHeightInSampleL],
 unsigned char Sluma1[PicWidthInSamplesL][FrameHeightInSampleL],
 unsigned char Sluma_cur[PicWidthInSamplesL][FrameHeightInSampleL],
 int startx,
 int starty,
 int xint0,
 int yint0,
 int xint1,
 int yint1,
 int rMb[4][4],
 unsigned char rmbflag)
{
#pragma HLS ARRAY_PARTITION variable=rMb complete dim=1
#pragma HLS ARRAY_PARTITION variable=rMb complete dim=2

  int i,j;
  int x0,y0;
  int x1,y1;
  for(i=0;i<4;i++)
    for(j=0;j<4;j++)
    {
      x0=Clip3(0,PicWidthInSamplesL-1,xint0+i);
      y0=Clip3(0,FrameHeightInSampleL-1,yint0+j);
      x1=Clip3(0,PicWidthInSamplesL-1,xint1+i);
      y1=Clip3(0,FrameHeightInSampleL-1,yint1+j);

      Sluma_cur[startx+i][starty+j]= Clip1y(rmbflag*rMb[i][j]+((Sluma0[x0][y0]+Sluma1[x1][y1]+1)>>1) );
    }
}

void inter_luma_double_skip
(
 unsigned char Sluma_cur[PicWidthInSamplesL][FrameHeightInSampleL],
 int startx,
 int starty,
 unsigned char xfrac0,
 unsigned char yfrac0,
 unsigned char xfrac1,
 unsigned char yfrac1,
 unsigned char xoffset0,
 unsigned char yoffset0,
 unsigned char xoffset1,
 unsigned char yoffset1,
 unsigned char temp0[9][9],
 unsigned char temp1[9][9],
 int rMb[4][4],
 unsigned char rmbflag
 )
{
#pragma HLS ARRAY_PARTITION variable=temp1 complete dim=1
#pragma HLS ARRAY_PARTITION variable=temp1 complete dim=2

#pragma HLS ARRAY_PARTITION variable=temp0 complete dim=1
#pragma HLS ARRAY_PARTITION variable=temp0 complete dim=2

#pragma HLS ARRAY_PARTITION variable=rMb complete dim=1
#pragma HLS ARRAY_PARTITION variable=rMb complete dim=2


//#pragma HLS PIPELINE
  int sum0,sum1;
  unsigned char h0,b0,h1,b1;
  unsigned char G0,G1;
  unsigned char bitoff0,bitoff1;


  int i,j;
  int x,y;


  for(i=0;i<4;i++)
    #pragma HLS PIPELINE
    for(j=0;j<4;j++)
    {
      if(yfrac0>0 && xfrac0!=2)
        h0=Clip1y((temp0[i+2+xoffset0][j]-5*temp0[i+2+xoffset0][j+1]+20*temp0[i+2+xoffset0][j+2]+20*temp0[i+2+xoffset0][j+3]-5*temp0[i+2+xoffset0][j+4]+temp0[i+2+xoffset0][j+5]+16)>>5);
      else
        h0=0;

      if(xfrac0>0 && yfrac0!=2)
        b0=Clip1y((temp0[i][j+2+yoffset0]-5*temp0[i+1][j+2+yoffset0]+20*temp0[i+2][j+2+yoffset0]+20*temp0[i+3][j+2+yoffset0]-5*temp0[i+4][j+2+yoffset0]+temp0[i+5][j+2+yoffset0]+16)>>5);
      else
        b0=0;


      if((yfrac0>0 && xfrac0==2) || (xfrac0>0 && yfrac0==2))
      {
        sum0=0;
        for(x=0;x<6;x++)
        //#pragma HLS UNROLL
          for(y=0;y<6;y++)
          {
            //#pragma HLS UNROLL
            sum0=sum0+temp0[x+i][y+j]*inter_tab[x][y];
          }
        sum0=(sum0+512)>>10;
      }
      else
        sum0=0;

      if( (yfrac0==0 && xfrac0!=2) || (xfrac0==0 && yfrac0!=2) ||(xfrac0==0 && yfrac0==0))
        G0=temp0[i+2+xfrac0/2][j+2+yfrac0/2];
      else
        G0=0;

      if(xfrac0%2==0 && yfrac0%2==0 )
        bitoff0=0;
      else
        bitoff0=1;

      if(yfrac1>0 && xfrac1!=2)
        h1=Clip1y((temp1[i+2+xoffset1][j]-5*temp1[i+2+xoffset1][j+1]+20*temp1[i+2+xoffset1][j+2]+20*temp1[i+2+xoffset1][j+3]-5*temp1[i+2+xoffset1][j+4]+temp1[i+2+xoffset1][j+5]+16)>>5);
      else
        h1=0;

      if(xfrac1>0 && yfrac1!=2)
        b1=Clip1y((temp1[i][j+2+yoffset1]-5*temp1[i+1][j+2+yoffset1]+20*temp1[i+2][j+2+yoffset1]+20*temp1[i+3][j+2+yoffset1]-5*temp1[i+4][j+2+yoffset1]+temp1[i+5][j+2+yoffset1]+16)>>5);
      else
        b1=0;


      if((yfrac1>0 && xfrac1==2) || (xfrac1>0 && yfrac1==2))
      {
        sum1=0;
        for(x=0;x<6;x++)
        //#pragma HLS UNROLL
          for(y=0;y<6;y++)
          {
            //#pragma HLS UNROLL
            sum1=sum1+temp1[x+i][y+j]*inter_tab[x][y];
          }
        sum1=(sum1+512)>>10;
      }
      else
        sum1=0;

      if( (yfrac1==0 && xfrac1!=2) || (xfrac1==0 && yfrac1!=2) ||(xfrac1==0 && yfrac1==0))
        G1=temp1[i+2+xfrac1/2][j+2+yfrac1/2];
      else
        G1=0;

      if(xfrac1%2==0 && yfrac1%2==0 )
        bitoff1=0;
      else
        bitoff1=1;
      Sluma_cur[startx+i][starty+j]=   Clip1y(rmbflag*rMb[i][j]+( ( ((G0+h0+b0+sum0+bitoff0)>>bitoff0)+ ((G1+h1+b1+sum1+bitoff1)>>bitoff1)+1)>>1 )   );
    }
}

void inter_luma_single
(
 unsigned char Sluma_cur[PicWidthInSamplesL][FrameHeightInSampleL],
 int rMb[4][4],
 int startx,
 int starty,
 unsigned char xfrac,
 unsigned char yfrac,
 unsigned char xoffset,
 unsigned char yoffset,
 unsigned char temp[9][9],
 unsigned char rmbflag)
{
#pragma HLS ARRAY_PARTITION variable=temp complete dim=1
#pragma HLS ARRAY_PARTITION variable=temp complete dim=2

#pragma HLS ARRAY_PARTITION variable=rMb complete dim=1
#pragma HLS ARRAY_PARTITION variable=rMb complete dim=2

//#pragma HLS PIPELINE
  int sum;
  unsigned char h,b;
  unsigned char G;
  unsigned char bitoff;

  int i,j;
  int x,y;

  for(i=0;i<4;i++)
    #pragma HLS PIPELINE
    for(j=0;j<4;j++)
    {
      if(yfrac>0 && xfrac!=2)
        h=Clip1y((temp[i+2+xoffset][j]-5*temp[i+2+xoffset][j+1]+20*temp[i+2+xoffset][j+2]+20*temp[i+2+xoffset][j+3]-5*temp[i+2+xoffset][j+4]+temp[i+2+xoffset][j+5]+16)>>5);
      else
        h=0;

      if(xfrac>0 && yfrac!=2)
        b=Clip1y((temp[i][j+2+yoffset]-5*temp[i+1][j+2+yoffset]+20*temp[i+2][j+2+yoffset]+20*temp[i+3][j+2+yoffset]-5*temp[i+4][j+2+yoffset]+temp[i+5][j+2+yoffset]+16)>>5);
      else
        b=0;


      if((yfrac>0 && xfrac==2) || (xfrac>0 && yfrac==2))
      {
        sum=0;
        for(x=0;x<6;x++)
        //#pragma HLS UNROLL
          for(y=0;y<6;y++)
          {
            //#pragma HLS UNROLL
            sum=sum+temp[x+i][y+j]*inter_tab[x][y];
          }
        sum=(sum+512)>>10;
      }
      else
        sum=0;

      if( (yfrac==0 && xfrac!=2) || (xfrac==0 && yfrac!=2) ||(xfrac==0 && yfrac==0))
        G=temp[i+2+xfrac/2][j+2+yfrac/2];
      else
        G=0;

      if(xfrac%2==0 && yfrac%2==0 )
        bitoff=0;
      else
        bitoff=1;

      Sluma_cur[startx+i][starty+j]=   Clip1y( rmbflag*rMb[i][j]+((G+h+b+sum+bitoff)>>bitoff) );
    }
}
