#include "circle.h"


// 默认构造函数
Circle::Circle()
{
}

// 带参数的构造函数
Circle::Circle(QVector<QPoint> args)
{
    _args=args;
}

// 判断点是否在几何形状内
bool Circle::contain(QPoint pos)
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
QPoint Circle::center() const
{
    return _center;
}


// 获取几何形状的参数（顶点等）
QVector<QPoint> Circle::args() const
{
    return _args;
}

// 设置几何形状的参数并计算形状的所有点及中心点
void Circle::setArgs(QVector<QPoint> args)
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




