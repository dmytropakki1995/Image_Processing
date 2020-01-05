#include "pch.h"
#include <iostream>
#include <fstream>
#include <opencv2\core.hpp>
#include <opencv2\highgui.hpp>
#include <opencv2\imgcodecs.hpp>
#include "opencv2\imgproc\imgproc.hpp"
#include "opencv2\shape.hpp"
#include "opencv2\highgui\highgui.hpp"
#include "opencv2\video.hpp"
#include "NetWork.h"
#include "Cluster.h"
#include "time.h"

/*
	IplImage * image = nullptr;
	image = cvLoadImage("C:/Users/dimap/source/repos/FIlters/Pictures/ordos.jpg");

	int h = image->height, w = image->width;
	float *data = Gray(image);

	int in_size = 9;
	int layer_size_1 = 8;
	int layer_size_2 = 12;
	int layer_size_3 = 3;

	int layer = 0;

	POINT * point = new POINT[(h - 2) * (w - 2)];

	Cluster cluster1 = Cluster();
	Cluster cluster2 = Cluster();
	Cluster cluster3 = Cluster();

	Neuron * layer_1 = new Neuron[layer_size_1];
	Neuron * layer_2 = new Neuron[layer_size_2];
	Neuron * layer_3 = new Neuron[layer_size_3];

	NeuralNetwork netWork = NeuralNetwork(layer_1, layer_2, layer_3);

	float ** w_1 = netWork.Weight(weight_1);
	float ** w_3 = netWork.Weight(weight_3);

	for (int i = 0; i < h - 2; i++)
	{
		for (int j = 0; j < w - 2; j++)
		{
			float *input_data = new float[9];
			for (int k = 0; k < 3; k++)
				for (int l = 0; l < 3; l++)
					input_data[k * 3 + l] = data[(i * w + j) + (k * w + l)];

			try
			{
				netWork.defineLayer(input_data, w_1, layer_size_1, in_size, layer = 1);
			}
			catch (const char* massage)
			{
				cout << massage << endl;
			}

			float ** w_2 = netWork.Weight(weight_2);

			try
			{
				netWork.defineLayer(netWork._data_layer_1, w_2, layer_size_2, layer_size_1, layer = 2);
			}
			catch (const char* massage)
			{
				cout << massage << endl;
			}

			try
			{
				netWork.defineLayer(netWork._data_layer_2, w_3, layer_size_3, layer_size_2, layer = 3);
			}
			catch (const char* massage)
			{
				cout << massage << endl;
			}

			float a11 = netWork._data_layer_3[0]._value;
			float a12 = netWork._data_layer_3[1]._value;
			float a22 = netWork._data_layer_3[2]._value;

			float lambda_1 = 0.5 * (a11 + a22) + sqrt(pow(((a11 + a22) / 2), 2) - (a11 * a22 - a12 * a12));
			float lambda_2 = 0.5 * (a11 + a22) - sqrt(pow(((a11 + a22) / 2), 2) - (a11 * a22 - a12 * a12));

			point[i * (w - 2) + j].setValues(j, i, lambda_1, lambda_2);

			if (point[i * (w - 2) + j].getMin() < 30)
				cluster1.setPOINT(point[i * (w - 2) + j]);
			else
			{
				POINT empty_point = POINT();
				cluster1.setPOINT(empty_point);
			}

			if ((point[i * (w - 2) + j].getMin() >= 30) && (point[i * (w - 2) + j].getMin() < 200))
				cluster2.setPOINT(point[i * (w - 2) + j]);
			else
			{
				POINT empty_point = POINT();
				cluster2.setPOINT(empty_point);
			}

			if (point[i * (w - 2) + j].getMin() >= 200)
				cluster3.setPOINT(point[i * (w - 2) + j]);
			else
			{
				POINT empty_point = POINT();
				cluster3.setPOINT(empty_point);
			}
		}
	}

	cout << cluster1.getCenter_x() << endl;

	netWork.~NeuralNetwork();

	IplImage * cluster_1 = Color(cluster1, h - 2, w - 2);
	IplImage * cluster_2 = Color(cluster2, h - 2, w - 2);
	IplImage * cluster_3 = Color(cluster3, h - 2, w - 2);

	cvShowImage("G0", cluster_1);
	cvShowImage("G1", cluster_2);
	cvShowImage("G2", cluster_3);

	cvWaitKey();
	*/

using namespace std;

float * Gray(IplImage *img)
{
	CvMat* image = cvCreateMat(img->height, img->width, CV_32FC3);
	cvConvert(img, image);

	int h = image->height;
	int w = image->width;

	float* result = new float[h * w];

	for (int i = 0; i < h; i++)
		for (int j = 0; j < w; j++)
		{
			result[i * w + j] = int((cvGet2D(image, i, j).val[0] * 0.2125) +
				(cvGet2D(image, i, j).val[1] * 0.7154) +
				(cvGet2D(image, i, j).val[2] * 0.0721));
		}

	return result;

}
IplImage * Color(Cluster cluster, int h, int w)
{
	cv::Mat img(h, w, CV_8UC3, cvScalar(0, 0, 0));

	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < w; j++)
		{
			if ((j == cluster.getPOINT(i * w + j)._x) && (i == cluster.getPOINT(i * w + j)._y))
			{
				img.at<cv::Vec3b>(i, j)[0] = 255;
				img.at<cv::Vec3b>(i, j)[1] = 255;
				img.at<cv::Vec3b>(i, j)[2] = 255;

			}
		}
	}

	IplImage* result = cvCloneImage(&(IplImage)img);
	return result;
}

IplImage * show_Data(vector<int> x, vector<int> y, POINT * point)
{
	cv::Mat img(1000, 1000, CV_8UC3, cvScalar(0, 0, 0));

	for (int i = 0; i < x.size(); i++)
	{
		img.at<cv::Vec3b>(y[i], x[i])[0] = 255;
		img.at<cv::Vec3b>(y[i], x[i])[1] = 255;
		img.at<cv::Vec3b>(y[i], x[i])[2] = 255;

	}

	for (int i = 0; i < CLUSTER_N; i++)
	{
		int x = point[i]._x;
		int y = point[i]._y;
		img.at<cv::Vec3b>(y, x)[0] = 0;
		img.at<cv::Vec3b>(y, x)[1] = 0;
		img.at<cv::Vec3b>(y, x)[2] = 255;
	}

	IplImage* result = cvCloneImage(&(IplImage)img);
	return result;
}

IplImage * Image_Centers(IplImage * img, POINT * point)
{

	int h = img->height;
	int w = img->width;

	CvMat* image = cvCreateMat(h, w, CV_32FC3);
	cvConvert(img, image);

	cv::Mat a(h, w, CV_8UC3, cvScalar(0, 0, 0));

	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < w; j++)
		{
			a.at<cv::Vec3b>(i, j)[0] = cvGet2D(image, i, j).val[0];
			a.at<cv::Vec3b>(i, j)[1] = cvGet2D(image, i, j).val[1];
			a.at<cv::Vec3b>(i, j)[2] = cvGet2D(image, i, j).val[2];
		}
	}

	for (int i = 0; i < CLUSTER_N; i++)
	{
		int x_c = point[i]._x;
		int y_c = point[i]._y;
		a.at<cv::Vec3b>(y_c, x_c)[0] = point[i]._r;
		a.at<cv::Vec3b>(y_c, x_c)[1] = point[i]._g;;
		a.at<cv::Vec3b>(y_c, x_c)[2] = point[i]._b;;

		cv::Point pt1(x_c - 50, y_c - 50);
		cv::Point pt2(x_c + 50, y_c + 50);
		cv::rectangle(a, pt1, pt2, cv::Scalar(255, 0, 0));
	}

	IplImage* result = cvCloneImage(&(IplImage)a);
	return result;
}

IplImage * showClusters(Cluster * cluster)
{
	cv::Mat img(1000, 1000, CV_8UC3, cvScalar(0, 0, 0));

	for (int i = 0; i < CLUSTER_N; i++)
	{
		int first = rand() % 255;
		int second = rand() % 255;
		int third = rand() % 255;
		for (int j = 0; j < cluster[i].getSize(); j++)
		{
			int x = cluster[i].getPOINT(j)._x;
			int y = cluster[i].getPOINT(j)._y;

			img.at<cv::Vec3b>(y, x)[0] = first;
			img.at<cv::Vec3b>(y, x)[1] = second;;
			img.at<cv::Vec3b>(y, x)[2] = third;;
		}
	}

	IplImage* result = cvCloneImage(&(IplImage)img);
	return result;
}

IplImage * showCl(Cluster * cluster, IplImage * image)
{
	cv::Mat img(image->height, image->width, CV_8UC3, cvScalar(0, 0, 0));

	for (int i = 0; i < CLUSTER_N; i++)
	{
		int first = rand() % 255;
		int second = rand() % 255;
		int third = rand() % 255;
		for (int j = 0; j < cluster[i].getSize(); j++)
		{
			int x = cluster[i].getPOINT(j)._x;
			int y = cluster[i].getPOINT(j)._y;

			img.at<cv::Vec3b>(y, x)[0] = first;
			img.at<cv::Vec3b>(y, x)[1] = second;;
			img.at<cv::Vec3b>(y, x)[2] = third;;
		}
	}

	for (int i = 0; i < CLUSTER_N; i++)
	{
		int x_c = cluster[i].getCenter()._x;
		int y_c = cluster[i].getCenter()._y;
		img.at<cv::Vec3b>(y_c, x_c)[0] = 0;
		img.at<cv::Vec3b>(y_c, x_c)[1] = 0;
		img.at<cv::Vec3b>(y_c, x_c)[2] = 0;
	}

	IplImage* result = cvCloneImage(&(IplImage)img);
	return result;
}


int main()
{
	/*
	IplImage * image = nullptr;
	image = cvLoadImage("C:/Users/dimap/source/repos/FIlters/Pictures/ordos.jpg");

	int h = image->height, w = image->width;
	float *data = Gray(image);

	int in_size = 9;
	int layer_size_1 = 8;
	int layer_size_2 = 12;
	int layer_size_3 = 3;

	int layer = 0;

	POINT * point = new POINT[(h - 2) * (w - 2)];

	Cluster cluster1 = Cluster();
	Cluster cluster2 = Cluster();
	Cluster cluster3 = Cluster();

	Neuron * layer_1 = new Neuron[layer_size_1];
	Neuron * layer_2 = new Neuron[layer_size_2];
	Neuron * layer_3 = new Neuron[layer_size_3];

	NeuralNetwork netWork = NeuralNetwork(layer_1, layer_2, layer_3);

	float ** w_1 = netWork.Weight(weight_1);
	float ** w_3 = netWork.Weight(weight_3);

	for (int i = 0; i < h - 2; i++)
	{
		for (int j = 0; j < w - 2; j++)
		{
			float *input_data = new float[9];
			for (int k = 0; k < 3; k++)
				for (int l = 0; l < 3; l++)
					input_data[k * 3 + l] = data[(i * w + j) + (k * w + l)];

			try
			{
				netWork.defineLayer(input_data, w_1, layer_size_1, in_size, layer = 1);
			}
			catch (const char* massage)
			{
				cout << massage << endl;
			}

			float ** w_2 = netWork.Weight(weight_2);

			try
			{
				netWork.defineLayer(netWork._data_layer_1, w_2, layer_size_2, layer_size_1, layer = 2);
			}
			catch (const char* massage)
			{
				cout << massage << endl;
			}

			try
			{
				netWork.defineLayer(netWork._data_layer_2, w_3, layer_size_3, layer_size_2, layer = 3);
			}
			catch (const char* massage)
			{
				cout << massage << endl;
			}

			float a11 = netWork._data_layer_3[0]._value;
			float a12 = netWork._data_layer_3[1]._value;
			float a22 = netWork._data_layer_3[2]._value;

			float lambda_1 = 0.5 * (a11 + a22) + sqrt(pow(((a11 + a22) / 2), 2) - (a11 * a22 - a12 * a12));
			float lambda_2 = 0.5 * (a11 + a22) - sqrt(pow(((a11 + a22) / 2), 2) - (a11 * a22 - a12 * a12));

			point[i * (w - 2) + j].setValues(j, i, lambda_1, lambda_2);

			if (point[i * (w - 2) + j].getMin() < 30)
				cluster1.setPOINT(point[i * (w - 2) + j]);
			else
			{
				POINT empty_point = POINT();
				cluster1.setPOINT(empty_point);
			}

			if ((point[i * (w - 2) + j].getMin() >= 30) && (point[i * (w - 2) + j].getMin() < 200))
				cluster2.setPOINT(point[i * (w - 2) + j]);
			else
			{
				POINT empty_point = POINT();
				cluster2.setPOINT(empty_point);
			}

			if (point[i * (w - 2) + j].getMin() >= 200)
				cluster3.setPOINT(point[i * (w - 2) + j]);
			else
			{
				POINT empty_point = POINT();
				cluster3.setPOINT(empty_point);
			}
		}
	}

	cout << cluster1.getCenter_x() << endl;

	netWork.~NeuralNetwork();

	IplImage * cluster_1 = Color(cluster1, h - 2, w - 2);
	IplImage * cluster_2 = Color(cluster2, h - 2, w - 2);
	IplImage * cluster_3 = Color(cluster3, h - 2, w - 2);

	cvShowImage("G0", cluster_1);
	cvShowImage("G1", cluster_2);
	cvShowImage("G2", cluster_3);

	cvWaitKey();
	*/

	/*
	vector<int> x;
	vector<int> y;

	fstream file;
	file.open("C:/Users/dimap/PythonProjects/123.csv", ios::in);

	string line;

	while (getline(file, line, '\n'))
	{
		string tempLine_1;
		string tempLine_2;
		for (int i = 0; i < line.size(); i++)
		{
			if (line[i] == ',')
			{
				for (int j = i + 1; j < line.size(); j++)
					tempLine_2 += line[j];
				break;

			}
			tempLine_1 += line[i];
		}
		x.push_back(int(atof(tempLine_1.c_str())) / 1000);

		if ((int(atof(tempLine_2.c_str())) / 10) > 1000)
			y.push_back(int(atof(tempLine_2.c_str())) / 1000);
		else
			y.push_back(int(atof(tempLine_2.c_str())) / 10);
	}

	file.close();

	Cluster * cluster = new Cluster[CLUSTER_N];
	POINT * point = new POINT[CLUSTER_N];

	point = Cluster::kMeans(x, y, cluster);

	IplImage * cluster_img = show_Data(x, y, point);
	cvShowImage("CLUSTER", cluster_img);

	IplImage * a = showClusters(cluster);
	cvShowImage("CLUS", a);
	*/

	
	IplImage * image = nullptr;
	image = cvLoadImage("C:/Users/dimap/source/repos/FIlters/Pictures/123.jpg");

	POINT * point = new POINT[CLUSTER_N];
	Cluster * cluster = new Cluster[CLUSTER_N];

	point = Cluster::kMeansImage(image, cluster);

	IplImage * cluster_data = showCl(cluster, image);
	IplImage * cl_detect = Image_Centers(image, point);

	cvShowImage("CLUSTERS", cluster_data);
	cvShowImage("123", cl_detect);
	

	cvWaitKey();

	return 0;
}

