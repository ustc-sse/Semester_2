#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <string>
#include <cmath>
#include <algorithm>

using namespace std;
using namespace cv;

struct MyRect{
	int minCol, minRow;
	int maxCol, maxRow;
	void set(int a, int b, int c, int d){
		minCol = a;
		minRow = b;
		maxCol = c;
		maxRow = d;
	}
};

// 我的算法
void myFindTextRegion(Mat & img, Mat & img_preprocessed){
	int nRows = img_preprocessed.rows, nCols = img_preprocessed.cols;
	int minRow = 0x0fffffff, minCol = 0x0fffffff;
	int maxRow = 0, maxCol = 0;
	vector<MyRect> rect_list;

	int cnt_blank_row = 0;
	bool find_roi = false;
	MyRect rect;
	for( int i = 0; i < nRows; i++ ){
		bool is_blank_row = true;
		for( int j = 0; j < nCols; j++ ){
			if( *img_preprocessed.ptr<uchar>(i, j) > 200 ){
				find_roi = true;
				is_blank_row = false;
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
				rect.set(minCol - 2, minRow - 2, maxCol + 2, maxRow + 2);
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

	Mat out = img.clone();
	for( MyRect rect : rect_list ){
		rectangle(out, Point(rect.minCol, rect.minRow), Point(rect.maxCol, rect.maxRow), Scalar(0, 255, 0), 2);
	}

	imshow("myTextarea", out);
}

// 参考资料的算法
// https://blog.csdn.net/lgh0824/article/details/76100599
void findTextRegion(Mat & img, Mat & img_preprocessed){
	// 1. 查找轮廓
	vector<RotatedRect> rect_list;
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(img_preprocessed, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

	// 2. 筛选那些面积小的
	for( int i = 0; i < contours.size(); i++ ){
		//计算当前轮廓的面积
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

	Mat out = img.clone();
	for( RotatedRect rect : rect_list ){
		Point2f P[4];
		rect.points(P);
		for( int j = 0; j <= 3; j++ ){
			line(out, P[j], P[(j + 1) % 4], Scalar(0, 255, 0), 2);
		}
	}
	imshow("textarea", out);
}

void lab(){
	cout << "综合实验: 文字区域的提取\n";
	cout << "1. 取灰度图\n";
	cout << "2. sobel提取边缘\n";
	cout << "3. 二值化\n";
	cout << "4. 膨胀腐蚀\n";
	cout << "5. 查找轮廓\n";
	cout << "6. 标记轮廓边界\n";
	if( true ){
		/* 1. 得到灰度图 */
		Mat img_gray = imread("text.jpg", 0);
		// imshow("灰度图", img_gray);

		/* 2. sobel 检测轮廓 */
		Mat img_sobel;
		// Sobel(src, dst, ddepth, x_order, y_order)
		Sobel(img_gray, img_sobel, CV_8U, 1, 0);
		// imshow("sobel", img_sobel);

		/* 3. 二值化 */
		Mat img_threshold;
		threshold(img_sobel, img_threshold, 0, 255, THRESH_OTSU + THRESH_BINARY);
		// imshow("threshold", img_threshold);

		/* 4. 腐蚀和膨胀 */
		Mat element_erode = getStructuringElement(MORPH_RECT, Size(30, 9));
		// 控制高度设置可以控制上下行的膨胀程度
		// 例如 3 比 4 的区分能力更强, 但也会造成漏检
		Mat element_dilate = getStructuringElement(MORPH_RECT, Size(24, 4));

		/* 4.1 先膨胀, 让轮廓突出 */
		Mat dilate1;
		dilate(img_threshold, dilate1, element_dilate);
		// imshow("dilate1", dilate1);

		/* 4.2 腐蚀一次, 去掉细节, 表格线等. 这里去掉的是竖直的线 */
		Mat erode1;
		erode(dilate1, erode1, element_erode);
		imshow("erode1", erode1);

		/* 5. 再次膨胀, 让轮廓明显一些 */
		Mat dilate2;
		dilate(erode1, dilate2, element_dilate);
		imshow("dilate2", dilate2);

		Mat img_preprocessed = dilate2;

		// 6. 查找轮廓
		Mat img = imread("text.jpg");
		myFindTextRegion(img, img_preprocessed);
		findTextRegion(img, img_preprocessed);
	}
	waitKey(0);
	destroyAllWindows();
}

int main(){
	ios::sync_with_stdio(false);
	cin.tie(0);

	lab();
	destroyAllWindows();
	return 0;
}
