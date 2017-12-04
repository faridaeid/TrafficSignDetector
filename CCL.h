//
//  CCL.hpp
//  trafiic-sign-detector
//
//  Created by Farida Eid on 11/30/17.
//  Copyright © 2017 Farida Eid. All rights reserved.
//

#ifndef CCL_hpp
#define CCL_hpp

#include "Blob.h"
#include <vector>
#include <set>
#include <unordered_map>

std::vector<Blob> componentLabelling(const cv::Mat&, cv::Mat&);


#endif /* CCL_hpp */
