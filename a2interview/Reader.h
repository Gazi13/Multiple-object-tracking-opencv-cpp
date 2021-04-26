#pragma once
#include <iostream>
#include <cstdio>
#include <mutex>

#include "SafeQueue.h"
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

class Reader {
    static Reader* instance;
    int data;
    mutable std::mutex my_mutex;
    // Private constructor so that no objects can be created.
    Reader() {
        data = 0;
    }

public:
    static Reader* getInstance() {
        if (!instance)
            instance = new Reader;
        return instance;
    }

    int getData() {
        return this->data;
    }

    void setData(int data) {
        this->data = data;
    }

    /**
     *  Verilen path üzerinden video framelerini okuyuo queue pushlar 
     * \param[in] sq : framelerin saklandigi threadsafe queue
     * \param[in] video_path : okunacak videonun konumu      
     * \return ...
     */
    void readAndSend(SafeQueue<cv::Mat>& sq, std::string video_path);
};