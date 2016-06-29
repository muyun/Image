#ifndef MASKED_FACE_DETECTION_H
#define MASKED_FACE_DETECTION_H

//#include <iostream>
//#include <string>

//opencv
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/ml/ml.hpp>

#include <cv.h>
#include <highgui.h>

//c++
#include <string>

//svm
#include "svm.h"

#define _MAX_NUM  150  // max #num to be sorted
//char *file_ptr[_MAX_NUM];   //pointers to files; pointer arrays

typedef struct {
	char nir_file_name[_MAX_FNAME];  //, filename lengh, _MAX_FNAME is defined in stdlib.h
	char visible_file_name[_MAX_FNAME];
	double nir_norm_mean;
	double visible_norm_mean;
	float value;
} Image;

//file_ptr[i] is a Image pointer,
// and *file_ptr[i] is the Image it points to, the first character of the i-th saved the file name
//Image *file_ptr[_MAX_NUM]; //pointers to files (Image); pointer arrays

double GetMean(const std::string& image);

bool DetectMask(const std::string& image1, const std::string& image2);

//void ReadFiles(const std::string& files);

void NormalizeAll(Image* image_ptr, int nfiles, double max);
double  Normalize(double x, double max);
void PrintInfo(Image* image_ptr, int nfiles);

//#define MAX_MEAN 0
//#define MIN_MEAN 1000

#endif