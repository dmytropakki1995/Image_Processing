#include "FilterFactory.h"
#include "imgArr.h"

filter createWithSelectedType2D(Filter2DTypes type)
{
	filter result;
	float * A = nullptr;
	switch (type)
	{
	case Dx_2S_2D:
	{
		A = getEmptyArray(4);
		A[0] = 1; A[1] = -1;
		A[2] = 1; A[3] = -1;
		result = createFilter(A, 2, 2, 2);
		return result;
	}
	case Dy_2S_2D:
	{
		A = getEmptyArray(4);
		A[0]= 1;   A[1] = 1;
		A[2] = -1; A[3] = -1;
		result = createFilter(A, 2, 2, 2);
		return result;
	}
	case Dxx_3S_2D:
	{
		A = getEmptyArray(9);
		A[0] = 1; A[1] = -2; A[2] = 1;
		A[3] = 2; A[4] = -4; A[5] = 2;
		A[6] = 1; A[7] = -2; A[8] = 1;
		result = createFilter(A, 3, 4, 2);
		return result;
	}
	case Dyy_3S_2D:
	{
		A = getEmptyArray(9);
		A[0] = 1;  A[1] = 2;  A[2] = 1;
		A[3] = -2; A[4] = -4; A[5] = -2;
		A[6] = 1;  A[7] = 2;  A[8] = 1;
		result = createFilter(A, 3, 4, 2);
		return result;
	}
	case Dxy_3S_2D:
	{
		A = getEmptyArray(9);
		A[0] = 1;   A[1] = 0;  A[2] = -1;
		A[3] = 0;   A[4] = 0;  A[5] = 0;
		A[6] = -1;  A[7] = 0;  A[8] = 1;
		result = createFilter(A, 3, 4, 2);
		return result;
	}
	case Laplace_3S_2D:
	{
		A = getEmptyArray(9);
		A[0] = 2; A[1] = 0;  A[2] = 2;
		A[3] = 0; A[4] = -8; A[5] = 0;
		A[6] = 2; A[7] = 0;  A[8] = 2;
		result = createFilter(A, 3, 4, 2);
		return result;
	}

	case smooth_2S_2D:
	{
		A = getEmptyArray(4);
		A[0] = 1; A[1] = 1;
		A[2] = 1; A[3] = 1;
		result = createFilter(A, 2, 2, 2);
		return result;
	}
	case smooth_3S_2D:
	{
		A = getEmptyArray(9);

		A[0] = 1; A[1] = 2; A[2] = 1;
		A[3] = 2; A[4] = 4; A[5] = 2;
		A[6] = 1; A[7] = 2; A[8] = 1;

		result = createFilter(A, 3, 16, 2);
		return result;
	}
	case smooth_4S_2D:
	{
		A = getEmptyArray(16);
		A[0] = 1; A[1] = 3; A[2] = 3; A[3] = 1;
		A[4] = 3; A[5] = 9; A[6] = 9; A[7] = 3;
		A[8] = 3; A[9] = 9; A[10] = 9; A[11] = 3;
		A[12] = 1; A[13] = 3; A[14] = 3; A[15] = 1;
		result = createFilter(A, 4, 64, 2);
		return result;
	}
	case smooth_5S_2D:
	{
		A = getEmptyArray(25);

		A[0] = 1;  A[1] = 4;   A[2] = 6;   A[3] = 4;   A[4] = 1;
		A[5] = 4;  A[6] = 16;  A[7] = 24;  A[8] = 16;  A[9] = 4;
		A[10] = 6; A[11] = 24; A[12] = 36; A[13] = 24; A[14] = 6;
		A[15] = 4; A[16] = 16; A[17] = 24; A[18] = 16; A[19] = 4;
		A[20] = 1; A[21] = 4;  A[22] = 6;  A[23] = 4;  A[24] = 1;

		result = createFilter(A, 5, 256, 2);
		return result;

	}
	case smooth_6S_2D:
	{
		A = getEmptyArray(36);

		A[0] = 1;   A[1] = 5;   A[2] = 10;   A[3] = 10;    A[4] = 5;    A[5] = 1;
		A[6] = 5;   A[7] = 25;  A[8] = 50;   A[9] = 50;    A[10] = 25;  A[11] = 5;
		A[12] = 10; A[13] = 50; A[14] = 100; A[15] = 100;  A[16] = 50;  A[17] = 10;
		A[18] = 10; A[19] = 50; A[20] = 100; A[21] = 100;  A[22] = 50;  A[23] = 10;
		A[24] = 5;  A[25] = 25; A[26] = 50;  A[27] = 50;   A[28] = 25;  A[29] = 5;
		A[30] = 1;  A[31] = 5;  A[32] = 10;  A[33] = 10;   A[34] = 5;   A[35] = 1;

		result = createFilter(A, 5, 1024, 2);
		return result;
	}
	case DOG_5S_2D:
	{
		A = getEmptyArray(25);

		A[0] = 1;    A[1] = -12;  A[2] = -26;  A[3] = -12;  A[4] = 1;
		A[5] = -12;  A[6] = -16;  A[7] = -40;  A[8] = -16;  A[9] = -12;
		A[10] = -26; A[11] = -40; A[12] = -92; A[13] = -40; A[14] = -26;
		A[15] = -12; A[16] = -16; A[17] = -40; A[18] = -16; A[19] = -12;
		A[20] = 1;   A[21] = -12; A[22] = -26; A[23] = -12; A[24] = 1;

		result = createFilter(A, 5, 256, 2);
		return result;
	}
	}
	return result;
}

filter createWithSelectedType1D(Filter1DTypes type)
{
	filter result;
	result.dimension = 1;
	float * A = nullptr;
	switch (type)
	{
	case Dx_3S_1D:
	{
		A = getEmptyArray(3);
		A[0] = -1; A[1] = 0; A[2] = 1;
		result = createFilter(A, 3, 2, 1);
		return result;
	}
	case Dxx_3S_1D:
	{
		A = getEmptyArray(3);
		A[0] = 1; A[1] = -2; A[2] = 1;
		result = createFilter(A, 3, 4, 1);
		return result;
	}
	case smooth_2S_1D:
	{
		A = getEmptyArray(2);
		A[0] = 1; A[1] = 1;
		result = createFilter(A, 2, 2, 1);
		return result;

	}
	case smooth_3S_1D:
	{
		A = getEmptyArray(3);
		A[0] = 1; A[1] = 2; A[2] = 1;
		result = createFilter(A, 3, 4, 1);
		return result;
	}
	case smooth_4S_1D:
	{
		A = getEmptyArray(4);
		A[0] = 1; A[1] = 3; A[2] = 3; A[3] = 1;
		result = createFilter(A, 4, 8, 1);
		return result;
	}
	case smooth_5S_1D:
	{
		A = getEmptyArray(5);
		A[0] = 1; A[1] = 4; A[2] = 6; A[3] = 4; A[4] = 1;
		result = createFilter(A, 5, 16, 1);
		return result;
	}
	case smooth_6S_1D:
	{
		A = getEmptyArray(6);
		A[0] = 1; A[1] = 5; A[2] = 10; A[3] = 10; A[4] = 5; A[5] = 1;
		result = createFilter(A, 6, 32, 1);
		return result;
	}
	}
	return result;
}
