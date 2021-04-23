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

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/videoio.hpp"



void read(SafeQueue <cv::Mat>& sq)
{

    Reader* r = r->getInstance();
    r->readAndSend(sq);

}

void detect(SafeQueue <cv::Mat>& sq, SafeQueue <std::vector<cv::Rect>>& sq2)
{
    std::this_thread::sleep_for(std::chrono::seconds(1));
    detector* p = p->getInstance();
    p->readAndDetect(sq,sq2);

}


void track(SafeQueue <cv::Mat>& sq, SafeQueue <std::vector<cv::Rect>>& sq2)
{
    std::this_thread::sleep_for(std::chrono::seconds(3));
    Tracker* p = p->getInstance();
    p->readAndTrack(sq,sq2);

}



int main()
{
    SafeQueue <cv::Mat> sq;
    SafeQueue <std::vector<cv::Rect>> sq2;

    std::thread th1(read, ref(sq));     // spawn new thread that calls foo()
    std::thread th2(detect, ref(sq), ref(sq2));  // spawn new thread that calls bar(0)
    std::thread th3(track, ref(sq), ref(sq2));

    std::cout << "main, foo and bar now execute concurrently...\n";
    
    
    th1.join();
    th2.join();
    th3.join();

    std::cout << "foo and bar completed.\n";

    return 0;
}


