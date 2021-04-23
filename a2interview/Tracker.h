#pragma once

#ifdef _DEBUG               
#pragma comment(lib, "opencv_core430d.lib")       
#pragma comment(lib, "opencv_highgui430d.lib")    
#pragma comment(lib, "opencv_imgcodecs430d.lib")  
#pragma comment(lib, "opencv_videoio430d.lib")  
#pragma comment(lib, "opencv_imgproc430d.lib")
#pragma comment(lib, "opencv_tracking430d.lib")

#else       
#pragma comment(lib, "opencv_core430.lib")       
#pragma comment(lib, "opencv_highgui430.lib")    
#pragma comment(lib, "opencv_imgcodecs430.lib")  
#pragma comment(lib, "opencv_videoio430.lib")  
#pragma comment(lib, "opencv_imgproc430.lib")
#pragma comment(lib, "opencv_tracking430.lib")

#endif  

#include "SafeQueue.h"
#include "YoloNeuralNetwork.h"

#include <iostream>
#include <vector>
#include <cstring>
#include <ctime>

#include <mutex>
#include <thread>
#include <queue>
#include <fstream>
#include <sstream>

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/tracking.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/core/utility.hpp>


class Tracker {
    static Tracker* instance;
    int data;
    //cv::MultiTracker trackers;
    //cv::Ptr<cv::MultiTracker> trackers = cv::MultiTracker::create();
    std::vector<cv::Rect2d> objects;
    std::vector<cv::Rect> ROIs;
    std::vector<cv::Ptr<cv::Tracker>> algorithms;
    // Private constructor so that no objects can be created.
    Tracker() {
        data = 0;
    }

public:

    static Tracker* getInstance() {
        if (!instance)
            instance = new Tracker;
        return instance;
    }

    int getData() {
        return this->data;
    }

    void setData(int data) {
        this->data = data;
    }

    void readAndTrack(SafeQueue <cv::Mat>& sq, SafeQueue <std::vector<cv::Rect>>& sq2);


};