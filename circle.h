#ifndef CIRCLE_H
#define CIRCLE_H


#include <QPen>
#include <QPoint>
#include <QVector>
#include <QPainter>
#include <QtMath>
#include <QtAlgorithms>
#include <QDebug>
#include <functional>

class Circle
{
public:
    Circle();
    Circle(QVector<QPoint> args);
    bool contain(QPoint p);	// 判断图元包含某点
    QPoint center() const;	// 获取图元中心
    QVector<QPoint> args() const;	// 获取图元参数
    void setArgs(QVector<QPoint> args);	// 设置图元参数

private:
    QPoint _center;	// 图元中心，用于旋转和缩放
    QVector<QPoint> _args;	// 图元参数 （圆心和圆上一点坐标）

};


#endif // CIRCLE_H
