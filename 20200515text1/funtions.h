#pragma once
#include <opencv.hpp>


using namespace cv;
using namespace std;


//gamma������ʾ����
int gammaMain();
//ֱ��ͼ���⻯��ʾ����
int equalizeMain();



//gamma�������ܺ���
int gammaCorrection(cv::Mat srcMat, cv::Mat & dstMat, float gamma);


//ֱ��ͼ���ܺ���
int drawIntenHist(cv::Mat & histMat, float * srcHist, int bin_width, int bin_heght);
int calcIntenHist(const cv::Mat src, float * dstHist);
int calcIntenHistWithMask(const cv::Mat src, const cv::Mat mask, float * dstHist);