#include "rect.h"


// 默认构造函数
Rect::Rect()
{
}

// 带参数的构造函数
Rect::Rect(QVector<QPoint> args)
{
    _args=args;
}

// 判断点是否在几何形状内
bool Rect::contain(QPoint pos)
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
QPoint Rect::center() const
{
    return _center;
}


// 获取几何形状的参数（顶点等）
QVector<QPoint> Rect::args() const
{
    return _args;
}

// 设置几何形状的参数并计算形状的中心点
void Rect::setArgs(QVector<QPoint> args)
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

