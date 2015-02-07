//
//  line.h
//  rulerDetect
//
//  Created by Sándor Fehérvári on 2015.02.01..
//  Copyright (c) 2015 sandorfehervari. All rights reserved.
//

#ifndef __rulerDetect__line__
#define __rulerDetect__line__

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

class Line {
private:
    float angle;
    Point startPoint;
    pair<Point, Point> getLinePoints(int, int);
public:
    Line(float angle, Point referencept):angle(angle),startPoint(referencept) {}
    void applyToImage(Mat& inputImg, const Scalar& color);
    void applyScalePoints(Mat& inputImg, const int length);
    vector<uchar> getImageData(const Mat& input);
};

#endif /* defined(__rulerDetect__line__) */
