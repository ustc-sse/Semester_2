/*
1.	利用均值模板平滑灰度图像
	具体内容：利用 OpenCV 对图像像素进行操作，分别利用 3*3、5*5 和 9*9 尺寸的均值模板平滑灰度图像

2.	利用高斯模板平滑灰度图像
	具体内容：利用 OpenCV 对图像像素进行操作，分别利用 3*3、5*5 和 9*9 尺寸的高斯模板平滑灰度图像

3.	利用 Laplacian 模板锐化灰度图像, 用 Robert, Sobel 算子检测边缘

4.	利用高提升滤波算法增强灰度图像
	具体内容：利用 OpenCV 对图像像素进行操作，设计高提升滤波算法增强图像

5.	利用均值模板平滑彩色图像
	具体内容：利用 OpenCV 分别对图像像素的 RGB 三个通道进行操作，利用 3*3、5*5 和 9*9 尺寸的均值模板平滑彩色图像

6.	利用高斯模板平滑彩色图像
	具体内容：利用 OpenCV 分别对图像像素的 RGB 三个通道进行操作，分别利用 3*3、5*5 和 9*9 尺寸的高斯模板平滑彩色图像

7.	利用 Laplacian 模板锐化彩色图像, 用 Robert, Sobel 算子检测边缘
*/

#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <string>
#include <math.h>
#include <algorithm>

using namespace std;
using namespace cv;

void my_avg_filter(Mat & img, int size = 3){
	int nRows = img.rows, nCols = img.cols;
	int offset = size / 2;
	int ttl = size * size;
	int beginR = offset, beginC = offset;
	int endR = nRows - offset, endC = nCols - offset;
	// Mat out( nRows, nCols, CV_8U, Scalar(0) );
	Mat out = img.clone();	// 方便起见, 不处理最外面一圈
	for( int i = beginR; i < endR; i++ ){
		for( int j = beginC; j < endC; j++ ){
			int sum = 0;
			for( int x = 0; x < size; x++ ){
				for( int y = 0; y < size; y++ ){
					uchar val_img = *img.ptr<uchar>(i + x - offset, j + y - offset);
					sum += val_img * 1;
				}
			}
			uchar val_out = uchar(sum / ttl);
			*out.ptr<uchar>(i, j) = val_out;
		}
	}
	imshow("均值模糊", out);
}

void lab01(){
	cout << "1. 利用均值模板平滑灰度图像\n";
	cout << "请输入均值平滑模板尺寸(3, 5, 7): ";
	int size = 3;
	cin >> size;

	if( true ){
		Mat img = imread("lenna.jpg", 0);
		imshow("原图", img);
		my_avg_filter(img, size);
	}

	waitKey(0);
	destroyAllWindows();
}

void get_gaussian_filter(vector<vector<double>> & gaussian_filter, int size, double sigma, double & ttl){
	int offset = size / 2;
	// f = k * exp[ - (x^2 + y^2) / (2 * sigma^2) ]
	double sigma2_2 = 2.0 * pow(sigma, 2);
	for( int i = 0; i < size; i++ ){
		double x2 = pow(i - offset, 2);
		for( int j = 0; j < size; j++ ){
			double y2 = pow(j - offset, 2);
			gaussian_filter[i][j] = exp( -(x2 + y2) / sigma2_2 );
		}
	}

	// 将左上角的系数归一化为1
	double k = 1.0 / gaussian_filter[0][0];
	ttl = 0.0;
	for( int i = 0; i < size; i++ ){
		for( int j = 0; j < size; j++ ){
			gaussian_filter[i][j] *= k;
			ttl += gaussian_filter[i][j];
		}
	}
}

/*
	https://www.cnblogs.com/wangguchangqing/p/6407717.html
	https://blog.csdn.net/L_inYi/article/details/8915116
	高斯滤波器是一种线性低通滤波器，能够有效的抑制噪声，平滑图像。
	模板系数随着距离增大而减小。所以，对图像个模糊程度较小。
	平滑效果更柔和，而且边缘保留的也更好。
	整数形式的，则需要进行归一化处理，将模板左上角的值归一化为1
	如果σ较小，那么模板中心系数较大，而周围系数较小, 这样平滑效果就不是很明显
	反之，σ较大，则模板的各个系数相差就不是很大，类似均值模板，平滑效果比较明显。
*/
void my_gaussian_filter(Mat & img, Mat & out, int size = 3, double sigma = 0.8){
	int nRows = img.rows, nCols = img.cols;
	int offset = size / 2;
	int beginR = offset, beginC = offset;
	int endR = nRows - offset, endC = nCols - offset;
	double ttl = 0.0;

	vector<vector<double>> gaussian_filter(size, vector<double>(size, 0));
	get_gaussian_filter(gaussian_filter, size, sigma, ttl);

	// Mat out( nRows, nCols, CV_8U, Scalar(0) );
	out = img.clone();
	for( int i = beginR; i < endR; i++ ){
		for( int j = beginC; j < endC; j++ ){
			double sum = 0.0;
			for( int x = 0; x < size; x++ ){
				for( int y = 0; y < size; y++ ){
					double val_filter = gaussian_filter[x][y];
					uchar val_img = *img.ptr<uchar>(i + x - offset, j + y - offset);
					sum += val_filter * val_img;
				}
			}
			uchar val_out = uchar(sum / ttl);
			*out.ptr<uchar>(i, j) = val_out;
		}
	}
	imshow("高斯模糊", out);
}

void lab02(){
	cout << "2. 利用高斯模板平滑灰度图像\n利用高斯模板平滑灰度图像, size 变化的时候, 差异不是很明显\n但是改变 sigma 后, 模糊效果差异很大, sigma = 高斯半径\nsigma 越大, 越平滑(模糊), sigma 越小, 细节保留越多\n";

	cout << "请输入高斯平滑模板尺寸(3, 5, 7): ";
	int size = 3;
	cin >> size;
	cout << "请输入标准差 sigma: ";
	double sigma = 0.8;
	cin >> sigma;

	if( true ){
		Mat img = imread("lenna.jpg", 0), out;
		imshow("原图", img);
		my_gaussian_filter(img, out, size, sigma);
	}

	waitKey(0);
	destroyAllWindows();
}

void get_laplacian_filter(vector<vector<int>> laplacian_filter, bool diagonal = false){
	if( diagonal == false ){
		// laplace1, 不含对角项
		laplacian_filter[0][1] = 1;
		laplacian_filter[1][0] = 1;
		laplacian_filter[1][1] = -4;
		laplacian_filter[1][2] = 1;
		laplacian_filter[2][1] = 1;
	}
	else{
		// laplace2, 含对角项
		laplacian_filter[0][0] = 1;
		laplacian_filter[0][1] = 1;
		laplacian_filter[0][2] = 1;

		laplacian_filter[1][0] = 1;
		laplacian_filter[1][1] = -8;
		laplacian_filter[1][2] = 1;

		laplacian_filter[2][0] = 1;
		laplacian_filter[2][1] = 1;
		laplacian_filter[2][2] = 1;
	}
}

/*
	拉普拉斯模板
	https://blog.csdn.net/scottly1/article/details/44408343
	https://blog.csdn.net/wl1070325332/article/details/73291694
	https://blog.csdn.net/fengbingchun/article/details/79321200

	拉普拉斯算子定义 ▼2f = ∂2f/∂x2 + ∂2f/∂y2

	二阶微分
	∂2f/∂x2 ≈  f(x + 1, y) + f(x - 1, y) - 2f(x, y)
	∂2f/∂y2 =   - 2f(x, y)
	▼2f =   f(x + 1, y)
	   + f(x - 1, y)
	   + f(x, y + 1)
	   + f(x, y - 1)
	   - 4f(x, y)

	filter = [[0, 1, 0], [1, -4, 1], [0, 1, 0]]
	未标定, g = mask
	标定, g = [mask - min(mask)] * 255.0 / [max(mask) - min(mask)]
	增强, 设中心系数为负, g = f - 标定 + 修正
	则 g = f - (mask - 0) * 255.0 / [max(mask) - min(mask)]
	注意限定 g 在 0~255 之间, 防止溢出, if g < 0, g = 0, if g > 255 , g = 255
*/
void my_Laplacian_filter(Mat & img){
	int nRows = img.rows, nCols = img.cols;
	int size = 3;
	int offset = size / 2;
	int beginR = offset, beginC = offset;
	int endR = nRows - offset, endC = nCols - offset;
	int ttl = 0;

	vector<vector<int>> laplacian_filter(size, vector<int>(size, 0));
	get_laplacian_filter(laplacian_filter);

	vector<vector<int>> mask(nRows, vector<int>(nCols));
	int minval = 0x0fffffff;
	int maxval = -minval;
	for( int i = beginR; i < endR; i++ ){
		for( int j = beginC; j < endC; j++ ){
			int sum = 0;
			for( int x = 0; x < size; x++ ){
				for( int y = 0; y < size; y++ ){
					int val_filter = laplacian_filter[x][y];
					uchar val_img = *img.ptr<uchar>(i + x - offset, j + y - offset);
					sum += val_filter * val_img;
				}
			}
			// mask 是轮廓细节
			mask[i][j] = sum;
			if( sum < minval ){
				minval = sum;
			}
			if( sum > maxval ){
				maxval = sum;
			}
		}
	}

	Mat out = img.clone();
	double tmp = 255.0 / (maxval - minval);
	for( int i = beginR; i < endR; i++ ){
		for( int j = beginC; j < endC; j++ ){
			// 锐化加强
			int val_out = *out.ptr<uchar>(i, j) - int(mask[i][j] * tmp);

			// 未缩放(标定)的边界
			// int val = mask[i][j];

			// 缩放(标定, scaling)后的边界
			// int val = int((mask[i][j] - minval) * tmp);

			if( val_out < 0 ){
				val_out = 0;
			}
			if( val_out > 255 ){
				val_out = 255;
			}
			*out.ptr<uchar>(i, j) = uchar(val_out);
		}
	}
	imshow("Laplacian", out);
}

/*
	原理, 一阶微分(梯度), 灰度变化, 检测边缘
	robert operators
	M(x, y) = | ▼f | = | grad(f) | = √(g2x + g2y)
	M(x, y) ≈ |gx| + |gy|
	     ≈ |z9 - z5| + |z8 - z6|
	robert1 = [ [-1, 0], [0, 1] ]
	robert2 = [ [0, -1], [1, 0] ]
*/
void my_robert_filter(Mat & img){
	int nRows = img.rows, nCols = img.cols;
	int size = 2;
	int offset = size / 2;
	int beginR = 0, beginC = 0;
	int endR = nRows - offset, endC = nCols - offset;
	Mat out(nRows, nCols, CV_8U, Scalar(0));

	for( int i = 0; i < endR; i++ ){
		for( int j = 0; j < endC; j++ ){
			uchar v1 = *img.ptr<uchar>(i, j);
			uchar v2 = *img.ptr<uchar>(i, j + 1);
			uchar v3 = *img.ptr<uchar>(i + 1, j);
			uchar v4 = *img.ptr<uchar>(i + 1, j + 1);
			int val_out = abs(v4 - v1) + abs(v3 - v2);
			if( val_out > 255 ){
				val_out = 255;
			}
			*out.ptr<uchar>(i, j) = uchar(val_out);
		}
	}
	imshow("Robert", out);
}

/*
	原理, 一阶微分, 灰度变化, 检测边缘
	M(x, y) = | ▼f | = | grad(f) | = √(g2x + g2y)
	M(x, y) ≈ |gx| + |gy|
	     ≈   |(z7 + 2 * z8 + z9) - (z1 + 2 * z2 + z3)|
	       + |(z3 + 2 * z6 + z9) - (z1 + 2 * z4 + z7)|
	Sobel对边缘的响应较宽而且更加强烈，Robert算子对边缘响应较弱，而且对弯曲的边缘敏感度低

	疑问
	结果和书上的光学镜片不一样
	sobel 做完了是不是还要再进行平滑, 然后二值化？
*/
void my_sobel_filter(Mat & img){
	int nRows = img.rows, nCols = img.cols;
	int size = 3;
	int offset = size / 2;
	int beginR = offset, beginC = offset;
	int endR = nRows - offset, endC = nCols - offset;

	Mat out(nRows, nCols, CV_8U, Scalar(0));
	for( int i = beginR; i < endR; i++ ){
		for( int j = endR; j < endC; j++ ){
			int z1 = int( *img.ptr<uchar>(i - 1, j - 1) );
			int z2 = int( *img.ptr<uchar>(i - 1, j + 0) );
			int z3 = int( *img.ptr<uchar>(i - 1, j + 1) );
			int z4 = int( *img.ptr<uchar>(i + 0, j - 1) );

			int z6 = int( *img.ptr<uchar>(i + 0, j + 1) );
			int z7 = int( *img.ptr<uchar>(i + 1, j - 1) );
			int z8 = int( *img.ptr<uchar>(i + 1, j + 0) );
			int z9 = int( *img.ptr<uchar>(i + 1, j + 1) );

			int val_out = abs(z7 + 2 * z8 + z9 - z1 - 2 * z2 - z3) + abs(z3 + 2 * z6 + z9 - z1 - 2 * z4 - z7);
			if( val_out > 255 ){
				val_out = 255;
			}
			*out.ptr<uchar>(i, j) = uchar(val_out);
		}
	}
	imshow("Sobel", out);
}

void lab03(){
	cout << "3. 利用 Laplacian 模板锐化灰度图像, 用 Robert, Sobel 算子检测边缘\n";

	Mat img = imread("lenna.jpg", 0);
	imshow("原图", img);
	if( true ){
		// part 3.1, laplacian
		my_Laplacian_filter(img);
	}

	if( true ){
		// part 3.2, robert
		my_robert_filter(img);
	}

	if( true ){
		// part 3.3, sobel
		my_sobel_filter(img);
	}

	waitKey(0);
	destroyAllWindows();
}

void get_contour(Mat & img, Mat & out, Mat & contour){
	int nRows = img.rows, nCols = img.cols;
	contour = img.clone();

	// 轮廓 contour = 原图像 - 高斯滤波后的图像
	for( int i = 0; i < nRows; i++ ){
		for( int j = 0; j < nCols; j++ ){
			uchar & pixel_contour = *contour.ptr<uchar>(i, j);
			uchar & pixel_out = *out.ptr<uchar>(i, j);
			int val_contour = int(pixel_contour) - int(pixel_out);
			if( val_contour < 0 ){
				val_contour = 0;
			}
			pixel_contour = uchar(val_contour);
		}
	}
}

void my_highboost_filter(Mat & img, Mat & contour, double k = 1.5){
	// g = f + k * mask
	// k = 1 时, 得到 unsharp masking, 非锐化 mask
	// k > 1 时, 得到 highboost filtering, 高提升
	// k 不能太大, 否则会产生盐
	for( int i = begin; i <= endR; i++ ){
		for( int j = begin; j <= endC; j++ ){
			uchar & pixel_img = *img.ptr<uchar>(i, j);
			uchar & pixel_contour = *contour.ptr<uchar>(i, j);
			int val_img = int(pixel_img) + int(k * pixel_contour);
			if( val_img > 255 ){
				val_img = 255;
			}
			pixel_img = uchar(val_img);
		}
	}
	imshow("highboost", img);
}

void lab04(){
	cout << "4. 利用高提升滤波算法增强灰度图像\n先对图像进行高斯模糊, 5x5, sigma=3\n然后 轮廓 = 原图 - 模糊\n高提升 = 原图 + k * 轮廓\nk不能太大, 否则会产生盐\n";

	if( true ){
		Mat img = imread("lenna.jpg", 0), out, contour;
		imshow("原图", img);
		int size = 5;
		double sigma = 3;
		my_gaussian_filter(img, out, size, sigma);
		get_contour(img, out, contour);
		my_highboost_filter(img, contour);
	}

	waitKey(0);
	destroyAllWindows();
}

void lab05(){
	cout << "5. 利用均值模板平滑彩色图像(单通道变成三通道, 略)\n";

	if( true ){

	}

	waitKey(0);
	destroyAllWindows();
}

void lab06(){
	cout << "5. 利用高斯模板平滑彩色图像(单通道变成三通道, 略)\n";

	if( true ){

	}

	waitKey(0);
	destroyAllWindows();
}

void lab07(){
	cout << "7. 利用 Laplacian 模板锐化彩色图像, 用 Robert, Sobel 算子检测边缘(单通道变成三通道, 略)\n";

	if( true ){

	}

	waitKey(0);
	destroyAllWindows();
}

int main(){
	ios::sync_with_stdio(false);
	cin.tie(0);

	cout << "1. 利用均值模板平滑灰度图像\n";
	cout << "2. 利用高斯模板平滑灰度图像\n";
	cout << "3. 利用 Laplacian 模板锐化灰度图像, 用 Robert, Sobel 算子检测边缘\n";
	cout << "4. 利用高提升滤波算法增强灰度图像\n";
	cout << "5. 利用均值模板平滑彩色图像\n";
	cout << "6. 利用高斯模板平滑彩色图像\n";
	cout << "7. 利用 Laplacian 模板锐化彩色图像, 用 Robert, Sobel 算子检测边缘\n";

	char option = '1';
	while(true){
		cout << "\n输入实验编号: ";
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
			case '4':
				lab04();
				break;
			case '5':
				lab05();
				break;
			case '6':
				lab06();
				break;
			case '7':
				lab07();
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
