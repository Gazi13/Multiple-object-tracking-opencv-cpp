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
        cv::Mat frame2;
        //cv::namedWindow(wName);
        for (;;)
        {
            if (!vCap.read(frame2))
                break;
            // alternatif vCap >> frame2;
            sq.push(frame2.clone());
            //cv::imshow(wName, frame2);
            //if (cv::waitKey(1000 / fps) >= 0)
            //   break;
        }
        vCap.release();
        //cv::destroyWindow(wName);

    }
    else
    {
        std::cout << "Video File " << vFileName << " Not Opened ..." << std::endl;
    }


}