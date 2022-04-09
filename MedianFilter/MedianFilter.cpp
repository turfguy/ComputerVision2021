
#include <iostream>
#include<opencv2/highgui.hpp>
#include<opencv2/features2d.hpp>
#include<opencv2/imgproc.hpp>

using namespace cv;
using namespace std;

void median(Mat& src, int window, Mat& dst);

int win = 0;
int main(int argc, const char* argv[])
{
    Mat f = imread("C:/Users/K/Downloads/Input.png", 0);
    f.convertTo(f, CV_32F, 1 / 255.f);
    Mat g(f.size(), CV_32FC1);
    
    for (int win = 1;win < 6; win ++)
    {
         median(f, win, g);
    }


    return 0;
}


void median(Mat& src, int window, Mat& dst)
{
    
    
    for (auto y = 0; y < dst.rows; y++) for (auto x = 0; x < dst.cols; x++)
    {
        float k = 0;
        vector<float>neighbors;
        for (auto s = -window; s <= window; s++) for (auto t = -window; t <= window; t++) 
        {
          neighbors.push_back(src.at<float>(min(dst.rows - 1, max(0, y + t)),  min(dst.cols - 1, max(0, x + s))));
        }

        sort(neighbors.begin(), neighbors.end());

        k = neighbors[neighbors.size() / 2];

        dst.at<float>(Point(x, y)) = k;

        neighbors.clear();
    }



    imshow("image", dst);
    waitKey();  
}

