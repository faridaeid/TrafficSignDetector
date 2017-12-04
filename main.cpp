//
//  main.cpp
//  trafiic-sign-detector
//
//  Created by Farida Eid on 11/27/17.
//  Copyright © 2017 Farida Eid. All rights reserved.
//

#include <opencv2/opencv.hpp>
#include "ColorSegmentation.h"

using namespace cv;

int main(int argc, const char ** argv) {
    
    if (argc < 2) {
        
        std::cerr<<"Please provide the image path"<<std::endl;
        return -1;
        
    }
    
    std::string imageNamesDef = "Am_Rojo00";
    
    for (int i = 1 ; i < 26; i++) {
        
        std::string imageName = imageNamesDef;
        
        if (i < 10) {
            
            imageName += '0';
            
        }
        
        imageName +=  std::to_string(i) + ".jpg";
        std::string windowName = "Output Image Number : " + std::to_string(i);
        
        Mat imageOriginal = imread(imageName, IMREAD_COLOR);

        cv::Mat thresh = signColorThreshold(imageOriginal);
        
        namedWindow(windowName, WINDOW_NORMAL);
        imshow(windowName, imageOriginal);
        
    }
    
    waitKey();
    
}
