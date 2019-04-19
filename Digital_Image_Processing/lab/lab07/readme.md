#	高图综合设计实验报告

<br>

学号：

姓名：

<br><br><br><br>

##	实验环境

*	`Windows`

*	`Visual Studio`

*	`OpenCV 2.4.9`

<br>

##	实验题目

*	提取图像中的文本区域

<br>

##	实验内容和原理

1.	对图像进行灰度化

	```cpp
	/* 1. 得到灰度图 */
	Mat img_gray = imread("text.jpg", 0);
	```

2.	使用 Sobel 算子提取边缘

	```cpp
	/* 2. sobel 检测轮廓 */
	Mat img_sobel;
	// Sobel(src, dst, ddepth, x_order, y_order)
	Sobel(img_gray, img_sobel, CV_8U, 1, 0);
	```

3.	对图像进行二值化

	```cpp
	/* 3. 二值化 */
	Mat img_threshold;
	threshold(img_sobel, img_threshold, 0, 255, THRESH_OTSU + THRESH_BINARY);
	```

4.	进行形态学操作, 利用腐蚀和膨胀, 消除小噪声, 填充孔洞, 得到大致的文本区域

	```cpp
	/* 4. 腐蚀和膨胀 */
	// 腐蚀结构元
	Mat element_erode = getStructuringElement(MORPH_RECT, Size(30, 9));
	// 膨胀结构元
	Mat element_dilate = getStructuringElement(MORPH_RECT, Size(24, 4));
	// 控制高度设置可以控制上下行的膨胀程度，例如3比4的区分能力更强,但也会造成漏检

	/* 4.1 先膨胀, 让轮廓突出 */
	Mat dilate1;
	dilate(img_threshold, dilate1, element_dilate);

	/* 4.2 腐蚀一次, 去掉细节, 表格线等. 这里去掉的是竖直的线 */
	Mat erode1;
	erode(dilate1, erode1, element_erode);

	/* 5. 再次膨胀, 让轮廓明显一些 */
	Mat dilate2;
	dilate(erode1, dilate2, element_dilate);
	```

5.	用矩形标记出所有文本区域

	我的算法思路: 逐行扫描

	由于二值化后, 像素灰度值只有 `0` 或 `255` 两种取值. 而且经过形态学操作, 基本没有孤立的 `255` 像素点了. 所以当出现第一个灰度值为 `255` 的像素时, 可以认为找到了文本区域. 记此时的行数为 `minRow`, 同理可以找到 `minCol`. 与此同时, 不断更新当前区域的 `maxRow` 和 `maxCol`.

	以上是某一行中有 255 像素点的情况.

	当扫描完一行都没有出现 `255`, 说明这是一个空行, 记为 `blank`. 这也是不同区域之间分隔的标志. 当出现连续 `8` 个空行, 则可以认为上一块区域已经标记完全. 于是我们把上一个区域的矩形的顶点坐标放到 `rect_list` 里面.

	以上思路的代码如下.

	```cpp
	// 标记出文本区域
	void myFindTextRegion(Mat & img){
		int nRows = img.rows, nCols = img.cols;
		int minRow = 0x0fffffff, minCol = 0x0fffffff;
		int maxRow = 0, maxCol = 0;
		vector<MyRect> rect_list;

		int cnt_blank_row = 0;
		bool find_roi = false;
		MyRect rect;
		for( int i = 0; i < nRows; i++ ){
			// 逐行扫描
			bool is_blank_row = true;
			for( int j = 0; j < nCols; j++ ){
				if( *img.ptr<uchar>(i, j) > 200 ){
					find_roi = true;
					is_blank_row = false;
					// 更新 minRow, minCol, maxRow, maxCol
					if( i < minRow ){
						minRow = i;
					}
					if( i > maxRow ){
						maxRow = i;
					}
					if( j < minCol ){
						minCol = j;
					}
					if( j > maxCol ){
						maxCol = j;
					}
				}
			}
			if( is_blank_row == true ){
				if( find_roi ){
					cnt_blank_row++;
					rect.set(minCol, minRow, maxCol, maxRow);
				}
			}
			else{
				if( cnt_blank_row > 8 ){
					rect_list.push_back(rect);
					cnt_blank_row = 0;
					swap(minRow, maxRow);
					swap(minCol, maxCol);
				}
			}
		}
		if( cnt_blank_row > 8 ){
			rect_list.push_back(rect);
		}

		// 画出矩形, 即标记出文本区域
		for( MyRect rect : rect_list ){
			rectangle(img, Point(rect.minCol, rect.minRow),
				Point(rect.maxCol, rect.maxRow), Scalar(150), 1);
		}

		imshow("area", img);
	}
	```

	这是标记文本区域的最基本思路, 但是得到的矩形区域可能不够精细.

	比如一个横向区域中可能存在多个列向区域, 我的算法就只能找到整体的横向区域.

	此外, 如果文本区域是倾斜的, 那么本算法只能找到沿坐标行列方向的最小外接矩形, 但是不能得到沿文本区域方向的最小外接矩形.

	要做到这些, 需要更复杂的算法, 当然我们也可以直接调用 `opencv` 已经集成好的函数帮我们找到这些不规律的区域, 代码如下

	```cpp
	void findTextRegion(Mat & img, vector<RotatedRect> & rect_list){
		// 1. 查找轮廓
		vector<vector<Point>> contours;
		vector<Vec4i> hierarchy;
		findContours( img, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

		// 2. 筛选那些面积小的
		for( int i = 0; i < contours.size(); i++ ){
			// 计算当前轮廓的面积
			double area = contourArea(contours[i]);

			//面积小于1000的全部筛选掉
			if (area < 1000){
				continue;
			}

			// 找到最小矩形，该矩形可能有方向
			RotatedRect rect = minAreaRect(contours[i]);

			// 计算高和宽
			int m_width = rect.boundingRect().width;
			int m_height = rect.boundingRect().height;

			//筛选那些太细的矩形，留下扁的
			if( m_height > m_width * 1.2 ){
				continue;
			}

			//符合条件的rect添加到rects集合中
			rect_list.push_back(rect);
		}
	}
	```

<br>

##	实验结果

*	01_灰度图

	![01](https://s2.ax1x.com/2019/04/20/EP8wUH.jpg)

*	02_Sobel边缘检测

	![02](https://s2.ax1x.com/2019/04/20/EP8r8I.jpg)

*	03_二值化

	![03](https://s2.ax1x.com/2019/04/20/EP8s2t.jpg)

*	04_第1次膨胀

	![04](https://s2.ax1x.com/2019/04/20/EP8cKf.jpg)

*	05_腐蚀

	![05](https://s2.ax1x.com/2019/04/20/EP82qS.jpg)

*	06_第2次膨胀

	![06](https://s2.ax1x.com/2019/04/20/EP8faQ.jpg)

*	07_01_参考资料算法标记的区域

	![07_01](https://s2.ax1x.com/2019/04/20/EPGmRI.jpg)

*	07_02_我的算法标记的区域

	![07_02](https://s2.ax1x.com/2019/04/20/EPGnzt.jpg)

*	08_01_参考资料的结果

	![08_01](https://s2.ax1x.com/2019/04/20/EPGMsf.jpg)

*	08_02_我的算法的结果

	![08_02](https://s2.ax1x.com/2019/04/20/EPGQL8.jpg)

<br>

##	参考资料

*	[数字图像处理](https://book.douban.com/subject/4285832/)

*	[Opencv实战(二) 文字区域的提取](https://blog.csdn.net/lgh0824/article/details/76100599)

*	[Sobel Derivatives](https://docs.opencv.org/2.4.13.7/doc/tutorials/imgproc/imgtrans/sobel_derivatives/sobel_derivatives.html)

*	[opencv docs, Eroding and Dilating](https://docs.opencv.org/2.4.13.7/doc/tutorials/imgproc/erosion_dilatation/erosion_dilatation.html#morphology-1)

*	[opencv docs, Basic Drawing](https://docs.opencv.org/2.4.13.7/doc/tutorials/core/basic_geometric_drawing/basic_geometric_drawing.html#drawing-1)

*	[opencv docs, Finding contours in your image](https://docs.opencv.org/2.4.13.7/doc/tutorials/imgproc/shapedescriptors/find_contours/find_contours.html)

*	[opencv docs, RotatedRect](https://docs.opencv.org/ref/2.4/db/dd6/classcv_1_1RotatedRect.html)
