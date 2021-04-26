#include <iostream>
#include "Reader.h"

Reader* Reader::instance = 0;

void Reader::readAndSend(SafeQueue<cv::Mat>& sq, std::string video_path) {

    std::cout << " Read Frames " << std::endl;
    cv::VideoCapture cap;
    cap.open(video_path);
    if (cap.isOpened())
    {
        cv::Mat frame;
        while(cap.read(frame)){

            // alternatif cap >> frame2;
            sq.push(frame.clone());

            // Eger queue dolmus ise okuma islemini yavaslat 
            if (sq.slow_down())
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        cap.release();
    }
    else
    {
        std::cout << "Video File " << video_path << " Not Opened ..." << std::endl;
    }

}