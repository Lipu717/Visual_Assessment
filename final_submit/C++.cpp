////c++应用题
//#include <iostream>
//#include <cmath>
//#include <iomanip>
//using namespace std;
//
//// 1.定义坐标点结构体
//struct Point {
//    double x, y;
//    Point(double x = 0, double y = 0) : x(x), y(y) {}
//};
//
////2.Rect结构体
//struct Rect {
//    int id;         // 数字ID
//    int color;      // 装甲板颜色（0为蓝色，1为红色）
//    Point point;    // 左上角坐标
//    double width;   // 宽度
//    double height;  // 高度
//
//    Rect(int id_val, int color_val, double x, double y, double width_val, double height_val)
//        : id(id_val), color(color_val), point(x, y), width(width_val), height(height_val)
//    {}
//};
//
////3.Armor类
//class Armor {
//private:
//    Rect rect;  // 装甲板的 Rect 结构体
//
//public:
//    // Armor 类的构造函数
//    Armor(int id, int color, double x, double y, double width, double height)
//        : rect(id, color, x, y, width, height) {
//    }
//
//    // 计算装甲板中心坐标的函数
//    Point Central_Point() {
//        double centerX = rect.point.x + rect.width/2;
//        double centerY = rect.point.y + rect.height/2;
//        return Point(centerX, centerY);
//    }
//
//    // 计算装甲板对角线长度的函数
//    double Diagonal() {
//        double diagonal = sqrt(pow(rect.width, 2) + pow(rect.height, 2));
//        return round(diagonal * 100) / 100;
//    }
//
//    // 输出装甲板 4 点坐标的函数（从左上角坐标开始顺时针输出）
//    void Armor_Point()
//    {
//        Point topLeft = rect.point;
//        Point topRight(rect.point.x + rect.width, rect.point.y);
//        Point bottomRight(rect.point.x + rect.width, rect.point.y + rect.height);
//        Point bottomLeft(rect.point.x, rect.point.y + rect.height);
//
//        cout << topLeft.x << " " << topLeft.y << " "
//            << topRight.x << " " << topRight.y << " "
//            << bottomRight.x << " " << bottomRight.y << " "
//            << bottomLeft.x << " " << bottomLeft.y << endl;
//    }
//
//    // 输出装甲板颜色的功能
//    string Armor_Color() {
//        string color;
//        if (rect.color == 0) {
//            color = "蓝色";
//        }
//        else {
//            color = "红色";
//        }
//        return color;
//    }
//
//    // 获取装甲板的 ID
//    int GetId() {
//        return rect.id;
//    }
//};
//
//int main() {
//    int id, color;
//    double x, y, width, height;
//
//    // 4.输入数字ID和颜色
//    cin >> id >> color;
//
//    // 5.输入左上角点坐标和宽与高
//    cin >> x >> y >> width >> height;
//
//    // 6.创建 Armor 对象
//    Armor armor(id, color, x, y, width, height);
//
//    // 7.输出数字ID和颜色
//    cout << armor.GetId() << " " << (color == 0 ? "蓝色" : "红色") << endl;
//
//    // 8.输出装甲板中心坐标和对角线长度
//    Point center = armor.Central_Point();
//    double diagonal = armor.Diagonal();
//    cout << center.x << " " << center.y << " " << diagonal << endl;
//
//    // 9.输出装甲板 4 点坐标
//    armor.Armor_Point();
//
//    return 0;
//}