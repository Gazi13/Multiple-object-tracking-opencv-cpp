#include <thread>
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/tracking.hpp>
#include <opencv2/videoio.hpp>
#include "detector.h"
#include "YoloNeuralNetwork.h"

detector* detector::instance = 0;

void detector::readAndDetect(SafeQueue <cv::Mat>& sq, SafeQueue <std::vector<YoloNeuralNetwork::BoundingBox>>& sq2, std::vector<std::string> configures) {

    cv::Mat frame;
    // Modeli tanimla
    YoloNeuralNetwork yoloNeuralNetwork(std::move(configures[0]), std::move(configures[1]), std::move(configures[2]));

    while (!sq.empty()) {

        // Frameleri okuyup detect islemi gerceklestirir ve sonra kutulari Rect olarak sq2-queue'e pushlar
        sq.timeout_front(frame, 10);
        detector::detectObjects(yoloNeuralNetwork, frame, sq2);

    }
}

void detector::detectObjects(YoloNeuralNetwork& yoloNeuralNetwork, cv::Mat& frame, SafeQueue <std::vector<YoloNeuralNetwork::BoundingBox>>& sq2)
{
    float confThreshold = 0.20;
    float nmsThreshold = 0.40;
    std::vector<YoloNeuralNetwork::BoundingBox> filtered_boxes;
    std::vector<YoloNeuralNetwork::BoundingBox> bBoxes = yoloNeuralNetwork.getBoundingBoxes(frame, confThreshold, nmsThreshold);

    for (auto it = bBoxes.begin(); it != bBoxes.end(); ++it) {

        // Eger araba - tir - kamyon - otobus ise kaydet.
        if (((*it).classID) == 2 || ((*it).classID) == 3 || ((*it).classID) == 5 || ((*it).classID) == 7) {

            int top, left, width, height;
            top = (*it).roi.y;
            left = (*it).roi.x;
            width = (*it).roi.width;
            height = (*it).roi.height;

            if (width < (frame.cols / 2) && height < (frame.rows / 2) && width>20 && height>20) {
                cv::Rect r(left, top, width, height);
                filtered_boxes.push_back((*it));
            }
        }
    }

    // Tespit edilen kutular trackerin kullanmasi icin sq2 aktarilir.
    sq2.push(filtered_boxes);
}
