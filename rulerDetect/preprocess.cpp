#include "preprocess.h"
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

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
		cout << a << endl;
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

void psdt(const Mat& input, Mat& output) {
	Mat padded;                            //expand input image to optimal size
	int m = getOptimalDFTSize(input.rows);
	int n = getOptimalDFTSize(input.cols); // on the border add zero values
	copyMakeBorder(input, padded, 0, m - input.rows, 0, n - input.cols, BORDER_CONSTANT, Scalar::all(0));
	Mat planes[] = { Mat_<float>(padded), Mat::zeros(padded.size(), CV_32F) };

	Mat complexI, conjugateI;
	merge(planes, 2, complexI);         // Add to the expanded another plane with zeros
	dft(complexI, complexI);            // this way the result may fit in the source matrix
	Mat planesconj[] = {Mat_<float>(padded), Mat_<float>((planes[1]*-1))};
	merge(planesconj, 2, conjugateI);                   // planes[0] = Re(DFT(I), planes[1] = conj(Im(DFT(I)))
	Mat result = complexI * conjugateI;
	Mat resultplanes[2];
	split(result, resultplanes);                   // planes[0] = Re(DFT(I), planes[1] = Im(DFT(I))

	magnitude(resultplanes[0], resultplanes[1], resultplanes[0]);// planes[0] = magnitude
	Mat magI = resultplanes[0];
	/*
	magI += Scalar::all(1);                    // switch to logarithmic scale
	log(magI, magI);
	*/
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
	magI.copyTo(output);
	// viewable image form (float between values 0 and 1).
	imshow("psdt", magI);
}

void fourier(Mat Image, Mat& result, bool isConjugated) {
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
	if (isConjugated) {
		planes[1] = planes[1] * -1;
	}
	magnitude(planes[0], planes[1], planes[0]);// planes[0] = magnitude
	Mat magI = planes[0];

	magI += Scalar::all(1);                    // switch to logarithmic scale
	log(magI, magI);

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
	if (isConjugated) {
		imshow("fourier", magI);
	}
	else {
		imshow("conjugated", magI);
	}

}




void gaussianWindow(Mat& input, Point center, int windowSize, Mat& output) {
	Rect roi(center.x - windowSize / 2, center.y - windowSize / 2, windowSize, windowSize);
	Mat windowedImage = input(roi);
	adaptiveThreshold(input, output, 255, ADAPTIVE_THRESH_GAUSSIAN_C, CV_THRESH_BINARY, 3, 5);

}

void hough(const Mat& inputmat, Mat& output) {
	Mat input, dst, cdst;
	Mat temp;
	double Max, Min;
	inputmat.copyTo(input);
	minMaxLoc(input, &Min, &Max);
	//cout << input;
	input *= 255.0;
	input *= 2.0;
	input.convertTo(temp, CV_8U);
	imshow("temp", temp);

	minMaxLoc(temp, &Min, &Max);
	Canny(temp, dst, 50, 200, 3);
	cvtColor(dst, cdst, CV_GRAY2BGR);

	vector<Vec4i> lines;
	HoughLinesP(dst, lines, 1, CV_PI / 180, 50, 50, 10);
	for (size_t i = 0; i < lines.size(); i++)
	{
		Vec4i l = lines[i];
		line(cdst, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0, 0, 255), 3, CV_AA);
	}
	imshow("detected lines", cdst);
}