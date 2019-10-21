#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <iomanip>
#include "Convertation.h"
#include <opencv/cv.h>
#include <opencv2\core.hpp>
#include <opencv2\highgui.hpp>
#include <opencv2\imgcodecs.hpp>
#include "opencv2\imgproc\imgproc.hpp"
#include "opencv2\shape.hpp"
#include "opencv2\highgui\highgui.hpp"

using namespace std;


IplImage* convertToImage(float** data, int m, int n)
{
	cv::Mat matrix = cv::Mat(m, n, CV_32FC1);

	for (int i = 0; i < m; i++)
	{
		for (int j = 0; j < n; j++)
		{
			matrix.at<float>(i, j) = data[i][j];
		}
	}

	IplImage* image = cvCloneImage(&(IplImage)matrix);

	return image;
}

IplImage* convertToImage(float* data, int m, int n)
{
	cv::Mat matrix = cv::Mat(m, n, CV_32FC1);

	for (int i = 0; i < m; i++)
	{
		for (int j = 0; j < n; j++)
		{
			matrix.at<float>(i, j) = data[i * n + j];
		}
	}

	IplImage* image = cvCloneImage(&(IplImage)matrix);

	return image;
}

imgArr OPCVImageToArray(IplImage* _img)
{
	IplImage* img = _img;
	CvMat* image = cvCreateMat(img->height, img->width, CV_32FC1);
	cvConvert(img, image);
	
	float* result = getEmptyArray(image->height * image->width);

	for (int i = 0; i < image->height; i++)
		for (int j = 0; j < image->width; j++)
		{
			result[i * image->width + j] = cvGet2D(image, i, j).val[0];
		}

	imgArr res = imgArr{ result, image->height, image->width };
	return res;

}
