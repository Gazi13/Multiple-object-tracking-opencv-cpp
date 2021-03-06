#pragma once
#include "SafeQueue.h"
#include "YoloNeuralNetwork.h"
#include <iostream>
#include <vector>

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/tracking.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/core/utility.hpp>


class Tracker {
    static Tracker* instance;
    int data;
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

    /**
     *  sq ve sq2'den frame ve varsa bounding boxlari okur.
     *  Bu bounding boxlari trackera aktarir ve g?nceller.
     *  Boxlari, label ve id bilgilerini ekrana cizer.
     * \param[in] sq : framelerin okundugu threadsafe queue
     * \param[in] sq2 : tespit edilen bounding boxlarin okundugu queue
     * \return ...
     */
    void readAndTrack(SafeQueue <cv::Mat>& sq, SafeQueue <std::vector<YoloNeuralNetwork::BoundingBox>>& sq2);


};