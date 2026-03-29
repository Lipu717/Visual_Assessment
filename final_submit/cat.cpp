////openCV基础题3
//#include <opencv2/opencv.hpp>
//#include <iostream>
//
//using namespace cv;
//using namespace std;
//
//// 1.定义全局变量：标记是否正在拖动、矩形起点、原图、临时显示图
//bool isDragging = false;
//Point startPoint;
//Mat image, tempImage;
//
//// 2.鼠标事件回调函数
//void mouseCallback(int event, int x, int y, int flags, void* userdata) {
//    if (event == EVENT_LBUTTONDOWN) {
//        // 左键按下：标记开始拖动，记录起点，复制原图到临时图像
//        isDragging = true;
//        startPoint = Point(x, y);
//        image.copyTo(tempImage);
//    }
//    else if (event == EVENT_MOUSEMOVE && isDragging) {
//        // 鼠标拖动：绘制实时矩形，显示当前像素的坐标和RGB
//        tempImage = image.clone(); // 每次拖动都重新复制原图，避免残留旧矩形
//        Point endPoint(x, y);
//        rectangle(tempImage, startPoint, endPoint, Scalar(0, 255, 0), 2); // 绿色动态矩形
//
//        // 3.边界检查：确保鼠标坐标在图像范围内
//        if (x >= 0 && x < tempImage.cols && y >= 0 && y < tempImage.rows) {
//            Vec3b pixel = tempImage.at<Vec3b>(y, x); // 获取像素的BGR值
//            string info = "坐标: (" + to_string(x) + ", " + to_string(y) + "), "
//                + "RGB: (" + to_string(pixel[2]) + ", " + to_string(pixel[1]) + ", " + to_string(pixel[0]) + ")";
//            putText(tempImage, info, Point(x + 10, y), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(255, 255, 255), 1);
//        }
//    }
//    else if (event == EVENT_LBUTTONUP) {
//        // 左键释放：停止拖动，裁剪图像并保存
//        isDragging = false;
//        Point endPoint(x, y);
//
//        // 4.计算矩形的有效范围
//        int x1 = min(startPoint.x, endPoint.x);
//        int y1 = min(startPoint.y, endPoint.y);
//        int x2 = max(startPoint.x, endPoint.x);
//        int y2 = max(startPoint.y, endPoint.y);
//
//        // 5.确保矩形有有效尺寸（宽、高均>0）
//        if (x1 < x2 && y1 < y2) {
//            Mat croppedImage = image(Rect(x1, y1, x2 - x1, y2 - y1)); // 裁剪图像
//            imshow("Cropped Image", croppedImage); // 显示裁剪后的图像
//            imwrite("D:/images/效果展示/cropped_cat.jpg", croppedImage); // 保存裁剪后的图像到本地
//
//            //6. 计算并输出矩形中心坐标
//            Point center((x1 + x2) / 2, (y1 + y2) / 2);
//            cout << "矩形中心像素坐标: (" << center.x << ", " << center.y << ")" << endl;
//
//            //7.在原图上绘制最终的红色矩形（可选，标记框选结果）
//            rectangle(image, Point(x1, y1), Point(x2, y2), Scalar(0, 0, 255), 2);
//        }
//    }
//}
//
//int main() {
//    // 8. 读取图像
//    image = imread("D:/images/cat.png"); // 猫咪图片路径
//    if (image.empty()) {
//        cout << "无法读取图像，请检查路径！" << endl;
//        return -1;
//    }
//    tempImage = image.clone(); // 初始化临时显示图像
//
//    // 9. 创建窗口并注册鼠标回调
//    namedWindow("Image");
//    setMouseCallback("Image", mouseCallback);
//
//    // 10. 循环显示图像，等待ESC键退出
//    while (true) {
//        imshow("Image", tempImage);
//        char key = waitKey(10);
//        if (key == 27) { // ESC键的ASCII码是27
//            break;
//        }
//    }
//
//    destroyAllWindows();
//    return 0;
//}