////openCV应用题1
//#include <opencv2/opencv.hpp>
//#include <iostream>
//
//using namespace cv;
//using namespace std;
//
//int main() {
//    // 1. 读取图像
//    Mat img = imread("D:/images/apple.png"); // 替换为你的图像路径
//    if (img.empty()) {
//        cout << "无法读取图像！" << endl;
//        return -1;
//    }
//
//    // 2. 手动框选苹果的大致区域（弹出窗口交互选择）
//    Rect rect = selectROI("选择苹果区域", img, false);
//    if (rect.width == 0 || rect.height == 0) {
//        cout << "未选择有效区域！" << endl;
//        return -1;
//    }
//
//    // 3. 初始化 GrabCut 所需变量
//    Mat mask, bgdModel, fgdModel;
//    mask.create(img.size(), CV_8UC1);   // 掩码（0=背景，1=前景，2=可能背景，3=可能前景）
//    mask = Scalar::all(0);              // 初始化为全背景
//
//    // 4. 执行 GrabCut 分割（用矩形初始化，迭代5次优化）
//    grabCut(img, mask, rect, bgdModel, fgdModel, 5, GC_INIT_WITH_RECT);
//
//    // 5. 提取前景区域（合并“确定前景”和“可能前景”）
//    Mat foreground;
//    compare(mask, GC_PR_FGD, foreground, CMP_EQ); // 标记“可能前景”
//    compare(mask, GC_FGD, mask, CMP_EQ);          // 标记“确定前景”
//    foreground = foreground + mask;               // 合并两类前景
//    foreground.convertTo(foreground, CV_8UC1);
//    bitwise_and(img, img, img, foreground);       // 只保留前景像素
//
//    // 6. 检测前景区域的轮廓
//    vector<vector<Point>> contours;
//    vector<Vec4i> hierarchy;
//    findContours(foreground.clone(), contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
//
//    // 7. 筛选最大轮廓（假设为苹果）并绘制
//    Mat result = img.clone();
//    if (!contours.empty()) {
//        int maxIdx = 0;
//        double maxArea = contourArea(contours[0]);
//        for (size_t i = 1; i < contours.size(); i++) {
//            double area = contourArea(contours[i]);
//            if (area > maxArea) {
//                maxArea = area;
//                maxIdx = i;
//            }
//        }
//        drawContours(result, contours, maxIdx, Scalar(0, 255, 0), 2); // 绘制绿色轮廓
//        Rect bbox = boundingRect(contours[maxIdx]);
//        rectangle(result, bbox, Scalar(0, 0, 255), 2);               // 绘制红色矩形框
//    }
//
//    // 8. 显示与保存结果
//    imshow("原始图像", img);
//    imshow("前景区域", foreground);
//    imshow("最终结果", result);
//    imwrite("apple_full_grabcut.jpg", result);
//
//    waitKey(0);
//    destroyAllWindows();
//    return 0;
//}