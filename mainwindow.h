#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include"circle.h"
#include "line.h"
#include "rect.h"
#include "arc.h"
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
    void drawLine(int x1,int y1,int x2,int y2,QPen tempen);//画线
    void drawRect(int x1,int y1,int x2,int y2,QPen tempen);//画矩形
    void drawCircle(int x0,int y0,int r,QPen tempen);//画圆
    void drawArc(int x0,int y0, double radius, double startAngle, double endAngle);//画圆弧

    void scanLineFill4(QPoint seed, const QColor &oldColor);

private slots:
    // 程序按钮对应的槽函数，切换程序状态
    void on_action_line_triggered();
    void on_action_circle_triggered();
    void on_action_rect_triggered();

    void on_action_fill_triggered();

    void on_action_arc_triggered();

    void on_action_clip_triggered();

    void on_action_addpoint_triggered();

    void on_action_minuspoint_triggered();

    void on_action_palette_triggered();

private:

    Ui::MainWindow *ui;
    enum State {Lines, Triangle,Arcs, Rectangle,Circles, Ellipse, Polygon, Curve,
                Translate, Rotate, Clip, ZoomIn, ZoomOut, Trash,Fill} state;	// 程序状态
    QVector<QPoint> mouse;			// 记录鼠标点击位置
    QList<Line *> lines;
    QList<Circle *> circles;
    QList<Rect *> rects;
    QList<Arc *> arcs;
    QImage image;					// 画布
    QPixmap pixmap;
    QList<ColorPoints *> fillpoints;
    QPen pen;						// 点的颜色和大小
    QPainter painter;				// 画笔，用于绘制单个点
    QString s;

};

#endif // MAINWINDOW_H
