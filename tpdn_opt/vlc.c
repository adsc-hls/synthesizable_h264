#include "global.h"
const unsigned char NCBP[48][2]=
{
  {47, 0},{31,16},{15, 1},{ 0, 2},{23, 4},{27, 8},{29,32},{30, 3},{ 7, 5},{11,10},{13,12},{14,15},
  {39,47},{43, 7},{45,11},{46,13},{16,14},{ 3, 6},{ 5, 9},{10,31},{12,35},{19,37},{21,42},{26,44},
  {28,33},{35,34},{37,36},{42,40},{44,39},{ 1,43},{ 2,45},{ 4,46},{ 8,17},{17,18},{18,20},{20,24},
  {24,19},{ 6,21},{ 9,26},{22,28},{25,23},{32,27},{33,29},{34,30},{36,22},{40,25},{38,38},{41,41},
};

unsigned char u_1(NALU_t* nalu)
{
	int bitpos;
	int bytepos;
	bytepos=nalu->bit_offset/8;
	bitpos=7-nalu->bit_offset%8;
	nalu->bit_offset++;

	return  (unsigned char )(((nalu->buf[bytepos])>>(bitpos))& 0x01);
}

unsigned char u_8(NALU_t* nalu)
{
	int bytepos;
	#if _N_HLS_
	if(nalu-> bit_offset%8)
	{
		puts("Encountering exception: u_8 not byte aligned");
	}
	#endif


	bytepos=nalu-> bit_offset/8;
	nalu-> bit_offset+=8;

	return nalu->buf[bytepos];
}


unsigned int bytes_reverse_32(unsigned int num)
{
	// Reverse a 32-bit integer
	// e.g abcd --> dcba
	// whereas a,b,c,d are bytes

	return  (((num & 0xFF)) << 24) |
		(((num >> 8) & 0xFF) << 16) |
		(((num >> 16) & 0xFF) << 8) |
		(((num >> 24) & 0xFF));
}


unsigned int u_n(int n, NALU_t* nalu)
{
	/*
	int ret=0;
	int i;
	for(i=0;i<n;i++)
	{
		ret=(ret<<1)|u_1(nalu);
	}
	return ret;
	*/

	// TAN: Get rid of the ForLoop by loading
	// 4 bytes and to n-bit grouping with
	// appropriate bit shift and masking
	// NOTE: the below algorithm can only work
	// if n is not greater than 16 (so no more than
	// 3 bytes are loaded).
	assert(n <= 16);

	int first_byte = nalu->bit_offset / 8;
	int last_byte = (nalu->bit_offset + n) / 8;
	int num_bytes = last_byte - first_byte + 1;
	int last_bit = 7 - (nalu->bit_offset + n) % 8;

	// Loading 4 consecutive bytes to temp by casting
	unsigned int *temp = (unsigned int*)&nalu->buf[first_byte];
	unsigned int temp0 = bytes_reverse_32(*temp);
	unsigned int temp1 = temp0 >> (8 * (4 - num_bytes));
	unsigned int temp2 = temp1 >> (last_bit + 1);
	int ret = temp2 & ((1 << n) - 1);
	nalu->bit_offset += n;

	return ret;
}

unsigned int u_e(NALU_t* nalu)
{
	int leadingZeroBits=0;
	while(!(u_1(nalu)))
		leadingZeroBits++;
	#if _N_HLS_
	if (leadingZeroBits>31)
	{
		puts("exglomb exceed width");
		printf("%d \n",leadingZeroBits );
	}
	#endif
	return (1<<leadingZeroBits)-1+u_n(leadingZeroBits,nalu);
}

int s_e(NALU_t* nalu)
{
	int ret;
	ret=u_e(nalu);
	if(ret % 2)
		return (ret+1)/2;
	else
		return -(ret+1)/2;
}


unsigned char m_e(int prediction_type, NALU_t* nalu)
{
	int index;
	index=u_e(nalu);
	return NCBP[index][prediction_type];
}


unsigned char t_e(int n, NALU_t *nalu)
{
	if(n>1)
		return u_e(nalu);
	else
		return (u_1(nalu)!=1);
}


int showbits(int n, unsigned int temp0, int offset)
{
	/*
	int temp_out=0;
	int i;
	if(n==0)
		return -1;
	for(i=0;i<n;i++)
	{
		temp_out=temp_out<<1;
		temp_out+=u_1(nalu);
	}
	nalu->bit_offset-=n;
	return temp_out;
	*/

	// TAN: Get rid of the ForLoop by loading
	// 4 bytes and to n-bit grouping with
	// appropriate bit shift and masking
	// NOTE: the below algorithm can only work
	// if n is not greater than 16 (so no more than
	// 3 bytes are loaded).
	assert(n <= 16);

	if(n==0)
		return -1;

	// As this function does not update nalu->bit_offset
	// (unlike u_n()), we do not need to pass nalu into
	// the function. Rather, nalu->buf can be loaded prior
	// to calling this function, so that it does not need
	// to do redundant loads everytime it is being called.

	int first_byte = offset / 8;
	int last_byte = (offset + n) / 8;
	int num_bytes = last_byte - first_byte + 1;
	int last_bit = 7 - (offset + n) % 8;
	unsigned int temp1 = temp0 >> (8 * (4 - num_bytes));
	unsigned int temp2 = temp1 >> (last_bit + 1);
	int ret = temp2 & ((1 << n) - 1);
	return ret;
}








