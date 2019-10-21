#pragma once
#include <stdio.h>
#include <list> 
#include <stdlib.h>
#include <iostream>
#include <iomanip>
#include <opencv2\core.hpp>
#include <opencv2\highgui.hpp>
#include <opencv2\imgcodecs.hpp>
#include "opencv2\imgproc\imgproc.hpp"
#include "opencv2\shape.hpp"
#include "opencv2\highgui\highgui.hpp"
//Структура для хранения картинок
struct imgArr
{
	float* image = nullptr;
	int height = 0;
	int width = 0;
	int deviser = 0;
};
struct color
{
	int R = 0;
	int G = 0;
	int B = 0;
};

//Стурктура для хранения фильтров
struct  filter
{
	float *matrix = nullptr;
	int size = 0;
	int deviser = 0;
	int dimension = 0;
};
struct rgbImgArr
{
	imgArr R;
	imgArr G;
	imgArr B;
	
};
imgArr getEmptyImgArr(int height, int width);

int ** getEmptyArray(int height, int width);
float * getEmptyArray(int length);

void deleteEmptyArray(int** image, int height);

void deleteImgArr(imgArr);

float ** getEmptyDoubleArray(int height, int width);

filter createFilter(float* matrix, int size,int deviser, int dimension);

