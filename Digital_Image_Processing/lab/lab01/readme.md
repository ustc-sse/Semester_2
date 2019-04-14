#	高图 lab01 实验报告

<br>

学号：

姓名：

<br><br><br><br>

##	实验环境

*	`Windows + Visual Studio`

*	`OpenCV 2.4.9`

*	`Snipaste` (截图工具)

<br>

##	实验目的

*	掌握 OpenCV 的安装

*	掌握 Opencv 进行图像处理的基础，实现图像的灰度变换处理

<br>

##	实验内容

1.	利用 `OpenCV` 读取图像

	具体内容：用打开 `OpenCV` 打开图像，并在窗口中显示

2.	灰度图像二值化处理

	具体内容：设置并调整阈值对图像进行二值化处理。

3.	灰度图像的对数变换, `s = c·lg(1 + r)`

	具体内容：设置并调整 `r` 值对图像进行对数变换。

4.	灰度图像的伽马变换, `s = c·r^gamma`

	具体内容：设置并调整 `γ` 值对图像进行伽马变换。

5.	彩色图像的补色变换

<br>

##	实验完成情况

*	所有实验全部完成, 具体说明如下

*	实验 1, 利用 OpenCV 读取图像, 核心代码如下

	```cpp
	// 设置文件名
	string filename = "lenna.jpg";
	// 读取文件
	Mat img = imread(filename);
	// 显示文件
	imshow("原图", img);
	// 等待按键
	waitKey(0);
	destroyAllWindows();
	}
	```

*	实验 2, 灰度图像二值化处理, 核心代码如下

	```cpp
	// 设置灰度阈值
	uchar threshold = 100;

	Mat img = imread("lenna.jpg", 0);
	imshow("原图", img);

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
	waitKey(0);
	destroyAllWindows();
	```

*	实验 3, 对数变换, 核心代码如下

	```cpp
	double c = 5;

	Mat img = imread("lenna.jpg", 0);
	imshow("原图", img);

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

	waitKey(0);
	destroyAllWindows();
	```

*	实验 4, 幂变换, 核心代码如下:

	```cpp
	double gamma = 5;
	Mat img = imread("lenna.jpg", 0);
	imshow("原图", img);

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

	waitKey(0);
	destroyAllWindows();
	```

*	实验 5, 彩色图像的补色变换, 核心代码如下:

	```cpp
	Mat img = imread("lenna.jpg");
	imshow("原图", img);

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

	waitKey(0);
	destroyAllWindows();
	```

<br>

##	实验中的问题

*	疑问 1, 使用 opencv 单通道打开得到的灰度图, 保存后得到的文件的大小, 比手动实现的灰度化得到的图片文件要小

*	疑问 2, 二值化后得到的图片, 虽然尺寸没变, 但是文件体积比灰度图要小, 猜测是 jpg 对图像进行了压缩

*	最初实现的对数变换和幂变换, 不能达到书上的效果, 原因是没有进行归一化处理

*	对数变换中, c 越大, 图像越亮

*	幂变换(gamma 变换)中, gamma > 1 时 拉伸暗的, 原图太亮的时候用, 变暗; gamma < 1 时 拉伸亮的, 原图太暗的时候用, 变亮

*	(为什么要使用 gamma 变换) 镜头、显示器等物理器件的失真曲线, 近似 gamma 函数(的逆函数), 因此用 gamma 曲线来校正

*	最初实现的补色变换, 想当然地理解为了 s = 255 - r, 这其实是反色变换, 经过老师的指正以及查阅网上资料, 彩色图像补色规则如下

	```
	minval = min(r, g, b)
	maxval = max(r, g, b)
	sum = minval + maxval
	r = sum - r
	g = sum - g
	b = sum - b
	```

<br>

##	实验结果

*	由于整个工程文件夹较大, 因此只打包了原码文件 `main.cpp`, 并且放在了 `./lab1/` 文件夹中, 同时该文件夹下还有实验用到的图片.

<br>

##	参考资料

*	[数字图像处理](https://book.douban.com/subject/4285832/)

*	[Mat 结构](https://docs.opencv.org/3.4/d3/d63/classcv_1_1Mat.html)

*	[Mat - 基本图像容器](http://www.opencv.org.cn/opencvdoc/2.3.2/html/doc/tutorials/core/mat%20-%20the%20basic%20image%20container/mat%20-%20the%20basic%20image%20container.html)

*	[OpenCV成长之路(2)：图像的遍历](https://www.cnblogs.com/ronny/p/opencv_road_2.html)

*	[彩色图像--彩色变换 补色处理](https://blog.csdn.net/TonyShengTan/article/details/44305801)

<br>
