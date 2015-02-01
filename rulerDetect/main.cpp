#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/features2d/features2d.hpp>
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include "preprocess.h"
#include "eventhandler.h"
#include "line.h"

using namespace cv;
using namespace std;

void blobDetection(Mat,Mat);
void preprocessImage(Mat& inputImage, Mat& processedImage);
static EventHandler mouseHandler;

void static mouseHandlerWrapper(int event, int x, int y, int flags, void *param) {
	mouseHandler(event, x, y, flags, param);
}

void cropImage(Mat& input, Mat& output) {
	Mat temp = input.clone();
	//Mat roi2(temp, Rect(Point(255,159), Point(310,216)));
	//Mat curr_imgT = roi2.clone();
	temp.copyTo(output);
	//output = curr_imgT;
}

int main(int argc, char** argv)
{
	const char* filename = argc >= 2 ? argv[1] : "img/2.png";
	Mat Icol = imread(filename, CV_LOAD_IMAGE_COLOR);
	Mat processed;
	Mat result, resultconj;
	//namedWindow("My_Win", 1);

	//imshow("My_Win", Icol);
	cropImage(Icol, processed);
	//imshow("processed", processed);
	//preprocessImage(processed, processed);
	cv::cvtColor(processed, processed, CV_RGB2GRAY);
	//adaptiveThreshold(processed, result, 255, ADAPTIVE_THRESH_GAUSSIAN_C, CV_THRESH_BINARY, 3, 5);
	psdt(processed, result);
	Mat stg;
	float angle = getLineAngle(result, stg);
	cout << angle << endl;

    Point P1(result.cols / 2, result.rows / 2);
    Line* generatedLine = new Line(-angle, P1);
    
    vector<uchar> values = generatedLine->getImageData(processed);

    
    
	Mat colored;
	cvtColor(processed, colored, CV_GRAY2BGR);
    generatedLine->applyToImage(colored, Scalar(255,0,0));
	imshow("processed", colored);
	waitKey();

	return 0;

}

void preprocessImage(Mat& inputImage, Mat& processedImage) {
	Mat blurred, gray;
	cv::cvtColor(inputImage, gray, CV_RGB2GRAY);
	GaussianBlur(gray, blurred, cv::Size(3,3), 3);
	addWeighted(gray, 1.5, blurred, -0.5, 0, processedImage);
}



