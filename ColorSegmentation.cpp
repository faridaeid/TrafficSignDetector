//
//  ColorSegmentation.cpp
//  trafiic-sign-detector
//
//  Created by Farida Eid on 11/27/17.
//  Copyright © 2017 Farida Eid. All rights reserved.
//

#include "ColorSegmentation.h"

void floodFillPicture(const cv::Mat& image,
                      cv::Mat& floodFill) {
    
    floodFill = image.clone();
    
    cv::floodFill(floodFill, cv::Point(0,0), cv::Scalar(255));
    
    bitwise_not(floodFill, floodFill);
    
    floodFill = image | floodFill;
    
}

cv::Mat signColorThreshold (cv::Mat& originalImage) {
    
    cv::Mat originalCopy;
    cv::cvtColor(originalImage, originalCopy, CV_BGR2HSV);
    
    const int colors = 2;
    
    std::pair<cv::Scalar, cv::Scalar> boundaries [colors] = {
        std::make_pair(cv::Scalar(0,115,51), cv::Scalar(7,255,255)),
        std::make_pair(cv::Scalar(171,115,51), cv::Scalar(179,255,255)),
    };
    
    cv::Mat segmentedImage(originalImage.rows, originalImage.cols, CV_8UC1, cv::Scalar(0));
    
    for (int i = 0; i < colors; i++) {
        
        cv::Mat Mask;
        
        cv::inRange(originalCopy, boundaries[i].first , boundaries[i].second, Mask);
                
        segmentedImage = segmentedImage | Mask;
    }
    
    floodFillPicture(segmentedImage.clone(), segmentedImage);
    
    cv::Mat output;
    
    std::vector<Blob> components = componentLabelling(segmentedImage, output);
    
    for (int i = 0; i < components.size(); i++) {
        
        components[i].drawOnImage(originalImage);
    }
    
    output.convertTo(output, CV_8U);
            
    return segmentedImage;
}
