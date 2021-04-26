#include <iostream>
#include "Reader.h"

Reader* Reader::instance = 0;

void Reader::readAndSend(SafeQueue<cv::Mat>& sq, std::string vFileName) {

    std::cout << " Read Frames " << std::endl;
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

            // Eger queue dolmus ise okuma islemini yavaslat 
            if (sq.slow_down())
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        vCap.release();
    }
    else
    {
        std::cout << "Video File " << vFileName << " Not Opened ..." << std::endl;
    }

}