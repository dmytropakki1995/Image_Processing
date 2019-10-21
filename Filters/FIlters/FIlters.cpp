
#include "pch.h"
#include <iostream>
#include "imgArr.h"
#include "Convertation.h"
#include "FilterFactory.h" 
#include <opencv2\core.hpp>
#include <opencv2\highgui.hpp>
#include <opencv2\imgcodecs.hpp>
#include "opencv2\imgproc\imgproc.hpp"
#include "opencv2\shape.hpp"
#include "opencv2\highgui\highgui.hpp"
#include "opencv2/video/video.hpp"
#include "time.h"
#include <Windows.h>
#include <list>
#include <iterator>
#include <array>
#include <vector>

using namespace std;

#define THRESHOLD_STUCT 3000
#define THRESHOLD 30																		    // Threshold value for the interest points of high gradient   
#define S 7		

enum MyEnum
{
	NOTHING,
	checkThreshold,
	sphConditionSTR,
	sphConditionGESSE,
};
// Demension of coordinate vector
// Convertation into gray image
IplImage * Gray(IplImage *img)
{
	CvMat* image = cvCreateMat(img->height, img->width, CV_32FC3);
	cvConvert(img, image);

	float** result = getEmptyDoubleArray(image->height, image->width);

	for (int i = 0; i < image->height; i++)
		for (int j = 0; j < image->width; j++)
		{
			result[i][j] = int((cvGet2D(image, i, j).val[0] * 0.2125) +
				(cvGet2D(image, i, j).val[1] * 0.7154) +
				(cvGet2D(image, i, j).val[2] * 0.0721));

		}

	IplImage* image_gray = convertToImage(result, image->height, image->width);

	return image_gray;

} // fdsfsf
IplImage * Color(IplImage * image, int * arr, int h, int w)
{
	cv::Mat img(h, w, CV_8UC3, cvScalar(0, 0, 0));

	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < w; j++)
		{
			if (arr[i * w + j] != 0)
			{
				img.at<cv::Vec3b>(i, j)[0] = 0;
				img.at<cv::Vec3b>(i, j)[1] = 255;
				img.at<cv::Vec3b>(i, j)[2] = 255;
			}
			else
			{
				img.at<cv::Vec3b>(i, j)[0] = cvGet2D(image, i, j).val[0];
				img.at<cv::Vec3b>(i, j)[1] = cvGet2D(image, i, j).val[1];
				img.at<cv::Vec3b>(i, j)[2] = cvGet2D(image, i, j).val[2];
			}
		}
	}
	IplImage* result = cvCloneImage(&(IplImage)img);
	return result;
}
IplImage * twoColors(float * arr1, float * arr2, int h, int w)
{
	cv::Mat img(h, w, CV_8UC3, cvScalar(0, 0, 0));

	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < w; j++)
		{
			if (arr1[i * w + j] != 0)
			{
				img.at<cv::Vec3b>(i, j)[0] = 0;
				img.at<cv::Vec3b>(i, j)[1] = 0;
				img.at<cv::Vec3b>(i, j)[2] = 255;
			}
			if (arr2[i * w + j] != 0)
			{
				img.at<cv::Vec3b>(i, j)[0] = 255;
				img.at<cv::Vec3b>(i, j)[1] = 255;
				img.at<cv::Vec3b>(i, j)[2] = 0;
			}

		}
	}
	IplImage* result = cvCloneImage(&(IplImage)img);
	return result;
}
float * AppFilter(float *data, filter filter, int h, int w)
{
	if (filter.dimension == 2)
	{
		int size = filter.size;

		float *result = new float[w * h];

		for (int i = 0; i < h; i++)
		{
			for (int j = 0; j < w; j++)
			{
				result[i * w + j] = 0;
				for (int k = 0; k < size; k++)
				{
					for (int l = 0; l < size; l++)
					{
						result[i * w + j] += (data[(i + k) * (w + (size - 1)) + j + l] *
							filter.matrix[(size * size - 1) - (k * size + l)]) / filter.deviser;
					}
				}
			}

		}
		return result;
	}

	if (filter.dimension = 1)
	{
		cout << "I havn't done yet, sorry!" << endl;
		return nullptr;
	}
}
float * interest_point_grad(float * a, float * b, float * image, int h, int w, float &max_grad, float &x, float &y, vector<array<float, 2>> & frequency)
{
	vector<array<float, 3>> vect;																	    // Creating list of arrays to store value of maximal grad and its coordinates

	float * res = getEmptyArray(h * w);																	// Allocation memory for result array
	float max = sqrt(pow(a[0], 2) + pow(b[0], 2));
	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < w; j++)
		{

			frequency.push_back({ a[i * w + j], b[i * w + j] });
			if (sqrt(pow(a[i * w + j], 2) + pow(b[i * w + j], 2)) <= THRESHOLD)
			{
				frequency.push_back({ 0, 0 });
				res[i * w + j] = 0;
			}
			else
			{
				frequency.push_back({ a[i * w + j], b[i * w + j] });
				res[i * w + j] = image[i * w + j];



				if (sqrt(pow(a[i * w + j], 2) + pow(b[i * w + j], 2)) > max)
				{																						// Searching max value of gradient
					max = sqrt(pow(a[i * w + j], 2) + pow(b[i * w + j], 2));
				}

				vect.push_back({ float(sqrt(pow(a[i * w + j], 2) + pow(b[i * w + j], 2))),
					float(j), float(i) });
			}
		}
	}
	max_grad = max;																					    // max_grad has a reference type and we can change one.

	float min = sqrt(pow(float(w) / 2 - vect[0][1], 2) + pow(float(h) / 2 - vect[0][2], 2));

	int count = 0;
	for (int i = 1; i < vect.size(); i++)
	{
		if (vect[i][0] == max)
		{																								// The idea is such that we are looking for the coordinates of a maximal point among
			++count;																					// the all points in the list. If there is more then one point, we need to find such point that has 
			if (count == 1)																				// a minimal distance value from the center. Counter shows to us how many points of maximum we have.
			{
				x = vect[i][1]; y = vect[i][2];                                                         // x and y also have a reference type and we can change them. Here we are recieving coordinates of desirable point. 
				min = sqrt(pow(float(w) / 2 - vect[i][1], 2) + pow(float(h) / 2 - vect[i][2], 2));
			}
			else
			{
				if (sqrt(pow(float(w) / 2 - vect[i][1], 2) + pow(float(h) / 2 - vect[i][2], 2)) < min)  // case when there are two or more maximal points
				{
					min = sqrt(pow(float(w) / 2 - vect[i][1], 2) + pow(float(h) / 2 - vect[i][2], 2));
					x = vect[i][1]; y = vect[i][2];
				}
			}
		}
		else
			continue;
	}

	return res;
}
float * spheric_condition(float * a_11, float * a_22, float * a_12, float * image, int h, int w, MyEnum check)
{
	int threshold = 5;
	float *res = getEmptyArray(h * w);
	float a11, a22, a12;
	float lambda_1, lambda_2;

	if (check == sphConditionGESSE)
	{
		for (int i = 0; i < h; i++)
		{
			for (int j = 0; j < w; j++)
			{
				a11 = a_11[i * w + j];
				a22 = a_22[i * w + j];
				a12 = a_12[i * w + j];

				if ((abs(a11) <= threshold) && (abs(a22) <= threshold) && (abs(a12) <= threshold))
					res[i * w + j] = 0;
				else
				{
					//cout << a11 << "  " << a22 << "  " << a12 << endl;
					if ((abs(a11 - a22) == 2 * abs(a12)))
						res[i * w + j] = image[i * w + j];
					else
					{
						res[i * w + j] = 0;
					}
				}
			}
		}
	}

	if (check == sphConditionSTR)
	{
		for (int i = 0; i < h; i++)
		{
			for (int j = 0; j < w; j++)
			{
				a11 = a_11[i * w + j];
				a22 = a_22[i * w + j];
				a12 = a_12[i * w + j];

				lambda_1 = (a11 + a22) / 2 + sqrt(pow(((a11 + a22) / 2), 2) - ((a11 * a22) - (pow(a12, 2))));
				lambda_2 = (a11 + a22) / 2 - sqrt(pow(((a11 + a22) / 2), 2) - ((a11 * a22) - (pow(a12, 2))));

				if (abs(lambda_1 - lambda_2) > THRESHOLD_STUCT)
				{
					res[i * w + j] = image[i * w + j];
				}
				else
					res[i * w + j] = 0;
			}
		}
	}

	return res;
}
float * multiple_eigenvalues(float * a_11, float * a_22, float * a_12, float * image, int h, int w)
{
	float *res = getEmptyArray(h * w);
	int threshold = 2;
	float lambda_1, lambda_2 = 0;
	float a11, a22, a12 = 0;

	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < w; j++)
		{

			a11 = a_11[i * w + j]; a22 = a_22[i * w + j]; a12 = a_12[i * w + j];

			if ((abs(a11) <= threshold) && (abs(a22) <= threshold) && (abs(a12) <= threshold))
				res[i * w + j] = 0;
			else
			{
				lambda_1 = ((a11 + a22) / 2) + sqrt(pow(((a11 + a22) / 2), 2) - ((a11 * a22) - (pow(a12, 2))));
				lambda_2 = ((a11 + a22) / 2) - sqrt(pow(((a11 + a22) / 2), 2) - ((a11 * a22) - (pow(a12, 2))));

				if (lambda_1 == lambda_2)
				{
					res[i * w + j] = image[i * w + j];
				}
				else
					res[i * w + j] = 0;
			}
		}
	}
	return res;
}
float * equal_eigenvalues(float * struct_val, float * gesse_val, float * image, int h, int w, float * a_11, float * a_22, float * a_12,
	vector<array<float, S>> & coordinates, float x_center, float y_center, float lambda_max)
{
	float x, y, r, lambda, c_1, r_val, i_val = 0;
	float lambda_1, lambda_2;
	float *res = getEmptyArray(h * w);

	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < w; j++)
		{
			if ((struct_val[i * w + j] != 0) && (gesse_val[i * w + j] != 0))							// By the previous method we have got matrices with the the interest(multiple eigenvalues) points  
			{
				// that have pixel values from initial gray image, other points have zero pixels values.
				res[i * w + j] = image[i * w + j];														// Therefore if both matrices have non zero values then they both have multiple eigenvalues at the same point. 
				x = (j - x_center);  y = (y_center - i);

				float a11 = a_11[i * w + j];
				float a22 = a_22[i * w + j];
				float a12 = a_12[i * w + j];

				lambda_1 = (a11 + a22) / 2 + sqrt(pow(((a11 + a22) / 2), 2) - ((a11 * a22) - (pow(a12, 2))));
				lambda_2 = (a11 + a22) / 2 - sqrt(pow(((a11 + a22) / 2), 2) - ((a11 * a22) - (pow(a12, 2))));

				if (lambda_1 < lambda_2)
					lambda = lambda_1;
				else
					lambda = lambda_2;

				if (abs(lambda) <= 1)
				{
					res[i * w + j] = 0;
					coordinates.push_back({ 0, 0, 0, 0, 0, 0 });
					cout << " +";
					continue;
				}

				r = sqrt(lambda_max / abs(lambda));
				c_1 = pow(x, 2) + pow(y, 2) - pow(r, 2);

				r_val = (1 - c_1) / 2;
				i_val = (1 + c_1) / 2;
				//cout << r << endl;
				coordinates.push_back({ x, y, r_val, i_val, r, float(j), float(i) });
			}
			else
			{
				res[i * w + j] = 0;
				coordinates.push_back({ 0, 0, 0, 0, 0, 0 });
			}
		}
	}
	return res;
}
float ** fromVectToArray(vector<array<float, S>> & list)
{
	float **arr = new float *[list.size()];
	for (int i = 0; i < list.size(); i++)
	{
		arr[i] = new float[S];
		for (int j = 0; j < S; j++)
		{
			arr[i][j] = list[i][j];
		}
	}
	return arr;
}
float innerProduct(float *a, float *b)
{
	return (a[0] * b[0]) + (a[1] * b[1]) + (a[2] * b[2]) - (a[3] * b[3]);
}
void drawCircle(int _x, int _y, int _r, float *data, int h, int w)
{
	int x = 0;
	int y = _r;
	int delta = (2 - 2 * _r);

	int error = 0;
	while (y >= 0)
	{
		if ((_x <= x) && (_y <= y))
			data[(_y + y) * w + _x + x] = 255;
		else if ((_x <= x) && (_y + y >= h))
			data[(_y - y) * w + _x + x] = 255;
		else if ((_x + x >= w) && (_y <= y))
			data[(_y + y) * w + _x - x] = 255;
		else if ((_x + x >= w) && (_y + y >= h))
			data[(_y - y) * w + _x - x] = 255;

		else if (_y <= y)
		{
			data[(_y + y) * w + _x + x] = 255;
			data[(_y + y) * w + _x - x] = 255;
		}
		else if (_x <= x)
		{
			data[(_y + y) * w + _x + x] = 255;
			data[(_y - y) * w + _x + x] = 255;
		}
		else if (_x + x >= w)
		{
			data[(_y + y) * w + _x - x] = 255;
			data[(_y - y) * w + _x - x] = 255;
		}
		else if (_y + y >= h)
		{
			data[(_y - y) * w + _x + x] = 255;
			data[(_y - y) * w + _x - x] = 255;
		}
		else
		{
			data[(_y + y) * w + _x + x] = 255;
			data[(_y - y) * w + _x + x] = 255;
			data[(_y - y) * w + _x - x] = 255;
			data[(_y + y) * w + _x - x] = 255;
		}
		/*
		if ((_x <= x) && (_y <= y))
			data[(_x + x) * w + _y + y] = 255;
		else if ((_x <= x) && (_y + y >= w))
			data[(_x + x) * w + _y - y] = 255;
		else if ((_x + x >= h) && (_y <= y))
			data[(_x - x) * w + _y + y] = 255;
		else if ((_x + x >= h) && (_y + y >= w))
			data[(_x - x) * w + _y - y] = 255;

		else if (_x <= x)
		{
			data[(_x + x) * w + _y + y] = 255;
			data[(_x + x) * w + _y - y] = 255;
		}
		else if (_y <= y)
		{
			data[(_x + x) * w + _y + y] = 255;
			data[(_x - x) * w + _y + y] = 255;
		}
		else if (_x + x >= h)
		{
			data[(_x - x) * w + _y + y] = 255;
			data[(_x - x) * w + _y - y] = 255;
		}
		else if (_y + y >= w)
		{
			data[(_x + x) * w + _y - y] = 255;
			data[(_x - x) * w + _y - y] = 255;
		}
		else
		{
			data[(_x + x) * w + _y + y] = 255;
			data[(_x - x) * w + _y + y] = 255;
			data[(_x - x) * w + _y - y] = 255;
			data[(_x + x) * w + _y - y] = 255;
		}
		*/

		error = 2 * (delta + y) - 1;
		if ((delta < 0) && (error <= 0))
		{
			x++;
			delta += 2 * x + 1;
			continue;
		}
		if ((delta > 0) && (error > 0))
		{
			y--;
			delta -= 2 * y + 1;
			continue;
		}
		x++;
		delta += 2 * (x - y);
		y--;
	}
}
void Circles(float ** result, float * m_eigenvalues, int h, int w, float * circles)
{
	float x = 0, y = 0, r = 0;
	float x1 = 0, y1 = 0, r1 = 0;
	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < w; j++)
		{
			if (m_eigenvalues[i * w + j] != 0)
			{
				for (int k = 0; k < h; k++)
				{
					for (int l = j; l < w; l++)
					{
						if (m_eigenvalues[k * w + l] != 0)
						{
							if (innerProduct(result[i * w + j], result[k * w + l]) == 0)
							{
								circles[i * w + j] = m_eigenvalues[i * w + j];
								circles[k * w + l] = m_eigenvalues[k * w + l];

								r = result[i * w + j][4];
								x = result[i * w + j][5];
								y = result[i * w + j][6];

								r1 = result[k * w + l][4];
								x1 = result[k * w + l][5];
								y1 = result[k * w + l][6];

								drawCircle(x, y, r, circles, h, w);
								drawCircle(x1, y1, r1, circles, h, w);

								//cout << r << "  " << r1 << endl;
								/*
								if ((pow((x - x1), 2) + pow((y - y1), 2)) < r + r1)
								{
									circles[i * w + j] = m_eigenvalues[i * w + j];
									circles[k * w + l] = m_eigenvalues[k * w + l];

									drawCircle(x, y, r, circles, h, w);
									drawCircle(x1, y1, r1, circles, h, w);
								}
								else
								{
									circles[i * w + j] = 0;
									circles[k * w + l] = 0;
								}
								*/
							}
						}
					}
				}
			}
		}
	}
}
void Centers(float ** result, float * m_eigenvalues, int h, int w, float * centers, int x_center, int y_center)
{
	float r1, r2, x1, x2, y1, y2;
	int x = 0, y = 0;
	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < w; j++)
		{
			if (m_eigenvalues[i * w + j] != 0)
			{
				for (int k = 0; k < h; k++)
				{
					for (int l = j; l < w; l++)
					{
						if (m_eigenvalues[k * w + l] != 0)
						{
							if (innerProduct(result[i * w + j], result[k * w + l]) == 0)
							{
								//cout << m_eigenvalues[k * w + l] << "   " << result[k * w + l][0] << endl;

								r1 = result[i * w + j][4];
								x1 = result[i * w + j][5];
								y1 = result[i * w + j][6];

								r2 = result[k * w + l][4];
								x2 = result[k * w + l][5];
								y2 = result[k * w + l][6];

								x = int(x2 + ((x1 - x2) / sqrt((x1 - x2)*(x1 - x2) + (y1 - y2) * (y1 - y2)) * r2 / (r1 + r2) * sqrt(r1 * r1 + r2 * r2)));
								y = int(y2 + ((y1 - y2) / sqrt((x1 - x2)*(x1 - x2) + (y1 - y2) * (y1 - y2)) * r2 / (r1 + r2) * sqrt(r1 * r1 + r2 * r2)));

								if (((y * w + x) > h * w) || ((y * w + x) < 0) || ((x1 == x2) && (y1 == y2)))
								{
									continue;
								}
								else
									centers[y * w + x] = 255;
							}
						}
					}
				}
			}
		}
	}
}
float * drawPoint(float * a_11, float * a_22, float * a_12, int h, int w)
{
	float a11, a22, a12, lambda_1, lambda_2, lambda_min;
	int threshold = 20;
	float *res = getEmptyArray(h * w);

	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < w; j++)
		{
			a11 = a_11[i * w + j];
			a22 = a_22[i * w + j];
			a12 = a_12[i * w + j];

			if ((abs(a11) <= threshold) && (abs(a22) <= threshold) && (abs(a12) <= threshold))
			{
				res[i * w + j] = 0;
				continue;
			}

			lambda_1 = (a11 + a22) / 2 + sqrt(pow(((a11 + a22) / 2), 2) - ((a11 * a22) - (pow(a12, 2))));
			lambda_2 = (a11 + a22) / 2 - sqrt(pow(((a11 + a22) / 2), 2) - ((a11 * a22) - (pow(a12, 2))));

			if (lambda_1 < lambda_2)
				lambda_min = lambda_1;
			else
				lambda_min = lambda_2;

			if (abs(lambda_min) >= 2)
			{
				res[i * w + j] = 255;
			}
			else
				res[i * w + j] = 0;
		}
	}
	return res;
}

// TO DO  FREQUENCY OF (Lx, Ly), NEURAL NETWORK, LAMBDA_MIN.
int main(int argc, char **argv)
{
	IplImage * image = nullptr;
	IplImage * gray = nullptr;
	IplImage * leha = nullptr;

	image = cvLoadImage("C:/Users/dimap/source/repos/Filters/apple1.jpg");								// Load imae from the computer memory
	gray = Gray(image);																					// Gray - function for the convertation from color image to gray image
	//cvShowImage("adsa", image);
	vector<array<float, S>> vect;																		// Creating coordinate vector
																										// Creating filters via FIlterFactory methods
	vector<array<float, 2>> freq;

	filter Dx = createWithSelectedType2D(Dx_2S_2D);
	filter Dy = createWithSelectedType2D(Dy_2S_2D);
	filter SMOOTH = createWithSelectedType2D(smooth_2S_2D);
	filter Dxx = createWithSelectedType2D(Dxx_3S_2D);
	filter Dyy = createWithSelectedType2D(Dyy_3S_2D);
	filter Dxy = createWithSelectedType2D(Dxy_3S_2D);

	imgArr data = OPCVImageToArray(gray);																// Writing image data in imgArr struct. It is very convenient since we will 
																										// know pixels values and picture dimension at the same time.
	int size = Dx.size - 1;																			    // Some transformations for short expressions and calculations

	int h = data.height - size;
	int w = data.width - size;
	// Memory allocation by using getEmptyArray method
	// to store arrays of interest points, struct tensors, etc.
	float *dx = getEmptyArray(h * w);
	float *dy = getEmptyArray(h * w);
	float *grad = getEmptyArray(h * w);

	float *s_x = getEmptyArray(h * w);
	float *s_y = getEmptyArray(h * w);
	float *s_xy = getEmptyArray(h * w);

	float *a_11 = getEmptyArray((h - 1) * (w - 1));
	float *a_22 = getEmptyArray((h - 1) * (w - 1));
	float *a_12 = getEmptyArray((h - 1) * (w - 1));
	float *sph_cond_struct = getEmptyArray((h - 1) * (w - 1));
	float *sph_cond_gesse = getEmptyArray((h - 1) * (w - 1));
	float *comp = getEmptyArray((h - 1) * (w - 1));

	float *a11_x = getEmptyArray((h - 1) * (w - 1));
	float *a22_y = getEmptyArray((h - 1) * (w - 1));
	float *a12_xy = getEmptyArray((h - 1) * (w - 1));

	float *m_eigenval_struct = getEmptyArray((h - 1) * (w - 1));
	float *m_eigenval_gesse = getEmptyArray((h - 1) * (w - 1));
	float *m_eigenvalues = getEmptyArray((h - 1) * (w - 1));
	float *lambda_zero = getEmptyArray((h - 1) * (w - 1));
	// There are two kinds of AppFilter methods. 
	// AppFilter_Str applies to the struct data function parameter. 
	// AppFilter_Arr applies to array data.
	// Applying filter for data. data is a struct with pixels value, dimension and deviser. Dx is a filter.
	dx = AppFilter(data.image, Dx, h, w);
	dy = AppFilter(data.image, Dy, h, w);

	float max_grad = 0;
	float x_center = 0, y_center = 0;

	grad = interest_point_grad(dx, dy, data.image, h, w, max_grad, x_center, y_center, freq);                 // Points of interest with high gradient value. Decsription of the method is in inside
	/*
	vector<array<int, 2>> a;
	a.push_back({ 1, 3 });
	a.push_back({ 2, 4 });
	a.push_back({ 3, 4 });
	a.push_back({ 4, 4 });
	a.push_back({ 5, 4 });
	a.push_back({ 6, 4 });
	a.push_back({ 7, 4 });

	a.erase(a.begin() + (3 - 1));

	for (int i = 0; i < a.size(); i++)
		cout << a[i][0] << "   " << a[i][1] << endl;
		*/


	for (int i = 0; i < freq.size(); i++)
	{
		if ((freq[i][0] == 0) && (freq[i][1] == 0))
		{
			continue;
		}
		else
		{
			int count = 0;
			for (int j = 0; j < freq.size(); j++)
			{
				if ((freq[j][0] == 0) && (freq[j][1] == 0))
					continue;
				else
				{
					if ((freq[i][0] == freq[j][0]) && (freq[i][1] == freq[j][1]))
					{
						count++;
						freq.erase(freq.begin() + (j - 1));
					}
				}
			}
			cout << count << endl;
		}
	}


		float lambda_max = pow(max_grad, 2) / 2;															// Defenition of lambda_max

		for (int i = 0; i < h; i++)																			// This is how we defned (L_2)^2
			for (int j = 0; j < w; j++)
			{
				s_x[i * w + j] = dx[i * w + j] * dx[i * w + j];
				s_y[i * w + j] = dy[i * w + j] * dy[i * w + j];
				s_xy[i * w + j] = dx[i * w + j] * dy[i * w + j];
				//cout << s_x[i * w + j] << " ";
			}

		a_11 = AppFilter(s_x, SMOOTH, h - 1, w - 1);										// ApppFilter allows us to get points of struct_tensor matricies. 
		a_22 = AppFilter(s_y, SMOOTH, h - 1, w - 1);
		a_12 = AppFilter(s_xy, SMOOTH, h - 1, w - 1);
		// ApppFilter allows us to get points of Gesse matrix. 
		a11_x = AppFilter(data.image, Dxx, h - 1, w - 1);											// ApppFilter allows us to get points of Gesse matrix. 
		a22_y = AppFilter(data.image, Dyy, h - 1, w - 1);
		a12_xy = AppFilter(data.image, Dxy, h - 1, w - 1);

		sph_cond_struct = spheric_condition(a_11, a_22, a_12, data.image, h - 1, w - 1, sphConditionSTR);				// Method to find points with the spherical condition
		sph_cond_gesse = spheric_condition(a11_x, a22_y, a12_xy, data.image, h - 1, w - 1, sphConditionGESSE);

		lambda_zero = drawPoint(a_11, a_22, a_12, h - 1, w - 1);

		//m_eigenval_struct = multiple_eigenvalues(a_11, a_22, a_12, data.image, h - 1, w - 1);             // Looking for multiple_values of struct_tensor matrices
		//m_eigenval_gesse = multiple_eigenvalues(a11_x, a22_y, a12_xy, data.image, h - 1, w - 1);          // Looking for multiple_values of gesse matrices
		// THE REASON OF MISMATCH IS SUCH THAT IN ONE OF THE METHODS WE HAVE USED, SQUARE ROOT IS AN IRRATIONAL NUMBER.

		m_eigenvalues = equal_eigenvalues(sph_cond_struct, sph_cond_gesse,
			data.image, h - 1, w - 1, a_11, a_22, a_12, vect, x_center, y_center, lambda_max);              // This method calculates vectors with 4 coordinates for constructing circles. Deteil description is in inside.

		float ** result = fromVectToArray(vect);                                                            // Convertation from list to array because it faster and more convinent
																											// Scalar product of the given vectors.
		float *circles = getEmptyArray((h - 1) * (w - 1));
		float *centers = getEmptyArray((h - 1) * (w - 1));

		Circles(result, m_eigenvalues, h - 1, w - 1, circles);
		Centers(result, m_eigenvalues, h - 1, w - 1, centers, x_center, y_center);

		IplImage *sphere_struct = convertToImage(sph_cond_struct, h - 1, w - 1);
		IplImage *cent = convertToImage(centers, h - 1, w - 1);
		IplImage *circle = convertToImage(circles, h - 1, w - 1);
		IplImage *gradient = convertToImage(grad, h, w);
		IplImage *sphere_gesse = convertToImage(sph_cond_gesse, h - 1, w - 1);
		IplImage *eigen_gesse = convertToImage(m_eigenval_gesse, h - 1, w - 1);
		IplImage *mult_eigen = convertToImage(m_eigenvalues, h - 1, w - 1);
		IplImage * res = twoColors(centers, circles, h - 1, w - 1);
		IplImage * zeroLambda = convertToImage(lambda_zero, h - 1, w - 1);

		cvShowImage("GRADIENT", gradient);
		//cvShowImage("SPH_STRUCT", sphere_struct);
		//cvShowImage("SPH_GESSE", sphere_gesse);
		//cvShowImage("EIGEN_GESSE", eigen_gesse);

		//cvShowImage("MULT_EIGEN_INTEREST", mult_eigen);
		//cvShowImage("RES", res);
		cvShowImage("LAMBDA_ZERO", zeroLambda);
		//cvShowImage("ORTHOGONAL_SPH", circle);
		//cvShowImage("CENTERS", cent);
		cvWaitKey();
		return 0;
	}

	/*
	#include<opencv2/core/core.hpp>
	#include<opencv2/highgui/highgui.hpp>
	#include<iostream>
	#include<cmath>


	using namespace cv;
	using namespace std;

	int find1(int **a, int k, int l, int iPside, int jPside, int max_w, int max_h)
	{
		int NUM = 0;
		for (int i = k; i < iPside && i < max_h; i++)
		{
			for (int j = l; j < jPside && j < max_w; j++)
			{
				if (a[i][j] == 255)
				{
					NUM = NUM + 1;
					break;
				}
			}
		}
		if (NUM > 0)
			return 1;
		else
			return 0;

	}
	double MNK(double *a, double *b, int k)
	{
		double sum1 = 0;
		double sum2 = 0;
		double sum3 = 0;
		double sum4 = 0;
		for (int i = 0; i < k; i++)
		{
			sum1 = sum1 + a[i] * b[i];
			sum2 = sum2 + a[i];
			sum3 = sum3 + b[i];
			sum4 = sum4 + a[i] * a[i];
		}
		double uns;
		uns = ((k*sum1) - (sum2*sum3)) / (k*sum4 - pow(sum2, 2));
		return uns;
	}

	int main()
	{

		Mat im = imread("C:/Users/dimap/source/repos/FIlters/3_3.bmp", 0);
		Mat_<uchar> img3 = im;
		cout << "Size of img: " << im.cols << "x" << im.rows << endl;
		for (int i = 0; i < im.rows; i++)
		{
			for (int j = 0; j < im.cols; j++)
			{
				if (img3(i, j) < 127)
					img3(i, j) = 0;
				else
					img3(i, j) = 255;
			}
		}

		int **NewMass = new int*[im.rows];
		for (int count = 0; count < im.rows; count++)
			NewMass[count] = new int[im.cols];
		for (int i = 0; i < im.rows; i++)
		{
			for (int j = 0; j < im.cols; j++)
			{
				NewMass[i][j] = img3(i, j);
			}
		}

		int ScreenWeight = im.cols;
		int ScreenHeight = im.rows;
		double eps[30];
		double Neps[30];
		int Side = ScreenWeight / 2;
		int NumSteps = 0;
		int NumBox = 0;
		int x = 0;
		int y;
		int z = 0;
		while (Side >= 1)
		{
			NumBox = 0;
			y = 0;
			while (y < ScreenHeight)
			{
				while (x < ScreenWeight)
				{
					z = find1(NewMass, x, y, x + Side, y + Side, ScreenWeight, ScreenHeight);
					if (z == 1)
						NumBox = NumBox + 1;
					x = x + Side;
				}
				x = 0;
				y = y + Side;
			}
			eps[NumSteps] = log(Side);
			Neps[NumSteps] = log(NumBox);
			NumSteps = NumSteps + 1;
			Side = Side / 2;
		}

		cout << endl;
		cout << "ln(Size of Side)\t" << "ln(Count of boxes)" << endl;
		for (int i = 0; i < NumSteps; i++)
		{
			cout << eps[i] << "\t\t\t" << Neps[i] << endl;
		}
		cout << endl;
		double unsw = MNK(eps, Neps, NumSteps);
		cout << "Fractal DIM is " << abs(unsw) << endl;

		return 0;

	}
	*/