#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace cv;

const int PYR_LEVEL = 10;
std::vector<Mat> buildLaplacianPyramid(const Mat& src)
{
    std::vector<Mat> pyramid(PYR_LEVEL);
    pyramid[0] = src;
    for (auto i = 1; i < PYR_LEVEL; i++) 
    {
        pyrDown(pyramid[i-1], pyramid[i]);
        Mat temp;
        pyrUp(pyramid[i], temp, pyramid[i-1].size());
        pyramid[i-1] -= temp;
    }
    return pyramid;
}


std::vector<Mat> buildGaussianPyramid(const Mat& mask)
{
    std::vector<Mat> GaussianPyramid(PYR_LEVEL);
    GaussianPyramid[0] = mask;
    for (auto i = 1; i < PYR_LEVEL; i++)
        pyrDown(GaussianPyramid[i-1], GaussianPyramid[i]);
    return GaussianPyramid;
}

Mat reconstruct(const std::vector<Mat>& pyramid)
{
    Mat out = pyramid.back();
    for (int i = int (pyramid.size()) - 2; i >= 0; i--)
    {
        pyrUp(out, out, pyramid[i].size());
        out += pyramid[i];
    }
    return out;
}

int main(int argc, const char* argv[])
{
    Mat input = imread("C:\\Users\\K\\Downloads\\burt_apple.png",1);
    Mat input2 = imread("C:\\Users\\K\\Downloads\\burt_orange.png");
    Mat inmask = imread("C:\\Users\\K\\Downloads\\burt_mask.png");
    Mat inmask2 = imread("C:\\Users\\K\\Downloads\\burt_mask.png");
    Mat src;
    Mat src2;
    Mat mask;
    Mat mask2;
    input.convertTo(src, CV_32F, 1 / 255.f);
    input2.convertTo(src2, CV_32F, 1 / 255.f);
    inmask.convertTo(mask, CV_32F, 1 / 255.f);
    inmask.convertTo(mask2, CV_32F, 1 / 255.f);
    imshow("src", src); //사과 원본
    imshow("src2", src2); //오렌지 원본
    std::vector<Mat> pyramid = buildLaplacianPyramid(src);
    std::vector<Mat> pyramid2 = buildLaplacianPyramid(src2);
    flip(mask, mask2, 1);
    std::vector<Mat> GaussianPyramid = buildGaussianPyramid(mask);
    std::vector<Mat> GaussianPyramid2 = buildGaussianPyramid(mask2);
    std::vector <Mat> mix(PYR_LEVEL);
    std::vector <Mat> mix2(PYR_LEVEL);
    std::vector <Mat> result(PYR_LEVEL);

    for (auto i = 0; i < PYR_LEVEL; i++)
    {
        

        multiply(pyramid[i], GaussianPyramid[i], mix[i]); // pyramid == src , gaussian == mask
        multiply(pyramid2[i], GaussianPyramid2[i], mix2[i]);
  
        add(mix[i], mix2[i],result[i] );
    }
    Mat res=  reconstruct(result); 
    imshow("m", res);// 결과물 출력


    
    waitKey();
    return 0;
  
}

