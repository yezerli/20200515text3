#include<iostream>
#include<opencv.hpp>
#include <opencv.hpp>
#include "funtions.h"

using namespace cv;
using namespace std;

int main()
{
	//��ʼ��ʱ
	double start = static_cast<double>(cvGetTickCount());

	//gamma������ʾ
	gammaMain();

	//ֱ��ͼ������ʾ
	//equalizeMain();

	//������ʱ
	double time = ((double)cvGetTickCount() - start) / cvGetTickFrequency();
	//��ʾʱ��
	cout << "processing time:" << time / 1000 << "ms" << endl;

	//�ȴ�������Ӧ�����������������
	system("pause");
	return 0;
}
