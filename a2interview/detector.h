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


class detector {
    static detector* instance;
    int data;
    

    // Private constructor so that no objects can be created.
    detector() {
        data = 0;
    }

public:
    std::vector<std::string> classes;
    static detector* getInstance() {
        if (!instance)
            instance = new detector;
        return instance;
    }

    int getData() {
        return this->data;
    }

    void setData(int data) {
        this->data = data;
    }

    void readAndDetect(SafeQueue <cv::Mat>& sq, SafeQueue <std::vector<cv::Rect>>& sq2);
    void detectObjects(YoloNeuralNetwork& yoloNeuralNetwork, cv::Mat& frame, SafeQueue <std::vector<cv::Rect>>& sq2);

    YoloNeuralNetwork initModel() {

        // load class names from file
        std::string yoloBasePath = "C:/Users/ahmet/source/repos/a2interview/a2interview/model/";
        std::string yoloClassesFile = yoloBasePath + "coco.names";
        std::string yoloModelConfiguration = yoloBasePath + "yolov3.cfg";
        std::string yoloModelWeights = yoloBasePath + "yolov31.weights";

        YoloNeuralNetwork yoloNeuralNetwork(std::move(yoloClassesFile), std::move(yoloModelConfiguration), std::move(yoloModelWeights));
        classes = yoloNeuralNetwork.getClassesNames();

        return yoloNeuralNetwork;
    }

};