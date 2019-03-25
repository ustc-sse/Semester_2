/*
1.	利用 OpenCV 读取图像。
	具体内容：用打开 OpenCV 打开图像，并在窗口中显示

1.1	彩色图像灰度化

2.	灰度图像二值化处理
	具体内容：设置并调整阈值对图像进行二值化处理。

3.	灰度图像的对数变换, s = c·lg(1 + r)
	具体内容：设置并调整 r 值对图像进行对数变换。

4.	灰度图像的伽马变换, s = c·r^gamma
	具体内容：设置并调整γ值对图像进行伽马变换。

5.	彩色图像的补色变换, (Rs, Gs, Bs) = (sum - Rr, sum - Gr, sum - Br)
	sum = min + max
	具体内容：对彩色图像进行补色变换。

取色/截屏 软件：Snipaste

PS: jpg, bmp, png 文件的 header, 最好能了解一下
*/

#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>
#include <math.h>

using namespace std;
using namespace cv;

// Mat 结构
// https://docs.opencv.org/master/d3/d63/classcv_1_1Mat.html

void show_raw_image();
void grayscale_trans();
void threshold_trans(uchar threshold);
void log_trans();
void gamma_trans();
void complementary_trans();

int main()
{
	// part 1, 显示原始图像
	// show_raw_image();

	// part 1.1, 得到灰度图
	// grayscale_trans();

	// part 2, 二值化
	// threshold_trans(100);

	// part 3, 对数变换
	// log_trans();

	// part 4, 幂变换
	// gamma_trans();

	// part 5, 补色变换
	// complementary_trans();
	return 0;
}

void show_raw_image()
{
	string filename = "lenna.jpg"; 
	Mat img = imread(filename);
	imshow("raw", img);
	waitKey(0);

	// 疑问, 如下单通道打开(得到灰度图), 保存得到的图片的大小, 比手写灰度化得到的图片要小
	// Mat img = imread(filename, 0);
	// imwrite("logo_grayscale_cv.jpg", img);
}

// 首先将图像变成灰度图, 再做二值化
// Mat 按照 BGR 格式存储 RGB 图像
void grayscale()
{
	int channel = 1;
	string filename = "lenna.jpg"; 
	Mat img = imread(filename);
	int nRows = img.rows;
	int nCols = img.cols;
	if( channel == 1 )
	{
		img.create(nRows, nCols, 0);
		for( int i = 0; i < nRows; i++ )
		{
			for( int j = 0; j < nCols; j++ )
			{
				// https://www.cnblogs.com/ronny/p/opencv_road_2.html
				// https://blog.csdn.net/wc781708249/article/details/78447756
				// at(), create()
				auto pixel = img.at<Vec3b>(i, j);
				int gray = int(0.299 * pixel[0] + 0.587 * pixel[1] + 0.114 * pixel[2]);
				img.at<uchar>(i, j) = gray;
			}
		}
		imwrite("lenna_grayscale.jpg", img);
	}
	else
	{
		typedef Point3_<uchar> Pixel;
		img = img;
		for( int i = 0; i < nRows; i++ )
		{
			for( int j = 0; j < nCols; j++ )
			{
				// https://docs.opencv.org/master/d3/d63/classcv_1_1Mat.html#a13acd320291229615ef15f96ff1ff738
				// img.ptr(i) Returns a pointer to the specified matrix row.
				// 返回指向第 i 行的指针
				Pixel * pixel = img.ptr<Pixel>(i, j);
				int gray = int(0.299 * pixel->x + 0.587 * pixel->y + 0.114 * pixel->z);
				pixel->x = pixel->y = pixel->z = gray;
			}
		}
		imwrite("lenna_gray_channel_3.jpg", img);
	}
}

// 二值化后得到的图片体积比灰度图要小
// jpg 把空白图像压缩了?
void threshold_trans(uchar threshold = 100)
{
	// http://www.opencv.org.cn/opencvdoc/2.3.2/html/doc/tutorials/core/mat%20-%20the%20basic%20image%20container/mat%20-%20the%20basic%20image%20container.html
	Mat img = imread("lenna.jpg", 0);
	imshow("raw", img);
	int nRows = img.rows;
	int nCols = img.cols;
	for( int i = 0; i < nRows; i++ )
	{
		for( int j = 0; j < nCols; j++ )
		{
			uchar * p = img.ptr<uchar>(i, j);
			if( *p > threshold )
			{
				// 白色
				*p = 255;
			}
			else
			{
				// 黑色
				*p = 0;
			}
		}
	}
	imshow("threshold", img);
	waitKey(0);
	imwrite("lenna_threshold.jpg", img);
}

// 拉伸暗的, 压缩亮的, c 越大, 越亮
// s = c·lg(1 + r)
// r = 0 时 s = 0
// r = 255 时 s = 255
void log_trans()
{
	float c = 5;
	uchar table[256];

	for( int r = 0; r < 256; r++ )
	{
		// 归一化, 假设以 2 为底
		int s = int( 255 * c * log( (1 + r / 255.0) ) );
		// int s = int( c * log(5 + 5 * r) );
		if( s > 255 )
		{
			table[r] = 255;
		}
		else
		{
			table[r] = uchar(s);
		}
	}

	Mat img = imread("lenna.jpg", 0);
	imshow("raw", img);
	int nRows = img.rows;
	int nCols = img.cols;
	for( int i = 0; i < nRows; i++ )
	{
		for( int j = 0; j < nCols; j++ )
		{
			uchar * p = img.ptr<uchar>(i, j);
			*p = table[*p];
		}
	}
	imshow("log_trans", img);
	waitKey(0);
	imwrite("lenna_trans.jpg", img);
}

// s = c·r^gamma
// gamma > 1 时 拉伸暗的, 原图太亮的时候用, 变暗
// gamma < 1 时 拉伸亮的, 原图太暗的时候用, 变亮
// 为什么要用 gamma 变换? 镜头、显示器等物理器件的失真曲线, 近似 gamma 函数(的逆函数), 因此用 gamma 曲线来校正
void gamma_trans()
{
	float gamma = 5;
	uchar table[256];
	for( int r = 0; r < 256; r++ )
	{
		// 归一化
		int s = int( 255 * (pow( r / 255.0, gamma) ) );
		if( s > 255 )
		{
			table[r] = 255;
		}
		else
		{
			table[r] = uchar(s);
		}
	}
	Mat img = imread("lenna.jpg", 0);
	imshow("raw", img);
	int nRows = img.rows;
	int nCols = img.cols;
	for( int i = 0; i < nRows; i++ )
	{
		for( int j = 0; j < nCols; j++ )
		{
			uchar * p = img.ptr<uchar>(i, j);
			*p = table[*p];
		}
	}
	imshow("gamma_trans", img);
	waitKey(0);
	// imwrite("part4/gamma_trans.jpg", img);
}

// sum = min + max
// (Rs, Gs, Bs) = (sum - Rr, sum - Gr, sum - Br)
void complementary_trans()
{
	Mat img = imread("lenna.jpg");
	imshow("raw", img);
	Mat_<Vec3b>::iterator it = img.begin<Vec3b>();
	Mat_<Vec3b>::iterator end = img.end<Vec3b>();
	while( it != end )
	{
		Vec3b & now = (* it);
		uchar b = now[0];
		uchar g = now[1];
		uchar r = now[2];
		uchar maxval = max(max(r, g), b);
		uchar minval = min(min(r, g), b);
		ushort sum = maxval + minval;
		now[0] = sum - b;
		now[1] = sum - g;
		now[2] = sum - r;
		it++;
	}
	imshow("complementary_trans", img);
	waitKey(0);
	// imwrite("lenna_com.jpg", img);
}