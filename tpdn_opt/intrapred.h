
#ifndef _INTRAPRED_H_
#define _INTRAPRED_H_

void IntraInfo(
NALU_t* nalu,
char pred_mode[PicWidthInMBs*4][FrameHeightInMbs*4],
char refidx0[PicWidthInMBs*2][FrameHeightInMbs*2],
char refidx1[PicWidthInMBs*2][FrameHeightInMbs*2],
char tmpintramode[16],
unsigned char constrained_intra_pred_flag,
int startx,
int starty);
void prediction_Chroma(
	unsigned char predC[4][4][4],
	unsigned char SChroma[PicWidthInSamplesC][FrameHeightInSampleC],
	unsigned char avaimode,
	int startx,
	int starty,
	unsigned char pred_mod);

void predict_intra16x16_luma_NonField(unsigned char predL[16][4][4], unsigned char Sluma[PicWidthInSamplesL][FrameHeightInSampleL] , unsigned char predmode, unsigned char avaiMode, unsigned int startx, unsigned int starty);
void predict_intra4x4_luma_NonField(
	unsigned char predL[4][4],
	unsigned char Sluma[PicWidthInSamplesL][FrameHeightInSampleL],
	unsigned char predmode,
	unsigned char avaiMode,
	int startx,
	int starty,
	unsigned int blk);
#endif // _INTRAPRED_H
