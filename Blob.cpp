//
//  Blob.cpp
//  trafiic-sign-detector
//
//  Created by Farida Eid on 11/30/17.
//  Copyright © 2017 Farida Eid. All rights reserved.
//

#include "Blob.h"

Blob::Blob():blobCount(0), blobLabel(0) {}

void Blob::setLabel(const int& label) {
    
    blobLabel = label;
    
}

int Blob::getLabel() const{
    
    return blobLabel;
}

int Blob::getCount() const {
    
    return blobCount;
    
}

Blob Blob::operator++(int value) {
    
    Blob blob;
    
    blobCount++;
    
    return blob;
    
}

Blob Blob::operator+=(int value) {
    
    Blob blob;
    
    blobCount += value;
    
    return blob;
    
}

cv::Point& Blob::operator[](const int& index) {
    
    return corners[index];
    
}

void Blob::drawOnImage(cv::Mat & image) {
        
    for (int i = 0; i < 4; i++) {
        
        cv::line(image, corners[i], corners[(i+1)%4], cv::Scalar(239,246,37), 3, 8);
    }
    
}

double Blob::getAspectRatio() {
    
    int sideHor = corners[1].x - corners[0].x;
    int sideVer = corners[2].y - corners[1].y;
    
    if (sideHor > sideVer) {
        
        return double(sideHor)/double(sideVer);
    }
    
    return double(sideVer)/double(sideHor);
    
}
