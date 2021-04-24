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

#include "detector.h"
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

detector* detector::instance = 0;


void detector::readAndDetect(SafeQueue<cv::Mat>& sq, SafeQueue <std::vector<cv::Rect>>& sq2) {

    std::cout << "Video Player " << std::endl;
    std::string vFileName = "C:/Users/ahmet/Desktop/Video-a2.mp4";
    std::string wName = "Video Player";

    cv::Rect rectangle;
    cv::VideoCapture cap;

    YoloNeuralNetwork yoloNeuralNetwork = initModel();

    cap.open(vFileName);
    if (cap.isOpened())
    {
        double fps = cap.get(cv::CAP_PROP_FPS);
        cv::Mat frame;
        for (;;)
        {
            if (!cap.read(frame))
                break;
            // alternatif cap >> frame2;
            sq.push(frame.clone());
            detector::detectObjects(yoloNeuralNetwork, frame, sq2);
            //cv::imshow("detect", frame);
            //cv::waitKey(1);
        }
        cap.release();

    }
    else
    {
        std::cout << "Video File " << vFileName << " Not Opened ..." << std::endl;
    }

}


void detector::detectObjects(YoloNeuralNetwork& yoloNeuralNetwork, cv::Mat& frame, SafeQueue <std::vector<cv::Rect>>& sq2)
{
    float confThreshold = 0.20;
    float nmsThreshold = 0.40;
    std::vector<cv::Rect> boxes;
    std::vector<YoloNeuralNetwork::BoundingBox> filtered_boxes;

    std::vector<YoloNeuralNetwork::BoundingBox> bBoxes = yoloNeuralNetwork.getBoundingBoxes(frame, confThreshold, nmsThreshold);
    for (auto it = bBoxes.begin(); it != bBoxes.end(); ++it) {

        if (((*it).classID) == 2 || ((*it).classID) == 3 || ((*it).classID) == 5 || ((*it).classID) == 7) {

            //filtered_boxes.push_back((*it));

            int top, left, width, height;
            top = (*it).roi.y;
            left = (*it).roi.x;
            width = (*it).roi.width;
            height = (*it).roi.height;
            //cv::rectangle(visImg, cv::Point(left, top), cv::Point(left + width, top + height), cv::Scalar(0, 255, 0), 2);

            if(width<(frame.cols/2) && height< (frame.rows/2) && width>20 && height>20){
                cv::Rect r(left, top, width, height);
                boxes.push_back(r);
                filtered_boxes.push_back((*it));
            }
        }
    }
    sq2.push(boxes);
    frame = yoloNeuralNetwork.drawBoundingBoxes(frame, filtered_boxes);

}
void detector::readAndDetect2(SafeQueue <cv::Mat>& sq, SafeQueue <std::vector<cv::Rect>>& sq2) {

    int turn = 10;
    cv::Mat frame;
    cv::Rect rectangle;
    cv::Mat& frame_ref = frame;
    YoloNeuralNetwork yoloNeuralNetwork = initModel();
    while (cv::waitKey(1) < 0) {


        turn++;
        sq.timeout_front(frame, 10); // timeout_front
        detector::detectObjects(yoloNeuralNetwork, frame, sq2);
        //cv::imshow("detect", frame);
        std::this_thread::sleep_for(std::chrono::milliseconds(50));

    }


}