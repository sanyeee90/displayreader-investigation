#ifndef PREPROCESS_H
#define PREPROCESS_H
#include <opencv2/core/core.hpp>

using namespace cv;
using namespace std;

void gaussianWindow(Mat& input, Point center, int windowSize, Mat& output);
void fourier(Mat Image, Mat& result, bool isConjugated);
void psdt(const Mat& input, Mat& output);
void blobDetection(Mat orig, Mat src);
float getLineAngle(const Mat& input, Mat& output);
#endif