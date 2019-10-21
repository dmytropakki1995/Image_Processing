#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <iomanip>
#include <opencv2\core.hpp>
#include <opencv2\highgui.hpp>
#include <opencv2\imgcodecs.hpp>
#include "opencv2\imgproc\imgproc.hpp"
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
#include "opencv2\shape.hpp"
#include "opencv2\highgui\highgui.hpp"
#include "imgArr.h"

imgArr OPCVImageToArray(IplImage* image);
IplImage* convertToImage(float** data, int m, int n);
IplImage* convertToImage(float* data, int m, int n);

