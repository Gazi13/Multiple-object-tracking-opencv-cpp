#include <queue>
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/tracking.hpp>
#include <opencv2/videoio.hpp>

#include "Tracker.h"
#include "YoloNeuralNetwork.h"

Tracker* Tracker::instance = 0;


void Tracker::readAndTrack(SafeQueue <cv::Mat>& sq, SafeQueue <std::vector<YoloNeuralNetwork::BoundingBox>>& sq2) {

    int turn = 10;
    cv::Mat frame;
    // Bizim baktigimiz siniflar 2-3-5-7 
    std::vector<std::string> classes{"bicycle","bicycle","car","motorbike","aeroplane","bus","train","truck"};

    std::vector<YoloNeuralNetwork::BoundingBox> box_list;
    cv::Ptr<cv::MultiTracker> trackers = cv::MultiTracker::create();

    // Queuedan frame al
    sq.timeout_front(frame, 10);
    // Aracin takip cizgisini saklamak icin bos bir mask olusturuyoruz
    cv::Mat3b mask = cv::Mat3b(frame.size(), cv::Vec3b(0, 0, 0));
    
    // Video olarak kaydetmek icin writer olusturuyoruz
    cv::VideoWriter video("outcpp-final-plus.avi", cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), 25, cv::Size(frame.size()));

    while (cv::waitKey(1) < 0) {

        turn++;
        // Queuedan frame oku
        sq.timeout_pop(frame,100);
        // Trackerin calistigini anlamak icin ekrana detection yazisi yazilir
        cv::putText(frame, " TRACKER ", cvPoint(30, 60),
            cv::FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(200, 200, 250), 1, cv::LINE_AA);

        // Olusan takip cizgisini temizle
        if (turn % 30 == 0) {
            mask = cv::Mat3b(frame.size(), cv::Vec3b(0, 0, 0));
        }

        // Eger yeni bir detection yapilmis ise tracker guncelle degilse var olan tracker ile devam et.
        if (sq2.try_pop(box_list)) {
            
            trackers = cv::MultiTracker::create();
            for (auto it = box_list.begin(); it != box_list.end(); ++it) {
                //trackers->add(cv::TrackerCSRT::create(), frame, (*it).roi);
                trackers->add(cv::TrackerKCF::create(), frame, (*it).roi);
                //trackers->add(cv::TrackerMOSSE::create(), frame, (*it).roi);

            }
            // Detection calistigini anlamak icin ekrana detection yazisi yazilir
            cv::putText(frame, " DETECTION ", cvPoint(30, 30),
                cv::FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(200, 200, 250), 1, cv::LINE_AA);

        }

        // Tracker yeni frame ile guncelle
        trackers->update(frame);

        // Elimizdeki bounding boxlari ekrana cizdirme
        for (auto it = box_list.begin(); it != box_list.end(); ++it) {
            
            cv::Rect r((*it).roi.x, (*it).roi.y, (*it).roi.width, (*it).roi.height);
            if ((*it).roi.y >200)
                cv::Rect r((*it).roi.x+10, (*it).roi.y-30, (*it).roi.width, (*it).roi.height);
            
            cv::rectangle(frame, r, cv::Scalar(0, 255, 0), 2, 1);
            cv::Point center((*it).roi.x + int((*it).roi.width / 2), (*it).roi.y + int((*it).roi.height / 2));
            cv::circle(mask, center, 5, cv::Scalar(0, 0, 255), cv::FILLED, cv::LINE_8);

            std::string s = classes[(*it).classID]+ ": " + std::to_string((*it).boxID);
            cv::putText(frame, s, cvPoint((*it).roi.x, (*it).roi.y - 10), cv::FONT_ITALIC, 0.76, cvScalar(200, 0, 250), 1, cv::LINE_AA);
        
        }

        // Hareket cizgisi ile orginal frame birlestirip goster/kaydet
        cv::addWeighted(frame, 1, mask, 1, 0.0, frame);
        video.write(frame);        
        cv::imshow("Window", frame);
        cv::waitKey(10);
        if (sq.empty())
            break;
    }
    //video.release();
    cv::destroyAllWindows();

}


