//
//  util.h
//  rulerDetect
//
//  Created by Sándor Fehérvári on 2015.02.01..
//  Copyright (c) 2015 sandorfehervari. All rights reserved.
//

#ifndef __rulerDetect__util__
#define __rulerDetect__util__

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

//template <class T>
void applyYinAlgorithm(vector<uchar>& inputValues, vector<uchar>& output);

#endif /* defined(__rulerDetect__util__) */
