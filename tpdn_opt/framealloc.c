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
                //if(tpvalue1>curFrame_NUM)
                    //tpvalue1=tpvalue1- maxframNUM;
            }
            else
            tpvalue1=-1;


            if(PicBff[listX[j+1]].use_for_char )
            {
                tpvalue2=PicBff[listX[j+1]].frame_num;
                //if(tpvalue2>curFrame_NUM)
                    //tpvalue2=tpvalue2- maxframNUM;
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

/*    for(i=0;i<MAX_REFERENCE_PICTURES;i++)
    {
        list0[i]=i;
    }

    for(i=0;i<MAX_REFERENCE_PICTURES;i++)
    {
        for(j=i;j<MAX_REFERENCE_PICTURES-1;j++)
        {
            if(PicBff[list0[j]].use_for_char )
            {
                tpvalue1=PicBff[list0[j]].Picorder_num;
            }
            else
            tpvalue1=-65535;


            if(PicBff[list0[j+1]].use_for_char)
            {
                tpvalue2=PicBff[list0[j+1]].Picorder_num;
            }
            else
                tpvalue2=-65535;

            if(tpvalue1<tpvalue2)
            {
                temp=list1[j];
                list1[j]=list1[j+1];
                list1[j+1]=temp;
            }

        }
    }

    for(i=0;i<MAX_REFERENCE_PICTURES;i++)
    {
        list1[i]=i;
    }

    for(i=0;i<MAX_REFERENCE_PICTURES;i++)
    {
        for(j=i;j<MAX_REFERENCE_PICTURES-1;j++)
        {
            if(PicBff[list1[j]].use_for_char )
            {
                tpvalue1=PicBff[list1[j]].Picorder_num;
            }
            else
            tpvalue1=65535;


            if(PicBff[list1[j+1]].use_for_char )
            {
                tpvalue2=PicBff[list1[j+1]].Picorder_num;
            }
            else
            tpvalue2=65535;

            if(tpvalue1>tpvalue2)
            {
                temp=list1[j];
                list1[j]=list1[j+1];
                list1[j+1]=temp;
            }

        }
    }*/
}
