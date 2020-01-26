#pragma once
#include <string>
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include "opencv2/imgproc\imgproc.hpp"
#include "opencv2/shape.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/video/video.hpp"

#ifdef IMAGELIBRARY_EXPORTS
#define IMAGELIBRARY_API __declspec(dllexport)
#else
#define IMAGELIBRARY_API __declspec(dllimport)
#endif

struct  filter
{
    static float * _matrix;
	static int _size;
	static int _deviser;
	static int _dimension;
};

struct Image
{
	int _width;
	int _height;
	float * _data;

	Image()
	{
		_width = 0;
		_height = 0;
		_data = nullptr;
	}

	Image(Image const & data_copy) : _width(data_copy._width),
		_height(data_copy._height), _data(new float[data_copy._width * data_copy._height])
	{
		for (int i = 0; i < _height; i++)
			for (int j = 0; j < _width; j++)
				_data[i * _width + j] = data_copy._data[i * _width + j];
	}
};

enum Filter2DTypes
{
	Dx_2S_2D, Dy_2S_2D,	Dxx_3S_2D, Dyy_3S_2D,
	Dxy_3S_2D, Laplace_3S_2D, smooth_2S_2D,	smooth_3S_2D,
	smooth_4S_2D, smooth_5S_2D,	smooth_6S_2D, DOG_5S_2D
};

enum Ident
{
	STRUCT,
	GESSE
};
void defineFilter(float * matrix, int size, int deviser, int dim);

void  defineFilterWithSelectedType2D(Filter2DTypes type);

extern "C" { 
	IMAGELIBRARY_API Image 
		APPLY_FILTER(Image & image,
			Filter2DTypes image_type); // The size of _RESULT_IMAGE is less then original image by the size of _FILTER - 1. 
	
	IMAGELIBRARY_API Image 
		GRAD(Image & image);

	IMAGELIBRARY_API Image 
		LAPLASS_DET(Image & image, 
			Image & LAPLASS_DET);
	
	IMAGELIBRARY_API Image
		STRUCT_TENSOR(Image & image);

	IMAGELIBRARY_API Image 
		LAPLASS(Image & image);
	
	IMAGELIBRARY_API void 
		EIGENVALUES(Image & data, 
			Image & eigen_1, Image & eigen_2, Ident id);

	IMAGELIBRARY_API Image 
		EMPTY_IMAGE(int width, int height);

	IMAGELIBRARY_API Image 
		GET_WINDOW(Image & image,
		int x_pos, int y_pos, int window_size);

	IMAGELIBRARY_API void
		PRINT_DATA(Image & image);

	IMAGELIBRARY_API Image
		OPCV_IMAGE_TO_ARRAY(IplImage* _image);

	IMAGELIBRARY_API IplImage 
		* ARRAY_TO_IMAGE(Image & _image);

	IMAGELIBRARY_API IplImage 
		* COLOR_TO_GRAY(IplImage * _image);

	IMAGELIBRARY_API void DELETE_DATA();
}


