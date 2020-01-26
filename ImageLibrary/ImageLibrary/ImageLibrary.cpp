// ImageLibrary.cpp : Defines the exported functions for the DLL application.
//
#include "stdafx.h"
#include "ImageLibrary.h"
#include <cmath>
#include <string>
#include <fstream>
#include <iostream>
#include <vector>

float * filter::_matrix = nullptr;
int filter::_size = 0;
int filter::_deviser = 0;
int filter::_dimension = 0;

static filter _filter;
static float * _EMPTY_DATA;
static float * _FILTER_DATA;
static Image _METHOD_RESULT;

void defineFilter(float * matrix, int size, int deviser, int dim)
{
	_filter._matrix = matrix;
	_filter._size = size;
	_filter._deviser = deviser;
	_filter._dimension = dim;
}

void defineFilterWithSelectedType2D(Filter2DTypes type)
{
	switch (type)
	{
	case Dx_2S_2D:
	{
		_FILTER_DATA = new float[4];
		_FILTER_DATA[0] = 1; _FILTER_DATA[1] = -1;
		_FILTER_DATA[2] = 1; _FILTER_DATA[3] = -1;

		defineFilter(_FILTER_DATA, 2, 2, 2);
		break;
	}
	case Dy_2S_2D:
	{
		_FILTER_DATA = new float[4];
		_FILTER_DATA[0] = 1;   _FILTER_DATA[1] = 1;
		_FILTER_DATA[2] = -1; _FILTER_DATA[3] = -1;

		defineFilter(_FILTER_DATA, 2, 2, 2);
		break;
	}
	case Dxx_3S_2D:
	{
		_FILTER_DATA = new float[9];
		_FILTER_DATA[0] = 1; _FILTER_DATA[1] = -2; _FILTER_DATA[2] = 1;
		_FILTER_DATA[3] = 2; _FILTER_DATA[4] = -4; _FILTER_DATA[5] = 2;
		_FILTER_DATA[6] = 1; _FILTER_DATA[7] = -2; _FILTER_DATA[8] = 1;

		defineFilter(_FILTER_DATA, 3, 4, 2);
		break;
	}
	case Dyy_3S_2D:
	{
		_FILTER_DATA = new float[9];
		_FILTER_DATA[0] = 1;  _FILTER_DATA[1] = 2;  _FILTER_DATA[2] = 1;
		_FILTER_DATA[3] = -2; _FILTER_DATA[4] = -4; _FILTER_DATA[5] = -2;
		_FILTER_DATA[6] = 1;  _FILTER_DATA[7] = 2;  _FILTER_DATA[8] = 1;

		defineFilter(_FILTER_DATA, 3, 4, 2);
		break;
	}
	case Dxy_3S_2D:
	{
		_FILTER_DATA = new float[9];
		_FILTER_DATA[0] = 1;   _FILTER_DATA[1] = 0;  _FILTER_DATA[2] = -1;
		_FILTER_DATA[3] = 0;   _FILTER_DATA[4] = 0;  _FILTER_DATA[5] = 0;
		_FILTER_DATA[6] = -1;  _FILTER_DATA[7] = 0;  _FILTER_DATA[8] = 1;

		defineFilter(_FILTER_DATA, 3, 4, 2);
		break;
	}
	case Laplace_3S_2D:
	{
		_FILTER_DATA = new float[9];
		_FILTER_DATA[0] = 2; _FILTER_DATA[1] = 0;  _FILTER_DATA[2] = 2;
		_FILTER_DATA[3] = 0; _FILTER_DATA[4] = -8; _FILTER_DATA[5] = 0;
		_FILTER_DATA[6] = 2; _FILTER_DATA[7] = 0;  _FILTER_DATA[8] = 2;

		defineFilter(_FILTER_DATA, 3, 4, 2);
		break;
	}

	case smooth_2S_2D:
	{
		_FILTER_DATA = new float[4];
		_FILTER_DATA[0] = 1; _FILTER_DATA[1] = 1;
		_FILTER_DATA[2] = 1; _FILTER_DATA[3] = 1;

		defineFilter(_FILTER_DATA, 2, 2, 2);
		break;
	}
	case smooth_3S_2D:
	{
		_FILTER_DATA = new float[9];

		_FILTER_DATA[0] = 1; _FILTER_DATA[1] = 2; _FILTER_DATA[2] = 1;
		_FILTER_DATA[3] = 2; _FILTER_DATA[4] = 4; _FILTER_DATA[5] = 2;
		_FILTER_DATA[6] = 1; _FILTER_DATA[7] = 2; _FILTER_DATA[8] = 1;

		defineFilter(_FILTER_DATA, 3, 16, 2);
		break;
	}
	case smooth_4S_2D:
	{
		_FILTER_DATA = new float[16];
		_FILTER_DATA[0]  = 1; _FILTER_DATA[1]  = 3; _FILTER_DATA[2]  = 3; _FILTER_DATA[3]  = 1;
		_FILTER_DATA[4]  = 3; _FILTER_DATA[5]  = 9; _FILTER_DATA[6]  = 9; _FILTER_DATA[7]  = 3;
		_FILTER_DATA[8]  = 3; _FILTER_DATA[9]  = 9; _FILTER_DATA[10] = 9; _FILTER_DATA[11] = 3;
		_FILTER_DATA[12] = 1; _FILTER_DATA[13] = 3; _FILTER_DATA[14] = 3; _FILTER_DATA[15] = 1;

		defineFilter(_FILTER_DATA, 4, 64, 2);
		break;
	}
	case smooth_5S_2D:
	{
		_FILTER_DATA = new float[25];

		_FILTER_DATA[0]  = 1;  _FILTER_DATA[1] = 4;   _FILTER_DATA[2] = 6;   _FILTER_DATA[3] = 4;   _FILTER_DATA[4] = 1;
		_FILTER_DATA[5]  = 4;  _FILTER_DATA[6] = 16;  _FILTER_DATA[7] = 24;  _FILTER_DATA[8] = 16;  _FILTER_DATA[9] = 4;
		_FILTER_DATA[10] = 6; _FILTER_DATA[11] = 24; _FILTER_DATA[12] = 36; _FILTER_DATA[13] = 24; _FILTER_DATA[14] = 6;
		_FILTER_DATA[15] = 4; _FILTER_DATA[16] = 16; _FILTER_DATA[17] = 24; _FILTER_DATA[18] = 16; _FILTER_DATA[19] = 4;
		_FILTER_DATA[20] = 1; _FILTER_DATA[21] = 4;  _FILTER_DATA[22] = 6;  _FILTER_DATA[23] = 4;  _FILTER_DATA[24] = 1;

		defineFilter(_FILTER_DATA, 5, 256, 2);
		break;

	}
	case smooth_6S_2D:
	{
		_FILTER_DATA = new float[36];

		_FILTER_DATA[0]  = 1;   _FILTER_DATA[1] = 5;   _FILTER_DATA[2] = 10;   _FILTER_DATA[3] = 10;   _FILTER_DATA[4]  = 5;    _FILTER_DATA[5] = 1;
		_FILTER_DATA[6]  = 5;   _FILTER_DATA[7] = 25;  _FILTER_DATA[8] = 50;   _FILTER_DATA[9] = 50;   _FILTER_DATA[10] = 25;  _FILTER_DATA[11] = 5;
		_FILTER_DATA[12] = 10; _FILTER_DATA[13] = 50; _FILTER_DATA[14] = 100; _FILTER_DATA[15] = 100;  _FILTER_DATA[16] = 50;  _FILTER_DATA[17] = 10;
		_FILTER_DATA[18] = 10; _FILTER_DATA[19] = 50; _FILTER_DATA[20] = 100; _FILTER_DATA[21] = 100;  _FILTER_DATA[22] = 50;  _FILTER_DATA[23] = 10;
		_FILTER_DATA[24] = 5;  _FILTER_DATA[25] = 25; _FILTER_DATA[26] = 50;  _FILTER_DATA[27] = 50;   _FILTER_DATA[28] = 25;  _FILTER_DATA[29] = 5;
		_FILTER_DATA[30] = 1;  _FILTER_DATA[31] = 5;  _FILTER_DATA[32] = 10;  _FILTER_DATA[33] = 10;   _FILTER_DATA[34] = 5;   _FILTER_DATA[35] = 1;

		defineFilter(_FILTER_DATA, 5, 1024, 2);
		break;
	}
	case DOG_5S_2D:
	{
		_FILTER_DATA = new float[25];

		_FILTER_DATA[0]  = 1;    _FILTER_DATA[1] = -12;  _FILTER_DATA[2] = -26;  _FILTER_DATA[3] = -12;  _FILTER_DATA[4] = 1;
		_FILTER_DATA[5]  = -12;  _FILTER_DATA[6] = -16;  _FILTER_DATA[7] = -40;  _FILTER_DATA[8] = -16;  _FILTER_DATA[9] = -12;
		_FILTER_DATA[10] = -26; _FILTER_DATA[11] = -40; _FILTER_DATA[12] = -92; _FILTER_DATA[13] = -40; _FILTER_DATA[14] = -26;
		_FILTER_DATA[15] = -12; _FILTER_DATA[16] = -16; _FILTER_DATA[17] = -40; _FILTER_DATA[18] = -16; _FILTER_DATA[19] = -12;
		_FILTER_DATA[20] = 1;   _FILTER_DATA[21] = -12; _FILTER_DATA[22] = -26; _FILTER_DATA[23] = -12; _FILTER_DATA[24] = 1;

		defineFilter(_FILTER_DATA, 5, 256, 2);
		break;
	}
	}
}

Image APPLY_FILTER(Image & image, Filter2DTypes image_type)
{
	defineFilterWithSelectedType2D(image_type);

	int size = _filter._size;
	int height = image._height - (size - 1);
	int width = image._width - (size - 1);

	_METHOD_RESULT._width = width;
	_METHOD_RESULT._height = height;
	_METHOD_RESULT._data = new float[height * width];

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			_METHOD_RESULT._data[i * width + j] = 0;

			for (int k = 0; k < size; k++)
			{
				for (int l = 0; l < size; l++)
				{
					_METHOD_RESULT._data[i * width + j] += (image._data[(i + k) * (width + (size - 1)) + j + l] *
						_filter._matrix[(size * size - 1) - (k * size + l)]) / _filter._deviser;
				}
			}
		}
	}
	return _METHOD_RESULT;
}

Image GRAD(Image & image)
{
	static Image _DX_result;
	static Image _DY_result;

	_DX_result = APPLY_FILTER(image, Dx_2S_2D);
	_DY_result = APPLY_FILTER(image, Dy_2S_2D);

	int height = image._height - 1;
	int width = image._width - 1;

	_METHOD_RESULT._height = height;
	_METHOD_RESULT._width = width;
	_METHOD_RESULT._data = new float[height * width];

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			_METHOD_RESULT._data[i * width + j] = pow(_DX_result._data[i * width + j], 2) +
				pow(_DY_result._data[i * width + j], 2);
		}
	}

	delete [] _DX_result._data;
	delete [] _DY_result._data;

	return _METHOD_RESULT;
}

Image STRUCT_TENSOR(Image & image)
{
	static Image _DX_result;
	static Image _DY_result;
	static Image _DX_DY_result;

	_DX_result = APPLY_FILTER(image, Dx_2S_2D);
	_DY_result = APPLY_FILTER(image, Dy_2S_2D);

	int height = image._height - 1;
	int width = image._width - 1;

	_DX_DY_result._data = new float[height * width];
	_DX_DY_result._width = width;
	_DX_DY_result._height = height;

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			_DX_DY_result._data[i * width + j] = _DX_result._data[i * width + j] * _DY_result._data[i * width + j];
			_DX_result._data[i * width + j] *= _DX_result._data[i * width + j];
			_DY_result._data[i * width + j] *= _DY_result._data[i * width + j];
		}
	}

	static Image _MX_RESULT = APPLY_FILTER(_DX_result, smooth_2S_2D);
	static Image _MY_RESULT = APPLY_FILTER(_DY_result, smooth_2S_2D);
	static Image _MX_MY_RESULT = APPLY_FILTER(_DX_DY_result, smooth_2S_2D);

	delete[] _DX_result._data;
	delete[] _DY_result._data;
	delete[] _DX_DY_result._data;

	height = _MX_RESULT._height;
	width  = _MX_RESULT._width;

	_METHOD_RESULT._data = new float[height * width];
	_METHOD_RESULT._height = height;
	_METHOD_RESULT._width = width;

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			_METHOD_RESULT._data[i * width + j] = _MX_RESULT._data[i * width + j] 
				+ _MY_RESULT._data[i * width + j];
		}
	}

	return _METHOD_RESULT;
}

Image LAPLASS_DET(
	Image & image, Image & LAPLASS_DET) // Return _DET of the _LAPLASS_MATRIX (by the argument) and its LAPLASS_VALUE
{
	static Image _DXX_result;
	static Image _DYY_result;
	static Image _DXY_result;

	_DXX_result = APPLY_FILTER(image, Dxx_3S_2D);
	_DYY_result = APPLY_FILTER(image, Dyy_3S_2D);
	_DXY_result = APPLY_FILTER(image, Dxy_3S_2D);

	int height = image._height - 2;
	int width = image._width - 2;

	_METHOD_RESULT._height = height;
	_METHOD_RESULT._width = width;
	_METHOD_RESULT._data = new float[height * width];

	LAPLASS_DET._height = height;
	LAPLASS_DET._width = width;
	LAPLASS_DET._data = new float[height * width];

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			LAPLASS_DET._data[i * width + j] = _DXX_result._data[i * width + j] * _DYY_result._data[i * width + j]
				- 2 * _DXY_result._data[i * width + j];

			_METHOD_RESULT._data[i * width + j] = _DXX_result._data[i * width + j]
				+ _DYY_result._data[i * width + j];
		}
	}

	delete[] _DXX_result._data;
	delete[] _DYY_result._data;
	delete[] _DXY_result._data;

	return _METHOD_RESULT;
}

Image LAPLASS(
	Image & image)
{
	static Image _DXX_result;
	static Image _DYY_result;
	static Image _DXY_result;

	_DXX_result = APPLY_FILTER(image, Dxx_3S_2D);
	_DYY_result = APPLY_FILTER(image, Dyy_3S_2D);
	_DXY_result = APPLY_FILTER(image, Dxy_3S_2D);

	int height = image._height - 2;
	int width = image._width - 2;

	_METHOD_RESULT._height = height;
	_METHOD_RESULT._width = width;
	_METHOD_RESULT._data = new float[height * width];

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			_METHOD_RESULT._data[i * width + j] = _DXX_result._data[i * width + j]
				+ _DYY_result._data[i * width + j];
		}
	}

	delete[] _DXX_result._data;
	delete[] _DYY_result._data;
	delete[] _DXY_result._data;

	return _METHOD_RESULT;
}

void EIGENVALUES(
	Image & data, Image & eigen_min, Image & eigen_max, Ident id)
{
	switch (id)
	{
	case GESSE:
	{
		static Image _DXX_result;
		static Image _DYY_result;
		static Image _DXY_result;

		_DXX_result = APPLY_FILTER(data, Dxx_3S_2D);
		_DYY_result = APPLY_FILTER(data, Dyy_3S_2D);
		_DXY_result = APPLY_FILTER(data, Dxy_3S_2D);

		int height = data._height - 2;
		int width = data._width - 2;

		eigen_min._height = height;
		eigen_min._width = width;
		eigen_min._data = new float[height * width];

		eigen_max._height = height;
		eigen_max._width = width;
		eigen_max._data = new float[height * width];

		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				float value_1 = (_DXX_result._data[i * width + j] + _DYY_result._data[i * width + j]) / 2
					+ sqrt((pow(_DXX_result._data[i * width + j] + _DYY_result._data[i * width + j], 2)
						- (_DXX_result._data[i * width + j] + _DYY_result._data[i * width + j])
						- _DXY_result._data[i * width + j] * _DXY_result._data[i * width + j]));

				float value_2 = (_DXX_result._data[i * width + j] + _DYY_result._data[i * width + j]) / 2
					- sqrt((pow(_DXX_result._data[i * width + j] + _DYY_result._data[i * width + j], 2)
						- (_DXX_result._data[i * width + j] + _DYY_result._data[i * width + j])
						- _DXY_result._data[i * width + j] * _DXY_result._data[i * width + j]));

				if (abs(value_1) >= abs(value_2))
				{
					eigen_min._data[i * width + j] = value_2;
					eigen_max._data[i * width + j] = value_1;
				}
				else
				{
					eigen_min._data[i * width + j] = value_1;
					eigen_max._data[i * width + j] = value_2;
				}
			}
		}

		delete[] _DXX_result._data;
		delete[] _DYY_result._data;
		delete[] _DXY_result._data;

		break;
	}
	case STRUCT:
	{
		static Image _DX_result;
		static Image _DY_result;
		static Image _DX_DY_result;

		_DX_result = APPLY_FILTER(data, Dx_2S_2D);
		_DY_result = APPLY_FILTER(data, Dy_2S_2D);

		int height = data._height - 1;
		int width = data._width - 1;

		_DX_DY_result._data = new float[height * width];
		_DX_DY_result._width = width;
		_DX_DY_result._height = height;

		eigen_min._height = height;
		eigen_min._width = width;
		eigen_min._data = new float[height * width];

		eigen_max._height = height;
		eigen_max._width = width;
		eigen_max._data = new float[height * width];

		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				_DX_DY_result._data[i * width + j] = _DX_result._data[i * width + j] * _DY_result._data[i * width + j];
				_DX_result._data[i * width + j] *= _DX_result._data[i * width + j];
				_DY_result._data[i * width + j] *= _DY_result._data[i * width + j];

				float value_1 = (_DX_result._data[i * width + j] + _DY_result._data[i * width + j]) / 2
					+ sqrt((pow(_DX_result._data[i * width + j] + _DY_result._data[i * width + j], 2)
						- (_DX_result._data[i * width + j] + _DY_result._data[i * width + j])
						- _DX_DY_result._data[i * width + j] * _DX_DY_result._data[i * width + j]));

				float value_2 = (_DX_result._data[i * width + j] + _DY_result._data[i * width + j]) / 2
					- sqrt((pow(_DX_result._data[i * width + j] + _DY_result._data[i * width + j], 2)
						- (_DX_result._data[i * width + j] + _DY_result._data[i * width + j])
						- _DX_DY_result._data[i * width + j] * _DX_DY_result._data[i * width + j]));

				if (abs(value_1) >= abs(value_2))
				{
					eigen_min._data[i * width + j] = value_2;
					eigen_max._data[i * width + j] = value_1;
				}
				else
				{
					eigen_min._data[i * width + j] = value_1;
					eigen_max._data[i * width + j] = value_2;
				}
			}
		}
	}
	default:
		break;
	}
}

Image EMPTY_IMAGE(
	int width, int height)
{
	_METHOD_RESULT._height = height;
	_METHOD_RESULT._width = width;
	_METHOD_RESULT._data = new float[height * width];
	
	for (int row = 0; row < height; row++)
	{
		for (int elem = 0; elem < width; elem++)
			_METHOD_RESULT._data[row * width + elem] = 0;
	}

	return _METHOD_RESULT;
}

Image GET_WINDOW(Image & image,
	int x_pos, int y_pos, int window_size)
{
	_METHOD_RESULT._height = window_size;
	_METHOD_RESULT._width = window_size;
	_METHOD_RESULT._data = new float[window_size * window_size];

	for (int i = 0; i < window_size; i++)
	{
		for (int j = 0; j < window_size; j++)
		{
			_METHOD_RESULT._data[i * window_size + j] = image._data[(x_pos + i) 
				* image._width + (y_pos + j)];
		}
	}

	return _METHOD_RESULT;
}

void PRINT_DATA(Image & data)
{
	for (int i = 0; i < data._height; i++)
	{
		for (int j = 0; j < data._width; j++)
		{
			std::cout << data._data[i * data._width + j] << " ";
		}
		std::cout << std::endl;
	}
}

Image OPCV_IMAGE_TO_ARRAY(IplImage* _image)
{
	CvMat* image = cvCreateMat(_image->height, _image->width, CV_32FC1);
	cvConvert(_image, image);

	_METHOD_RESULT._height = _image->height;
	_METHOD_RESULT._width = _image->width;
	_METHOD_RESULT._data = new float[_image->width * _image->height];

	for (int i = 0; i < image->height; i++)
		for (int j = 0; j < image->width; j++)
		{
			_METHOD_RESULT._data[i * image->width + j] = cvGet2D(image, i, j).val[0];
		}

	return _METHOD_RESULT;
}

IplImage* ARRAY_TO_IMAGE(Image & _image)
{
	cv::Mat matrix = cv::Mat(_image._height, _image._width, CV_32FC1);

	for (int i = 0; i < _image._height; i++)
	{
		for (int j = 0; j < _image._width; j++)
		{
			matrix.at<float>(i, j) = _image._data[i * _image._width + j];
		}
	}

	IplImage* _RESULT = cvCloneImage(&(IplImage)matrix);
	matrix.deallocate();

	return _RESULT;
}

IplImage * COLOR_TO_GRAY(IplImage * _image)
{
	CvMat* image = cvCreateMat(_image->height, _image->width, CV_32FC3);
	cvConvert(_image, image);

	Image temp_data = EMPTY_IMAGE(_image->width, _image->height);

	for (int i = 0; i < _image->height; i++)
		for (int j = 0; j < _image->width; j++)
		{
			temp_data._data[i * _image->width + j] = int((cvGet2D(image, i, j).val[0] * 0.2125) +
				(cvGet2D(image, i, j).val[1] * 0.7154) +
				(cvGet2D(image, i, j).val[2] * 0.0721));
			/*
			cout << (cvGet2D(image, i, j).val[0] * 0.2125) +
				(cvGet2D(image, i, j).val[1] * 0.7154) +
				(cvGet2D(image, i, j).val[2] * 0.0721) << " ";*/
		}

	IplImage* image_gray = ARRAY_TO_IMAGE(temp_data);

	delete[] temp_data._data;

	return image_gray;

} // fdsfsf

void DELETE_DATA()
{
	delete[] _METHOD_RESULT._data;
}
