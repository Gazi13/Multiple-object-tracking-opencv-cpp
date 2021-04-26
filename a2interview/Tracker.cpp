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
    std::vector<YoloNeuralNetwork::BoundingBox> box_list;
    cv::Ptr<cv::MultiTracker> trackers = cv::MultiTracker::create();
    // Bizim baktigimiz siniflar 2-3-5-7 
    std::vector<std::string> classes{ "bicycle","bicycle","car","motorbike","aeroplane","bus","train","truck" };

    // Queuedan frame al
    sq.timeout_front(frame, 10);
    // Aracin takip cizgisini saklamak icin bos bir mask olusturuyoruz
    cv::Mat3b mask = cv::Mat3b(frame.size(), cv::Vec3b(0, 0, 0));
    
    // Video olarak kaydetmek icin writer olusturuyoruz
    cv::VideoWriter video("outvideo-final-2.avi", cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), 25, cv::Size(frame.size()));

    while (!sq.empty()) {

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

        // sq2 icerisinde detectorun tespit ettigi boxlar var.
        // detector, trackere kiyas ile yavas calistigi icin bir kontrol yapisi kullaniyoruz.
        // Eger yeni bir detection yapilmis ise tracker guncelle yapilmamis ise var olan tracker ile devam et.
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
            
            YoloNeuralNetwork::BoundingBox bBox = (*it);

            // Tracker ile detector arasindaki ufak kaymayi düzenlemek icin boxlarin konumu yarim boy yukari tasiyoruz  
            cv::Rect r(bBox.roi.x, (bBox.roi.y-(bBox.roi.height/2)+10), bBox.roi.width, bBox.roi.height);
            cv::rectangle(frame, r, cv::Scalar(0, 255, 0), 2, 1);

            // Aracin takip cizgisi icin her framede box ortasina bir nokta yerlestiriyoruz ve mask icinde sakliyoruz. 
            cv::Point center(bBox.roi.x + int(bBox.roi.width / 2), bBox.roi.y + int(bBox.roi.height / 2));
            cv::circle(mask, center, 5, cv::Scalar(0, 0, 255), cv::FILLED, cv::LINE_8);

            // Bounding box üzerine label eklemek icin beyaz bir arka plan olusturup üzerine class ve id yaziyoruz.
            int baseLine;
            std::string label = classes[bBox.classID] + ": " + std::to_string(bBox.boxID+1);
            cv::Size labelSize = cv::getTextSize(label, cv::FONT_ITALIC, 0.5, 1, &baseLine);
            cv::rectangle(frame, cv::Point(bBox.roi.x, int(bBox.roi.y - bBox.roi.height / 2) - round(1.5 * labelSize.height)), cv::Point(bBox.roi.x + round(1.5 * labelSize.width), int(bBox.roi.y - bBox.roi.height / 2) + baseLine), cv::Scalar(255, 255, 255), cv::FILLED);
            cv::putText(frame, label, cvPoint(bBox.roi.x, int(bBox.roi.y - bBox.roi.height / 2)), cv::FONT_ITALIC, 0.75, cvScalar(0, 0, 0), 1, cv::LINE_AA);
        
        }

        // Hareket cizgisi ile orginal frame birlestirip goster/kaydet
        cv::addWeighted(frame, 1, mask, 1, 0.0, frame);
        video.write(frame);        
        cv::imshow("Window", frame);
        cv::waitKey(10);

    }
    //video.release();
    cv::destroyAllWindows();

}


