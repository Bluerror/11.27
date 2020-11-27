#include <iostream>
#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;

int calcHOG(Mat scr, float* hist, int nAngle, int blockSize) {
	Mat gx, gy;
	Mat mag, angle;
	Sobel(scr, gx, CV_32F, 1, 0, 1);
	Sobel(scr, gy, CV_32F, 0, 1, 1);

	cartToPolar(gx, gy, mag, angle, true);

	Rect roi;
	roi.height = blockSize;
	roi.width = blockSize;
	int nX = scr.cols / blockSize;
	int nY = scr.rows / blockSize;
	int bins = nX * nY * nAngle;

	for (int i = 0; i < nY; i++) {
		for (int j = 0; j < nX; j++) {
			cv::Mat roiMat;
			cv::Mat roiMag;
			cv::Mat roiAgl;
			 roiMat = scr(roi);
			 roiMag = mag(roi);
			 roiAgl = angle(roi);

			int head = (i * nX + j) * nAngle;


			for (int n = 0; n < roiMat.rows; n++) {
				for (int m = 0; m < roiMat.cols; m++) {

					hist[head + (int)roiAgl.at<float>(n, m) / (360 / nAngle)] += roiMag.at<float>(n, m);

				}
			}
		}
	}
	return 0;
}


float normL2(float* a, float* b, int n)
{
	float dis = 0;
	float sum = 0;
	for (int i = 0; i < n; i++) {
		dis = (a[i] + b[i]) * (a[i] + b[i]);
	}
	sum = sqrt(dis);
	return sum;
}

int main()
{
	int blockSize = 16;
	int nAngel = 8;
	cv::Mat refMat = imread("C://Users//Lenovo//source//repos//11.27//ԭͼ.jpg");
	cv::Mat plMat = imread("C:\\Users\\Lenovo\\source\\repos\\11.27\\3.jpg");
	cv::Mat bgMat = imread("C:\\Users\\Lenovo\\source\\repos\\11.27\\1.jpg");

	int nX = refMat.cols / blockSize;
	int nY = refMat.rows / blockSize;
	int bins = nX * nY * nAngel;

	float* ref_hist = new float[bins];
	memset(ref_hist, 0, sizeof(float) * bins);
	float* pl_hist = new float[bins];
	memset(pl_hist, 0, sizeof(float) * bins);
	float* bg_hist = new float[bins];
	memset(bg_hist, 0, sizeof(float) * bins);

	int reCode = 0;

	reCode = calcHOG(refMat, ref_hist, nAngel, blockSize);
	reCode = calcHOG(plMat, pl_hist, nAngel, blockSize);
	reCode = calcHOG(bgMat, bg_hist, nAngel, blockSize);

	if (reCode != 0) {
		delete[] ref_hist;
		delete[]pl_hist;
		delete[]bg_hist;
		return -1;
	}
	

	float dis1 = normL2(ref_hist, pl_hist, bins);
	float dis2 = normL2(ref_hist, bg_hist, bins);

	cout << dis1 << endl;
	cout << dis2 << endl;


	if (dis1 < dis2)
	{
		std::cout << "show" << dis1 << std::endl;
	}
	else {
		std::cout << "show" << dis2 << std::endl;
	}
	waitKey(30);
	return 0;
}
	



	
