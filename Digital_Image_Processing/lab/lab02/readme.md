#	高图 lab02 实验报告

<br>

学号：

姓名：

<br><br><br><br>

##	实验环境

*	`Windows + Visual Studio`

*	`OpenCV 2.4.9`

<br>

##	实验目的

*	掌握对图像直方图进行操作，实现图像的直方图均衡算法。

*	掌握求灰度图像归一化直方图方法, 直方图均衡算法

<br>

##	实验内容

1.	计算灰度图像的归一化直方图

2.	灰度图像直方图均衡处理

3.	彩色图像直方图均衡处理

<br>

##	实验完成情况

*	所有实验全部完成, 具体说明如下

*	实验 1, 计算灰度图像的归一化直方图, 核心代码如下

	```cpp
	void show_gray_histogram(){
		string filename = "lenna.jpg";
		Mat img = imread(filename, 0);
		imshow("lenna", img);

		int nRows = img.rows;	// 250
		int nCols = img.cols;	// 250
		// 250 * 250 < 2^16, 也就是说, 就算每个点的灰度都是 gval
		// 那么最大高度是 250*250, 不会超过 unsigned short
		// 灰度最大值 255, 因此 x 轴最大长度 255
		vector<ushort> pdf(256, 0);
		// ushort max_height = 0xffff;
		Mat_<uchar>::iterator it = img.begin<uchar>();
		Mat_<uchar>::iterator end = img.end<uchar>();
		while( it != end ){
			uchar & now = (*it);
			uchar gval = now;
			pdf[gval]++;
			it++;
		}

		Mat histogram;// = Mat(270, 270, 0, Scalar(255));
		make_coordinate(histogram);
		for( ushort gval = 0; gval < 255; gval++ ){
			// 归一化是 height / 255, 但是太小了，看不清, 乘上一个系数10, 放大图片
			ushort height = pdf[gval]*10/255 ;
			MyLine(histogram, Point(gval, 270), Point(gval, 270 - height));
		}
		imshow("histogram", histogram);
		waitKey(0);
		destroyAllWindows();
	}
	```

*	实验 2, 灰度图像直方图均衡处理, 核心代码如下

	```cpp
	void gray_histogram_equalization(){
		string filename = "lenna.jpg";
		Mat img = imread(filename, 0);

		imshow("old", img);
		show_gray_histogram(img, true);
		int nRows = img.rows;	// 250
		int nCols = img.cols;	// 250
		vector<ushort> pdf_r(256, 0);
		Mat_<uchar>::iterator it = img.begin<uchar>();
		Mat_<uchar>::iterator end = img.end<uchar>();
		while( it != end ){
			uchar & now = (*it);
			uchar gval = now;
			pdf_r[gval]++;
			it++;
		}

		// sk = [(L-1)/(M*N)] * Σ(nj), j: 0->k
		// 从 r 到 s 的映射
		vector<ushort> r_to_s(256, 0);
		r_to_s[0] = pdf_r[0];
		ushort tmp = nRows * nCols / 255;
		for( ushort r = 1; r < 256; r++ ){
			r_to_s[r] = pdf_r[r] + r_to_s[r - 1];
			r_to_s[r - 1] /= tmp;
		}
		r_to_s[255] /= tmp;

		it = img.begin<uchar>();
		end = img.end<uchar>();
		while( it != end ){
			uchar & now = (*it);
			uchar gval = now;
			now = uchar(r_to_s[gval]);
			it++;
		}

		imshow("new", img);
		show_gray_histogram(img, false);
		destroyAllWindows();
	}
	```

*	实验 3, 彩色图像直方图均衡处理, 原理同实验 2, 核心代码如下

	```cpp
	void show_rgb_histogram(Mat & img, bool old = true){
		// 250 * 250 < 2^16, 也就是说, 就算每个点的灰度都是 gval
		// 那么最大高度是 250*250, 不会超过 unsigned short
		// 灰度最大值 255, 因此 x 轴最大长度 255
		vector<ushort> pdf_b(256, 0);
		vector<ushort> pdf_g(256, 0);
		vector<ushort> pdf_r(256, 0);
		// ushort max_height = 0xffff;
		Mat_<Vec3b>::iterator it = img.begin<Vec3b>();
		Mat_<Vec3b>::iterator end = img.end<Vec3b>();
		// 统计频度
		while( it != end ){
			Vec3b & now = (*it);
			uchar b = now[0];
			uchar g = now[1];
			uchar r = now[2];
			pdf_b[b]++;
			pdf_g[g]++;
			pdf_r[r]++;
			it++;
		}

		// 背景图
		Mat histogram = Mat(270, 1000, 0, Scalar(255));
		
		line(histogram, Point(50, 270), Point(50, 260), Scalar(0));
		line(histogram, Point(100, 270), Point(100, 260), Scalar(0));
		line(histogram, Point(150, 270), Point(150, 260), Scalar(0));
		line(histogram, Point(200, 270), Point(200, 260), Scalar(0));
		line(histogram, Point(250, 270), Point(250, 260), Scalar(0));
		
		line(histogram, Point(300, 270), Point(300, 260), Scalar(0));
		line(histogram, Point(350, 270), Point(350, 260), Scalar(0));
		line(histogram, Point(400, 270), Point(400, 260), Scalar(0));
		line(histogram, Point(450, 270), Point(450, 260), Scalar(0));
		line(histogram, Point(500, 270), Point(500, 260), Scalar(0));
		line(histogram, Point(550, 270), Point(550, 260), Scalar(0));
		
		line(histogram, Point(600, 270), Point(600, 260), Scalar(0));
		line(histogram, Point(650, 270), Point(650, 260), Scalar(0));
		line(histogram, Point(700, 270), Point(700, 260), Scalar(0));
		line(histogram, Point(750, 270), Point(750, 260), Scalar(0));
		line(histogram, Point(800, 270), Point(800, 260), Scalar(0));
		line(histogram, Point(850, 270), Point(850, 260), Scalar(0));

		for( ushort gval = 0; gval < 255; gval++ ){
			// 归一化是 height / 255, 但是太小了，看不清, 乘上一个系数10, 放大图片
			ushort height_b = pdf_b[gval]*10/255 ;
			ushort height_g = pdf_g[gval]*10/255 ;
			ushort height_r = pdf_r[gval]*10/255 ;
			MyLine(histogram, Point(gval, 270), Point(gval, 270 - height_b));
			MyLine(histogram, Point(gval + 300, 270), Point(gval + 300, 270 - height_g));
			MyLine(histogram, Point(gval + 600, 270), Point(gval + 600, 270 - height_r));
		}

		if( old == true ){
			imshow("histogram", histogram);
		}
		else{
			imshow("histogram_new", histogram);
		}
		
		waitKey(0);
		destroyAllWindows();
	}

	void rgb_histogram_equalization(){
		string filename = "lenna.jpg";
		Mat img = imread(filename);
		imshow("old", img);
		show_rgb_histogram(img);
		int nRows = img.rows;	// 160
		int nCols = img.cols;	// 250
		vector<ushort> pdf_br(256, 0);
		vector<ushort> pdf_gr(256, 0);
		vector<ushort> pdf_rr(256, 0);

		Mat_<Vec3b>::iterator it = img.begin<Vec3b>();
		Mat_<Vec3b>::iterator end = img.end<Vec3b>();
		while( it != end ){
			Vec3b & now = (* it);
			uchar gval_b = now[0];
			uchar gval_g = now[1];
			uchar gval_r = now[2];
			pdf_br[gval_b]++;
			pdf_gr[gval_g]++;
			pdf_rr[gval_r]++;
			it++;
		}
		vector<ushort> br_to_bs(256, 0);
		vector<ushort> gr_to_gs(256, 0);
		vector<ushort> rr_to_rs(256, 0);
		br_to_bs[0] = pdf_br[0];
		gr_to_gs[0] = pdf_gr[0];
		rr_to_rs[0] = pdf_rr[0];
		ushort tmp = nRows * nCols / 255;
		for( ushort r = 1; r < 256; r++ )
		{
			br_to_bs[r] = pdf_br[r] + br_to_bs[r - 1];
			gr_to_gs[r] = pdf_gr[r] + gr_to_gs[r - 1];
			rr_to_rs[r] = pdf_rr[r] + rr_to_rs[r - 1];
			br_to_bs[r - 1] /= tmp;
			gr_to_gs[r - 1] /= tmp;
			rr_to_rs[r - 1] /= tmp;
		}
		br_to_bs[255] /= tmp;
		gr_to_gs[255] /= tmp;
		rr_to_rs[255] /= tmp;

		it = img.begin<Vec3b>();
		end = img.end<Vec3b>();
		while( it != end )
		{
			Vec3b & now = (* it);
			uchar gval_b = now[0];
			now[0] = uchar(br_to_bs[gval_b]);
			uchar gval_g = now[1];
			now[1] = uchar(gr_to_gs[gval_g]);
			uchar gval_r = now[2];
			now[2] = uchar(rr_to_rs[gval_r]);
			it++;
		}
		imshow("new", img);
		show_rgb_histogram(img, false);
		destroyAllWindows();
	}
	```

<br>

##	实验中的问题

*	首先需要自己画出直方图的坐标系, 我选用的是 270 x 270 的大小, 由于灰度最大值是 255, 所以边上会留有空白, 方便观察. 画坐标系用到的函数是 make_coordinate(), MyLine()

*	灰度最大值 255, 因此直方图 x 轴最大长度 255. y 轴最大高度是 nRows x nCols

*	直方图均衡公式

	```
	sk = [(L-1)/(M*N)] * Σ(nj), j: 0->k
	从 r 到 s 的映射
	```

*	直方图均衡可能会产生噪声(失真), 因为, 假设原来是 100~200 的范围, 现在被扩展成了 0 ~ 255, 会出现类似椒盐的噪声

<br>

##	实验结果

*	灰度图像的归一化直方图

	![1](https://s2.ax1x.com/2019/04/13/ALqkse.png)

*	灰度图像直方图均衡处理

	![2](https://s2.ax1x.com/2019/04/13/ALqnit.png)

*	彩色图像直方图均衡处理

	![3](https://s2.ax1x.com/2019/04/13/ALqKRf.jpg)

	![4](https://s2.ax1x.com/2019/04/13/ALq1sg.jpg)

*	由于整个工程文件夹较大, 因此只打包了原码文件 `main.cpp`, 并且放在了 `./lab1/` 文件夹中, 同时该文件夹下还有实验用到的图片.

<br>

##	参考资料

*	[数字图像处理](https://book.douban.com/subject/4285832/)

<br>