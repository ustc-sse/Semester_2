/*
1.	均值滤波(空域)
	具体内容：利用 OpenCV 对灰度图像像素进行操作，分别利用算术均值滤波器、几何均值滤波器、谐波和逆谐波均值滤波器进行图像去噪。模板大小为5*5。（注：请分别为图像添加高斯噪声、胡椒噪声、盐噪声和椒盐噪声，并观察滤波效果）

2.	中值滤波(空域)
	具体内容：利用 OpenCV 对灰度图像像素进行操作，分别利用 5*5 和 9*9尺寸的模板对图像进行中值滤波。（注：请分别为图像添加胡椒噪声、盐噪声和椒盐噪声，并观察滤波效果）

3.	自适应均值滤波(空域)
	具体内容：利用 OpenCV 对灰度图像像素进行操作，设计自适应局部降低噪声滤波器去噪算法。模板大小 7*7（对比该算法的效果和均值滤波器的效果）

4.	自适应中值滤波(空域)
	具体内容：利用 OpenCV 对灰度图像像素进行操作，设计自适应中值滤波算法对椒盐图像进行去噪。模板大小 7*7（对比中值滤波器的效果）

5.	彩色图像均值滤波(空域)
	具体内容：利用 OpenCV 对彩色图像 RGB 三个通道的像素进行操作，利用算术均值滤波器和几何均值滤波器进行彩色图像去噪。模板大小为 5*5。
*/

#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <string>
#include <math.h>
#include <random>
#include <algorithm>
#include <numeric>

using namespace std;
using namespace cv;

static int nRows, nCols;

void normal_distribution_test(){
	default_random_engine generator;
	double mean = 5, sigma = 3;
	normal_distribution<double> distribution(mean, sigma);
	int pdf[10] = {0};
	for( int i = 0; i < 10000; i++ ){
		double noise = distribution(generator);
		if( noise >= 0 && noise < 10 ){
			pdf[int(noise)]++;
		}
	}
	int cnt = 0;
	for( int i = 0; i < 10; i++ ){
		cnt += pdf[i];
		cout << pdf[i] << '\n';
	}
	cout << '\n' << cnt << '\n';
}

/*
	http://www.cplusplus.com/reference/random/normal_distribution/
	https://stackoverflow.com/questions/32889309/adding-gaussian-noise

	https://stackoverflow.com/questions/47614998/box-muller-random-number-between-0-and-1-in-c
	https://blog.csdn.net/u012936765/article/details/53200918
	https://blog.csdn.net/dreamandxiaochouyu/article/details/45371471
	https://blog.csdn.net/sunmc1204953974/article/details/50623071
*/
void add_Gaussian_noise(Mat & img, Mat & img_with_noise, double mean = 0, double sigma = 1, int coefficient = 4){

	img_with_noise = img.clone();
	Mat_<uchar>::iterator it = img_with_noise.begin<uchar>();
	Mat_<uchar>::iterator end = img_with_noise.end<uchar>();

	default_random_engine generator;
	normal_distribution<double> distribution(mean, sigma);

	while( it != end )
	{
		int val = int(*it);
		int noise = coefficient * int( distribution(generator) );
		val += noise;
		if( val < 0 ){
			val = 0;
		}
		if( val > 255 ){
			val = 255;
		}
		*it = uchar(val);
		it++;
	}
	imshow("高斯噪声", img_with_noise);
}

void add_Gaussian_noise_rgb(Mat & img, Mat & img_with_noise, double mean = 0, double sigma = 1, int coefficient = 4){

	img_with_noise = img.clone();
	Mat_<Vec3b>::iterator it = img_with_noise.begin<Vec3b>();
	Mat_<Vec3b>::iterator end = img_with_noise.end<Vec3b>();

	default_random_engine generator;
	normal_distribution<double> distribution(mean, sigma);

	while( it != end )
	{
		int noise = coefficient * int( distribution(generator) );
		Vec3b & pixel = *it;
		if( int(pixel[0]) + noise < 0 ){
			pixel[0] = 0;
		}
		if( int(pixel[0]) + noise > 255 ){
			pixel[0] = 255;
		}
		if( int(pixel[1]) + noise < 0 ){
			pixel[1] = 0;
		}
		if( int(pixel[1]) + noise > 255 ){
			pixel[1] = 255;
		}
		if( int(pixel[2]) + noise < 0 ){
			pixel[2] = 0;
		}
		if( int(pixel[2]) + noise > 255 ){
			pixel[2] = 255;
		}
		
		it++;
	}
	imshow("高斯噪声rgb", img_with_noise);
}

/*
	防止随机数重复
	https://stackoverflow.com/questions/5382037/non-repeating-random-number-generator
	http://www.cplusplus.com/reference/algorithm/random_shuffle/
	http://www.cplusplus.com/reference/numeric/iota/?kw=iota

	简单起见, 没有做去重处理
*/
void add_pepper_and_salt_noise(Mat & img, Mat & img_with_noise, bool add_pepper = true, bool add_salt = true, int ttl_noise = 100){

	img_with_noise = img.clone();
	if( add_pepper == true ){
		for( int i = 0; i < ttl_noise; ){
			int r = rand() % nRows;
			int c = rand() % nCols;
			if( r >= 10 && r <= 240 && c > 10 && c <= 240 ){
				*img_with_noise.ptr<uchar>(r, c) = 0;
				i++;
			}
		}
	}

	if( add_salt == true ){
		for( int i = 0; i < ttl_noise; ){
			int r = rand() % nRows;
			int c = rand() % nCols;
			if( r >= 10 && r <= 240 && c > 10 && c <= 240 ){
				*img_with_noise.ptr<uchar>(r, c) = 255;
				i++;
			}
		}
	}

	if( add_pepper == true && add_salt == false ){
		imshow("椒噪声", img_with_noise);
	}
	else if( add_pepper == false && add_salt == true ){
		imshow("盐噪声", img_with_noise);
	}
	else if( add_pepper == true && add_salt == true ){
		imshow("椒盐噪声", img_with_noise);
	}
}

void my_arithmetic_mean_filter(Mat & img_with_noise, int size = 5){
	Mat img_filtered = img_with_noise.clone();

	int offset = size / 2;
	int ttl = size * size;
	int beginR = offset, beginC = offset;
	int endR = nRows - offset, endC = nCols - offset;
	for( int i = beginR; i < endR; i++ ){
		for( int j = beginC; j < endC; j++ ){
			int sum = 0;
			for( int x = 0; x < size; x++ ){
				for( int y = 0; y < size; y++ ){
					// 注意, 选取 img_with_noise 的像素, 而非 img_filtered 的像素, 否则是多重模糊
					uchar val_img = *img_with_noise.ptr<uchar>(i + x - offset, j + y - offset);
					sum += val_img;
				}
			}
			sum /= ttl;
			*img_filtered.ptr<uchar>(i, j) = uchar(sum);
		}
	}

	imshow("算术均值滤波", img_filtered);
}

void my_arithmetic_mean_filter_rgb(Mat & img_with_noise, int size = 5){
	Mat img_filtered = img_with_noise.clone();

	int offset = size / 2;
	int ttl = size * size;
	int beginR = offset, beginC = offset;
	int endR = nRows - offset, endC = nCols - offset;
	for( int i = beginR; i < endR; i++ ){
		for( int j = beginC; j < endC; j++ ){
			int sum0 = 0, sum1 = 0, sum2 = 0;
			for( int x = 0; x < size; x++ ){
				for( int y = 0; y < size; y++ ){
					Vec3b & pixel = *img_with_noise.ptr<Vec3b>(i + x - offset, j + y - offset);
					sum0 += pixel[0];
					sum1 += pixel[1];
					sum2 += pixel[2];
				}
			}
			sum0 /= ttl;
			sum1 /= ttl;
			sum2 /= ttl;
			*img_filtered.ptr<Vec3b>(i, j) = Vec3b(uchar(sum0), uchar(sum1), uchar(sum2));
		}
	}

	imshow("算术均值滤波rgb", img_filtered);
}

void my_geometric_mean_filter(Mat & img_with_noise, int size = 5){
	Mat img_filtered = img_with_noise.clone();

	int offset = size / 2;
	int ttl = size * size;
	int beginR = offset, beginC = offset;
	int endR = nRows - offset, endC = nCols - offset;
	for( int i = beginR; i < endR; i++ ){
		for( int j = beginC; j < endC; j++ ){
			double product = 1.0;
			for( int x = 0; x < size; x++ ){
				for( int y = 0; y < size; y++ ){
					// 注意, 选取 img_with_noise 的像素, 而非 img_filtered 的像素, 否则是多重模糊
					uchar val_img = *img_with_noise.ptr<uchar>(i + x - offset, j + y - offset);
					if( val_img == 0 ){
						val_img = 1;
					}
					product *= val_img;
				}
			}
			product = pow(product, 1.0/ttl);
			*img_filtered.ptr<uchar>(i, j) = uchar(product);
		}
	}

	imshow("几何均值滤波", img_filtered);
}

void my_geometric_mean_filter_rgb(Mat & img_with_noise, int size = 5){
	Mat img_filtered = img_with_noise.clone();

	int offset = size / 2;
	int ttl = size * size;
	int beginR = offset, beginC = offset;
	int endR = nRows - offset, endC = nCols - offset;
	for( int i = beginR; i < endR; i++ ){
		for( int j = beginC; j < endC; j++ ){
			double product0 = 1.0;
			double product1 = 1.0;
			double product2 = 1.0;
			for( int x = 0; x < size; x++ ){
				for( int y = 0; y < size; y++ ){
					Vec3b & pixel = *img_with_noise.ptr<Vec3b>(i + x - offset, j + y - offset);
					if( pixel[0] == 0 ){
						pixel[0] = 1;
					}
					product0 *= pixel[0];
					if( pixel[1] == 0 ){
						pixel[1] = 1;
					}
					product1 *= pixel[1];
					if( pixel[2] == 0 ){
						pixel[2] = 1;
					}
					product2 *= pixel[2];
				}
			}
			product0 = pow(product0, 1.0/ttl);
			product1 = pow(product1, 1.0/ttl);
			product2 = pow(product2, 1.0/ttl);
			
			*img_filtered.ptr<Vec3b>(i, j) = Vec3b(uchar(product0), uchar(product1), uchar(product2));
		}
	}

	imshow("几何均值滤波", img_filtered);
}

void my_harmonic_mean_filter(Mat & img_with_noise, int size = 5){
	Mat img_filtered = img_with_noise.clone();

	int offset = size / 2;
	int ttl = size * size;
	int beginR = offset, beginC = offset;
	int endR = nRows - offset, endC = nCols - offset;
	for( int i = beginR; i < endR; i++ ){
		for( int j = beginC; j < endC; j++ ){
			double sum = 0;
			for( int x = 0; x < size; x++ ){
				for( int y = 0; y < size; y++ ){
					// 注意, 选取 img_with_noise 的像素, 而非 img_filtered 的像素, 否则是多重模糊
					uchar val_img = *img_with_noise.ptr<uchar>(i + x - offset, j + y - offset);
					if( val_img != 0 ){
						sum += 1.0/val_img;
					}
				}
			}
			sum = ttl / sum;
			*img_filtered.ptr<uchar>(i, j) = uchar(sum);
		}
	}

	imshow("谐波均值滤波", img_filtered);
}

void my_contraharmonic_mean_filter(Mat & img_with_noise, int q = 1, int size = 5){
	Mat img_filtered = img_with_noise.clone();

	int offset = size / 2;
	int ttl = size * size;
	int beginR = offset, beginC = offset;
	int endR = nRows - offset, endC = nCols - offset;
	for( int i = beginR; i < endR; i++ ){
		for( int j = beginC; j < endC; j++ ){
			double sum1 = 0;
			double sum2 = 0;
			for( int x = 0; x < size; x++ ){
				for( int y = 0; y < size; y++ ){
					// 注意, 选取 img_with_noise 的像素, 而非 img_filtered 的像素, 否则是多重模糊
					uchar val_img = *img_with_noise.ptr<uchar>(i + x - offset, j + y - offset);
					sum1 += pow(val_img, q + 1);
					sum2 += pow(val_img, q);
				}
			}
			if( sum2 != 0 ){
				sum1 /= sum2;
				*img_filtered.ptr<uchar>(i, j) = uchar(sum1);
			}
		}
	}

	// Q > 0 时, lenna 似乎在翻白眼
	imshow("逆谐波均值滤波", img_filtered);
}

void my_median_filter(Mat & img_with_noise, int size = 5){
	Mat img_filtered = img_with_noise.clone();

	int offset = size / 2;
	int ttl = size * size;
	int beginR = offset, beginC = offset;
	int endR = nRows - offset, endC = nCols - offset;
	for( int i = beginR; i < endR; i++ ){
		for( int j = beginC; j < endC; j++ ){
			vector<uchar> val_list;
			for( int x = 0; x < size; x++ ){
				for( int y = 0; y < size; y++ ){
					// 注意, 选取 img_with_noise 的像素, 而非 img_filtered 的像素, 否则是多重模糊
					uchar val_img = *img_with_noise.ptr<uchar>(i + x - offset, j + y - offset);
					val_list.push_back(val_img);
				}
			}
			// for( int a = 0; a < ttl; a++ ){
			// 	for( int b = a + 1; b < ttl; b++ ){
			// 		if( val_list[a] > val_list[b] ){
			// 			swap(val_list[a], val_list[b]);
			// 		}
			// 	}
			// }
			sort(val_list.begin(), val_list.end());
			int mid = ttl / 2;
			*img_filtered.ptr<uchar>(i, j) = uchar(val_list[mid]);
		}
	}

	imshow("中值滤波", img_filtered);
}

/*
	https://stackoverflow.com/questions/7616511/calculate-mean-and-standard-deviation-from-a-vector-of-samples-in-c-using-boos/7616820

	http://www.cplusplus.com/reference/numeric/inner_product/
	https://zh.wikipedia.org/wiki/標準差
	https://zh.wikipedia.org/wiki/方差

	计算均值和方差
	#include <numeric>
	double sum = std::accumulate(v.begin(), v.end(), 0.0);
	double mean = sum / v.size();

	double sq_sum = std::inner_product(v.begin(), v.end(), v.begin(), 0.0);
	double variance = sq_sum / v.size() - mean * mean
	double stdev = std::sqrt(variance);
*/
void my_adaptive_mean_filter(Mat & img_with_noise, double stdev = 5, int size = 5){
	Mat img_filtered = img_with_noise.clone();

	int offset = size / 2;
	int ttl = size * size;
	int beginR = offset, beginC = offset;
	int endR = nRows - offset, endC = nCols - offset;
	double variance_global = stdev * stdev;

	for( int i = beginR; i < endR; i++ ){
		for( int j = beginC; j < endC; j++ ){
			vector<double> val_list;
			for( int x = 0; x < size; x++ ){
				for( int y = 0; y < size; y++ ){
					uchar val_img = *img_with_noise.ptr<uchar>(i + x - offset, j + y - offset);
					val_list.push_back( double(val_img) );
				}
			}
			double sum = accumulate(val_list.begin(), val_list.end(), 0.0);
			double mean = sum / ttl;
			double variance_local = 0;
			for( double val : val_list ){
				variance_local += pow(val - mean, 2);
			}
			variance_local /= (ttl - 1);

			double val = double( *img_with_noise.ptr<uchar>(i,j) );

			if( variance_local != 0 ){
				val -= (val - mean) * variance_global / variance_local;
			}
			*img_filtered.ptr<uchar>(i, j) = uchar(val);
		}
	}

	imshow("自适应均值滤波", img_filtered);
}

void my_adaptive_median_filter(Mat & img_with_noise, int size = 5){
	Mat img_filtered = img_with_noise.clone();

	int offset = size / 2;
	int ttl = size * size;
	int beginR = offset, beginC = offset;
	int endR = nRows - offset, endC = nCols - offset;

	int window_size = 3, window_size_max = size;
	for( int i = beginR; i < endR; i++ ){
		for( int j = beginC; j < endC; j++ ){

			while( window_size < window_size_max ){
				size = window_size;
				offset = size / 2;
				vector<int> val_list;
				for( int x = 0; x < size; x++ ){
					for( int y = 0; y < size; y++ ){
						int val_img = int(*img_with_noise.ptr<uchar>(i + x - offset, j + y - offset));
						val_list.push_back(val_img);
					}
				}
				sort(val_list.begin(), val_list.end());
				ttl = int( val_list.size() );
				int Zmin = val_list[0], Zmax = val_list[ttl - 1], Zmed = val_list[ttl/2];
				if( (Zmed - Zmin) > 0 && (Zmed - Zmax) < 0 ){
					// B
					int Zxy = int( *img_with_noise.ptr<uchar>(i, j) );
					if( (Zxy - Zmin) > 0 && (Zxy - Zmax) < 0 ){
						*img_filtered.ptr<uchar>(i, j) = uchar(Zxy);
					}
					else{
						*img_filtered.ptr<uchar>(i, j) = uchar(Zmed);
					}
					break;
				}else{
					window_size ++;
					if( window_size > window_size_max ){
						*img_filtered.ptr<uchar>(i, j) = uchar(Zmed);
						break;
					}
				}
			}
		}
	}

	imshow("自适应中值滤波", img_filtered);
}

/*
	算术均值滤波
	分别添加高斯噪声, 胡椒噪声, 盐噪声, 椒盐噪声
	并观察滤波效果
*/
void lab1(){
	Mat img, img_with_noise;
	img = imread("lenna.jpg", 0);

	nRows = img.rows;
	nCols = img.cols;
	if( true )
	{
		/******************高斯噪声*************************/
		// 原图
		imshow("原图", img);

		// 添加高斯噪声
		add_Gaussian_noise(img, img_with_noise, 0, 2);

		// 算术均值滤波后的效果
		my_arithmetic_mean_filter(img_with_noise);

		// 几何均值滤波
		my_geometric_mean_filter(img_with_noise);

		// 谐波均值滤波
		my_harmonic_mean_filter(img_with_noise);

		// 逆谐波均值滤波
		my_contraharmonic_mean_filter(img_with_noise);

		waitKey(0);
		destroyAllWindows();
		/*******************************************/
	}

	if( true )
	{
		/******************胡椒噪声*************************/
		// 原图
		imshow("原图", img);

		// 添加胡椒噪声
		add_pepper_and_salt_noise(img, img_with_noise, true, false);
		imshow("椒噪声", img_with_noise);

		// 算术均值滤波后的效果
		my_arithmetic_mean_filter(img_with_noise);

		// 几何均值滤波, 适合盐噪声, 不适合胡椒噪声
		my_geometric_mean_filter(img_with_noise);

		// 谐波均值滤波, 适合盐噪声, 不适合胡椒噪声
		my_harmonic_mean_filter(img_with_noise);

		// 逆谐波均值滤波, Q < 0 时去除盐噪声, Q > 0 时去除胡椒噪声
		my_contraharmonic_mean_filter(img_with_noise, 1);

		waitKey(0);
		destroyAllWindows();
		/*******************************************/
	}

	if( true )
	{
		/******************白盐噪声*************************/
		// 原图
		imshow("原图", img);

		// 添加盐噪声
		add_pepper_and_salt_noise(img, img_with_noise, false, true);
		imshow("盐噪声", img_with_noise);

		// 算术均值滤波后的效果
		my_arithmetic_mean_filter(img_with_noise);

		// 几何均值滤波, 适合盐噪声, 不适合胡椒噪声
		my_geometric_mean_filter(img_with_noise);

		// 谐波均值滤波, 适合盐噪声, 不适合胡椒噪声
		my_harmonic_mean_filter(img_with_noise);

		// 逆谐波均值滤波, Q < 0 时去除盐噪声, Q > 0 时去除胡椒噪声
		my_contraharmonic_mean_filter(img_with_noise, -1);

		waitKey(0);
		destroyAllWindows();
		/*******************************************/
	}

	if( true )
	{
		/******************椒盐噪声*************************/
		// 原图
		imshow("原图", img);

		// 添加椒盐噪声
		add_pepper_and_salt_noise(img, img_with_noise, true, true);
		imshow("椒盐噪声", img_with_noise);

		// 算术均值滤波后的效果
		my_arithmetic_mean_filter(img_with_noise);

		// 几何均值滤波, 适合盐噪声, 不适合胡椒噪声
		my_geometric_mean_filter(img_with_noise);

		// 谐波均值滤波, 适合盐噪声, 不适合胡椒噪声
		my_harmonic_mean_filter(img_with_noise);

		// 逆谐波均值滤波, Q < 0 时去除盐噪声, Q > 0 时去除胡椒噪声
		my_contraharmonic_mean_filter(img_with_noise, 0);

		waitKey(0);
		destroyAllWindows();
		/*******************************************/
	}
}

/*
	中值滤波, 与同尺寸线性平滑滤波相比, 模糊更小
	分别添加胡椒噪声, 盐噪声, 椒盐噪声
*/
void lab2(){
	Mat img, img_with_noise;
	img = imread("lenna.jpg", 0);

	nRows = img.rows;
	nCols = img.cols;
	if( true )
	{
		/******************高斯噪声*************************/
		// 原图
		imshow("原图", img);

		// 添加高斯噪声
		add_Gaussian_noise(img, img_with_noise, 0, 2);

		// 算术均值滤波后的效果
		my_arithmetic_mean_filter(img_with_noise);

		// 中值滤波
		int size = 3;
		my_median_filter(img_with_noise, size);

		waitKey(0);
		destroyAllWindows();
		/*******************************************/
	}

	if( true )
	{
		/******************胡椒噪声*************************/
		// 原图
		imshow("原图", img);

		// 添加胡椒噪声
		add_pepper_and_salt_noise(img, img_with_noise, true, false);
		imshow("椒噪声", img_with_noise);

		// 算术均值滤波后的效果
		my_arithmetic_mean_filter(img_with_noise);

		// 中值滤波
		int size = 3;
		my_median_filter(img_with_noise, size);

		waitKey(0);
		destroyAllWindows();
		/*******************************************/
	}

	if( true )
	{
		/******************白盐噪声*************************/
		// 原图
		imshow("原图", img);

		// 添加盐噪声
		add_pepper_and_salt_noise(img, img_with_noise, false, true);
		imshow("盐噪声", img_with_noise);

		// 算术均值滤波后的效果
		my_arithmetic_mean_filter(img_with_noise);

		// 中值滤波
		int size = 3;
		my_median_filter(img_with_noise, size);

		waitKey(0);
		destroyAllWindows();
		/*******************************************/
	}

	if( true )
	{
		/******************椒盐噪声*************************/
		// 原图
		imshow("原图", img);

		// 添加椒盐噪声
		add_pepper_and_salt_noise(img, img_with_noise, true, true);
		imshow("椒盐噪声", img_with_noise);

		// 算术均值滤波后的效果
		my_arithmetic_mean_filter(img_with_noise);

		// 中值滤波
		int size = 3;
		my_median_filter(img_with_noise, size);

		waitKey(0);
		destroyAllWindows();
		/*******************************************/
	}
}

/*
	自适应均值滤波, 设计自适应局部降低噪声滤波器去噪算法
	使用高斯噪声
	模板大小 7*7（对比均值滤波器的效果）
*/
void lab3(){
	Mat img, img_with_noise;
	img = imread("lenna.jpg", 0);

	nRows = img.rows;
	nCols = img.cols;

	if( true )
	{
		/******************高斯噪声*************************/
		// 原图
		imshow("原图", img);

		// 添加高斯噪声
		// 坑爹的系数 coef = 4 ... 混淆了 高斯噪声 的设定参数
		// 当初是为了放大高斯噪声, 使高斯噪声更明显, 添加的系数 coef
		// 但是这就不是标准差 = sigma 的高斯分布了, 实际的会偏大
		// 这也就是估计的参数 更大时, 图像能够变清晰的原因
		add_Gaussian_noise(img, img_with_noise, 0, 10, 1);

		// 算术均值滤波后的效果
		my_arithmetic_mean_filter(img_with_noise, 7);

		// 自适应均值滤波
		my_adaptive_mean_filter(img_with_noise, 10, 7);

		waitKey(0);
		destroyAllWindows();
		/*******************************************/
	}
}

/*
	自适应中值滤波, 设计自适应中值滤波算法对椒盐图像进行去噪
	模板大小 7*7（对比中值滤波器的效果）
*/

void lab4(){
	Mat img, img_with_noise;
	img = imread("lenna.jpg", 0);

	nRows = img.rows;
	nCols = img.cols;

	if( true )
	{
		/******************椒盐噪声*************************/
		// 原图
		imshow("原图", img);

		// 添加椒盐噪声
		add_pepper_and_salt_noise(img, img_with_noise);

		// 中值滤波后的效果
		my_median_filter(img_with_noise, 7);

		// 自适应中值滤波
		my_adaptive_median_filter(img_with_noise, 7);

		waitKey(0);
		destroyAllWindows();
		/*******************************************/
	}
}

/*
	彩色图像均值滤波(空域)
	具体内容：利用 OpenCV 对彩色图像 RGB 三个通道的像素进行操作，利用算术均值滤波器和几何均值滤波器进行彩色图像去噪。模板大小为 5*5。
*/
void lab5(){
	Mat img, img_with_noise;
	img = imread("lenna.jpg", 1);

	nRows = img.rows;
	nCols = img.cols;

	if( true )
	{
		/******************高斯噪声*************************/
		// 原图
		imshow("原图", img);

		// 添加高斯噪声
		add_Gaussian_noise_rgb(img, img_with_noise, 0, 8);

		// 算术均值滤波后的效果
		my_arithmetic_mean_filter_rgb(img_with_noise);

		// 几何均值滤波
		my_geometric_mean_filter_rgb(img_with_noise);

		waitKey(0);
		destroyAllWindows();
		/*******************************************/
	}
}

int main(){
	ios::sync_with_stdio(false);
	cin.tie(0);

	// normal_distribution_test();
	// return 0;

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
		cout << "实验 ";
		cout << option;
		cout << " 结束\n";
	}
END:
	return 0;
}