//#include <opencv2/opencv.hpp>
//#include <iostream>
//#include <vector>
//#include <algorithm>
//
//using namespace cv;
//using namespace std;
//
//struct LightBar {
//    RotatedRect rect;
//    Point2f center;
//    float width;
//    float height;
//    float angle;
//};
//
//int main() {
//    VideoCapture cap("D:/images/armor_red.mp4"); // 替换为你的视频路径
//    if (!cap.isOpened()) {
//        cout << "无法打开视频！" << endl;
//        return -1;
//    }
//
//    // ---------- 相机标定参数（硬编码） ----------
//    // 内参矩阵（基于标定结果，原始分辨率 1280×1024）
//    Mat cameraMatrix = (Mat_<double>(3, 3) <<
//        2335.8692, 0., 719.25127,
//        0., 2333.77331, 544.18238,
//        0., 0., 1.);
//
//    // 畸变系数 (k1, k2, p1, p2, k3)
//    Mat distCoeffs = (Mat_<double>(1, 5) <<
//        -0.105848, 0.096276, -0.001701, 0.000563, 0.000000);
//
//    // 装甲板实际物理坐标（小装甲板，单位 mm，顺序：左上、右上、右下、左下）
//    vector<Point3f> armorPoints3D = {
//        Point3f(-67.5,  27.5, 0),
//        Point3f(67.5,  27.5, 0),
//        Point3f(67.5, -27.5, 0),
//        Point3f(-67.5, -27.5, 0)
//    };
//    // -----------------------------------------
//
//    // HSV红色阈值
//    Scalar lower_red1(0, 60, 60);
//    Scalar upper_red1(10, 255, 255);
//    Scalar lower_red2(160, 60, 60);
//    Scalar upper_red2(180, 255, 255);
//
//    Mat kernel = getStructuringElement(MORPH_ELLIPSE, Size(5, 5));
//
//    namedWindow("Mask", WINDOW_NORMAL);
//    resizeWindow("Mask", 640, 360);
//
//    double totalTime = 0;
//    int frameCount = 0;
//    double avgFps = 0;
//    TickMeter tm;
//
//    Mat frame;
//    while (cap.read(frame)) {
//        // 根据当前帧尺寸缩放内参（若与标定尺寸不同）
//        const double CALIB_WIDTH = 1280.0;
//        const double CALIB_HEIGHT = 1024.0;
//        double scaleX = frame.cols / CALIB_WIDTH;
//        double scaleY = frame.rows / CALIB_HEIGHT;
//        Mat camMat = cameraMatrix.clone(); // 每帧使用缩放后的内参，避免修改原始矩阵
//        if (fabs(scaleX - 1.0) > 1e-6 || fabs(scaleY - 1.0) > 1e-6) {
//            camMat.at<double>(0, 0) *= scaleX;
//            camMat.at<double>(1, 1) *= scaleY;
//            camMat.at<double>(0, 2) *= scaleX;
//            camMat.at<double>(1, 2) *= scaleY;
//        }
//
//        tm.reset(); tm.start();
//
//        // 1. 颜色提取
//        Mat hsv, mask, mask1, mask2;
//        cvtColor(frame, hsv, COLOR_BGR2HSV);
//        inRange(hsv, lower_red1, upper_red1, mask1);
//        inRange(hsv, lower_red2, upper_red2, mask2);
//        mask = mask1 | mask2;
//
//        // 2. 形态学处理
//        morphologyEx(mask, mask, MORPH_OPEN, kernel);
//        morphologyEx(mask, mask, MORPH_CLOSE, kernel);
//
//        imshow("Mask", mask);
//
//        // 3. 查找轮廓
//        vector<vector<Point>> contours;
//        findContours(mask, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
//
//        // 4. 灯条筛选
//        vector<LightBar> lightBars;
//        for (auto& contour : contours) {
//            float area = contourArea(contour);
//            if (area < 30) continue;
//
//            RotatedRect rect = minAreaRect(contour);
//            float width = min(rect.size.width, rect.size.height);
//            float height = max(rect.size.width, rect.size.height);
//            float aspect = height / width;
//            if (aspect < 1.5) continue;
//
//            float rectArea = rect.size.width * rect.size.height;
//            float extent = area / rectArea;
//            if (extent < 0.4) continue;
//
//            float angle = rect.angle;
//            if (rect.size.width < rect.size.height) angle += 90.0f;
//            angle = fabs(angle);
//            if (angle > 90.0f) angle = 180.0f - angle;
//
//            LightBar bar;
//            bar.rect = rect;
//            bar.center = rect.center;
//            bar.width = width;
//            bar.height = height;
//            bar.angle = angle;
//            lightBars.push_back(bar);
//        }
//
//        Mat display = frame.clone();
//
//        // 绘制候选灯条（蓝色）
//        for (auto& bar : lightBars) {
//            Point2f pts[4];
//            bar.rect.points(pts);
//            for (int i = 0; i < 4; i++)
//                line(display, pts[i], pts[(i + 1) % 4], Scalar(255, 0, 0), 2);
//        }
//
//        // 5. 装甲板配对
//        if (lightBars.size() >= 2) {
//            sort(lightBars.begin(), lightBars.end(),
//                [](const LightBar& a, const LightBar& b) { return a.center.x < b.center.x; });
//            LightBar left = lightBars[0];
//            LightBar right = lightBars[1];
//
//            float heightRatio = min(left.height, right.height) / max(left.height, right.height);
//            Point2f diff = right.center - left.center;
//            float lineAngle = abs(atan2(diff.y, diff.x) * 180 / CV_PI);
//
//            if (heightRatio > 0.3 && lineAngle < 30) {
//                // 计算装甲板角点（基于灯条中心线和平均高度）
//                float avgHeight = (left.height + right.height) / 2.0f;
//                Point2f leftCenter = left.center;
//                Point2f rightCenter = right.center;
//
//                vector<Point2f> armorCorners = {
//                    Point2f(leftCenter.x, leftCenter.y - avgHeight / 2), // 左上
//                    Point2f(rightCenter.x, rightCenter.y - avgHeight / 2), // 右上
//                    Point2f(rightCenter.x, rightCenter.y + avgHeight / 2), // 右下
//                    Point2f(leftCenter.x, leftCenter.y + avgHeight / 2)   // 左下
//                };
//
//                // ---------- PnP 距离解算 ----------
//                Mat rvec, tvec;
//                solvePnP(armorPoints3D, armorCorners, camMat, distCoeffs, rvec, tvec);
//                double distance = tvec.at<double>(2); // Z 轴距离（mm）
//
//                // 在图像上绘制装甲板（绿色）和中心点（红色）
//                for (int i = 0; i < 4; i++)
//                    line(display, armorCorners[i], armorCorners[(i + 1) % 4], Scalar(0, 255, 0), 2);
//                Point2f center = (left.center + right.center) / 2.0f;
//                circle(display, center, 5, Scalar(0, 0, 255), -1);
//
//                // 显示距离（绿色文字）
//                string distText = format("Dist: %.0f mm", distance);
//                putText(display, distText, center + Point2f(-40, -20),
//                    FONT_HERSHEY_SIMPLEX, 0.6, Scalar(0, 255, 0), 2);
//
//                // 控制台输出
//                cout << "距离: " << distance << " mm" << endl;
//            }
//        }
//
//        // 计时和 FPS
//        tm.stop();
//        double timeMs = tm.getTimeMilli();
//        totalTime += timeMs;
//        frameCount++;
//        avgFps = frameCount / (totalTime / 1000.0);
//        string fpsText = format("FPS: %.1f (Frame: %.1f ms)", avgFps, timeMs);
//        putText(display, fpsText, Point(10, 30), FONT_HERSHEY_SIMPLEX, 0.7, Scalar(0, 255, 255), 2);
//
//        imshow("Result", display);
//        if (waitKey(30) == 27) break;
//    }
//
//    cap.release();
//    destroyAllWindows();
//    return 0;
//}