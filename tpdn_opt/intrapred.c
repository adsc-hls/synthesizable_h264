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

#include <string.h>

void IntraInfo(
NALU_t* nalu,
char pred_mode[PicWidthInMBs*4][FrameHeightInMbs*4],
char refidx0[PicWidthInMBs*2][FrameHeightInMbs*2],
char refidx1[PicWidthInMBs*2][FrameHeightInMbs*2],
char tmpintramode[16],
unsigned char constrained_intra_pred_flag,
int startx,
int starty)
{
    int x;
    int y;
    int xoff;
    int yoff;
    int k;
    int modetmp;
    char predmodeA,predmodeB;
    unsigned char previntramodeflag;

    for(k=0;k<16;k++)
    {

        x=KTOX(k);
        y=KTOY(k);

        xoff=startx+x;
        yoff=starty+y;

        refidx0[xoff/2][yoff/2]=-1;
        refidx1[xoff/2][yoff/2]=-1;

        predmodeA=2;
        predmodeB=2;


        if(xoff>0 && constrained_intra_pred_flag==0  && yoff>0 )
        {
            predmodeA=pred_mode[xoff-1][yoff];
            predmodeB=pred_mode[xoff][yoff-1];
        }

        modetmp=MIN(predmodeA,predmodeB);


        previntramodeflag=u_1(nalu);
        #if _N_HLS_
        fprintf(trace_bit,"%s %*d\n","prev_intra_pred_mode_flag",50-strlen("prev_intra_pred_mode_flag"),previntramodeflag);
        #endif // _N_HLS_

        if(previntramodeflag)
        {

           tmpintramode[k]=modetmp;

        }
        else
        {
            tmpintramode[k]=u_n(3,nalu);
            #if _N_HLS_
            fprintf(trace_bit,"%s %*d\n","rem_intra_pred_mode",50-strlen("rem_intra_pred_mode"),tmpintramode[k]);
            #endif // _N_HLS_

            if(tmpintramode[k]>=modetmp)
                tmpintramode[k]=tmpintramode[k]+1;
        }
        pred_mode[xoff][yoff]=tmpintramode[k];
    }







}





#define VERT_PRED             0
#define HOR_PRED              1
#define DC_PRED               2
#define DIAG_DOWN_LEFT_PRED   3
#define DIAG_DOWN_RIGHT_PRED  4
#define VERT_RIGHT_PRED       5
#define HOR_DOWN_PRED         6
#define VERT_LEFT_PRED        7
#define HOR_UP_PRED           8




// Notation for comments regarding prediction and predictors.
// The pels of the 4x4 block are labelled a..p. The predictor pels above
// are labelled A..H, from the left I..L, and from above left X, as follows:
//
//  X A B C D E F G H
//  I a b c d
//  J e f g h
//  K i j k l
//  L m n o p
//
void predict_intra4x4_luma_NonField(
	unsigned char predL[4][4],
	unsigned char Sluma[PicWidthInSamplesL][FrameHeightInSampleL],
	unsigned char predmode,
	unsigned char avaiMode,
	int startx,
	int starty,
	unsigned int blk)
{
#pragma HLS pipeline


	unsigned char P_X, P_A, P_B,P_C,P_D,P_E,P_F,P_G,P_H,P_I,P_J,P_K,P_L;


	unsigned char s0;
	if (avaiMode& 0x01)
	{
		P_A = Sluma[startx][starty-1];
		P_B = Sluma[startx+1][starty-1];
		P_C = Sluma[startx+2][starty-1];
		P_D = Sluma[startx+3][starty-1];
	}
	else
	{
		P_A = P_B = P_C = P_D = 128;
	}

	int i,j;
	    if( blk==3 || blk==11  || blk==13 || blk==7  || blk==15 || !(avaiMode & 0x01) || startx+4 >= PicWidthInSamplesL || starty==0)
	{
		P_E = P_F = P_G = P_H = P_D;
	}
	else
	{
		P_E = Sluma[startx+4][starty-1];
		P_F = Sluma[startx+5][starty-1];
		P_G = Sluma[startx+6][starty-1];
		P_H = Sluma[startx+7][starty-1];
	}

	if (avaiMode& 0x02)
	{
		P_I = Sluma[startx-1][starty];
		P_J = Sluma[startx-1][starty+1];
		P_K = Sluma[startx-1][starty+2];
		P_L = Sluma[startx-1][starty+3];
	}
	else
	{
		P_I = P_J = P_K = P_L = 128;
	}
	if(avaiMode == 0x03)
    {
        P_X=Sluma[startx-1][starty-1];
    }
    else
    {
        P_X=128;
    }

switch (predmode)
  {
  case DC_PRED:                         /* DC prediction */

    s0 = 0;
    switch (avaiMode)
	{
		case 3:
		{
		  // no edge
		  s0 = (P_A + P_B + P_C + P_D + P_I + P_J + P_K + P_L + 4)>>3;
		}
		break;
		case 2:
		{
		  // upper edge
		  s0 = (P_I + P_J + P_K + P_L + 2)>>2;
		}
		break;
	   case 1:
		{
		  // left edge
		  s0 = (P_A + P_B + P_C + P_D + 2)>>2;
		}
		break;
		default:
		{
		  // top left corner, nothing to predict from
		  s0 = 128;
		}
		break;
	}

    for (j=0; j < 4; j++)
    {
    // HLS PIPELINE
      for (i=0; i <4; i++)
      {

        // store DC prediction
        predL[i][j] = s0;
      }
    }
    break;


  case VERT_PRED:                       /* vertical prediction from block above */


      for(i=0;i<4;i++)
	  {
        predL[0][i]=P_A;
		predL[1][i]=P_B;
		predL[2][i]=P_C;
		predL[3][i]=P_D;
	  }
    break;

  case HOR_PRED:  /* horizontal prediction from left block */

      for(i=0;i<4;i++)
	  {
        predL[i][0]=P_I;
		predL[i][1]=P_J;
		predL[i][2]=P_K;
		predL[i][3]=P_L;
	  }
    break;

  case DIAG_DOWN_RIGHT_PRED:
   // if ((!block_available_up)||(!block_available_left)||(!block_available_up_left))
    //  printf ("warning: Intra_4x4_Diagonal_Down_Right prediction mode not allowed at mb %d\n",img->current_mb_nr);

    predL[0 ][3 ] = (P_L + 2*P_K + P_J + 2) / 4;
    predL[0 ][2 ] =
    predL[1 ][3 ] = (P_K + 2*P_J + P_I + 2) / 4;
    predL[0 ][1 ] =
    predL[1 ][2 ] =
    predL[2 ][3 ] = (P_J + 2*P_I + P_X + 2) / 4;
    predL[0 ][0 ] =
    predL[1 ][1 ] =
    predL[2 ][2 ] =
    predL[3 ][3 ] = (P_I + 2*P_X + P_A + 2) / 4;
    predL[1 ][0 ] =
    predL[2 ][1 ] =
    predL[3 ][2 ] = (P_X + 2*P_A + P_B + 2) / 4;
    predL[2 ][0 ] =
    predL[3 ][1 ] = (P_A + 2*P_B + P_C + 2) / 4;
    predL[3 ][0 ] = (P_B + 2*P_C + P_D + 2) / 4;
    break;

  case DIAG_DOWN_LEFT_PRED:
    //if (!block_available_up)
     // printf ("warning: Intra_4x4_Diagonal_Down_Left prediction mode not allowed at mb %d\n",img->current_mb_nr);

    predL[0 ][0 ] = (P_A + P_C + 2*(P_B) + 2) / 4;
    predL[1 ][0 ] =
    predL[0 ][1 ] = (P_B + P_D + 2*(P_C) + 2) / 4;
    predL[2 ][0 ] =
    predL[1 ][1 ] =
    predL[0 ][2 ] = (P_C + P_E + 2*(P_D) + 2) / 4;
    predL[3 ][0 ] =
    predL[2 ][1 ] =
    predL[1 ][2 ] =
    predL[0 ][3 ] = (P_D + P_F + 2*(P_E) + 2) / 4;
    predL[3 ][1 ] =
    predL[2 ][2 ] =
    predL[1 ][3 ] = (P_E + P_G + 2*(P_F) + 2) / 4;
    predL[3 ][2 ] =
    predL[2 ][3 ] = (P_F + P_H + 2*(P_G) + 2) / 4;
    predL[3 ][3 ] = (P_G + 3*(P_H) + 2) / 4;
    break;

  case  VERT_RIGHT_PRED:/* diagonal prediction -22.5 deg to horizontal plane */
   // if ((!block_available_up)||(!block_available_left)||(!block_available_up_left))
     // printf ("warning: Intra_4x4_Vertical_Right prediction mode not allowed at mb %d\n",img->current_mb_nr);

    predL[0 ][0 ] =
    predL[1 ][2 ] = (P_X + P_A + 1) / 2;
    predL[1 ][0 ] =
    predL[2 ][2 ] = (P_A + P_B + 1) / 2;
    predL[2 ][0 ] =
    predL[3 ][2 ] = (P_B + P_C + 1) / 2;
    predL[3 ][0 ] = (P_C + P_D + 1) / 2;
    predL[0 ][1 ] =
    predL[1 ][3 ] = (P_I + 2*P_X + P_A + 2) / 4;
    predL[1 ][1 ] =
    predL[2 ][3 ] = (P_X + 2*P_A + P_B + 2) / 4;
    predL[2 ][1 ] =
    predL[3 ][3 ] = (P_A + 2*P_B + P_C + 2) / 4;
    predL[3 ][1 ] = (P_B + 2*P_C + P_D + 2) / 4;
    predL[0 ][2 ] = (P_X + 2*P_I + P_J + 2) / 4;
    predL[0 ][3 ] = (P_I + 2*P_J + P_K + 2) / 4;
    break;

  case  VERT_LEFT_PRED:/* diagonal prediction -22.5 deg to horizontal plane */
   // if (!block_available_up)
    //  printf ("warning: Intra_4x4_Vertical_Left prediction mode not allowed at mb %d\n",img->current_mb_nr);

    predL[0 ][0 ] = (P_A + P_B + 1) / 2;
    predL[1 ][0 ] =
    predL[0 ][2 ] = (P_B + P_C + 1) / 2;
    predL[2 ][0 ] =
    predL[1 ][2 ] = (P_C + P_D + 1) / 2;
    predL[3 ][0 ] =
    predL[2 ][2 ] = (P_D + P_E + 1) / 2;
    predL[3 ][2 ] = (P_E + P_F + 1) / 2;
    predL[0 ][1 ] = (P_A + 2*P_B + P_C + 2) / 4;
    predL[1 ][1 ] =
    predL[0 ][3 ] = (P_B + 2*P_C + P_D + 2) / 4;
    predL[2 ][1 ] =
    predL[1 ][3 ] = (P_C + 2*P_D + P_E + 2) / 4;
    predL[3 ][1 ] =
    predL[2 ][3 ] = (P_D + 2*P_E + P_F + 2) / 4;
    predL[3 ][3 ] = (P_E + 2*P_F + P_G + 2) / 4;
    break;

  case  HOR_UP_PRED:/* diagonal prediction -22.5 deg to horizontal plane */
    //if (!block_available_left)
     // printf ("warning: Intra_4x4_Horizontal_Up prediction mode not allowed at mb %d\n",img->current_mb_nr);

    predL[0 ][0 ] = (P_I + P_J + 1) / 2;
    predL[1 ][0 ] = (P_I + 2*P_J + P_K + 2) / 4;
    predL[2 ][0 ] =
    predL[0 ][1 ] = (P_J + P_K + 1) / 2;
    predL[3 ][0 ] =
    predL[1 ][1 ] = (P_J + 2*P_K + P_L + 2) / 4;
    predL[2 ][1 ] =
    predL[0 ][2 ] = (P_K + P_L + 1) / 2;
    predL[3 ][1 ] =
    predL[1 ][2 ] = (P_K + 2*P_L + P_L + 2) / 4;
    predL[3 ][2 ] =
    predL[1 ][3 ] =
    predL[0 ][3 ] =
    predL[2 ][2 ] =
    predL[2 ][3 ] =
    predL[3 ][3 ] = P_L;
    break;

  case  HOR_DOWN_PRED:/* diagonal prediction -22.5 deg to horizontal plane */
   // if ((!block_available_up)||(!block_available_left)||(!block_available_up_left))
     // printf ("warning: Intra_4x4_Horizontal_Down prediction mode not allowed at mb %d\n",img->current_mb_nr);

    predL[0 ][0 ] =
    predL[2 ][1 ] = (P_X + P_I + 1) / 2;
    predL[1 ][0 ] =
    predL[3 ][1 ] = (P_I + 2*P_X + P_A + 2) / 4;
    predL[2 ][0 ] = (P_X + 2*P_A + P_B + 2) / 4;
    predL[3 ][0 ] = (P_A + 2*P_B + P_C + 2) / 4;
    predL[0 ][1 ] =
    predL[2 ][2 ] = (P_I + P_J + 1) / 2;
    predL[1 ][1 ] =
    predL[3 ][2 ] = (P_X + 2*P_I + P_J + 2) / 4;
    predL[0 ][2 ] =
    predL[2 ][3 ] = (P_J + P_K + 1) / 2;
    predL[1 ][2 ] =
    predL[3 ][3 ] = (P_I + 2*P_J + P_K + 2) / 4;
    predL[0 ][3 ] = (P_K + P_L + 1) / 2;
    predL[1 ][3 ] = (P_J + 2*P_K + P_L + 2) / 4;
    break;

  default:
    printf("Error: illegal intra_4x4 prediction mode: %d\n",predmode);
    break;
  }


}

// for this part, I have some other ideas about

void predict_intra16x16_luma_NonField(unsigned char predL[16][4][4], unsigned char Sluma[PicWidthInSamplesL][FrameHeightInSampleL] , unsigned char predmode, unsigned char avaiMode, unsigned int startx, unsigned int starty)
{

	int i,j,k;
	int x,y;
	unsigned char v[16];
	unsigned char h[16];
	unsigned char X;

    if(avaiMode/2)
        for(i=0;i<16;i++) h[i]=Sluma[startx-1][starty+i];
    else
        for(i=0;i<16;i++) h[i]=128;


    if(avaiMode%2)
        for(i=0;i<16;i++) v[i]=Sluma[startx+i][starty-1];
    else
        for(i=0;i<16;i++)  v[i]=128;


    if(avaiMode==3)
    {
        X=Sluma[startx-1][starty-1];
    }
    else
        X=128;

	switch(predmode)
	{
		case 0:
		{

		    for(k=0;k<16;k++)
                for(i=0;i<4;i++)
                    for(j=0;j<4;j++)
            {
                x=KTOX(k)*4+i;

                predL[k][i][j]=v[x];
            }
		}
		break;
		case 1:
		{

            for(k=0;k<16;k++)
                for(i=0;i<4;i++)
                    for(j=0;j<4;j++)
            {
                y=KTOY(k)*4+j;

                predL[k][i][j]=h[y];
            }
		}
		break;
		case 2:
		{

			int sumx=0;
			int sumy=0;

			if(avaiMode%2)
			for(x=0;x<16;x++)
			{
				// HLS PIPELINE

				sumx=sumx+v[x];

			}

			if(avaiMode/2)
			for(y=0;y<16;y++)
			{
				// HLS PIPELINE

				sumy=sumy+h[y];
			}

			int temp;
			switch (avaiMode)
			{
				case 3:
					temp = (sumx+sumy+16)>>5;
					break;
				case 2:
					temp = (sumy+8)>>4;
					break;
				case 1:
					temp = (sumx+8)>>4;
					break;
				case 0:
					temp = 128;
			}


            for(k=0;k<16;k++)
                for(i=0;i<4;i++)
                    for(j=0;j<4;j++)
            {
                predL[k][i][j]=temp;
            }


		}
		break;
		default:
		{



			int H;
			int V;
			int a;
			int tmp;


            H=v[8]-v[6]+2*(v[9]-v[5])+3*(v[10]-v[4])+4*(v[11]-v[3])+5*(v[12]-v[2])+6*(v[13]-v[1])+7*(v[14]-v[0])+8*(v[15]-X);
            V=h[8]-h[6]+2*(h[9]-h[5])+3*(h[10]-h[4])+4*(h[11]-h[3])+5*(h[12]-h[2])+6*(h[13]-h[1])+7*(h[14]-h[0])+8*(h[15]-X);


			H=(5*H+32)>>6;
			V=(5*V+32)>>6;
			a=16*(v[15]+h[15]);


            for(k=0;k<16;k++)
            for(i=0;i<4;i++)
            for(j=0;j<4;j++)
            {
                x=KTOX(k)*4+i;
                y=KTOY(k)*4+j;
                tmp=(a+H*(x-7)+V*(y-7)+16)>>5;

                if(tmp<0)
						predL[k][i][j]=0;
					else if(tmp>255)
						predL[k][i][j]=255;
					else
						predL[k][i][j]=tmp;
            }
		}
		break;
	}
}


//  X
//  v0 a b c d
//  v1 e f g h
//  v2 i j k l
//  v3 m n o p
//  v4 0 0 0 0
//  v5 0 0 0 0
//  v6 0 0 0 0
void prediction_Chroma(
	unsigned char predC[4][4][4],
	unsigned char SChroma[PicWidthInSamplesC][FrameHeightInSampleC],
	unsigned char avaimode,
	int startx,
	int starty,
	unsigned char pred_mod)
{

    unsigned char v[8];
#pragma HLS ARRAY_PARTITION variable=v complete dim=1
    unsigned char h[8];
#pragma HLS ARRAY_PARTITION variable=h complete dim=1

    unsigned char x,y;
    unsigned char X;
    int H,V;
    int i,j,k;
    int a;
    int temp;


    for(i=0;i<8;i++)
    {
#pragma HLS unroll factor=2
		if(avaimode/2)
			 h[i]=SChroma[startx-1][starty+i];
		else
			 h[i]=128;
    }


    if(avaimode%2)
        for(i=0;i<8;i++) v[i]=SChroma[startx+i][starty-1];
    else
        for(i=0;i<8;i++)  v[i]=128;


    if(avaimode==3)
    {
        X=SChroma[startx-1][starty-1];
    }
    else
        X=128;



	switch(pred_mod)
	{
		case 0:
		//prediction_Chroma_DC(predC,SChroma,avaimode,startx,starty);
		{
            int js1,js2,js3,js0;
            js0=0;
            js1=0;
            js2=0;
            js3=0;


            if(avaimode%2)
            {
                for(x=0;x<4;x++)
                {
                    js0+=v[x];
                    js1+=v[x+4];
                }
            }
            if(avaimode/2)
            {
                for(y=0;y<4;y++)
                {
                    js2+=h[y];
                    js3+=h[y+4];
                }
            }

            int temp[2][2];

            switch(avaimode)
            {
                case 0:
                temp[0][0] = 128;
                temp[0][1] = 128;
                temp[1][0] = 128;
                temp[1][1] = 128;
                break;
                case 1:
                temp[0][0] = (js0 + 2) >> 2;
                temp[0][1] = (js1 + 2) >> 2;
                temp[1][0] = (js0 + 2) >> 2;
                temp[1][1]=  (js1 + 2) >> 2;
                break;
                case 2:
                temp[0][0] = (js2 + 2) >> 2;
                temp[0][1] = (js2 + 2) >> 2;
                temp[1][0] = (js3 + 2) >> 2;
                temp[1][1] = (js3 + 2) >> 2;
                break;
                default:
                temp[0][0] = (js2 + js0 + 4) >> 3;
                temp[0][1] = (js1 + 2) >> 2;
                temp[1][0] = (js3 + 2) >> 2;
                temp[1][1] = (js1 + js3 + 4) >> 3;
                break;
            }

            int i,j;

            for(i=0;i<2;i++)
#pragma HLS pipeline
                for(j=0;j<2;j++)
                    for(x=0;x<4;x++)
                        for(y=0;y<4;y++)
                        {
                            predC[j+i*2][x][y]=temp[i][j];
                        }
		}
		break;
		case 1:
		   // prediction_Chroma_Horizontal(predC,SChroma,startx,starty);


            for(k=0;k<4;k++)
#pragma HLS pipeline
                for(i=0;i<4;i++)
                for(j=0;j<4;j++)
            {
                predC[k][i][j]=h[ (k/2)*4+j];
            }
		break;
		case 2:
		    //prediction_Chroma_Vertical(predC,SChroma,startx,starty);
            for(k=0;k<4;k++)
                for(i=0;i<4;i++)
                for(j=0;j<4;j++)
            {
                predC[k][i][j]=v[ (k%2)*4+i];
            }
		break;
		default:
		//prediction_Chroma_Plane(predC,SChroma,startx,starty);
		//flattem


		H=v[4]-v[2] +2*(v[5]-v[1])+3*(v[6]-v[0])+4*(v[7]-X);
        V=h[4]-h[2] +2*(h[5]-h[1])+3*(h[6]-h[0])+4*(h[7]-X);
        H=(17*H+16)>>5;
        V=(17*V+16)>>5;
        a=16*(v[7]+h[7]);


            for(k=0;k<4;k++)
#pragma HLS pipeline
            for(i=0;i<4;i++)
            for(j=0;j<4;j++)
            {
                temp= (a+H*( (k%2)*4+i-3)+V*((k/2)*4+j-3)+16)>>5;
            if(temp<0)
				predC[k][i][j]=0;
			else if(temp>255)
				predC[k][i][j]=255;
			else
				predC[k][i][j]=temp;
            }
		break;
	}

}


