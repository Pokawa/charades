#include "DrawWidget.hpp"
#include <QMouseEvent>
#include <QPainter>
#include <QRgb>
#include <spdlog/spdlog.h>
#include <utility>

static void clearCanvas(QImage &canvas, int width, int height)
{
    canvas = QImage(width,height,QImage::Format_RGB888);
    canvas.fill(QColor(Qt::white));
}

DrawWidget::DrawWidget(QWidget *parent) : QWidget(parent), painting(false), lastPoint(0, 0)
{
    drawingColor = QColor(Qt::black);
    this->setFixedSize(600, 600);
    clearCanvas(canvas, width(), height());
    spdlog::info("width {} height {}", width(), height());setFixedSize(600, 600);
}

DrawWidget::~DrawWidget() = default;

void DrawWidget::drawPixel(QPoint pt){
    QRgb value = drawingColor.rgb();
    canvas.setPixel(pt.x(), pt.y(), value);
}

void DrawWidget::drawLine(QPoint pt1, QPoint pt2) {
    drawLine(pt1, pt2, drawingColor);
}

void DrawWidget::drawLine(QPoint pt1, QPoint pt2, QColor color) {
    QPainter painter(&canvas);

    // Set the current settings for the pen
    painter.setPen(QPen(color, 1, Qt::SolidLine, Qt::RoundCap,
                        Qt::RoundJoin));

    // Draw a line from the last registered point to the current
    painter.drawLine(pt1, pt2);

    int rad = (1 / 2) + 2;

    // Call to update the rectangular space where we drew
    update(QRect(pt1, pt2).normalized()
                   .adjusted(-rad, -rad, +rad, +rad));
}

void DrawWidget::drawLineTo(QPoint pt)
{
    drawLine(lastPoint, pt);
    emit paintingLine(lastPoint, pt, drawingColor);
    lastPoint = pt;
}

void DrawWidget::mousePressEvent(QMouseEvent *event){
    if(event->buttons() & Qt::LeftButton){
        painting = true;
        lastPoint = event->pos();
    }
}

void DrawWidget::mouseMoveEvent(QMouseEvent *event){
    if(event->buttons() & Qt::LeftButton){
        drawLineTo(event->pos());
        repaint();
    }
}

void DrawWidget::mouseReleaseEvent(QMouseEvent * event)
{
    if(event->buttons() & Qt::LeftButton){
        painting = false;
    }
}

QColor DrawWidget::drawColor()
{
    return drawingColor;
}

void DrawWidget::setDrawColor(QColor color)
{
    drawingColor = std::move(color);
}

void DrawWidget::clear()
{
    clearCanvas(canvas, width(), height());
    update();
}

void DrawWidget::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
    QPainter painter(this);

    painter.drawPixmap(0,0,QPixmap::fromImage(canvas));
}

