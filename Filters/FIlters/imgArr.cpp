#include "imgArr.h"

imgArr getImgArr(float* image, int height, int width)
{
	imgArr arr;
	arr.image = image;
	arr.height = height;
	arr.width = width;
	arr.deviser = 1;
	return arr;
}
//TODO каждой точке изображения ставим в соответсвие нейрон.

imgArr getEmptyImgArr(int height, int width)
{
	imgArr arr;
	arr.image = getEmptyArray(height*width);
	arr.height = height;
	arr.width = width;
	arr.deviser = 1;
	return arr;
	
}

int ** getEmptyArray(int height, int width)
{
	int** arr = new int *[height];
	for (int i = 0; i < height ; i++)
		arr[i] = new int[width];
	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++)
			arr[i][j] = 0;
	
	return arr;
}
float * getEmptyArray(int length)
{
	float* arr = new float [length];
	for (int i = 0; i < length; i++)
			arr[i] = 0;
	return arr;
}

void deleteEmptyArray(int ** image, int height)
{
	for (int count = 0; count < height; count++)
		delete [] image[count];
	delete [] image;
}
void deleteImgArr(imgArr arr)
{
	delete[] arr.image;
}

float ** getEmptyDoubleArray(int height, int width)
{
	float** arr = new float *[height];
	for (int i = 0; i < height; i++)
		arr[i] = new float[width];
	return arr;
}



filter createFilter(float * matrix, int size,int deviser, int dim)
{
	filter f;
	f.matrix = matrix;
	f.size = size;
	f.deviser = deviser;
	f.dimension = dim;
	return f;
}
