#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include"circle.h"
#include "line.h"
#include "rect.h"
#include <QMainWindow>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QDesktopServices>
#include <QColorDialog>
#include <QFileDialog>
#include <QPainter>
#include <QBrush>
#include<QStack>
#include <QString>
#include <QUrl>
#include<QHash>


struct ColorPoints {
    QVector<QPoint> points; // 存储点的集合
    QColor color;           // 与点关联的颜色
};

namespace Ui {
class MainWindow;

}

class MainWindow : public QMainWindow
{
    Q_OBJECT


public:


    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void paintEvent(QPaintEvent *event);		// 绘制事件
    void mousePressEvent(QMouseEvent *event);	// 鼠标左键按下事件
    void mouseMoveEvent(QMouseEvent *event);	// 鼠标移动事件
    void mouseReleaseEvent(QMouseEvent *event);	// 鼠标松开事件
    void resizeEvent(QResizeEvent *event);		// 窗口调整事件
    void mouseDoubleClickEvent(QMouseEvent *event); //鼠标双击事件
    void contextMenuEvent(QContextMenuEvent *event); //鼠标右键按下事件

    //画图
    void drawLine(QVector<QPoint> args);//画线
    void drawRect(QVector<QPoint> args);//画矩形
    void drawCircle(QVector<QPoint> args);//画圆

    void scanLineFill4(QPoint seed, const QColor &oldColor);

private slots:
    // 程序按钮对应的槽函数，切换程序状态
    void on_action_line_triggered();
    void on_action_circle_triggered();
    void on_action_rect_triggered();

    void on_action_fill_triggered();

private:

    Ui::MainWindow *ui;
    enum State {Lines, Triangle, Rectangle,Circles, Ellipse, Polygon, Curve,
                Translate, Rotate, Clip, ZoomIn, ZoomOut, Trash,Fill} state;	// 程序状态
    QVector<QPoint> mouse;			// 记录鼠标点击位置
    QList<Line *> lines;
    QList<Circle *> circles;
    QList<Rect *> rects;
    QImage image;					// 画布
    QPixmap pixmap;
    QList<ColorPoints *> fillpoints;
    QPen pen;						// 点的颜色和大小
    QPainter painter;				// 画笔，用于绘制单个点
    QString s;

};

#endif // MAINWINDOW_H
