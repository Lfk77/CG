#include "mainwindow.h"
#include "ui_mainwindow.h"

Line *line;
Circle *circle;
Rect *rec;
ColorPoints *fillpoint;
// 构造函数
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),        // 调用父类 QMainWindow 的构造函数
    ui(new Ui::MainWindow),     // 初始化 UI
    pen(Qt::black, 3)           // 初始化画笔为黑色，宽度为3
{
    ui->setupUi(this);          // 设置 UI
    // 初始化 QImage 对象
        image = QImage(ui->label->size(), QImage::Format_RGB32);
        image.fill(Qt::white);  // 填充初始背景为白色

        // 初始化 QPainter 对象
        painter.begin(&image);
        painter.setPen(pen);
        painter.end();
}

// 析构函数
MainWindow::~MainWindow()
{
    // 释放所有图元指针的内存
    foreach (Line *l, lines)
        delete l;
    foreach (Circle *c, circles)
        delete c;
    foreach (Rect *r, rects)
        delete r;
    foreach(ColorPoints *f,fillpoints)
        delete f;
    delete ui;  // 释放 UI 的内存
}

// 绘图事件
void MainWindow::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    image.fill(Qt::white);
    painter.begin(&image);

    // 绘制用户加载的图片
    if (!s.isEmpty())
    {
        QImage temp(s);
        painter.drawImage(QPoint((pixmap.width() - temp.width()) / 2, (pixmap.height() - temp.height()) / 2), temp);
    }

    // 绘制所有存储的图元
    foreach (Line *l, lines)  // 画直线
    {
        painter.setPen(pen);
        drawLine(l->args());
    }
    foreach (Circle *c, circles)  // 画圆
    {
        painter.setPen(pen);
        drawCircle(c->args());
    }
    foreach (Rect *r, rects)  // 画矩形
    {
        painter.setPen(pen);
        drawRect(r->args());
    }

    foreach (ColorPoints *f, fillpoints)  // 填充
    {
        painter.setPen(f->color);
        painter.drawPoints(f->points);
    }


    painter.end();
    ui->label->setPixmap(QPixmap::fromImage(image));
}

// 鼠标左键按下事件
void MainWindow::mousePressEvent(QMouseEvent *event)
{
    mouse.clear();
    QPoint pos = event->pos();   // 获取鼠标点击位置
    pos.rx() -= 11;              // 调整坐标，使其适应窗口的偏移
    pos.ry() -= 51;
    mouse.append(pos);          // 将点击位置添加到控制点列表中
    switch (state)
    {
    case Lines:
        line = new Line({pos, pos});  // 创建一条线
        lines.append(line);
        break;
     case Circles:
        circle = new Circle({pos,pos});
        circles.append(circle);
        break;
     case Rectangle:
        rec = new Rect({pos,pos});
        rects.append(rec);
        break;
     case Fill:
        if(event->button()==Qt::LeftButton)
        {
            scanLineFill4(pos,image.pixelColor(pos));
        }
        else if(event->button()==Qt::RightButton&&!fillpoints.isEmpty())
            fillpoints.removeLast();
        break;

    }
    update();  // 更新绘图
}

//鼠标右键按下事件
void MainWindow::contextMenuEvent(QContextMenuEvent *event)
{
    QPoint pos = event->pos();   // 获取鼠标点击位置
    pos.rx() -= 11;              // 调整坐标，使其适应窗口的偏移
    pos.ry() -= 51;

    mouse.append(pos);          // 将点击位置添加到控制点列表中


    update();  // 更新绘图
}

// 鼠标移动事件
void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    QPoint pos = event->pos();  // 获取鼠标当前位置
    pos.rx() -= 11;             // 调整坐标
    pos.ry() -= 51;
    //mouse.append(pos);
    QVector<QPoint> args;
    switch (state)
    {
    case Lines:
        line->setArgs({mouse[0],pos});
        break;
    case Circles:
        circle->setArgs({mouse[0], pos});  // 更新圆的终点
        break;
    case Rectangle:
        rec->setArgs({mouse[0],pos});
        break;


    }
    update();  // 更新绘图
}

// 鼠标释放事件
void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    painter.begin(&image);
    QPoint pos = event->pos();  // 获取鼠标当前位置
    pos.rx() -= 11;             // 调整坐标
    pos.ry() -= 51;
    QVector<QPoint> args;
    switch (state)
    {
    case Lines:
        line->setArgs({mouse[0],pos});
        break;
    case Circles:
        circle->setArgs({mouse[0], pos});  // 更新圆的终点
        break;
    case Rectangle:
        rec->setArgs({mouse[0],pos});
        break;


    }
    painter.end();
    update();  // 更新绘图

    if (state != Curve && state != Polygon)
        mouse.clear();  // 清空控制点列表
}

//鼠标双击事件
void MainWindow::mouseDoubleClickEvent(QMouseEvent *event)
{


}

// 窗口大小改变事件
void MainWindow::resizeEvent(QResizeEvent *event)
{
    // 调整 QLabel 的尺寸
       ui->label->resize(ui->centralwidget->size() - QSize(22, 22));

       // 调整 QImage 对象的大小
       image = QImage(ui->label->size(), QImage::Format_RGB32);
       image.fill(Qt::white);  // 填充背景为白色
}


// 切换绘制模式
void MainWindow::on_action_line_triggered()
{
    state = Lines;
    mouse.clear();
}
void MainWindow::on_action_circle_triggered()
{
    state = Circles;
    mouse.clear();
}
void MainWindow::on_action_rect_triggered()
{
    state = Rectangle;
    mouse.clear();
}
void MainWindow::on_action_fill_triggered()
{
    state=Fill;
    mouse.clear();
}

//画线
void MainWindow::drawLine(QVector<QPoint> args)
{
    int x1 = args[0].x(), y1 = args[0].y(); // 获取起点 (x1, y1)
    int x2 = args[1].x(), y2 = args[1].y(); // 获取终点 (x2, y2)
    int dx = qAbs(x2 - x1), sx = x1 < x2 ? 1 : -1; // 计算x方向的增量dx，sx为x方向的步进（1或-1）
    int dy = qAbs(y2 - y1), sy = y1 < y2 ? 1 : -1; // 计算y方向的增量dy，sy为y方向的步进（1或-1）
    int err = (dx > dy ? dx : -dy) / 2, e; // 计算初始误差err，基于Bresenham算法的误差函数

    QVector<QPoint> points; // 存储所有扩展后的点，用于绘制
    int width = pen.width() / 3; // 设置线宽，除以3使线条更细致
    int halfWidth = width / 2; // 线宽的一半，用于扩展线条的边缘

    // 预估线宽扩展后的点数量，提前分配空间优化性能
    points.reserve((dx + dy) * width * width);

    // 主循环处理线条绘制，采用Bresenham算法绘制线条
    while (x1 != x2 || y1 != y2) { // 直到(x1, y1)等于(x2, y2)时退出循环
        // 对当前点(x1, y1)进行线宽扩展，将每个点按照线宽范围内的偏移量生成多个点
        for (int dxOffset = -halfWidth; dxOffset <= halfWidth; ++dxOffset) {
            for (int dyOffset = -halfWidth; dyOffset <= halfWidth; ++dyOffset) {
                points.append(QPoint(x1 + dxOffset, y1 + dyOffset)); // 将扩展后的点加入points列表
            }
        }

        // Bresenham算法的核心步骤，用于决定下一个点的坐标
        e = err; // 临时变量e存储当前误差
        if (e > -dx) { // 如果误差超过x方向的增量
            err -= dy; // 更新误差值
            x1 += sx;  // 在x方向上移动一个步长（sx可能为正或负）
        }
        if (e < dy) { // 如果误差小于y方向的增量
            err += dx; // 更新误差值
            y1 += sy;  // 在y方向上移动一个步长（sy可能为正或负）
        }
    }

    // 为终点(x2, y2)也进行线宽扩展，避免最后一个点漏掉
    for (int dxOffset = -halfWidth; dxOffset <= halfWidth; ++dxOffset) {
        for (int dyOffset = -halfWidth; dyOffset <= halfWidth; ++dyOffset) {
            points.append(QPoint(x2 + dxOffset, y2 + dyOffset)); // 添加终点的扩展点
        }
    }

    // 使用painter直接绘制所有扩展后的点集合，生成最终的线条效果
    painter.drawPoints(points);

}

//画矩形
void MainWindow::drawRect(QVector<QPoint> args)
{
    // 计算矩形的边界
    int xmin = qMin(args[0].x(), args[1].x());
    int ymin = qMin(args[0].y(), args[1].y());
    int xmax = qMax(args[1].x(), args[0].x());
    int ymax = qMax(args[1].y(), args[0].y());

    drawLine({QPoint(xmin, ymin), QPoint(xmin, ymax)});
    drawLine({QPoint(xmin, ymin), QPoint(xmax, ymin)});
    drawLine({QPoint(xmax, ymin), QPoint(xmax, ymax)});
    drawLine({QPoint(xmin, ymax), QPoint(xmax, ymax)});
}
inline uint qHash(const QPoint &p, uint seed = 0) {
    return qHash(QPair<int, int>(p.x(), p.y()), seed);
}

//画圆
void MainWindow::drawCircle(QVector<QPoint> args)
{
    int x0 = args[0].x(), y0 = args[0].y();
    int x1 = args[1].x(), y1 = args[1].y();
    int r = static_cast<int>(sqrt((x1 - x0) * (x1 - x0) + (y1 - y0) * (y1 - y0)));

    QVector<QPoint> points;
    points.reserve(8 * r);  // 预分配足够的空间

    int x = 0;
    int y = r;
    int p = 1 - r;  // 判决参数

    while (x <= y) {
        // 利用 8 对称性绘制点
        points.append(QPoint(x0 + x, y0 + y));  // 第一象限
        points.append(QPoint(x0 - x, y0 + y));  // 第二象限
        points.append(QPoint(x0 + x, y0 - y));  // 第四象限
        points.append(QPoint(x0 - x, y0 - y));  // 第三象限
        points.append(QPoint(x0 + y, y0 + x));  // 第一象限对称
        points.append(QPoint(x0 - y, y0 + x));  // 第二象限对称
        points.append(QPoint(x0 + y, y0 - x));  // 第四象限对称
        points.append(QPoint(x0 - y, y0 - x));  // 第三象限对称

        x++;

        if (p < 0) {
            p += 2 * x + 1;
        } else {
            y--;
            p += 2 * (x - y) + 1;
        }
    }

    int width = pen.width()/3;
    int halfWidth = width / 2;

    // 处理线宽
    QVector<QPoint> extendedPoints;
    extendedPoints.reserve(points.size() * (width * width));  // 预估线宽扩展后的点数量

    for (const QPoint& pt : points) {
        for (int dx = -halfWidth; dx <= halfWidth; ++dx) {
            for (int dy = -halfWidth; dy <= halfWidth; ++dy) {
                extendedPoints.append(QPoint(pt.x() + dx, pt.y() + dy));
            }
        }
    }
    painter.drawPoints(extendedPoints);
}

//填充
void MainWindow::scanLineFill4(QPoint seed, const QColor &oldColor) {

    QColor newColor=pen.color();
    QStack<QPoint> stack;
    stack.push(seed);

    QVector<QPoint> points;
    int width = image.width();
    int height = image.height();


    while (!stack.isEmpty()) {
        QPoint pt = stack.pop();
        int x = pt.x();
        int y = pt.y();

        // 扫描行，向右填充
        int xl = x;
        while (xl >= 0 && image.pixelColor(xl, y) == oldColor) {
            image.setPixelColor(xl, y, newColor);
            //painter.drawLine(QPoint(xl,y),QPoint(width,height));
            points.append(QPoint(xl,y));
            xl--;
        }
        xl++;

        // 扫描行，向左填充
        int xr = x+1;
        while (xr < width && image.pixelColor(xr, y) == oldColor) {
            image.setPixelColor(xr, y, newColor);
            points.append(QPoint(xr,y));
            xr++;
        }
        xr--;

        // 处理上面一条扫描线
        for (int i = xl; i <= xr; ++i) {
            if (y > 0) {
                if (image.pixelColor(i, y - 1) == oldColor) {
                    int left = i, right = i;
                    while (left >= xl && image.pixelColor(left, y - 1) == oldColor) left--;
                    while (right <= xr && image.pixelColor(right, y - 1) == oldColor) right++;
                    if (right > left) {
                        stack.push(QPoint((left + right) / 2, y - 1));
                    }
                }
            }

            // 处理下面一条扫描线
            if (y < height - 1) {
                if (image.pixelColor(i, y + 1) == oldColor) {
                    int left = i, right = i;
                    while (left >= xl && image.pixelColor(left, y + 1) == oldColor) left--;
                    while (right <= xr && image.pixelColor(right, y + 1) == oldColor) right++;
                    if (right > left) {
                        stack.push(QPoint((left + right) / 2, y + 1));
                    }
                }
            }
        }
    }
    fillpoint=new ColorPoints;
    fillpoint->points=points;
    fillpoint->color=newColor;
    fillpoints.append(fillpoint);
}


