#include <iostream>
#include "opencv2/core.hpp"
#include "opencv2/calib3d.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/features2d.hpp"


using namespace cv;
using namespace std;

int main(int argc, char* argv[])
{
	//1. 입력은 직접 촬영한 두 장의 사진을 사용합니다. 이때 카메라의 위치는 고정한 채 방향만 약 30도 정도 돌려서 촬영하는 것을 권장합니다.
	//당연히 움직이는 피사체는 피하는 것이 좋습니다.
	Mat Left = imread("C:\\Users\\K\\Downloads\\z1.jpg", 1);
	Mat Right = imread("C:\\Users\\K\\Downloads\\z2.jpg", 1);
	imshow("left", Left);
	imshow("right", Right);
	//2. 입력 영상으로 부터 특징점과 descriptor를 생성합니다.일반적으로 opencv의 descriptor 생성 기능은 특징점 추출 기능과 동시에 작동하기 때문에 각각을 별도로 수행할 필요는 없습니다.
	//Descriptor의 종류는 제한이 없습니다. (Orb, SURF 등)
	//Orb
	Ptr<ORB> o1 = ORB::create();
	Ptr<ORB> o2 = ORB::create();
	vector<KeyPoint> points1, points2;
	Mat desc1, desc2;
	vector<DMatch> matches;
	//3. 두 영상에서 얻은 desciptor를 match 합니다.
	//이때 OpenCV에서 제공하는 matcher를 사용해도 좋습니다.
	o1->detectAndCompute(Left, noArray(), points1, desc1);
	o2->detectAndCompute(Right, noArray(), points2, desc2);
	BFMatcher descriptorMatcher(NORM_HAMMING, true);
	descriptorMatcher.match(desc1, desc2, matches, Mat());
	Mat index;
	int nbMatch = int(matches.size());
	Mat tab(nbMatch, 1, CV_32F);
	for (int i = 0; i < nbMatch / 2; i++)
	{
		tab.at<float>(i, 0) = matches[i]
			.distance;
	}
	sortIdx(tab, index, SORT_EVERY_COLUMN + SORT_ASCENDING);
	vector<DMatch> bestMatches;
	vector<Point2f> src, dst;
	for (int i = 0; i < nbMatch / 2; i++)
	{
		int j = index.at<int>(i, 0);
		src.push_back(points1[matches[j].queryIdx].pt + Point2f(0, Left.rows));
		dst.push_back(points2[matches[j].trainIdx].pt);
	}
	//4. match를 이용하여 Homography를 계산합니다.
	Mat homG = findHomography(src, dst, RANSAC);
	Mat result;
	Mat midresult;
	drawMatches(Left, points1, Right, points2, matches, midresult);
	resize(midresult, midresult, Size(500, 500));
	imshow("mid_result", midresult);
	//5. Homograph를 이용하여 한 개의 image를 warping하고, 나마지 image는 copy 하여 최종 영상을 만듭니다.
	warpPerspective(Right, result, homG.inv(), Size(Right.cols + Left.cols, Right.rows + Left.rows));
	Mat result_roi(result, Rect(0, Left.rows, Left.cols, Left.rows));
	Left.copyTo(result_roi);
	Mat temp_show;
	//6. 결과 영상을 출력합니다.
	resize(result, temp_show, Size(800, 800));
	imshow("result_image", temp_show);
	waitKey();
	imwrite("result_image.jpg", result);
	waitKey();
	return 0;
}
