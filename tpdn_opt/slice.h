
#ifndef _SLICE_H_
#define _SLICE_H_

void ProcessSlice
(
    NALU_t* nalu,
    StorablePicture PIC[MAX_REFERENCE_PICTURES],
    StorablePictureInfo  PICINFO[MAX_REFERENCE_PICTURES],
    unsigned char Imode[PicWidthInMBs][FrameHeightInMbs],
    char IntraPredMode[PicWidthInMBs*4][FrameHeightInMbs*4],
    unsigned char NzLuma[PicWidthInMBs*4][FrameHeightInMbs*4],
    unsigned char NzChroma[2][PicWidthInMBs*2][FrameHeightInMbs*2],
    unsigned char constraint_intra_flag,
    slice_header_rbsp_t *SH,
    ImageParameters* img
);

#endif // _SLICE_H_
