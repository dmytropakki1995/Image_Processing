#include "imgArr.h"

enum Filter2DTypes
{
	Dx_2S_2D,
	Dy_2S_2D,
	Dxx_3S_2D,
	Dyy_3S_2D,
	Dxy_3S_2D,
	Laplace_3S_2D,
	smooth_2S_2D,
	smooth_3S_2D,
	smooth_4S_2D,
	smooth_5S_2D,
	smooth_6S_2D,
	DOG_5S_2D
};

enum Filter1DTypes
{
	Dx_3S_1D,
	Dxx_3S_1D,
	Dxxx_4S_1D,
	smooth_2S_1D,
	smooth_3S_1D,
	smooth_4S_1D,
	smooth_5S_1D,
	smooth_6S_1D,
};

filter createWithSelectedType2D(Filter2DTypes type);
filter createWithSelectedType1D(Filter1DTypes type);

