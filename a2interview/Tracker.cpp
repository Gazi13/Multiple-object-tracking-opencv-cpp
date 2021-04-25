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
    std::deque<cv::Mat> frame_deque;
    cv::Ptr<cv::MultiTracker> trackers = cv::MultiTracker::create();

    sq.timeout_front(frame, 10);
    // Default resolutions of the frame are obtained.The default resolutions are system dependent.
    int frame_width = frame.cols;
    int frame_height = frame.rows;
    cv::Mat3b mask = cv::Mat3b(frame.size(), cv::Vec3b(0, 0, 0));
    // Define the codec and create VideoWriter object.The output is stored in 'outcpp.avi' file.
    //cv::VideoWriter video("outcpp-speedTest.avi", cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), 25, cv::Size(frame_width, frame_height));

    while (cv::waitKey(1) < 0) {

        turn++;
        sq.timeout_pop(frame,100);
        cv::putText(frame, " TRACKER ", cvPoint(30, 60),cv::FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(200, 200, 250), 1, cv::LINE_AA);
        // Burada 30 frame biriktiriyoruz ki detector ile aramizda frame farkini azaltalim.
        /*frame_deque.push_back(frame);
        if (frame_deque.size() < 1)
            continue;

        frame = frame_deque.front();
        frame_deque.pop_front();*/

        // Too slow
        // Yeni koordinatlar
        // timeout_pop

        // || turn%30 == 0

        if (turn % 10 == 0) {
            mask = cv::Mat3b(frame.size(), cv::Vec3b(0, 0, 0));
        }

        if (sq2.try_pop(rectangle_list)) {

            trackers = cv::MultiTracker::create();
            for (cv::Rect r : rectangle_list) {
                //trackers->add(cv::TrackerCSRT::create(), frame, r);
                trackers->add(cv::TrackerKCF::create(), frame, r);
                //trackers->add(cv::TrackerMOSSE::create(), frame, r);

            }

        }

    
        /*if (turn % 10 == 1) {
            turn = 1;
            sq2.pop(rectangle_list);
            //cv::putText(frame, " DETECTION ", cvPoint(30, 30),cv::FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(200, 200, 250), 1, cv::LINE_AA);

            trackers = cv::MultiTracker::create();
            for (cv::Rect r : rectangle_list) {
                //trackers->add(cv::TrackerCSRT::create(), frame, r);
                trackers->add(cv::TrackerKCF::create(), frame, r);
                //trackers->add(cv::TrackerMOSSE::create(), frame, r);

            }

        }
        else {

            else {
            
                frame_deque.push_back(frame);
                if (frame_deque.size() < 30)
                    continue;

            }

        }*/

        trackers->update(frame);

        for (unsigned i = 0; i < trackers->getObjects().size(); i++) {
            cv::Rect2d rect = trackers->getObjects()[i];
            cv::rectangle(frame, rect, cv::Scalar(0, 255, 0), 2, 1);

            cv::Point center(rect.x+int(rect.width/2), rect.y+int(rect.height / 2));
            cv::circle(mask, center, 5, cv::Scalar(0, 0, 255),cv::FILLED,cv::LINE_8);
        }

        cv::addWeighted(frame, 1, mask, 1.5, 0.0, frame);
        //video.write(frame);        
        cv::imshow("Window", frame);
        cv::waitKey(10);
    }
    //video.release();
    cv::destroyAllWindows();


}


