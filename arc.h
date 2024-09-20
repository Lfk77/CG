#ifndef ARC_H
#define ARC_H


#include <QPen>
#include <QPoint>
#include <QVector>
#include <QPainter>
#include <QtMath>
#include <QtAlgorithms>
#include <QDebug>
#include <functional>

class Arc
{
public:
    Arc();
    Arc(QVector<QPoint> args);
    bool contain(QPoint p);	// 判断图元包含某点
    QPoint center() const;	// 获取图元中心
    QVector<QPoint> args() const;	// 获取图元参数
    void setArgs(QVector<QPoint> args);	// 设置图元参数

private:
    QVector<QPoint> _args;  //圆心、始点、终点


};


#endif // CIRCLE_H
