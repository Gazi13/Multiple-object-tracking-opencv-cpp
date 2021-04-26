#include <iostream>
#include <string>
#include <sstream>
#include <cstdio>
#include <mutex>
#include <thread>

#include "SafeQueue.h"
#include "Tracker.h"
#include "detector.h"
#include "Reader.h"
#include "YoloNeuralNetwork.h"

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/videoio.hpp"



void read(SafeQueue <cv::Mat>& sq, std::string vFileName)
{
    Reader* r = r->getInstance();
    r->readAndSend(sq, vFileName);
}

void detect(SafeQueue <cv::Mat>& sq, SafeQueue <std::vector<YoloNeuralNetwork::BoundingBox>>& sq2, std::vector<std::string> configures)
{
    std::this_thread::sleep_for(std::chrono::seconds(1));
    detector* p = p->getInstance();
    p->readAndDetect(sq,sq2,configures);
}

void track(SafeQueue <cv::Mat>& sq, SafeQueue <std::vector<YoloNeuralNetwork::BoundingBox>>& sq2)
{
    std::this_thread::sleep_for(std::chrono::seconds(2));
    Tracker* p = p->getInstance();
    p->readAndTrack(sq,sq2);
}


int main()
{
    SafeQueue <cv::Mat> sq;
    SafeQueue <std::vector<YoloNeuralNetwork::BoundingBox>> sq2;
    
    // Parameters
    std::string vFileName = "C:/Users/ahmet/Desktop/Video-a2.mp4";
    std::string yolo_base_path = "C:/Users/ahmet/source/repos/a2interview/a2interview/model/";
    std::string yolo_class_name = yolo_base_path + "coco.names";
    std::string yolo_cfg = yolo_base_path + "yolov3.cfg";
    std::string yolo_weights = yolo_base_path + "yolov31.weights";
    std::vector<std::string> configures{ yolo_class_name ,yolo_cfg,yolo_weights};

    std::thread th1(read, ref(sq), vFileName);     
    std::thread th2(detect, ref(sq), ref(sq2), configures);
    std::thread th3(track, ref(sq), ref(sq2));

    std::cout << "read, detect and track now execute concurrently...\n";
    
    th1.join();
    th2.join();
    th3.join();

    std::cout << "All thread completed.\n";

    return 0;
}


