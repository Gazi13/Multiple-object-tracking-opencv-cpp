#pragma once
#include "SafeQueue.h"
#include "YoloNeuralNetwork.h"
#include <iostream>
#include <vector>
#include <cstring>

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
    void readAndDetect(SafeQueue <cv::Mat>& sq, SafeQueue <std::vector<YoloNeuralNetwork::BoundingBox>>& sq2, std::vector<std::string> configures);
    void detectObjects(YoloNeuralNetwork& yoloNeuralNetwork, cv::Mat& frame, SafeQueue <std::vector<YoloNeuralNetwork::BoundingBox>>& sq2);

};