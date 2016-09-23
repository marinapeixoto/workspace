#include <iostream>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace cv;

int main() {
    Mat img = imread("../../img/lena.jpg", IMREAD_GRAYSCALE );
    Mat hist; 
    int channels[] = {0};
    float ranges[2] = {0, 255};
    const float *all_ranges =  {ranges}; 
    int histSize = 256;

    cv::calcHist(&img, 1, channels, Mat(), hist, 1, &histSize, &all_ranges, true, false); 
    cout<<hist.rows<<","<<hist.cols<<endl;

    Mat histImg = Mat::zeros(512, 500, CV_8UC1);
    uchar* data = hist.ptr<uchar>(0);
    for(int i=0; i<hist.rows; i++) {
        rectangle(histImg, Point(i*2, data[i]), Point(i+2, 0), Scalar::all(88), CV_FILLED);
        cout<<(int)data[i]<<",";
    }
    namedWindow("hist", CV_WINDOW_AUTOSIZE);
    imshow("hist", histImg);


    waitKey(0);


    return 0;
}
