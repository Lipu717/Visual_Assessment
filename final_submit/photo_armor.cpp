//#include <iostream>
//#include <opencv2/opencv.hpp>
//#include<vector>
//
//using namespace std;
//using namespace cv;
//
//struct LightBar 
//{
//	RotatedRect rect;   // 最小外接旋转矩形
//	Point2f center;     // 中心点
//	float width;        // 短边长度
//	float height;       // 长边长度
//	float angle;        // 长边与水平夹角（绝对值，0~90°）
//};
//
//struct Armor {
//    LightBar left;
//    LightBar right;
//    vector<Point2f> corners; // 四个角点（顺序：左上、右上、右下、左下）
//    Point2f center;
//};
//
//int main()
//{
//	// 读取图片
//	Mat original = imread("D:/images/armor_image.jpg");
//	if (original.empty())
//	{
//		cout << "未读取到图片" << endl;
//		return -1;
//	}
//	imshow("Original", original);
//
//    /*
// 	//图像滤波 
//    Mat blur_img;
//	GaussianBlur(original, blur_img, Size(5, 5), 0);
//    imshow("滤波", blur_img);
//    */
//
//	// HSV颜色空间
//    Mat hsv_img,mask;
//	cvtColor(original, hsv_img, COLOR_BGR2HSV);
//	Scalar lower(0,50,196);
//	Scalar upper(179, 255, 255);
//    //阈值化
//    inRange(hsv_img, lower, upper, mask);
//	imshow("HSV", mask);
//
//	//填充掩膜的白洞（形态学）
//	Mat kernel = getStructuringElement(MORPH_RECT, Size(5, 10));
//	morphologyEx(mask, mask, MORPH_CLOSE, kernel);
//	imshow("闭运算后", mask);
//
//	////canny边缘检测
//	//Mat gray, edges;
//	//cvtColor(original, gray, COLOR_BGR2GRAY);          // 转为灰度
//	//GaussianBlur(gray, gray, Size(5, 5), 0);           // 高斯滤波去噪
//	//Canny(gray, edges, 121, 212);                       // Canny边缘检测
//	//imshow("Edges", edges);
//
//	//查找轮廓
//    vector<vector<Point>> contours;
//    findContours(mask, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
//	//轮廓图
//	Mat contourImg = original.clone();  // original 是你的原图
//	drawContours(contourImg, contours, -1, Scalar(0, 255, 0), 2);  // -1 表示绘制所有轮廓，绿色线条，线宽2
//	imshow("Contours", contourImg);
//	
//    // 筛选灯条
//    vector<LightBar> lightBars;
//    for (const auto& contour : contours) {
//        float area = contourArea(contour);
//        if (area < 100 || area > 1200) continue;  // 面积阈值
//
//        RotatedRect rect = minAreaRect(contour);
//
//        // 位置筛选：只保留下半部分（y > 图像高度/2 或根据实际调整）
//        if (rect.center.y < 800) continue;  // 这里800是示例，请根据图像实际高度修改
//
//        float width = min(rect.size.width, rect.size.height);
//        float height = max(rect.size.width, rect.size.height);
//        float aspect = height / width;
//        if (aspect < 3.5 || aspect > 15.0) continue;  // 长宽比阈值
//
//        float angle = rect.angle;
//        if (rect.size.width < rect.size.height) angle += 90.0f;
//        angle = fabs(angle);
//        // 归一化到0~90度
//        if (angle > 90.0f) angle = 180.0f - angle;
//        if (angle < 70.0f || angle > 90.0f) continue;  // 角度阈值
//
//        //保存灯条
//        LightBar bar;
//        bar.rect = rect;
//        bar.center = rect.center;
//        bar.width = width;
//        bar.height = height;
//        bar.angle = angle;
//        lightBars.push_back(bar);
//    }
//
//    // 绘制筛选后的灯条（绿色）
//    Mat result = original.clone();
//    for (const auto& bar : lightBars) {
//        Point2f pts[4];
//        bar.rect.points(pts);
//        for (int i = 0; i < 4; i++)
//            line(result, pts[i], pts[(i + 1) % 4], Scalar(0, 255, 0), 2);
//    }
//    
//    // 筛选装甲板
//    vector<Armor> armors;
//    for (size_t i = 0; i < lightBars.size(); i++) {
//        for (size_t j = i + 1; j < lightBars.size(); j++) {
//            // 按x坐标区分左右
//            LightBar left, right;
//            if (lightBars[i].center.x < lightBars[j].center.x) {
//                left = lightBars[i];
//                right = lightBars[j];
//            }
//            else {
//                left = lightBars[j];
//                right = lightBars[i];
//            }
//
//            // 1. 角度差（归一化后）
//            float angleDiff = abs(left.angle - right.angle);
//            if (angleDiff > 8.0f) continue;  // 可调整
//
//            // 2. 长度差比例
//            float lengthRatio = abs(left.height - right.height) / max(left.height, right.height);
//            if (lengthRatio > 0.3f) continue;  // 放宽到0.3
//
//            // 3. 中心连线与水平夹角
//            Point2f diff = right.center - left.center;
//            float offsetAngle = abs(atan2(diff.y, diff.x) * 180 / CV_PI);
//            if (offsetAngle > 10.0f) continue;
//
//            // 4. 装甲板宽高比
//            float armorWidth = norm(diff);
//            float armorHeight = (left.height + right.height) / 2.0f;
//            float whRatio = armorWidth / armorHeight;
//            if (whRatio < 1.5f || whRatio > 4.0f) continue;
//
//            // 通过筛选，保存装甲板
//            Armor armor;
//            armor.left = left;
//            armor.right = right;
//
//            // 计算四个角点（利用灯条的旋转矩形顶点）
//            Point2f leftPts[4], rightPts[4];
//            left.rect.points(leftPts);
//            right.rect.points(rightPts);
//            // 按y排序取上下顶点
//            sort(leftPts, leftPts + 4, [](Point2f a, Point2f b) { return a.y < b.y; });
//            sort(rightPts, rightPts + 4, [](Point2f a, Point2f b) { return a.y < b.y; });
//
//            armor.corners = {
//                leftPts[0],   // 左上
//                rightPts[0],  // 右上
//                rightPts[3],  // 右下
//                leftPts[3]    // 左下
//            };
//
//            armor.center = (left.center + right.center) / 2.0f;
//            armors.push_back(armor);
//        }
//    }
//
//    // 绘制装甲板
//    // 在原图副本上绘制装甲板
//    Mat finalImg = original.clone();
//    for (const auto& armor : armors) {
//        // 绘制红色边框
//        for (int i = 0; i < 4; i++)
//            line(finalImg, armor.corners[i], armor.corners[(i + 1) % 4], Scalar(0, 0, 255), 2);
//        // 绘制中心点（黄色）
//        circle(finalImg, armor.center, 5, Scalar(0, 255, 255), -1);
//    }
//    imshow("装甲板检测", finalImg);
//
//	waitKey(0);
//	return 0;
//}