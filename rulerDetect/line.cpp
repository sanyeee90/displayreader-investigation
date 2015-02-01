//
//  line.cpp
//  rulerDetect
//
//  Created by Sándor Fehérvári on 2015.02.01..
//  Copyright (c) 2015 sandorfehervari. All rights reserved.
//

#include "line.h"

static const int LINE_START = 0;

pair<Point, Point> Line::getLinePoints(int width, int height) {
    float m = tan(angle);
    float n = startPoint.y - (tan (angle) * startPoint.x );
    Point p1(LINE_START, LINE_START*m + n);
    Point p2(width, width*m + n);
    return pair<Point, Point>(p1,p2);
}

void Line::applyToImage(cv::Mat &inputImg, const Scalar& color) {
    pair<Point, Point> linePoints = getLinePoints(inputImg.cols, inputImg.rows);
    line(inputImg, linePoints.first, linePoints.second, color);
}

vector<uchar> Line::getImageData(const cv::Mat &input) {
    pair<Point, Point> linePoints = getLinePoints(input.cols, input.rows);
    cv::LineIterator it(input, linePoints.first, linePoints.second, 8);
    vector<uchar> buf;
    buf.resize(it.count);
    for(int i = 0; i < it.count; i++, ++it)
    {
        buf[i] =(const uchar) it.ptr[i];
    }
    return buf;
}
