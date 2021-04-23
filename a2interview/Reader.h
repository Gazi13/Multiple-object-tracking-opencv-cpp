#pragma once

#ifdef _DEBUG               
#pragma comment(lib, "opencv_core430d.lib")       
#pragma comment(lib, "opencv_highgui430d.lib")    
#pragma comment(lib, "opencv_imgcodecs430d.lib")  
#pragma comment(lib, "opencv_videoio430d.lib")  
#pragma comment(lib, "opencv_imgproc430d.lib")  

#else       
#pragma comment(lib, "opencv_core430.lib")       
#pragma comment(lib, "opencv_highgui430.lib")    
#pragma comment(lib, "opencv_imgcodecs430.lib")  
#pragma comment(lib, "opencv_videoio430.lib")  
#pragma comment(lib, "opencv_imgproc430.lib")  
#endif  


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

    void readAndSend(SafeQueue<cv::Mat>& sq);
};