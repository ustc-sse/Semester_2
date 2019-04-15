/*
1.	计算灰度图像的归一化直方图。
	具体内容：利用 OpenCV 对图像像素进行操作，计算归一化直方图.并在窗口中以图形的方式显示出来
	图像 -> 直方图, 不可逆, 多对一

2.	灰度图像直方图均衡处理
	具体内容：通过计算归一化直方图,设计算法实现直方图均衡化处理

3.	彩色图像直方图均衡处理
	具体内容： 在灰度图像直方图均衡处理的基础上实现彩色直方图均衡处理

	注: 直方图均衡可能会产生噪声(失真), 因为, 假设原来是 100~200 的范围, 现在被扩展成了 0 ~ 255, 出现了椒盐

4.	cv::hconcant(src1, src2, dst), 将行列尺寸相同的 src1, src2 放到一个窗口上展示
	https://stackoverflow.com/questions/5089927/show-multiple-2-3-4-images-in-the-same-window-in-opencv
	
5.	代码已经经过重构, 与 readme.md 略有不同, 不过总体思路是一致的
*/

#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <string>
#include <cmath>
#include <vector>
#include <algorithm>

using namespace std;
using namespace cv;

class MyHistogram{
public:
	Mat histogram;
	vector<int> pdf;

	MyHistogram(){
		// pdf 是 Probability Distribution Function, 用于统计灰度频度
		pdf.resize(256, 0);
	}

	void calc_pdf(Mat & img, int channel = 1){
		// 不能用 pdf.resize(256, 0), 因为尺寸不变, 不会把原来的元素置0
		// pdf = vector<int>(256, 0);
		fill(pdf.begin(), pdf.end(), 0);
		if( channel == 1 ){
			Mat_<uchar>::iterator it = img.begin<uchar>();
			Mat_<uchar>::iterator end = img.end<uchar>();
			while( it != end ){
				uchar & pixel = *it;
				pdf[pixel]++;
				it++;
			}
		}
		else{
			// TODO: RGB, 3-channel
		}
	}

	void equalize_img(Mat & img){
		// 计算资源有限, 所选的图片都是 250x250 的尺寸
		int nRows = img.rows, nCols = img.cols;
		int tmp = nRows * nCols / 255;

		// sk = [(L-1)/(M*N)] * Σ(nj), j: 0->k
		// 从 r 到 s 的映射

		// vector<int> equalization_map(256, 0);
		// equalization_map[0] = pdf[0];
		// 这里 pdf 被重用为 equalization_map
		for( int r = 1; r < 256; r++ ){
			pdf[r] = pdf[r] + pdf[r - 1];
			// 	equalization_map[r] = pdf[r] + equalization_map[r - 1];
			pdf[r - 1] /= tmp;
			// 	equalization_map[r - 1] /= tmp;
		}
		pdf[255] /= tmp;
		// equalization_map[255] /= tmp;

		Mat_<uchar>::iterator it = img.begin<uchar>();
		Mat_<uchar>::iterator end = img.end<uchar>();
		while( it != end ){
			uchar & pixel = *it;
			// pixel = uchar(equalization_map[pixel]);
			pixel = uchar(pdf[pixel]);
			it++;
		}
	}

	void draw(string windowname){
		// 画出背景坐标系
		make_coordinate();
		// 将 pdf 的 {x:[0, 255], y:[0, 250*250]}
		// 映射到背景坐标系中的 {x:[0, 255], y:[0, 256]}
		for( int gval = 0; gval < 255; gval++ ){
			// 归一化到 height / 256, 但是每个灰度的统计值太小了，看不清
			// 乘上一个系数 8 进行放大, 方便观察
			int height = pdf[gval] * 8 / 256 ;
			line( histogram, Point(gval, 270), Point(gval, 270 - height), Scalar(100) );
		}
		show(windowname);
	}

	void make_coordinate(){
		// 背景尺寸 270x270, 底色为白色
		histogram = Mat( 270, 270, 0, Scalar(255) );

		// 画出坐标轴, 原点在左上角, 颜色为黑色
		// x 轴
		line( histogram, Point(50, 270), Point(50, 260), Scalar(0) );
		line( histogram, Point(100, 270), Point(100, 260), Scalar(0) );
		line( histogram, Point(150, 270), Point(150, 260), Scalar(0) );
		line( histogram, Point(200, 270), Point(200, 260), Scalar(0) );
		line( histogram, Point(250, 270), Point(250, 260), Scalar(0) );
		// y 轴
		line( histogram, Point(0, 220), Point(10, 220), Scalar(0) );
		line( histogram, Point(0, 170), Point(10, 170), Scalar(0) );
		line( histogram, Point(0, 120), Point(10, 120), Scalar(0) );
		line( histogram, Point(0, 70), Point(10, 70), Scalar(0) );
		line( histogram, Point(0, 20), Point(10, 20), Scalar(0) );
	}

	void show(string windowname){
		imshow(windowname, histogram);
	}
};

void lab01(){
	cout << "1. 显示原始图像, 并显示归一化直方图 (PDF)\n";

	if( true ){
		Mat img = imread("cell1.jpg", 0);
		imshow("灰度图", img);

		MyHistogram histogram;
		// 进行灰度频度统计
		histogram.calc_pdf(img);
		histogram.draw("灰度直方图");
	}

	waitKey(0);
	destroyAllWindows();
}

void lab02(){
	cout << "2. 灰度图像直方图均衡处理\n";

	if( true ){
		Mat img = imread("cell2.jpg", 0);
		imshow("灰度图", img);

		MyHistogram histogram;

		histogram.calc_pdf(img);
		histogram.draw("灰度直方图");

		histogram.equalize_img(img);
		imshow("均衡化灰度图", img);

		histogram.calc_pdf(img);
		histogram.draw("均衡化直方图");
	}
	waitKey(0);
	destroyAllWindows();
}

void lab03(){
	cout << "3. 彩色图像直方图均衡处理\n";

	if( true ){
		Mat img = imread("lenna.jpg");
		imshow("原图", img);

		// 分别在三个通道上进行均衡化
		// 略
	}
	waitKey(0);
	destroyAllWindows();
}

int main(){
	ios::sync_with_stdio(false);
	cin.tie(0);

	cout << "1. 显示原始图像, 并显示归一化直方图 (PDF)\n";
	cout << "2. 灰度图像直方图均衡处理\n";
	cout << "3. 彩色图像直方图均衡处理\n";
	char option = '1';

	while(true){
		cout << "输入实验编号: ";
		cin >> option;
		switch(option){
			case '1':
				lab01();
				break;
			case '2':
				lab02();
				break;
			case '3':
				lab03();
				break;
			default:
				destroyAllWindows();
				goto END;
				break;
		}
		cout << "实验 " << option << " 结束\n";
	}
	END:
	return 0;
}

