//
//  main.cpp
//  trafiic-sign-detector
//
//  Created by Farida Eid on 11/27/17.
//  Copyright © 2017 Farida Eid. All rights reserved.
//

#include <opencv2/opencv.hpp>

using namespace std;

int main(int argc, const char ** argv) {
    
    if (argc < 2) {
        
        std::cerr<<"Please provide the image path"<<std::endl;
        return -1;
        
    }
    
    Mat imageOriginal = cv::imread(argv[1], cv::IMREAD_COLOR);
    
    // insert code here...
    std::cout << "Hello, World!\n";
    return 0;
}
