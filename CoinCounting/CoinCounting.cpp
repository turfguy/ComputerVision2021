// Hough Transform
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
 
using namespace cv;
using namespace std;

int main(int argc, const char* argv[])
{
    Mat img = imread("C:\\Users\\K\\Downloads\\coins5.jpg", 0); 
    imshow("src", img);

    Mat gray;
    cvtColor(img, gray, COLOR_BayerBG2GRAY);  // grayscale
    
    Mat med;
    medianBlur(gray , med, 7); // median filter

    Mat gaus;
    GaussianBlur(med, gaus, Size(5, 5), 1); //gaussian filter



    Mat cimg;
    Canny(gaus, cimg, 70, 175, 3); // canny edge detector





    vector<Vec3f> circles;
    HoughCircles(cimg, circles, HOUGH_GRADIENT, 1, 90, 50, 20, 20, 100);  // hough circle transform(허프서클 트랜스폼)

    for (size_t i = 0; i < circles.size(); i++)
    {
        Vec3i c = circles[i];
        Point center(cvRound(circles[i][0]), cvRound(circles[i][1])); //검출된 자리에 원 그리기
        int radius = cvRound(circles[i][2]);
        Scalar scalar = Scalar(180, 0, 0);
        
        circle(cimg, center, 2, scalar, 2);
        circle(cimg, center, radius, scalar, 3);
        




    }
    printf("동전의 개수 : %d\n", circles.size()); // 동전 수 출력하기

    imshow("dst", cimg);
    waitKey();

    return 0;
}

