#pragma once
#include <vector>
#include <opencv2\core.hpp>

using namespace std;

#define MAX_ITER 5
#define CLUSTER_N 7

struct POINT
{
	int _x, _y;
	int _r, _g, _b;
	float _lambda_1, _lambda_2;
	POINT();
	void setValues(int x, int y, float lambda_1, float lambda_2);
	float getMin();
	~POINT();
};

class Cluster
{
private:
	vector<POINT> _cluster;
	int _X_center;
	int _Y_center;
	POINT _Center;
public:
	Cluster();
	void setPOINT(POINT point);
	void setCenter(POINT point);
	void deleteElements();
	POINT getPOINT(int iterator);
	POINT getCenter();
	static POINT * kMeans(vector <int> data_x, vector <int> data_y, Cluster * cluster);
	static POINT * kMeansImage(IplImage * image, Cluster * cluster);
	int getCenter_x();
	int getCenter_y();
	int getSize();
	~Cluster();
};

