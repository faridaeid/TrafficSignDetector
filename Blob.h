//
//  Blob.hpp
//  trafiic-sign-detector
//
//  Created by Farida Eid on 11/30/17.
//  Copyright © 2017 Farida Eid. All rights reserved.
//

#ifndef Blob_hpp
#define Blob_hpp

#include <opencv2/opencv.hpp>

class Blob {
    
private:
    int blobCount;
    int blobLabel;
    cv::Point corners[4];
    
public:
    Blob();
    void setLabel(const int&);
    int getLabel() const;
    int getCount() const;
    Blob operator++(int);
    Blob operator+=(int);
    cv::Point& operator[](const int&);
    void drawOnImage(cv::Mat&);
    double getAspectRatio();
    
};

#endif /* Blob_hpp */
