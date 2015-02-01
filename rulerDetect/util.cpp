//
//  util.cpp
//  rulerDetect
//
//  Created by Sándor Fehérvári on 2015.02.01..
//  Copyright (c) 2015 sandorfehervari. All rights reserved.
//

#include "util.h"
#include "../yinacf.h"

YinACF<uchar> yin;

void initYin(float sampleRate, float minFreq) {
    uchar w, tmax;
    w = ceil(sampleRate/minFreq);
    tmax = w;
    yin.build(w, tmax);
}

void applyYinAlgorithm(vector<uchar>& inputValues, vector<uchar>& output) {
    for (int i = 0; i < inputValues.size(); ++i)
        output[i] = yin.tick(inputValues[i]);
}