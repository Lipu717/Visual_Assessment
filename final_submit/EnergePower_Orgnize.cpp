#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <cmath>

using namespace cv;
using namespace std;

// 【复用函数】漫水填充处理
Mat floodFillProcess(const Mat& binary_mask) {
    if (binary_mask.empty()) return Mat();
    Mat flood_mask = binary_mask.clone();
    // 四角填充
    floodFill(flood_mask, Point(0, 0), Scalar(255));
    floodFill(flood_mask, Point(flood_mask.cols - 1, 0), Scalar(255));
    floodFill(flood_mask, Point(0, flood_mask.rows - 1), Scalar(255));
    floodFill(flood_mask, Point(flood_mask.cols - 1, flood_mask.rows - 1), Scalar(255));
    bitwise_not(flood_mask, flood_mask);
    return flood_mask;
}

// 计算圆形度
double circularity(const vector<Point>& contour) {
    double area = contourArea(contour);
    double perimeter = arcLength(contour, true);
    if (perimeter == 0) return 0;
    return 4 * CV_PI * area / (perimeter * perimeter);
}

int main() {
    VideoCapture cap("D:/images/2_red.mp4");
    if (!cap.isOpened()) {
        cout << "无法打开视频！" << endl;
        return -1;
    }

    // 窗口创建
    namedWindow("原图", WINDOW_NORMAL);
    namedWindow("漫水填充结果", WINDOW_NORMAL);
    namedWindow("检测结果", WINDOW_NORMAL);
    resizeWindow("原图", 640, 360);
    resizeWindow("漫水填充结果", 640, 360);
    resizeWindow("检测结果", 960, 540);

    // HSV阈值
    Scalar lower_red1(0, 60, 60);
    Scalar upper_red1(10, 255, 255);
    Scalar lower_red2(160, 60, 60);
    Scalar upper_red2(180, 255, 255);

    Mat frame;
    Rect towerROI;
    int frame_count = 0; // 帧计数器（优化核心）

    while (true) {
        if (towerROI.width == 0) {
            int w = frame.cols * 0.25;   // 右侧 25%
            int h = frame.rows * 0.35;   // 下方 35%

            towerROI = Rect(
                frame.cols - w,
                frame.rows - h,
                w,
                h
            );
        }
        cap >> frame;
        if (frame.empty()) break;

        // ===================== 优化1：隔帧处理（每2帧处理1次，速度翻倍）=====================
        frame_count++;
        if (frame_count % 2 != 0) {
            imshow("原图", frame);
            if (waitKey(1) == 'q') break; // 仅用1ms延时，极速播放
            continue;
        }

        // 颜色提取
        Mat hsv, mask, mask1, mask2;
        cvtColor(frame, hsv, COLOR_BGR2HSV);
        inRange(hsv, lower_red1, upper_red1, mask1);
        inRange(hsv, lower_red2, upper_red2, mask2);
        mask = mask1 | mask2;

        //屏蔽右下角哨塔
        rectangle(mask, towerROI, Scalar(0), FILLED);

        // 漫水填充
        Mat flood_result = floodFillProcess(mask);
        imshow("漫水填充结果", flood_result);

        // 查找轮廓
        vector<vector<Point>> contours;
        findContours(flood_result, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

        Mat result = frame.clone();
        vector<Point2f> centers;

        // 绘制矩形
        for (const auto& contour : contours) {
            RotatedRect rect = minAreaRect(contour);
            Point2f pts[4];
            rect.points(pts);
            for (int i = 0; i < 4; i++)
                line(result, pts[i], pts[(i + 1) % 4], Scalar(255, 0, 0), 2);
            centers.push_back(rect.center);
        }

        // 识别圆心
        Point2f circleCenter(-1, -1);
        float circleRadius = 0;
        double maxCircularity = 0.5;

        for (const auto& contour : contours) {
            double circ = circularity(contour);
            double area = contourArea(contour);
            if (circ > maxCircularity && area > 50 && area < 2000) {
                maxCircularity = circ;
                minEnclosingCircle(contour, circleCenter, circleRadius);
            }
        }

        // 备用圆心
        if (circleCenter.x < 0 && centers.size() >= 2) {
            Point2f sum(0, 0);
            for (const auto& c : centers) sum += c;
            circleCenter = sum / (float)centers.size();
        }

        // 绘制结果
        if (circleCenter.x > 0) {
            circle(result, circleCenter, 5, Scalar(0, 255, 0), -1);
            string text = format("Center: (%.1f, %.1f)", circleCenter.x, circleCenter.y);
            putText(result, text, Point(10, 30), FONT_HERSHEY_SIMPLEX, 0.6, Scalar(0, 255, 0), 2);
        }
        else {
            putText(result, "No center found", Point(10, 30), FONT_HERSHEY_SIMPLEX, 0.6, Scalar(0, 0, 255), 2);
        }

        // 显示
        imshow("原图", frame);
        imshow("检测结果", result);

        // ===================== 优化2：缩短延时 =====================
        if (waitKey(1) == 'q') break;
    }

    cap.release();
    destroyAllWindows();
    return 0;
}