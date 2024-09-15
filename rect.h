#ifndef RECT_H
#define RECT_H

#include <QPen>
#include <QPoint>
#include <QVector>
#include <QPainter>
#include <QtMath>
#include <QtAlgorithms>
#include <QDebug>
#include<QSet>
#include <functional>

class Rect
{
public:
    Rect();
    Rect(QVector<QPoint> args);
    bool contain(QPoint p);	// 判断图元包含某点
    QPoint center() const;	// 获取图元中心
    QVector<QPoint> args() const;	// 获取图元参数
    void setArgs(QVector<QPoint> args);	// 设置图元参数

private:
    QPoint _center;	// 图元中心，用于旋转和缩放
    QVector<QPoint> _args;	// 图元参数 (对角两点坐标）
};

#endif // RECT_H
