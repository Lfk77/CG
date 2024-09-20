#include "line.h"

// 默认构造函数
Line::Line()
{
}

// 带参数的构造函数
Line::Line(QPen pen, QVector<QPoint> args)
{
    _pen=pen;
    _args=args;
}

// 判断点是否在几何形状内
bool Line::contain(QPoint pos)
{
//    foreach (QPoint p, _points)
//    {
//        QPoint diff = p - pos;
//        // 判断点是否在形状的范围内，范围由半径平方确定
//        if (diff.x() * diff.x() + diff.y() * diff.y() < 25)
//            return true;
//    }
//    return false;
}

// 获取几何形状的中心点
QPoint Line::center() const
{
    return _center;
}

// 获取几何形状的参数（顶点等）
QVector<QPoint> Line::args() const
{
    return _args;
}

QPen Line::pen() const
{
    return _pen;
}

void Line::setArgs(QVector<QPoint> args)
{
    _args = args;
    int x = 0, y = 0;
    foreach (QPoint p, args)
    {
        x += p.x();
        y += p.y();
    }
    // 计算中心点
    _center = QPoint(x, y) / args.size();
}

QVector<QPoint> Line::clip(QPoint lt, QPoint rb)
{
    // 定义裁剪窗口的边界
    int l = qMin(lt.x(), rb.x()), r = qMax(lt.x(), rb.x());
    int t = qMin(lt.y(), rb.y()), b = qMax(lt.y(), rb.y());

    // 获取要裁剪的线段的两个端点
    QVector<QPoint> args = _args;
    int x1 = args[0].x(), y1 = args[0].y();
    int x2 = args[1].x(), y2 = args[1].y();

    // 匿名lambda函数，用于计算每个点的区域码
    auto computeCode = [=](int x, int y) {
        uint8_t code = 0;
        if (y < t) code |= 8;  // 上
        if (y > b) code |= 4;  // 下
        if (x > r) code |= 2;  // 右
        if (x < l) code |= 1;  // 左
        return code;
    };

    // 计算两个端点的区域码
    uint8_t code1 = computeCode(x1, y1);
    uint8_t code2 = computeCode(x2, y2);

    // 使用Cohen-Sutherland算法裁剪线段
    while (code1||code2)
    {
        if (code1 == 0 && code2 == 0) {
            // 如果两个端点都在裁剪区域内，直接接受
            break;
        }
        else if (code1 & code2) {
            // 如果两个端点的区域码有相同的外侧区域，线段不可见
            break;
        }
        else {
            // 选择一个区域码不为0的点进行裁剪
            uint8_t codeOut = code1 ? code1 : code2;
            int x = 0, y = 0;

            // 根据区域码裁剪线段与裁剪窗口边界的交点
            if (codeOut & 8) { // 上
                x = x1 + (x2 - x1) * (t - y1) / (y2 - y1);
                y = t;
            }
            else if (codeOut & 4) { // 下
                x = x1 + (x2 - x1) * (b - y1) / (y2 - y1);
                y = b;
            }
            else if (codeOut & 2) { // 右
                y = y1 + (y2 - y1) * (r - x1) / (x2 - x1);
                x = r;
            }
            else if (codeOut & 1) { // 左
                y = y1 + (y2 - y1) * (l - x1) / (x2 - x1);
                x = l;
            }

            // 将裁剪后的点更新回端点
            if (codeOut == code1) {
                x1 = x;
                y1 = y;
                code1 = computeCode(x1, y1);
            }
            else {
                x2 = x;
                y2 = y;
                code2 = computeCode(x2, y2);
            }
        }
    }
        args = {{x1, y1}, {x2, y2}};

    return args;
}

