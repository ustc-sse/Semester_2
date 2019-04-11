/*
1.	灰度图像的 DFT 和 IDFT。
	具体内容：利用 OpenCV 提供的 cvDFT 函数对图像进行 DFT 和 IDFT 变换

2	利用理想高通和低通滤波器对灰度图像进行频域滤波
	具体内容：利用 cvDFT 函数实现 DFT ，在频域上利用理想高通和低通滤波器进行滤波，并把滤波过后的图像显示在屏幕上（观察振铃现象），要求截止频率可输入。

3	利用布特沃斯高通和低通滤波器对灰度图像进行频域滤波。
	具体内容：利用 cvDFT 函数实现 DFT ，在频域上进行利用布特沃斯高通和低通滤波器进行滤波，并把滤波过后的图像显示在屏幕上（观察振铃现象），要求截止频率和 n 可输入。
*/
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <string>
#include <cmath>

using namespace std;
using namespace cv;
static int nRows, nCols;

/*
https://docs.opencv.org/2.4.13.7/doc/tutorials/core/discrete_fourier_transform/discrete_fourier_transform.html
官方文档描述的 DFT demo
*/
void dft_opencv(Mat & space, Mat & frequency){
	//expand input image to optimal size, 扩展
	Mat padded;
	int m = getOptimalDFTSize( space.rows );
	int n = getOptimalDFTSize( space.cols );

	// on the border add zero values, 填零
	copyMakeBorder(space, padded, 0, m - space.rows, 0, n - space.cols, BORDER_CONSTANT, Scalar::all(0));

	Mat planes[] = {
		Mat_<float>(padded),
		Mat::zeros(padded.size(), CV_32F)
	};
	// planes[0] 是扩展后的图像
	// planes[1] 是0

	Mat complexI;
	// Add to the expanded another plane with zeros
	// 把 planes[0], planes[1], 合并到 complex
	merge(planes, 2, complexI);

	// this way the result may fit in the source matrix
	dft(complexI, complexI);
	complexI.copyTo(frequency);
	// compute the magnitude and switch to logarithmic scale
	// => log[ 1 + sqrt(Re(DFT(I))^2 + Im(DFT(I))^2) ]
	split(complexI, planes);

	// planes[0] = Re( DFT(I) )
	// planes[1] = Im( DFT(I) )
	magnitude( planes[0], planes[1], planes[0] );
	// planes[0] = magnitude = sqrt(Re ^ 2, Im ^ 2)
	Mat magI = planes[0];

	// switch to logarithmic scale
	// log[ 1 + magI ]
	magI += Scalar::all(1);
	// 对数变换, 方便人眼观察
	log(magI, magI);

	// crop the spectrum, if it has an odd number of rows or columns
	// -2 = 0xfffffffe, 前面全 1, 最后一位是 0, & -2 就是把最后一位置零
	magI = magI( Rect(0, 0, magI.cols & -2, magI.rows & -2) );

	// rearrange the quadrants of Fourier image  so that the origin is at the image center
	// 交换左上和右下，交换右上和坐下，拼凑四个 1/4 周期的中心, 到 (m/2, n/2)
	int cx = magI.cols / 2;
	int cy = magI.rows / 2;

	// Top-Left - Create a ROI per quadrant
	Mat q0(magI, Rect(0, 0, cx, cy));
	// Top-Right
	Mat q1(magI, Rect(cx, 0, cx, cy));
	// Bottom-Left
	Mat q2(magI, Rect(0, cy, cx, cy));
	// Bottom-Right
	Mat q3(magI, Rect(cx, cy, cx, cy));

	// swap quadrants (Top-Left with Bottom-Right)
	Mat tmp;
	q0.copyTo(tmp);
	q3.copyTo(q0);
	tmp.copyTo(q3);

	// swap quadrant (Top-Right with Bottom-Left)
	q1.copyTo(tmp);
	q2.copyTo(q1);
	tmp.copyTo(q2);

	// Transform the matrix with float values into a
	// viewable image form (float between values 0 and 1).
	normalize(magI, magI, 0, 1, CV_MINMAX); 

	// Show the result
	// imshow("Input Image", I);
	imshow("频谱", magI);
}

void my_dft(Mat & space, Mat & frequency){
	// expand input image to optimal size, 扩展到合适的尺寸, 方便进行 DFT
	// 快速傅立叶变换要求最佳尺寸为 2 的 n 次方
	Mat padded;
	int m = getOptimalDFTSize( space.rows );
	int n = getOptimalDFTSize( space.cols );

	// on the border add zero values, 填零
	copyMakeBorder(space, padded, 0, m - space.rows, 0, n - space.cols, BORDER_CONSTANT, Scalar::all(0));

	// 官方文档的做法是, 先做变换, 然后
	// 交换左上和右下，交换右上和坐下，拼凑四个 1/4 周期的中心, 到 (m/2, n/2)
	// 而根据 课本, 我们可以先对空域的图像做一个变换
	// fp(x, y) = f(x, y) * (-1)^(x + y)
	// 这样, 傅里叶变换过后的中心点就平移到了 (m/2, n/2)
	for( int i = 0; i < m; i++ ){
		for( int j = 0; j < n; j++ ){
			*padded.ptr<uchar>(i, j) *= uchar( pow(-1, i + j) );
		}
	}

	// planes[0] 是扩展后的图像, planes[1] 全0
	Mat planes[] = {
		Mat_<float>(padded),
		Mat::zeros(padded.size(), CV_32F)
	};

	Mat complexI;
	// Add to the expanded another plane with zeros
	// 把 planes[0], planes[1], 合并到 complex
	merge(planes, 2, complexI);

	// this way the result may fit in the source matrix
	dft(complexI, complexI);
	complexI.copyTo(frequency);


	/****************** 下面是生成频谱的过程 *******************/
	// compute the magnitude and switch to logarithmic scale
	// => log[ 1 + sqrt(Re(DFT(I))^2 + Im(DFT(I))^2) ]
	// 将 complex 拆分为两部分, planes[0] 和 planes[1]
	// planes[0] = Re( DFT(I) ), 实部
	// planes[1] = Im( DFT(I) ), 虚部
	split(complexI, planes);

	// 计算幅值, sqrt(Re ^ 2, Im ^ 2)
	// 将幅值保存在 planes[0] 中
	magnitude( planes[0], planes[1], planes[0] );
	Mat magI = planes[0];

	// switch to logarithmic scale
	// 对数变换 s = log(1 + r), 方便人眼观察
	// log(1 + magI)
	magI += Scalar::all(1);
	log(magI, magI);

	// crop the spectrum, if it has an odd number of rows or columns
	// -2 = 0xfffffffe, 前面全 1, 最后一位是 0, & -2 就是把最后一位置零
	// 裁剪频谱, 使行列为偶数
	magI = magI( Rect(0, 0, magI.cols & -2, magI.rows & -2) );

	// Transform the matrix with float values into a
	// viewable image form (float between values 0 and 1).
	// 缩放, 便于观察
	normalize(magI, magI, 0, 1, CV_MINMAX); 

	imshow("频谱", magI);
	/****************** 频谱 *******************/
}

/*
	https://stackoverfLOWPASS.com/questions/19761526/how-to-do-inverse-dft-in-opencv
	https://stackoverfLOWPASS.com/questions/10269456/inverse-fourier-transformation-in-opencv
	如何进行傅里叶反变换
*/
void my_idft(Mat & frequency, Mat & space_img){
	Mat complexI = frequency;
	dft(complexI, space_img, DFT_INVERSE | DFT_REAL_OUTPUT | DFT_SCALE );
	// normalize(space_img, space_img, 0, 1, CV_MINMAX);
	space_img.convertTo(space_img, CV_8U);
	int m = space_img.rows;
	int n = space_img.cols;
	for( int i = 0; i < m; i++ ){
		for( int j = 0; j < n; j++ ){
			*space_img.ptr<uchar>(i, j) *= uchar( pow(-1, i + j) );
		}
	}
}

#define LOWPASS 0
#define HIGHPASS 1

void my_ideal_pass_filter(Mat & frequency, int D0 = 40, int pass = LOWPASS){
	Mat complexI = frequency;
	Mat planes[2];

	split(complexI, planes);
	int m = planes[0].rows, n = planes[0].cols;
	int cm = m / 2, cn = n / 2;
	int D0_sqr = D0 * D0;

	for( int i = 0; i < m; i++ ){
		for( int j = 0; j < n; j++ ){
			int D_sqr = (i - cm) * (i - cm) + (j - cn) * (j - cn);
			if( pass == LOWPASS && D_sqr > D0_sqr ){
				// 低通
				*planes[0].ptr<float>(i, j) = 0;
				*planes[1].ptr<float>(i, j) = 0;
			}
			else if( pass == HIGHPASS && D_sqr < D0_sqr ){
				// 高通
				*planes[0].ptr<float>(i, j) = 0;
				*planes[1].ptr<float>(i, j) = 0;
			}
		}
	}

	merge(planes, 2, complexI);
	frequency = complexI;
}

// n 越大, 振铃越明显
void my_Butterworth_pass_Filter(Mat & frequency, int D0 = 200, int expo_n = 20, int pass = LOWPASS){
	Mat complexI = frequency;
	Mat planes[2];

	split(complexI, planes);
	int m = planes[0].rows, n = planes[0].cols;
	int cm = m / 2, cn = n / 2;
	int D0_sqr = D0 * D0;
	for( int i = 0; i < m; i++ ){
		for( int j = 0; j < n; j++ ){
			int D_sqr = (i - cm) * (i - cm) + (j - cn) * (j - cn);
			double coef = 1.0 / (1 + pow( (D_sqr * 1.0 / D0), expo_n ) );
			if( pass == LOWPASS ){
				*planes[0].ptr<float>(i, j) *= float(coef);
				*planes[1].ptr<float>(i, j) *= float(coef);
			}
			else if( pass == HIGHPASS ){
				*planes[0].ptr<float>(i, j) *= 1 - float(coef);
				*planes[1].ptr<float>(i, j) *= 1 - float(coef);
			}
		}
	}

	merge(planes, 2, complexI);
	frequency = complexI;
}

void lab1(){
	Mat space_img, frequency_img;
	// space_img = imread("board.jpg", 0);
	space_img = imread("imageTextN.png", 0);
	imshow("原图", space_img);

	if( true ){
		my_dft(space_img, frequency_img);
	}

	if( true ){
		my_idft(frequency_img, space_img);
		imshow("反变换", space_img);
	}

	waitKey(0);
	destroyAllWindows();
}

void lab2(){
	cout << "请输入D0: ";
	int D0;
	cin >> D0;

	Mat space_img1, frequency_img, space_img2;
	space_img1 = imread("board.jpg", 0);
	imshow("原图", space_img1);

	if( true ){
		my_dft(space_img1, frequency_img);
		my_ideal_pass_filter(frequency_img, D0, LOWPASS);
		my_idft(frequency_img, space_img2);
		imshow("理想低通反变换", space_img2);
	}

	if( true ){
		my_dft(space_img1, frequency_img);
		my_ideal_pass_filter(frequency_img, D0, HIGHPASS);
		my_idft(frequency_img, space_img2);
		imshow("理想高通反变换", space_img2);
	}

	waitKey(0);
	destroyAllWindows();
}

void lab3(){
	cout << "请输入D0: ";
	int D0;
	cin >> D0;

	cout << "请输入n: ";
	int expo_n;
	cin >> expo_n;

	Mat space_img1, frequency_img, space_img2;
	space_img1 = imread("board.jpg", 0);
	imshow("原图", space_img1);

	if( true ){
		my_dft(space_img1, frequency_img);
		my_Butterworth_pass_Filter(frequency_img, D0, expo_n, LOWPASS);
		my_idft(frequency_img, space_img2);
		imshow("Butterworth低通反变换", space_img2);
	}

	if( true ){
		my_dft(space_img1, frequency_img);
		my_Butterworth_pass_Filter(frequency_img, D0, expo_n, HIGHPASS);
		my_idft(frequency_img, space_img2);
		imshow("Butterworth高通反变换", space_img2);
	}

	waitKey(0);
	destroyAllWindows();
}


int main(){
	ios::sync_with_stdio(false);
	cin.tie(0);

	char option = '1';
	while( true ){
		cout << "输入实验编号: ";
		cin >> option;
		switch(option){
			case '1':
				lab1();
				break;
			case '2':
				lab2();
				break;
			case '3':
				lab3();
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