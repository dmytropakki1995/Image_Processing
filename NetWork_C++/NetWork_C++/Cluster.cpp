#include "pch.h"
#include "Cluster.h"
#include <iostream>
#include <time.h>

POINT::POINT()
{
	this->_x = 0;
	this->_y = 0;
	this->_lambda_1 = 0;
	this->_lambda_2 = 0;
}

void POINT::setValues(int x, int y, float lambda_1, float lambda_2)
{
	_x = x;
	_y = y;
	_lambda_1 = lambda_1;
	_lambda_2 = lambda_2;
}

float POINT::getMin()
{
	if (_lambda_1 < _lambda_2)
		return _lambda_1;
	else
		return _lambda_2;
}

POINT::~POINT() { }

Cluster::Cluster()
{
	this->_cluster = {};
	this->_X_center = 0;
	this->_Y_center = 0;
}


int Cluster::getSize()
{
	return this->_cluster.size();
}

POINT Cluster::getPOINT(int iterator)
{
	return _cluster[iterator];
}

POINT Cluster::getCenter()
{
	return this->_Center;
}

POINT * Cluster::kMeans(vector <int> data_x, vector <int> data_y, Cluster * cluster)
{
	POINT * center = new POINT[CLUSTER_N];

	if (data_x.size() != data_y.size())
		throw "ERROR: INVALID DATA SIZE";

	int size_image = 1000;
	int size = data_x.size();

	int p = size / CLUSTER_N;

	for (int i = 0; i < CLUSTER_N; i++)
	{
		//srand(time(0));
		int k = rand() % p + i * p;
		center[i]._x = data_x[k];
		center[i]._y = data_y[k];
		cluster[i].setCenter(center[i]);

	}

	for (int k = 0; k < MAX_ITER; k++)
	{
		for (int i = 0; i < CLUSTER_N; i++)
			cluster[i].deleteElements();

		for (int i = 0; i < size; i++)
		{
			int x = data_x[i];
			int y = data_y[i];

			int x_c = cluster[0].getCenter_x();
			int y_c = cluster[0].getCenter_y();

			int _cl = 0;
			POINT p = POINT();

			float min = sqrt(pow(x_c - x, 2) + pow(y_c - y, 2));

			for (int j = 1; j < CLUSTER_N; j++)
			{
				if (sqrt(pow(cluster[j].getCenter_x() - x, 2) + pow(cluster[j].getCenter_y() - y, 2)) < min)
				{
					min = sqrt(pow(cluster[j].getCenter_x() - x, 2) + pow(cluster[j].getCenter_y() - y, 2));
					_cl = j;
				}
			}

			p._x = x;
			p._y = y;

			cluster[_cl].setPOINT(p);
		}

		for (int i = 0; i < CLUSTER_N; i++)
		{
			int x_mean = 0;
			int y_mean = 0;

			int summ_x = 0;
			int summ_y = 0;

			for (int j = 0; j < cluster[i].getSize(); j++)
			{
				x_mean += cluster[i].getPOINT(j)._x;
				y_mean += cluster[i].getPOINT(j)._y;
			}

			try
			{
				if (cluster[i].getSize() == 0)
					throw "ERROR";

				x_mean = int(x_mean / cluster[i].getSize());
				y_mean = int(y_mean / cluster[i].getSize());
			}
			catch (const char * massage)
			{
				cout << massage;
			}

			POINT p_new = POINT();
			p_new._x = x_mean;
			p_new._y = y_mean;

			cluster[i].setCenter(p_new);

		}

		for (int i = 0; i < CLUSTER_N; i++)
		{
			center[i]._x = cluster[i].getCenter_x();
			center[i]._y = cluster[i].getCenter_y();
		}

	}

	return center;
}

/*
POINT * Cluster::kMeansImage(IplImage * image, Cluster * cluster)
{
	int h = image->height;
	int w = image->width;

	CvMat* img = cvCreateMat(h, w, CV_32FC3);
	cvConvert(image, img);

	POINT * center = new POINT[CLUSTER_N];

	for (int i = 0; i < CLUSTER_N; i++)
	{
		center[i]._x = rand() % image->width;
		center[i]._y = rand() % image->height;
		center[i]._r = 0;
		center[i]._g = 0;
		center[i]._b = 0;

		cluster[i].setCenter(center[i]);
	}
	for (int iter = 0; iter < MAX_ITER; iter++)
	{

		for (int i = 0; i < CLUSTER_N; i++)
			cluster[i].deleteElements();

		for (int i = 0; i < h; i++)
		{
			for (int j = 0; j < w; j++)
			{
				int r_c = cluster[0].getCenter()._r;
				int g_c = cluster[0].getCenter()._g;
				int b_c = cluster[0].getCenter()._b;

				int _cl = 0;

				int im_b = cvGet2D(img, i, j).val[0];
				int im_g = cvGet2D(img, i, j).val[1];
				int im_r = cvGet2D(img, i, j).val[2];

				float min = sqrt(pow(b_c - im_b, 2) + pow(g_c - im_g, 2) + pow(r_c - im_r, 2));

				for (int k = 1; k < CLUSTER_N; k++)
				{
					if (sqrt(pow(cluster[k].getCenter()._b - im_b, 2) + pow(cluster[k].getCenter()._g - im_g, 2) + pow(cluster[k].getCenter()._r - im_r, 2)) < min)
					{
						min = sqrt(pow(cluster[k].getCenter()._b - im_b, 2) + pow(cluster[k].getCenter()._g - im_g, 2) + pow(cluster[k].getCenter()._r - im_r, 2));
						_cl = k;
					}
				}

				POINT p = POINT();

				p._x = j;
				p._y = i;
				p._b = im_b;
				p._g = im_g;
				p._r = im_r;

				cluster[_cl].setPOINT(p);
			}
		}

		for (int i = 0; i < CLUSTER_N; i++)
		{
			int x_mean = 0;
			int y_mean = 0;

			for (int j = 0; j < cluster[i].getSize(); j++)
			{
				x_mean += cluster[i].getPOINT(j)._x;
				y_mean += cluster[i].getPOINT(j)._y;
			}

			try
			{
				if (cluster[i].getSize() == 0)
					throw "ERROR";

				x_mean = int(x_mean / cluster[i].getSize());
				y_mean = int(y_mean / cluster[i].getSize());
			}
			catch (const char * massage)
			{
				cout << massage;
			}

			POINT p_new = POINT();
			p_new._x = x_mean;
			p_new._y = y_mean;

			cluster[i].setCenter(p_new);

		}

		for (int i = 0; i < CLUSTER_N; i++)
		{
			center[i]._x = cluster[i].getCenter_x();
			center[i]._y = cluster[i].getCenter_y();
		}
	}
	return center;
}

*/

POINT * Cluster::kMeansImage(IplImage * image, Cluster * cluster)
{
	int h = image->height;
	int w = image->width;

	CvMat* img = cvCreateMat(h, w, CV_32FC3);
	cvConvert(image, img);

	POINT * center = new POINT[CLUSTER_N];

	/*
	int x_0 = 10;
	int y_0 = 10;
	center[0]._x = x_0;
	center[0]._y = y_0;
	center[0]._r = cvGet2D(img, y_0, x_0).val[0];
	center[0]._g = cvGet2D(img, y_0, x_0).val[1];
	center[0]._b = cvGet2D(img, y_0, x_0).val[2];
	cluster[0].setCenter(center[0]);

	int x_1 = 250;
	int y_1 = 100;
	center[1]._x = 250;
	center[1]._y = 100;
	center[1]._r = cvGet2D(img, y_1, x_1).val[0];
	center[1]._g = cvGet2D(img, y_1, x_1).val[1];
	center[1]._b = cvGet2D(img, y_1, x_1).val[2];
	cluster[1].setCenter(center[1]);

	int x_2 = 300;
	int y_2 = 300;
	center[2]._x = x_2;
	center[2]._y = y_2;
	center[2]._r = cvGet2D(img, y_2, x_2).val[0];
	center[2]._g = cvGet2D(img, y_2, x_2).val[1];
	center[2]._b = cvGet2D(img, y_2, x_2).val[2];
	cluster[2].setCenter(center[2]);
	*/

	for (int i = 0; i < CLUSTER_N; i++)
	{
		//srand(time(0));
		int x = rand() % image->width;
		int y = rand() % image->height;
		center[i]._x = x;
		center[i]._y = y;
		center[i]._r = cvGet2D(img, y, x).val[0];
		center[i]._g = cvGet2D(img, y, x).val[1];
		center[i]._b = cvGet2D(img, y, x).val[2];

		cluster[i].setCenter(center[i]);
	}
	

	for (int iter = 0; iter < MAX_ITER; iter++)
	{
		for (int i = 0; i < CLUSTER_N; i++)
			cluster[i].deleteElements();

		for (int i = 0; i < h; i++)
		{
			for (int j = 0; j < w; j++)
			{
				int r_c = cluster[0].getCenter()._r;
				int g_c = cluster[0].getCenter()._g;
				int b_c = cluster[0].getCenter()._b;

				int _cl = 0;

				int im_b = cvGet2D(img, i, j).val[0];
				int im_g = cvGet2D(img, i, j).val[1];
				int im_r = cvGet2D(img, i, j).val[2];

				float min = sqrt(pow(b_c - im_b, 2) + pow(g_c - im_g, 2) + pow(r_c - im_r, 2));

				for (int k = 1; k < CLUSTER_N; k++)
				{
					if (sqrt(pow(cluster[k].getCenter()._b - im_b, 2) + pow(cluster[k].getCenter()._g - im_g, 2) + pow(cluster[k].getCenter()._r - im_r, 2)) < min)//Problem with center
					{
						min = sqrt(pow(cluster[k].getCenter()._b - im_b, 2) + pow(cluster[k].getCenter()._g - im_g, 2) + pow(cluster[k].getCenter()._r - im_r, 2));
						_cl = k;
					}
				}

				POINT p = POINT();

				p._x = j;
				p._y = i;
				p._b = im_b;
				p._g = im_g;
				p._r = im_r;

				cluster[_cl].setPOINT(p);
			}
		}
		
		for (int i = 0; i < CLUSTER_N; i++)
		{
			int r_mean = 0;
			int g_mean = 0;
			int b_mean = 0;
			int x_mean = 0;
			int y_mean = 0;

			for (int j = 0; j < cluster[i].getSize(); j++)
			{
				x_mean += cluster[i].getPOINT(j)._x;
				y_mean += cluster[i].getPOINT(j)._y;
				r_mean += cluster[i].getPOINT(j)._r;
				g_mean += cluster[i].getPOINT(j)._g;
				b_mean += cluster[i].getPOINT(j)._b;
			}

			try
			{
				if (cluster[i].getSize() == 0)
					throw "ERROR";

				x_mean = int(x_mean / cluster[i].getSize());
				y_mean = int(y_mean / cluster[i].getSize());
				r_mean = int(r_mean / cluster[i].getSize());
				g_mean = int(g_mean / cluster[i].getSize());
				b_mean = int(b_mean / cluster[i].getSize());
			}
			catch (const char * massage)
			{
				cout << massage;
			}

			POINT p_new = POINT();
			p_new._x = x_mean;
			p_new._y = y_mean;
			p_new._r = r_mean;
			p_new._g = g_mean;
			p_new._b = b_mean;

			cluster[i].setCenter(p_new);

		}

		for (int i = 0; i < CLUSTER_N; i++)
		{
			center[i]._x = cluster[i].getCenter()._x;
			center[i]._y = cluster[i].getCenter()._y;
			center[i]._r = cluster[i].getCenter()._r;
			center[i]._g = cluster[i].getCenter()._g;
			center[i]._b = cluster[i].getCenter()._b;
		}
	}

	return center;
}

int Cluster::getCenter_x()
{
	return this->_X_center;
}

int Cluster::getCenter_y()
{
	return this->_Y_center;
}

void Cluster::setCenter(POINT point)
{
	_X_center = point._x;
	_Y_center = point._y;
	this->_Center = point;
}

void Cluster::deleteElements()
{
	this->_cluster.erase(this->_cluster.begin(), this->_cluster.end());
}

void Cluster::setPOINT(POINT point)
{
	_cluster.push_back(point);
}

Cluster::~Cluster()
{
	this->_cluster.erase(this->_cluster.begin(), this->_cluster.end());
}
