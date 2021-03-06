#include<iostream>
#include<opencv.hpp>
#include "funtions.h"


using namespace cv;
using namespace std;


int gammaMain()
{

	cv::Mat srcMat = imread("E:\\Git\\gtest.jpg", 0);
	cv::Mat dstMat;

	if (srcMat.empty()) {
		cout << "fail to read pic!" << endl;
		return -1;
	}

	float gamma = 0.5;

	gammaCorrection(srcMat, dstMat, gamma);

	imshow("src", srcMat);
	imshow("dst", dstMat);
	waitKey(0);

	destroyAllWindows();

	return 0;
}

int equalizeMain()
{
	cv::Mat srcMat = imread("../testImages\\face.jpg", 0);
	cv::Mat dstMat;

	if (srcMat.empty()) {
		cout << "fail to read pic!" << endl;
		return -1;
	}

	//0.输入图像，类型是 8位单通道
	//1.输出图像，与输入同样尺寸同样类型
	if (srcMat.type() == CV_8UC1) {
		equalizeHist(srcMat, dstMat);
	}
	else if (srcMat.type() == CV_8UC3) {
		Mat channel[3];
		Mat out[3];
		split(srcMat, channel);

		for (int i = 0; i < 3; i++) {
			equalizeHist(channel[i], out[i]);
		}

		merge(out, 3, dstMat);

	}


	//绘制直方图
	float srcHist[256];
	float dstHist[256];

	cv::Mat srcHistMat;
	cv::Mat dstHistMat;

	//calcIntenHist(srcMat, srcHist);
	//calcSimpleHist(dstMat, dstHist);
	//drawSimpleHist(srcHistMat, srcHist,3,100);
	//drawSimpleHist(dstHistMat, dstHist,3,100);

	imshow("srcMat", srcMat);
	imshow("dstMat", dstMat);
	/*imshow("srcHistMat", srcHistMat);
	imshow("dstHistMat", dstHistMat);*/

	waitKey(0);

	destroyAllWindows();

	return 0;

}

int gammaCorrection(cv::Mat srcMat, cv::Mat & dstMat, float gamma)
{

	//本函数只处理单通道图像
	if (srcMat.channels() != 1)return -1;

	//建立查询表
	unsigned char lut[256];
	for (int i = 0; i < 256; i++)
	{
		//saturate_cast，防止像素值溢出，如果值<0,则返回0，如果大于255，则返回255
		lut[i] = saturate_cast<uchar>(pow((float)(i / 255.0), gamma) * 255.0f);
		float inten = (float)i;
	}

	srcMat.copyTo(dstMat);

	MatIterator_<uchar> it, end;
	for (it = dstMat.begin<uchar>(), end = dstMat.end<uchar>(); it != end; it++) {
		*it = lut[(*it)];
	}

	return 0;
}

int drawIntenHist(cv::Mat & histMat, float * srcHist, int bin_width, int bin_heght)
{
	histMat.create(100, 256 * bin_width, CV_8UC3);
	histMat = Scalar(255, 255, 255);

	float maxVal = *std::max_element(srcHist, srcHist + 256);

	for (int i = 0; i < 256; i++) {
		Rect binRect;
		binRect.x = i * bin_width;
		float height_i = (float)bin_heght*srcHist[i] / maxVal;
		binRect.height = (int)height_i;
		binRect.y = bin_heght - binRect.height;
		binRect.width = bin_width;
		rectangle(histMat, binRect, CV_RGB(255, 0, 0), -1);
	}

	return 0;
}

int calcIntenHist(const cv::Mat src, float * dstHist)
{

	//输入必为灰度图
	if (src.type() != CV_8UC1) {
		return -1;
	}

	memset(dstHist, 0, sizeof(float) * 256);
	int height = src.rows;
	int width = src.cols;
	//指针遍历
	for (int k = 0; k < height; k++)
	{
		// 获取第k行的首地址
		const uchar* inData = src.ptr<uchar>(k);
		//处理每个像素
		for (int i = 0; i < width; i++)
		{
			int gray = inData[i];
			dstHist[gray]++;
		}
	}

	//直方图归一化
	float norm = height * width;
	for (int n = 0; n < 256; n++) {
		dstHist[n] = dstHist[n] / norm;
	}


	return 0;
}

int calcIntenHistWithMask(const cv::Mat src, const cv::Mat mask, float * dstHist)
{
	//输入必为灰度图
	if (src.type() != CV_8UC1 ||
		mask.type() != CV_8UC1 ||
		src.size() != mask.size()) {
		return -1;
	}

	memset(dstHist, 0, sizeof(float) * 256);
	int height = src.rows;
	int width = src.cols;

	//指针遍历
	float norm = 0;
	for (int k = 0; k < height; k++)
	{
		// 获取第k行的首地址
		const uchar* inData = src.ptr<uchar>(k);
		const uchar* maskData = mask.ptr<uchar>(k);
		//处理每个像素
		for (int i = 0; i < width; i++)
		{
			int gray = inData[i];
			int maskFlag = maskData[i];
			if (maskFlag == 255) {
				dstHist[gray]++;
				norm++;
			}
		}
	}

	//直方图归一化
	for (int n = 0; n < 256; n++) {
		dstHist[n] = dstHist[n] / norm;
	}

	return 0;
}