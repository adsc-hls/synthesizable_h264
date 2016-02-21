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
#include "mathfunc.h"

int Clip1y(int x)
{
	/* bidDepth regarded as 8 */
	if(255< x)
	{
		return 255;
	}
	else if (0 > x)
	{
		return 0;
	}
	else
	{
		return x;
	}
	return 0;
}

int Clip1c(int x)
{
	/* bidDepth regarded as 8 */
	if(255< x)
	{
		return 255;
	}
	else if (0 > x)
	{
		return 0;
	}
	else
	{
		return x;
	}
	return 0;
}

int Clip3(int x, int y, int z)
{
	if(z < x)
	{
		return x;
	}
	else if (z > y)
	{
		return y;
	}
	else
	{
		return z;
	}
	return 0;
}

int minpositive(int x, int y)
{
    if(x>=0 && y>=0)
    {
        if(x>y)
            return y;
        else
            return x;
    }
    else
    {
        if(x<y)
            return y;
        else
            return x;
    }
}


int InverseRasterScan(int a, int b, int c, int d, int e)
{
	if(e==0)
	{
		return (a%(d/b))*b;
	}
	else
	{
		return (a/(d/b))*c;
	}
	return 0;
}
//this function returns whether only one of x,y,z equals to a;

unsigned char on_of_three(unsigned char a, unsigned char x, unsigned char y, unsigned char z)
{

    if( ( x==a && y!=a && z!=a  ) ||
       ( x!=a && y==a && z!=a  ) ||
       ( x!=a && y!=a && z==a  )
        )
            return 1;
    else
        return 0;
}

// this function find the median of three value

int median(int x, int y, int z)
{
    if(x>=y)
    {
        if(y>=z)
           return y;
        else if( x>=z)
            return z;
        else
            return x;

    }
    else
    {
        if(x>=z)
           return x;
        else if( y>=z)
            return z;
        else
            return y;
    }
}


int six_tab(int a, int b, int c, int d, int e, int f)
{
	return a-5*b+20*c+20*d-5*e+f;
}

