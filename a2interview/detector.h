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

    /**
     *  Yolo modelini initialize eder
     *  Queuedan frameleri okur ve detection yapmasi icin detecObjects()'e iletir.
     * \param[in] sq : framelerin okundugu threadsafe queue
     * \param[in] sq2 : tespit edilen bounding boxlarin pushlandigi queue
     * \param[in] configures : YOLO modeli initialize etmek icin gerekli pathler
     * \return ...
     */
    void readAndDetect(SafeQueue <cv::Mat>& sq, SafeQueue <std::vector<YoloNeuralNetwork::BoundingBox>>& sq2, std::vector<std::string> configures);
    
    /**
     *  Okunan frame üzerinde detection islemi yapip sq2'ye pushlar.
     * \param[in] yoloNeuralNetwork : YOLO model
     * \param[in] frame : okunan frame
     * \param[in] sq2 : tespit edilen bounding boxlarin pushlandigi queue
     * \return ...
     */
    void detectObjects(YoloNeuralNetwork& yoloNeuralNetwork, cv::Mat& frame, SafeQueue <std::vector<YoloNeuralNetwork::BoundingBox>>& sq2);

};