#ifndef _FRAMEALLOC_H_
#define _FRAMEALLOC_H_

void init_listB(StorablePicture PicBff[MAX_REFERENCE_PICTURES],int list0[MAX_REFERENCE_PICTURES], int list1[MAX_REFERENCE_PICTURES], int curPOC);
void init_listP(StorablePicture PicBff[MAX_REFERENCE_PICTURES], int listX[MAX_REFERENCE_PICTURES],int curFrame_NUM, int maxframNUM);
#endif // _FRAMEALLOC_H_
