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

#include "Tracker.h"
#include "YoloNeuralNetwork.h"
#include <iostream>

#include <mutex>
#include <thread>
#include <queue>
#include <fstream>
#include <sstream>
#include <iostream>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/tracking.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/core/utility.hpp>

Tracker* Tracker::instance = 0;

void Tracker::readAndTrack(SafeQueue <cv::Mat>& sq, SafeQueue <std::vector<cv::Rect>>& sq2) {

    int turn = 10;
    cv::Mat frame;
    cv::Rect rectangle;
    std::vector<cv::Rect> rectangle_list;

    cv::Ptr<cv::MultiTracker> trackers = cv::MultiTracker::create();

    while (cv::waitKey(1) < 0) {

        turn++;
        sq.timeout_pop(frame,10);

        // Too slow
        // Yeni koordinatlar
        if (sq2.try_pop(rectangle_list)) {
            std::cout << "NEW TRACKER"<< std::endl;
            trackers = cv::MultiTracker::create();
            for (cv::Rect r : rectangle_list) {
                trackers->add(cv::TrackerCSRT::create(), frame, r);
            }
        }

        trackers->update(frame);

        for (unsigned i = 0; i < trackers->getObjects().size(); i++) {
            cv::rectangle(frame, trackers->getObjects()[i], cv::Scalar(255, 0, 0), 2, 1);
        }

        cv::imshow("Window", frame);
        if (cv::waitKey(1) >= 0) break;
    }

}


