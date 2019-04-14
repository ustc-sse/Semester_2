/*
	1.	利用 OpenCV 读取图像。
		具体内容：用打开 OpenCV 打开图像，并在窗口中显示

	1.1	彩色图像灰度化

	2.	灰度图像二值化处理
		具体内容：设置并调整阈值对图像进行二值化处理。

	3.	灰度图像的对数变换, s = c·lg(1 + r)
		具体内容：设置并调整 r 值对图像进行对数变换。

	4.	灰度图像的伽马变换, s = c·r^gamma
		具体内容：设置并调整γ值对图像进行伽马变换

	5.	彩色图像的补色变换
		minval = min(r, g, b)
		maxval = max(r, g, b)
		sum = minval + maxval
		r = sum - r
		g = sum - g
		b = sum - b

	取色/截屏 软件：Snipaste

	PS: jpg, bmp, png 文件的 header, 最好能了解一下
*/

#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <string>
#include <cmath>

using namespace std;
using namespace cv;

void my_grayscale(Mat & src, Mat & dst){
	// Mat 结构
	// https://docs.opencv.org/3.4/d3/d63/classcv_1_1Mat.html
	dst = Mat::zeros(src.size(), CV_8U);
	// 或者
	// dst.create(src.size(), CV_8U);

	Mat_<Vec3b>::iterator it = src.begin<Vec3b>();
	Mat_<Vec3b>::iterator end = src.end<Vec3b>();
	Mat_<uchar>::iterator it_gray = dst.begin<uchar>();
	while( it != end ){
		Vec3b & pixel = *it++;
		// grayval = 0.299 * b + 0.587 * g + 0.114 * r
		// Mat 按照 BGR 格式存储 RGB 图像
		uchar grayval = uchar(0.299 * pixel[0] + 0.587 * pixel[1] + 0.114 * pixel[2]);
		*it_gray++ = grayval;
	}
}

void lab1(){
	cout << "读取图像\n";
	// part 1, 显示原始图像
	if( true ){
		string filename = "lenna.jpg";
		Mat img = imread(filename);
		imshow("原图", img);
	}

	// part 1.1, 利用 opencv 读取彩色图片并转为灰度图
	if( false ){
		Mat img = imread("lenna.jpg", 0);
		imshow("OpenCV灰度图", img);
	}

	// part 1.2, 自己实现灰度化
	// 疑问, 使用 opencv 单通道打开得到的灰度图, 保存后得到的文件的大小, 比手写灰度化得到的图片要小
	if( false ){
		Mat img_rgb = imread("lenna.jpg");
		Mat img_gray;
		my_grayscale(img_rgb, img_gray);
		imshow("灰度图", img_gray);
	}

	waitKey(0);
	destroyAllWindows();
}

void lab2(){
	cout << "读取灰度图像, 并进行二值化处理\n";
	// http://www.opencv.org.cn/opencvdoc/2.3.2/html/doc/tutorials/core/mat%20-%20the%20basic%20image%20container/mat%20-%20the%20basic%20image%20container.html
	cout << "请输入阈值0~255: ";
	uchar threshold = 100;
	cin >> threshold;
	if( threshold > 255 ){
		threshold = 255;
	}
	if( threshold < 0 ){
		threshold = 0;
	}

	Mat img = imread("lenna.jpg", 0);
	imshow("原图", img);

	if( true ){
		Mat_<uchar>::iterator it = img.begin<uchar>();
		Mat_<uchar>::iterator end = img.end<uchar>();
		while( it != end ){
			uchar & pixel = *it;
			if( pixel > threshold ){
				pixel = 255;
			}
			else{
				pixel = 0;
			}
			it++;
		}
		imshow("二值化", img);
		imwrite("二值化.jpg", img);
	}

	waitKey(0);
	destroyAllWindows();
}

void lab3(){
	cout << "对数变换, s = c·lg(1 + r)\n";
	cout << "在两个端点处\n";
	cout << "r = 0 时 s = 0\n";
	cout << "r = 255 时 s = 255\n";
	cout << "拉伸暗的, 压缩亮的, c 越大, 越亮\n";
	cout << "请输入 c\n";
	double c = 5;
	cin >> c;

	Mat img = imread("lenna.jpg", 0);
	imshow("原图", img);

	if( true ){
		// 先做好 0-255 的变换, 稍后可用查表法加快处理速度
		uchar table[256];
		for( int i = 0; i < 256; i++ ){
			// 归一化 r / 255.0
			double r = i / 255.0;
			// log 以 2 为底
			int s = int( 255 * c * log(1 + r) );
			if( s > 255 ){
				table[i] = 255;
			}
			else{
				table[i] = uchar(s);
			}
		}

		Mat_<uchar>::iterator it = img.begin<uchar>();
		Mat_<uchar>::iterator end = img.end<uchar>();
		while( it != end ){
			uchar & pixel = *it;
			pixel = table[pixel];
			it++;
		}
		imshow("对数变换", img);
		imwrite("对数变换.jpg", img);
	}

	waitKey(0);
	destroyAllWindows();
}

void lab4(){
	cout << "幂变换, s = c·r^gamma\n";
	cout << "镜头、显示器等物理器件的失真曲线, 近似 gamma 函数(的逆函数), 因此用 gamma 曲线来校正\n";
	cout << "gamma > 1 时 拉伸暗的, 原图太亮的时候用, 变暗\n";
	cout << "gamma < 1 时 拉伸亮的, 原图太暗的时候用, 变亮\n";
	cout << "请输入 gamma: ";
	double gamma = 5;
	cin >> gamma;

	Mat img = imread("lenna.jpg", 0);
	imshow("原图", img);

	if( true ){
		uchar table[256];
		for( int i = 0; i < 256; i++ ){
			// 归一化 r / 255.0
			double r = i / 255.0;
			// log 以 2 为底
			int s = int( 255 * pow(r, gamma) );
			if( s > 255 ){
				table[i] = 255;
			}
			else{
				table[i] = uchar(s);
			}
		}

		Mat_<uchar>::iterator it = img.begin<uchar>();
		Mat_<uchar>::iterator end = img.end<uchar>();
		while( it != end ){
			uchar & pixel = *it;
			pixel = table[pixel];
			it++;
		}
		imshow("幂变换", img);
		imwrite("幂变换.jpg", img);
	}

	waitKey(0);
	destroyAllWindows();
}

void lab5(){
	cout << "彩色图像的补色变换\n";

	Mat img = imread("lenna.jpg");
	imshow("原图", img);

	if( true ){
		Mat_<Vec3b>::iterator it = img.begin<Vec3b>();
		Mat_<Vec3b>::iterator end = img.end<Vec3b>();
		while( it != end ){
			Vec3b & pixel = *it;
			int b = pixel[0], g = pixel[1], r = pixel[2];
			int maxval = max( max(b, g), r);
			int minval = min( min(b, g), r);
			int sum = maxval + minval;
			pixel[0] = sum - b;
			pixel[1] = sum - g;
			pixel[2] = sum - r;
			it++;
		}
		imshow("补色变换", img);
		imwrite("补色变换.jpg", img);
	}

	waitKey(0);
	destroyAllWindows();
}

int main(){
	ios::sync_with_stdio(false);
	cin.tie(0);
	cout << "1. 显示原始图像\n";
	cout << "2. 二值化\n";
	cout << "3. 对数变换\n";
	cout << "4. 幂变换\n";
	cout << "5. 补色变换\n";

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
			case '4':
				lab4();
				break;
			case '5':
				lab5();
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
