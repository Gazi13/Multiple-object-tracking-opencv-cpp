#include <iostream>
#include "Reader.h"

Reader* Reader::instance = 0;

void Reader::readAndSend(SafeQueue<cv::Mat>& sq) {

    std::cout << "Video Player " << std::endl;
    std::string vFileName = "C:/Users/ahmet/Desktop/Video-a2.mp4";
    std::string wName = "Video Player";
    cv::VideoCapture vCap;
    vCap.open(vFileName);
    if (vCap.isOpened())
    {
        double fps = vCap.get(cv::CAP_PROP_FPS);
        cv::Mat frame;
        for (;;)
        {
            if (!vCap.read(frame))
                break;
            // alternatif vCap >> frame2;
            sq.push(frame.clone());

            if (sq.slow_down())
                std::this_thread::sleep_for(std::chrono::milliseconds(50));

        }
        vCap.release();

    }
    else
    {
        std::cout << "Video File " << vFileName << " Not Opened ..." << std::endl;
    }

}