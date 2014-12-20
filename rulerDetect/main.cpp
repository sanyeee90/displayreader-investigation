#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <opencv2/features2d/features2d.hpp>
#include <iostream>

using namespace cv;
using namespace std;

void fourier(Mat, Mat&);
void blobDetection(Mat,Mat);
void preprocessImage(Mat& inputImage, Mat& processedImage);
int main(int argc, char** argv)
{
	const char* filename = argc >= 2 ? argv[1] : "img/pfm20_ruler_num.png";
	Mat Icol = imread(filename, CV_LOAD_IMAGE_COLOR);
	Mat processed;
	preprocessImage(Icol, processed);

	//imshow("Binary", processed);
	waitKey();

	return 0;

	Mat I = imread(filename, CV_LOAD_IMAGE_GRAYSCALE);
	if (I.empty())
		return -1;
	Mat BinaryI=I;
	Point center = Point(BinaryI.cols / 2, BinaryI.rows / 2);
	double angle = -50.0;
	double scale = 0.6;
	//Mat rot_mat = getRotationMatrix2D(center, angle, scale);
	//warpAffine(BinaryI, BinaryI, rot_mat, BinaryI.size());
	//threshold(I, BinaryI, 100, 255, CV_THRESH_BINARY);
	//bitwise_not(BinaryI, BinaryI);
	//fourier(BinaryI, BinaryI);
	//blobDetection(Icol,BinaryI);
	//threshold(BinaryI, BinaryI, 1, 1, CV_THRESH_BINARY);
	//imshow("Input Image", I);    // Show the result
	Size size(300, 400);
	resize(BinaryI, BinaryI, size);//resize image
	imshow("Binary", BinaryI);
	waitKey();

	return 0;
}

void preprocessImage(Mat& inputImage, Mat& processedImage) {
	Mat grayImage, blurred;
	cv::cvtColor(inputImage, grayImage, CV_RGB2GRAY);
	GaussianBlur(grayImage, blurred, Size(3, 3), 0, 0);
	imshow("fff",(grayImage-blurred));
}

void fourier(Mat Image, Mat& result) {
	Mat padded;                            //expand input image to optimal size
	int m = getOptimalDFTSize(Image.rows);
	int n = getOptimalDFTSize(Image.cols); // on the border add zero values
	copyMakeBorder(Image, padded, 0, m - Image.rows, 0, n - Image.cols, BORDER_CONSTANT, Scalar::all(0));

	Mat planes[] = { Mat_<float>(padded), Mat::zeros(padded.size(), CV_32F) };
	Mat complexI;
	merge(planes, 2, complexI);         // Add to the expanded another plane with zeros

	dft(complexI, complexI);            // this way the result may fit in the source matrix

	// compute the magnitude and switch to logarithmic scale
	// => log(1 + sqrt(Re(DFT(I))^2 + Im(DFT(I))^2))
	split(complexI, planes);                   // planes[0] = Re(DFT(I), planes[1] = Im(DFT(I))
	magnitude(planes[0], planes[1], planes[0]);// planes[0] = magnitude
	Mat magI = planes[0];
	
	//magI += Scalar::all(1);                    // switch to logarithmic scale
	//log(magI, magI);
	
	// crop the spectrum, if it has an odd number of rows or columns
	magI = magI(Rect(0, 0, magI.cols & -2, magI.rows & -2));

	// rearrange the quadrants of Fourier image  so that the origin is at the image center
	int cx = magI.cols / 2;
	int cy = magI.rows / 2;

	Mat q0(magI, Rect(0, 0, cx, cy));   // Top-Left - Create a ROI per quadrant
	Mat q1(magI, Rect(cx, 0, cx, cy));  // Top-Right
	Mat q2(magI, Rect(0, cy, cx, cy));  // Bottom-Left
	Mat q3(magI, Rect(cx, cy, cx, cy)); // Bottom-Right

	Mat tmp;                           // swap quadrants (Top-Left with Bottom-Right)
	q0.copyTo(tmp);
	q3.copyTo(q0);
	tmp.copyTo(q3);

	q1.copyTo(tmp);                    // swap quadrant (Top-Right with Bottom-Left)
	q2.copyTo(q1);
	tmp.copyTo(q2);

	normalize(magI, magI, 0, 1, CV_MINMAX); // Transform the matrix with float values into a
	magI.copyTo(result);
	// viewable image form (float between values 0 and 1).
	imshow("spectrum magnitude", magI);
}

void blobDetection(Mat orig, Mat src) {

		bitwise_not(src, src);
		Mat dst2;
		//erode(src, dst2,100.0);
		int largest_area = 0;
		int largest_contour_index = 0;
		Mat temp(src.rows, src.cols, CV_8UC1);
		Mat dst(src.rows, src.cols, CV_8UC1, Scalar::all(0));
		Rect bounding_rect;
		src.copyTo(temp);

		vector<vector<Point>> contours; // storing contour
		vector<Vec4i> hierarchy;

		findContours(temp, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);

		for (int i = 0; i< contours.size(); i++) // iterate
		{
			double a = contourArea(contours[i], false);  //Find the largest area of contour
			cout << a<<endl;
			if (a>largest_area)
			{
				largest_area = a;
				largest_contour_index = i;
				bounding_rect = boundingRect(contours[i]);
				
			}
		}

		Scalar color(0, 255, 255);
		drawContours(orig, contours, largest_contour_index, color, CV_FILLED, 8, hierarchy); // Draw the largest contour using previously stored index.
		rectangle(orig, bounding_rect, Scalar(0, 255, 0), 1, 8, 0);
		imshow("blobs", orig);
		//imshow("dst", dst2);
}