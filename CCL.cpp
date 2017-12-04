//
//  CCL.cpp
//  trafiic-sign-detector
//
//  Created by Farida Eid on 11/30/17.
//  Copyright © 2017 Farida Eid. All rights reserved.
//

#include "CCL.h"

struct Boundaries {
    
    int xmin;
    int ymin;
    int xmax;
    int ymax;
    
    Boundaries() {
        xmax = ymax = -1;
        xmin = ymin = int(1e9);
    }
    
};

void getAllBlobs(const cv::Mat& image,
                 cv::Mat& labelledImage,
                 std::vector<Blob>& blobs,
                 std::vector<std::set<int>>& equivelantBlobs) {
    
    uint32_t nextLabel = 1;
    
    blobs.clear();
    equivelantBlobs.clear();
    
    cv::threshold(image, labelledImage, 254, 1, CV_THRESH_BINARY);
    labelledImage.convertTo(labelledImage, CV_32S);
    
    int offsetX[] = {-1, -1, 0, 1};
    int offsetY[] = {0, -1, -1, -1};
    int offsetCount = 4;
    
    Blob newBlob;
    newBlob++;
    blobs.push_back(newBlob);
    equivelantBlobs.push_back(std::set<int>());
    
    for (int y = 0; y < labelledImage.rows; y++) {
        
        for (int x = 0; x < labelledImage.cols; x++) {
            
            if (labelledImage.at<uint32_t>(y, x) == 1) {
                
                std::set<uint32_t> neighborCells;
                
                for (int i = 0; i < offsetCount; i++) {
                    
                    if ((i == 1 && x == 0) || (i > 1 && y == 0)) {
                        
                        continue;
                    }
                    
                    if (labelledImage.at<uint32_t>(y + offsetY[i], x + offsetX[i]) > 0) {
                        
                        neighborCells.insert(labelledImage.at<uint32_t>(y + offsetY[i], x + offsetX[i]));
                    }
                }
                
                if (neighborCells.size() == 0) {
                    
                    labelledImage.at<uint32_t>(y, x) = nextLabel;
                    newBlob.setLabel(nextLabel++);
                    blobs.push_back(newBlob);
                    equivelantBlobs.push_back(std::set<int>());
                    
                } else {
                    
                    uint32_t label = *neighborCells.begin();
                    blobs[label]++;
                    labelledImage.at<uint32_t>(y,x) = label;
                    
                    if (neighborCells.size() > 1) {
                        
                        std::set<uint32_t>::iterator it  = neighborCells.begin();
                        it++;
                        
                        for (; it != neighborCells.end(); it++) {
                            
                            equivelantBlobs[label].insert(*it);
                            equivelantBlobs[*it].insert(label);
                            
                        }
                        
                    }
                }
            }
        }
    }
}

int setParent(std::vector<int>& parents,
              const int& value) {
    
    if (value != parents[value]) {
        
        return parents[value] = setParent(parents, parents[value]);
        
    }
    
    return value;
    
}

void combineEquivelantBlobs(const std::vector<Blob>& blobs,
                            const std::vector<std::set<int>>& equivelance,
                            std::vector<Blob>& combinedBlobs,
                            std::vector<int>& parentLabel) {
    
    
    parentLabel.resize(blobs.size(), -1);
    parentLabel[0] = 0;
    
    for (int i = 1; i < equivelance.size(); i++) {
        
        if (parentLabel[i] == -1) {
            parentLabel[i] = i;
        }
        
        std::set<int>::iterator it;
        
        for (it = equivelance[i].begin(); it != equivelance[i].end(); it++) {
            
            if (parentLabel[*it] == -1) {
                parentLabel[*it] = i;
            }
            
            setParent(parentLabel, *it);
            
        }
    }
    
    std::unordered_map<int, int> newParentIDs;
    int newParentID = 1;
    combinedBlobs.push_back(Blob());
    
    
    for (int i = 1; i < blobs.size(); i++) {
        
        int blobParentLabel = parentLabel[blobs[i].getLabel()];
        
        if (!newParentIDs[blobParentLabel]) {
            
            Blob newBlob;
            newParentIDs[blobParentLabel] = newParentID;
            newBlob.setLabel(newParentID++);
            newBlob += blobs[i].getCount();
            combinedBlobs.push_back(newBlob);
            
        } else {
            
            combinedBlobs[newParentIDs[blobParentLabel]] += blobs[i].getCount();
            
        }
        
        parentLabel[i] = newParentIDs[blobParentLabel];
    }
}


std::vector<Blob> removeNoiseAndSetBoundaries(const cv::Mat& image,
                                              cv::vector<Blob>& blobs,
                                              const cv::vector<int>& parents,
                                              const int& thresholdMin) {
    
    std::vector<Blob> finalBlobs;
    std::vector<Boundaries> finalBlobBoundaries(blobs.size());
    
    for (int y = 0; y < image.rows; y++) {
        
        for (int x = 0; x < image.cols; x++) {
            
            if(image.at<uint32_t>(y,x) != 0) {
                
                int actualParent = parents[image.at<uint32_t>(y,x)];
                
                if (x < finalBlobBoundaries[actualParent].xmin) {
                    
                    finalBlobBoundaries[actualParent].xmin = x;
                }
                
                if (y < finalBlobBoundaries[actualParent].ymin) {
                    
                    finalBlobBoundaries[actualParent].ymin = y;
                }
                
                if (x >= finalBlobBoundaries[actualParent].xmax) {
                    
                    finalBlobBoundaries[actualParent].xmax = x;
                }
                
                if (y >= finalBlobBoundaries[actualParent].ymax) {
                    
                    finalBlobBoundaries[actualParent].ymax = y;
                }
            }
            
        }
        
    }
    
    for (int i = 0; i < blobs.size(); i++) {
        
        if (blobs[i].getCount() >= thresholdMin) {
            
            blobs[i][0] = cv::Point(finalBlobBoundaries[i].xmin, finalBlobBoundaries[i].ymin);
            blobs[i][1] = cv::Point(finalBlobBoundaries[i].xmax, finalBlobBoundaries[i].ymin);
            blobs[i][2] = cv::Point(finalBlobBoundaries[i].xmax, finalBlobBoundaries[i].ymax);
            blobs[i][3] = cv::Point(finalBlobBoundaries[i].xmin, finalBlobBoundaries[i].ymax);
            
            double blobAspectRatio = blobs[i].getAspectRatio();
            
            if (blobAspectRatio < 1.2) {
                
                finalBlobs.push_back(blobs[i]);
                
            }
        }
        
    }
    
    return finalBlobs;
    
}

std::vector<Blob> componentLabelling(const cv::Mat& image, cv::Mat& labelledImage) {
    
    std::vector<Blob> blobs;
    std::vector<std::set<int>> equivelantBlobs;
    
    getAllBlobs(image, labelledImage, blobs, equivelantBlobs);
    
    std::vector<Blob> combinedBlobs;
    std::vector<int> parentLabel;
    
    combineEquivelantBlobs(blobs, equivelantBlobs, combinedBlobs, parentLabel);
    
    return removeNoiseAndSetBoundaries(labelledImage, combinedBlobs, parentLabel, 900);
}

