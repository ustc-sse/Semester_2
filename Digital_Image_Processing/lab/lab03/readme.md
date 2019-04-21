#	高图 lab03 实验报告

<br>

学号：

姓名：　

<br><br><br><br>

##	实验环境

*	`Windows + Visual Studio`

*	`OpenCV 2.4.9`

<br>

##	实验目的

*	掌握利用模板对图像进行空域滤波操作，熟练掌握常用空域模板的使用

*	掌握图像平滑的空域方法，熟练掌握均值模板和高斯模板平滑图像

*	掌握图像锐化的空域方法，熟练掌握 Laplacian、Robert、Sobel 模板锐化
图像

*	掌握利用高提升滤波算法对图像进行增强

<br>

##	实验内容

1.	利用均值模板平滑灰度图像

2.	利用高斯模板平滑灰度图像

3.	利用 Laplacian 模板锐化灰度图像, 用 Robert, Sobel 算子检测边缘

4.	利用高提升滤波算法增强灰度图像

5.	利用均值模板平滑彩色图像

6.	利用高斯模板平滑彩色图像

7.	利用 Laplacian 模板锐化彩色图像, 用 Robert, Sobel 算子检测边缘

<br>

##	实验完成情况

*	所有实验全部完成, 具体说明如下

*	实验 1, 利用均值模板平滑灰度图像, 核心代码如下

	```cpp
	void my_avg_filter(int size = 3){
		Mat img = imread("lenna.jpg", 0);
		imshow("raw", img);
		Mat out = img.clone();

		int nRows = img.rows, nCols = img.cols;
		int offset = size / 2;
		int ttl = size * size;
		int beginR = offset, beginC = offset;
		int endR = nRows - offset, endC = nCols - offset;

		for( int i = beginR; i < endR; i++ ){
			for( int j = beginC; j < endC; j++ ){
				int sum = 0;
				for( int x = 0; x < size; x++ ){
					for( int y = 0; y < size; y++ ){
						uchar val_img = *img.ptr<uchar>(i + x - offset, j + y - offset);
						sum += val_img;
					}
				}
				sum /= ttl;
				*out.ptr<uchar>(i, j) = uchar(sum);
			}
		}
		imshow("new", out);
		waitKey(0);
		destroyAllWindows();
	}
	```

*	实验 2, 利用高斯模板平滑灰度图像, 核心代码如下:

	```cpp
	void my_gaussian_filter(int size = 3, double sigma = 0.8){
		int offset = size / 2;
		vector< vector<double> > gaussian_filter(size, vector<double>(size, 1));
		for( int i = 0; i < size; i++ ){
			double x2 = pow(i - offset, 2);
			for( int j = 0; j < size; j++ ){
				double y2 = pow(j - offset, 2);
				gaussian_filter[i][j] = exp(-(x2 + y2) / (2 * sigma * sigma) );
			}
		}
		// 将左上角的系数归一化为1
		double k = 1.0 / gaussian_filter[0][0];
		double ttl = 0;
		for( int i = 0; i < size; i++ ){
			for( int j = 0; j < size; j++ ){
				gaussian_filter[i][j] *= k;
				ttl += gaussian_filter[i][j];
			}
		}
		Mat img = imread("lenna.jpg", 0);
		imshow("raw", img);
		int nRows = img.rows;
		int nCols = img.cols;
		int begin = size - 1;
		int endR = nRows - size + 1;
		int endC = nCols - size + 1;
		Mat out = img.clone();
		for( int i = begin; i <= endR; i++ ){
			for( int j = begin; j <= endC; j++ ){
				double sum = 0;
				for( int x = 0; x < size; x++ ){
					for( int y = 0; y < size; y++ ){
						double val_filter = gaussian_filter[x][y];
						uchar val_img = *img.ptr<uchar>(i + x - offset, j + y - offset);
						sum += val_filter * val_img;
					}
				}
				sum /= ttl;
				*out.ptr<uchar>(i, j) = uchar(sum);
			}
		}
		imshow("new", out);
		waitKey(0);
		destroyAllWindows();
	}
	```

*	实验 3, 利用 Laplacian 模板锐化灰度图像, 用 Robert, Sobel 算子检测边缘, 核心代码如下:

	```cpp
	void my_Laplacian_filter(){
		// filter size == 3x3
		int size = 3;
		int offset = size / 2;
		vector< vector<int> > laplacian_filter(size, vector<int>(size, 0));
		// laplace1, 不含对角项
		laplacian_filter[0][1] = 1;
		laplacian_filter[1][0] = 1;
		laplacian_filter[1][1] = -4;
		laplacian_filter[1][2] = 1;
		laplacian_filter[2][1] = 1;

		// laplace2, 含对角项
		// laplacian_filter[0][0] = 1;
		// laplacian_filter[0][1] = 1;
		// laplacian_filter[0][2] = 1;

		// laplacian_filter[1][0] = 1;
		// laplacian_filter[1][1] = -8;
		// laplacian_filter[1][2] = 1;

		// laplacian_filter[2][0] = 1;
		// laplacian_filter[2][1] = 1;
		// laplacian_filter[2][2] = 1;

		Mat img = imread("lenna.jpg", 0);
		imshow("raw", img);
		int nRows = img.rows;
		int nCols = img.cols;
		int begin = offset;
		int endR = nRows - size + 1;
		int endC = nCols - size + 1;
		vector< vector<int> > mask(nRows, vector<int>(nCols));
		int minval = 0x0fffffff;
		int maxval = -minval;
		for( int i = begin; i <= endR; i++ )
		{
			for( int j = begin; j <= endC; j++ )
			{
				int sum = 0;
				for( int x = 0; x < size; x++ )
				{
					for( int y = 0; y < size; y++ )
					{
						int val_filter = laplacian_filter[x][y];
						uchar val_img = *img.ptr<uchar>(i + x - offset, j + y - offset);
						sum += val_filter * val_img;
					}
				}
				mask[i][j] = sum;
				if( sum < minval )
				{
					minval = sum;
				}
				if( sum > maxval )
				{
					maxval = sum;
				}
			}
		}

		Mat out = img.clone();
		double tmp = 255.0 / (maxval - minval);
		for( int i = begin; i <= endR; i++ )
		{
			for( int j = begin; j <= endC; j++ )
			{
				// 锐化加强
				int val = *out.ptr<uchar>(i, j) - int(mask[i][j] * tmp);

				// 未缩放(标定)的边界
				// int val = mask[i][j];

				// 缩放(标定, scaling)后的边界
				// int val = int((mask[i][j] - minval) * tmp);

				if( val < 0 )
				{
					val = 0;
				}
				if( val > 255 )
				{
					val = 255;
				}
				*out.ptr<uchar>(i, j) = (uchar)val;
			}
		}
		imshow("Laplacian", out);
		waitKey(0);
	}

	void my_robert_filter(){
		int size = 2;
		Mat img = imread("lenna.jpg", 0);
		imshow("raw", img);
		int nRows = img.rows;
		int nCols = img.cols;
		int endR = nRows - 1;
		int endC = nCols - 1;
		Mat out = img.clone();
		for( int i = 0; i < endR; i++ )
		{
			for( int j = 0; j < endC; j++ )
			{
				uchar v1 = *img.ptr<uchar>(i, j);
				uchar v2 = *img.ptr<uchar>(i, j + 1);
				uchar v3 = *img.ptr<uchar>(i + 1, j);
				uchar v4 = *img.ptr<uchar>(i + 1, j + 1);
				int sum = abs(v4 - v1) + abs(v3 - v2);
				*out.ptr<uchar>(i, j) = uchar(sum);
			}
		}
		imshow("Robert", out);
		waitKey(0);
	}

	void my_sobel_filter(){
		int size = 3;
		int offset = size / 2;
		Mat img = imread("lenna.jpg", 0);
		imshow("raw", img);
		int nRows = img.rows;
		int nCols = img.cols;
		int endR = nRows - 1;
		int endC = nCols - 1;
		Mat out(nRows, nCols, 0, Scalar(0));
		for( int i = 1; i < endR; i++ )
		{
			for( int j = 1; j < endC; j++ )
			{
				int z1 = int( *img.ptr<uchar>(i - 1, j - 1) );
				int z2 = int( *img.ptr<uchar>(i - 1, j + 0) );
				int z3 = int( *img.ptr<uchar>(i - 1, j + 1) );
				int z4 = int( *img.ptr<uchar>(i + 0, j - 1) );

				int z6 = int( *img.ptr<uchar>(i + 0, j + 1) );
				int z7 = int( *img.ptr<uchar>(i + 1, j - 1) );
				int z8 = int( *img.ptr<uchar>(i + 1, j + 0) );
				int z9 = int( *img.ptr<uchar>(i + 1, j + 1) );

				int val = abs(z7 + 2 * z8 + z9 - z1 - 2 * z2 - z3) + abs(z3 + 2 * z6 + z9 - z1 - 2 * z4 - z7);
				if( val > 255 )
				{
					val = 255;
				}
				*out.ptr<uchar>(i, j) = uchar(val);
			}
		}
		imshow("Sobel", out);
		waitKey(0);
	}
	```

*	实验 4, 利用高提升滤波算法增强灰度图像, 核心代码如下:

	```cpp
	void my_highboost_filter(){
		int size = 3;
		double sigma = 1;
		int offset = size / 2;
		vector< vector<double> > gaussian_filter(size, vector<double>(size, 1));
		// 生成高斯模板
		for( int i = 0; i < size; i++ )
		{
			double x2 = pow(i - offset, 1);
			for( int j = 0; j < size; j++ )
			{
				double y2 = pow(j - offset, 2);
				gaussian_filter[i][j] = exp(-(x2 + y2) / (2 * sigma * sigma) );
			}
		}
		// 生成高斯模板, 将左上角的系数归一化为1
		double k = 1.0 / gaussian_filter[0][0];
		double ttl = 0;
		for( int i = 0; i < size; i++ )
		{
			for( int j = 0; j < size; j++ )
			{
				gaussian_filter[i][j] *= k;
				ttl += gaussian_filter[i][j];
			}
		}

		// 原图像
		Mat img_a = imread("dip.jpg", 0);
		// Mat img_a = imread("lenna.jpg", 0);
		int nRows = img_a.rows;
		int nCols = img_a.cols;
		int begin = size - 1;
		int endR = nRows - size + 1;
		int endC = nCols - size + 1;
		imshow("a", img_a);

		// 高斯滤波后的图像
		Mat img_b = img_a.clone();
		for( int i = begin; i <= endR; i++ )
		{
			for( int j = begin; j <= endC; j++ )
			{
				double sum = 0;
				for( int x = 0; x < size; x++ )
				{
					for( int y = 0; y < size; y++ )
					{
						double val_filter = gaussian_filter[x][y];
						uchar val_img = *img_a.ptr<uchar>(i + x - offset, j + y - offset);
						sum += val_filter * val_img;
					}
				}
				sum /= ttl;
				*img_b.ptr<uchar>(i, j) = uchar(sum);
			}
		}
		imshow("b", img_b);

		// 轮廓 mask = 原图像 - 高斯滤波后的图像
		Mat img_c = img_a.clone();
		for( int i = begin; i <= endR; i++ ){
			for( int j = begin; j <= endC; j++ ){
				int val = *img_a.ptr<uchar>(i, j) - *img_b.ptr<uchar>(i, j);
				if( val < 0 ){
					val = 0;
				}
				*img_c.ptr<uchar>(i, j) = uchar(val);
			}
		}
		// mask
		imshow("c", img_c);

		// g = f + k * mask
		// k = 1 时, 得到 unsharp masking, 非锐化mask
		// k > 1 时, 得到 highboost filtering, 高提升
		// k 不能太大, 否则会产生盐
		k = 1;
		Mat img_d = img_a.clone();
		for( int i = begin; i <= endR; i++ ){
			for( int j = begin; j <= endC; j++ ){
				int val = *img_a.ptr<uchar>(i, j);
				val += int( k * ( *img_c.ptr<uchar>(i, j) ) );
				if( val > 255 ){
					val = 255;
				}
				*img_d.ptr<uchar>(i, j) = uchar(val);
			}
		}
		imshow("d", img_d);
		waitKey(0);
	}
	```

*	实验 5, 利用均值模板平滑彩色图像, 核心代码如下:

	```cpp
	void my_avg_filter_rgb(int size = 3){
		Mat img = imread("lenna.jpg");
		imshow("raw", img);
		int nRows = img.rows, nCols = img.cols;
		int offset = size / 2;
		int ttl = size * size;
		int beginR = offset, beginC = offset;
		int endR = nRows - offset, endC = nCols - offset;
		Mat out = img.clone();

		for( int i = beginR; i < endR; i++ ){
			for( int j = beginC; j < endC; j++ ){
				int sum_b = 0;
				int sum_g = 0;
				int sum_r = 0;
				for( int x = 0; x < size; x++ ){
					for( int y = 0; y < size; y++ ){
						Vec3b & pixel = *img.ptr<Vec3b>(i + x - offset, j + y - offset);
						sum_b += pixel[0];
						sum_g += pixel[1];
						sum_r += pixel[2];
					}
				}
				sum_b /= ttl;
				sum_g /= ttl;
				sum_r /= ttl;
				Vec3b & pixel_out = * out.ptr<Vec3b>(i, j);
				pixel_out[0] = sum_b;
				pixel_out[1] = sum_g;
				pixel_out[2] = sum_r;
			}
		}
		imshow("new", out);
		waitKey(0);
		destroyAllWindows();
	}
	```

*	实验 6, 利用高斯模板平滑彩色图像, 核心代码如下:

	```cpp
	void my_gaussian_filter_rgb(int size = 3, double sigma = 0.5){
		int offset = size / 2;
		vector< vector<double> > gaussian_filter(size, vector<double>(size, 1));
		for( int i = 0; i < size; i++ )
		{
			double x2 = pow(i - offset, 2);
			for( int j = 0; j < size; j++ )
			{
				double y2 = pow(j - offset, 2);
				gaussian_filter[i][j] = exp(-(x2 + y2) / (2 * sigma * sigma) );
			}
		}
		// 将左上角的系数归一化为1
		double k = 1.0 / gaussian_filter[0][0];
		double ttl = 0;
		for( int i = 0; i < size; i++ )
		{
			for( int j = 0; j < size; j++ )
			{
				gaussian_filter[i][j] *= k;
				ttl += gaussian_filter[i][j];
			}
		}
		Mat img = imread("lenna.jpg");
		imshow("raw", img);
		int nRows = img.rows;
		int nCols = img.cols;
		int begin = size - 1;
		int endR = nRows - size + 1;
		int endC = nCols - size + 1;

		Mat out = img.clone();
		for( int i = begin; i <= endR; i++ )
		{
			for( int j = begin; j <= endC; j++ )
			{
				double sum_b = 0;
				double sum_g = 0;
				double sum_r = 0;
				for( int x = 0; x < size; x++ )
				{
					for( int y = 0; y < size; y++ )
					{
						double val_filter = gaussian_filter[x][y];
						Vec3b & pixel = *img.ptr<Vec3b>(i + x - offset, j + y - offset);
						sum_b += val_filter * pixel[0];
						sum_g += val_filter * pixel[1];
						sum_r += val_filter * pixel[2];
					}
				}
				sum_b /= ttl;
				sum_g /= ttl;
				sum_r /= ttl;
				Vec3b & pixel_out = * out.ptr<Vec3b>(i, j);
				pixel_out[0] = uchar(sum_b);
				pixel_out[1] = uchar(sum_g);
				pixel_out[2] = uchar(sum_r);
			}
		}
		imshow("new", out);
		waitKey(0);
	}
	```

*	实验 7, 利用 Laplacian 模板锐化彩色图像, 用 Robert, Sobel 算子检测边缘, 核心代码如下:

	```cpp
	void my_Laplacian_filter_rgb(){
		int size = 3;
		int offset = size / 2;
		vector< vector<int> > laplacian_filter(size, vector<int>(size, 0));

		// laplace1, 不含对角项
		laplacian_filter[0][1] = 1;
		laplacian_filter[1][0] = 1;
		laplacian_filter[1][1] = -4;
		laplacian_filter[1][2] = 1;
		laplacian_filter[2][1] = 1;

		// laplace2, 含对角项
		// laplacian_filter[0][0] = 1;
		// laplacian_filter[0][1] = 1;
		// laplacian_filter[0][2] = 1;

		// laplacian_filter[1][0] = 1;
		// laplacian_filter[1][1] = -8;
		// laplacian_filter[1][2] = 1;

		// laplacian_filter[2][0] = 1;
		// laplacian_filter[2][1] = 1;
		// laplacian_filter[2][2] = 1;

		Mat img = imread("lenna.jpg");
		int nRows = img.rows;
		int nCols = img.cols;
		int begin = offset;
		int endR = nRows - size + 1;
		int endC = nCols - size + 1;
		imshow("raw", img);

		int mask_b[250][250];
		int mask_g[250][250];
		int mask_r[250][250];
		int minval_b = 0x0fffffff;
		int minval_g = 0x0fffffff;
		int minval_r = 0x0fffffff;
		int maxval_b = -minval_b;
		int maxval_g = -minval_g;
		int maxval_r = -minval_r;

		for( int i = begin; i <= endR; i++ )
		{
			for( int j = begin; j <= endC; j++ )
			{
				int sum_b = 0;
				int sum_g = 0;
				int sum_r = 0;
				for( int x = 0; x < size; x++ )
				{
					for( int y = 0; y < size; y++ )
					{
						int val_filter = laplacian_filter[x][y];
						Vec3b & pixel = *img.ptr<Vec3b>(i + x - offset, j + y - offset);
						sum_b += val_filter * pixel[0];
						sum_g += val_filter * pixel[1];
						sum_r += val_filter * pixel[2];
					}
				}
				mask_b[i][j] = sum_b;
				mask_g[i][j] = sum_g;
				mask_r[i][j] = sum_r;
				if( sum_b < minval_b )
				{
					minval_b = sum_b;
				}
				if( sum_g < minval_g )
				{
					minval_g = sum_g;
				}
				if( sum_r < minval_r )
				{
					minval_r = sum_r;
				}

				if( sum_b > maxval_b )
				{
					maxval_b = sum_b;
				}
				if( sum_g > maxval_g )
				{
					maxval_g = sum_g;
				}
				if( sum_r > maxval_r )
				{
					maxval_r = sum_r;
				}
			}
		}

		Mat out = img.clone();
		double tmp_b = 255.0 / (maxval_b - minval_b);
		double tmp_g = 255.0 / (maxval_g - minval_g);
		double tmp_r = 255.0 / (maxval_r - minval_r);
		for( int i = begin; i <= endR; i++ )
		{
			for( int j = begin; j <= endC; j++ )
			{
				// 锐化加强
				Vec3b & pixel = *out.ptr<Vec3b>(i, j);
				int val_b = pixel[0] - int(mask_b[i][j] * tmp_b);
				int val_g = pixel[1] - int(mask_g[i][j] * tmp_g);
				int val_r = pixel[2] - int(mask_r[i][j] * tmp_r);
				if( val_b < 0 ){
					val_b = 0;
				}
				if( val_b > 255 ){
					val_b = 255;
				}

				if( val_g < 0 ){
					val_g = 0;
				}
				if( val_g > 255 ){
					val_g = 255;
				}

				if( val_r < 0 ){
					val_r = 0;
				}
				if( val_r > 255 ){
					val_r = 255;
				}


				// 未缩放(标定)的边界
				// int val = mask[i][j];

				// 缩放(标定, scaling)后的边界
				// int val = int((mask[i][j] - minval) * tmp);

				pixel[0] = val_b;
				pixel[1] = val_g;
				pixel[2] = val_r;
			}
		}
		imshow("Laplacian RGB", out);
		waitKey(0);
	}

	void my_robert_filter_rgb(){
		int size = 2;
		Mat img = imread("lenna.jpg");
		imshow("raw", img);
		int nRows = img.rows;
		int nCols = img.cols;
		int endR = nRows - 1;
		int endC = nCols - 1;

		Mat out = img.clone();
		for( int i = 0; i < endR; i++ )
		{
			for( int j = 0; j < endC; j++ )
			{
				Vec3b & pixel_1 = *out.ptr<Vec3b>(i, j);
				int val_1b = pixel_1[0];
				int val_1g = pixel_1[1];
				int val_1r = pixel_1[2];

				Vec3b & pixel_2 = *out.ptr<Vec3b>(i, j + 1);
				int val_2b = pixel_2[0];
				int val_2g = pixel_2[1];
				int val_2r = pixel_2[2];

				Vec3b & pixel_3 = *out.ptr<Vec3b>(i + 1, j);
				int val_3b = pixel_3[0];
				int val_3g = pixel_3[1];
				int val_3r = pixel_3[2];

				Vec3b & pixel_4 = *out.ptr<Vec3b>(i + 1, j + 1);
				int val_4b = pixel_4[0];
				int val_4g = pixel_4[1];
				int val_4r = pixel_4[2];

				int sum_b = abs(val_4b - val_1b) + abs(val_3b - val_2b);
				int sum_g = abs(val_4g - val_1g) + abs(val_3g - val_2g);
				int sum_r = abs(val_4r - val_1r) + abs(val_3r - val_2r);
				pixel_1[0] = uchar(sum_b);
				pixel_1[1] = uchar(sum_g);
				pixel_1[2] = uchar(sum_r);
			}
		}

		imshow("Robert RGB", out);
		waitKey(0);
	}

	void my_sobel_filter_rgb(){
		int size = 3;
		int offset = size / 2;
		Mat img = imread("lenna.jpg");
		imshow("raw", img);
		int nRows = img.rows;
		int nCols = img.cols;
		int endR = nRows - 1;
		int endC = nCols - 1;
		Mat out = img.clone();
		for( int i = 1; i < endR; i++ )
		{
			for( int j = 1; j < endC; j++ )
			{
				Vec3b & pixel1 = *img.ptr<Vec3b>(i - 1, j - 1);
				int val_1b = pixel1[0];
				int val_1g = pixel1[1];
				int val_1r = pixel1[2];

				Vec3b & pixel2 = *img.ptr<Vec3b>(i - 1, j + 0);
				int val_2b = pixel2[0];
				int val_2g = pixel2[1];
				int val_2r = pixel2[2];

				Vec3b & pixel3 = *img.ptr<Vec3b>(i - 1, j + 1);
				int val_3b = pixel3[0];
				int val_3g = pixel3[1];
				int val_3r = pixel3[2];

				Vec3b & pixel4 = *img.ptr<Vec3b>(i - 0, j - 1);
				int val_4b = pixel4[0];
				int val_4g = pixel4[1];
				int val_4r = pixel4[2];

				Vec3b & pixel6 = *img.ptr<Vec3b>(i - 0, j + 1);
				int val_6b = pixel6[0];
				int val_6g = pixel6[1];
				int val_6r = pixel6[2];

				Vec3b & pixel7 = *img.ptr<Vec3b>(i + 1, j - 1);
				int val_7b = pixel7[0];
				int val_7g = pixel7[1];
				int val_7r = pixel7[2];

				Vec3b & pixel8 = *img.ptr<Vec3b>(i + 1, j + 0);
				int val_8b = pixel8[0];
				int val_8g = pixel8[1];
				int val_8r = pixel8[2];

				Vec3b & pixel9 = *img.ptr<Vec3b>(i + 1, j + 1);
				int val_9b = pixel9[0];
				int val_9g = pixel9[1];
				int val_9r = pixel9[2];

				int sum_b = abs(val_7b + 2 * val_8b + val_9b - val_1b - 2 * val_2b - val_3b) + abs(val_3b + 2 * val_6b + val_9b - val_1b - 2 * val_4b - val_7b);
				if( sum_b > 255 )
				{
					sum_b = 255;
				}
				
				int sum_g = abs(val_7g + 2 * val_8g + val_9g - val_1g - 2 * val_2g - val_3g) + abs(val_3g + 2 * val_6g + val_9g - val_1g - 2 * val_4g - val_7g);
				if( sum_g > 255 )
				{
					sum_g = 255;
				}

				int sum_r = abs(val_7r + 2 * val_8r + val_9r - val_1r - 2 * val_2r - val_3r) + abs(val_3r + 2 * val_6r + val_9r - val_1r - 2 * val_4r - val_7r);
				if( sum_r > 255 )
				{
					sum_r = 255;
				}
				Vec3b & pixel = *out.ptr<Vec3b>(i, j);
				pixel[0] = uchar(sum_b);
				pixel[1] = uchar(sum_g);
				pixel[2] = uchar(sum_r);
			}
		}
		imshow("Sobel RGB", out);
		waitKey(0);
	}
	```

<br>

##	实验中的问题

*	高斯滤波器是一种线性低通滤波器, 能够有效的抑制噪声, 平滑图像, 如果σ较小, 那么模板中心系数较大, 而周围系数较小, 这样平滑效果就不是很明显. 反之, σ较大, 则模板的各个系数相差就不是很大, 类似均值模板, 平滑效果比较明显.

*	拉普拉斯算子定义 ▼2f = ∂2f/∂x2 + ∂2f/∂y2, 二阶微分, 双边缘, 且对噪声很敏感

	```
	∂2f/∂x2 ≈  f(x + 1, y) + f(x - 1, y) - 2f(x, y)
	∂2f/∂y2 =   - 2f(x, y)
	▼2f =   f(x + 1, y)
	       + f(x - 1, y)
	       + f(x, y + 1)
	       + f(x, y - 1)
	       - 4f(x, y)
	```

	需要进行标定 g = [mask - min(mask)] * 255.0 / [max(mask) - min(mask)]

*	robert, 一阶微分(梯度), 灰度变化, 检测边缘

	```
	M(x, y) = | ▼f | = | grad(f) | = √(g2x + g2y)
	M(x, y) ≈ |gx| + |gy|
	        ≈ |z9 - z5| + |z8 - z6|
	robert1 = [ [-1, 0], [0, 1] ]
	robert2 = [ [0, -1], [1, 0] ]
	```

*	sobel, 一阶微分, 灰度变化, 检测边缘

	```
	M(x, y) = | ▼f | = | grad(f) | = √(g2x + g2y)
	M(x, y) ≈ |gx| + |gy|
	        ≈   |(z7 + 2 * z8 + z9) - (z1 + 2 * z2 + z3)|
	          + |(z3 + 2 * z6 + z9) - (z1 + 2 * z4 + z7)|
	Sobel对边缘的响应较宽而且更加强烈，Robert算子对边缘响应较弱，而且对弯曲的边缘敏感度低
	```

*	注, 根据 `DIP` 第 6 章, 对于彩色图像, 直方图均衡、平滑、锐化, 不推荐在 RGB 三通道上分别进行然后综合

	更好的办法是使用 `HSI` 模型, 仅在 `I` (亮度)分量进行, 这样不会影响色调和饱和度

<br>

##	实验结果

*	利用均值模板平滑灰度图像, 3x3

	![1](https://s2.ax1x.com/2019/04/13/ALvY8I.jpg)

*	利用高斯模板平滑灰度图像, 3x3, sigma=0.8

	![2](https://s2.ax1x.com/2019/04/13/ALvWrT.jpg)

*	利用 Laplacian 模板锐化灰度图像, 用 Robert, Sobel 算子检测边缘

	![3](https://s2.ax1x.com/2019/04/13/ALxKWn.jpg)

*	利用高提升滤波算法增强灰度图像

	![4](https://s2.ax1x.com/2019/04/13/ALxfSI.jpg)

*	利用均值模板平滑彩色图像, 3x3

	![5](https://s2.ax1x.com/2019/04/13/ALvaKf.jpg)

*	利用高斯模板平滑彩色图像, 3x3, sigma=0.5

	![6](https://s2.ax1x.com/2019/04/13/ALvHR1.jpg)

*	利用 Laplacian 模板锐化彩色图像, 用 Robert, Sobel 算子检测边缘

	![7](https://s2.ax1x.com/2019/04/13/ALxBy6.jpg)

*	由于整个工程文件夹较大, 因此只打包了原码文件 `main.cpp`, 并且放在了 `./lab03/` 文件夹中, 同时该文件夹下还有实验用到的图片.

<br>

##	参考资料

*	[数字图像处理](https://book.douban.com/subject/4285832/)

*	[图像处理基础(4)：高斯滤波器详解](https://www.cnblogs.com/wangguchangqing/p/6407717.html)

*	[图像处理--高斯滤波](https://blog.csdn.net/L_inYi/article/details/8915116)

*	[拉普拉斯(laplacian)滤波实现图像锐化分析](https://blog.csdn.net/scottly1/article/details/44408343)

*	[【数字图像处理】实现拉普拉斯锐化](https://blog.csdn.net/wl1070325332/article/details/73291694)

*	[图像边缘检测之拉普拉斯(Laplacian)C++实现](https://blog.csdn.net/fengbingchun/article/details/79321200)

<br>