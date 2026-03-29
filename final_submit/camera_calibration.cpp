//#include <opencv2/opencv.hpp>
//#include <iostream>
//
//using namespace cv;
//using namespace std;
//
//int main()
//{
//    // ========= 参数 =========
//    Size patternSize(10, 7);   // 内角点
//    float squareSize = 15.0f;  // mm
//
//    vector<vector<Point2f>> imagePoints;
//    vector<vector<Point3f>> objectPoints;
//
//    // 世界坐标
//    vector<Point3f> obj;
//    for (int i = 0; i < patternSize.height; i++)
//    {
//        for (int j = 0; j < patternSize.width; j++)
//        {
//            obj.push_back(Point3f(j * squareSize, i * squareSize, 0));
//        }
//    }
//
//    // ========= 打开摄像头 =========
//    VideoCapture cap(0);
//    if (!cap.isOpened())
//    {
//        cout << "摄像头打开失败！" << endl;
//        return -1;
//    }
//
//    cout << "按 s 保存图像，按 c 开始标定，按 q 退出" << endl;
//
//    Size imageSize;
//
//    while (true)
//    {
//        Mat frame, gray;
//        cap >> frame;
//        if (frame.empty()) break;
//
//        imageSize = frame.size();
//
//        cvtColor(frame, gray, COLOR_BGR2GRAY);
//
//        vector<Point2f> corners;
//
//        //推荐使用 SB 检测
//        bool found = findChessboardCornersSB(gray, patternSize, corners);
//
//        if (found)
//        {
//            drawChessboardCorners(frame, patternSize, corners, found);
//        }
//
//        imshow("Camera", frame);
//
//        char key = waitKey(30);
//
//        if (key == 's' && found)
//        {
//            cout << "保存一帧" << endl;
//            imagePoints.push_back(corners);
//            objectPoints.push_back(obj);
//        }
//        else if (key == 'c')
//        {
//            if (imagePoints.size() < 5)
//            {
//                cout << "至少需要5张图！" << endl;
//                continue;
//            }
//
//            Mat cameraMatrix = Mat::eye(3, 3, CV_64F);
//            Mat distCoeffs = Mat::zeros(8, 1, CV_64F);
//            vector<Mat> rvecs, tvecs;
//
//            double rms = calibrateCamera(
//                objectPoints,
//                imagePoints,
//                imageSize,
//                cameraMatrix,
//                distCoeffs,
//                rvecs,
//                tvecs
//            );
//
//            cout << "\n===== 标定结果 =====" << endl;
//            cout << "RMS误差: " << rms << endl;
//            cout << "相机矩阵:\n" << cameraMatrix << endl;
//            cout << "畸变系数:\n" << distCoeffs << endl;
//
//            // 保存
//            FileStorage fs("camera.yml", FileStorage::WRITE);
//            fs << "cameraMatrix" << cameraMatrix;
//            fs << "distCoeffs" << distCoeffs;
//            fs.release();
//
//            cout << "已保存 camera.yml" << endl;
//
//            // 测试去畸变
//            while (true)
//            {
//                cap >> frame;
//                if (frame.empty()) break;
//
//                Mat undistorted;
//                undistort(frame, undistorted, cameraMatrix, distCoeffs);
//
//                imshow("Undistorted", undistorted);
//
//                if (waitKey(30) == 'q') break;
//            }
//        }
//        else if (key == 'q')
//        {
//            break;
//        }
//    }
//
//    return 0;
//}