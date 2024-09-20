#ifndef LINE_H
#define LINE_H


#include <QPen>
#include <QPoint>
#include <QVector>
#include <QPainter>
#include <QtMath>
#include <QtAlgorithms>
#include <QDebug>
#include<QSet>
#include <functional>

class Line
{
public:
    Line();
    Line(QPen pen,QVector<QPoint> args);
    bool contain(QPoint p);	// 判断图元包含某点
    QPoint center() const;	// 获取图元中心
    QPen pen() const;
    QVector<QPoint> args() const;	// 获取图元参数
    void setArgs(QVector<QPoint> args);	// 设置图元参数
    QVector<QPoint> clip(QPoint lt, QPoint rb);
private:
    QPen _pen;
    QPoint _center;	// 图元中心，用于旋转和缩放
    QVector<QPoint> _args;	// 图元参数 (首尾两点坐标）
};

#endif // LINE_H
