#ifndef DRAWWIDGET_H
#define DRAWWIDGET_H

#include <QObject>
#include <QWidget>

class QPaintEvent;
class QMouseEvent;
class DrawWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DrawWidget(QWidget *parent = nullptr);
    ~DrawWidget() override;
    void drawPixel(QPoint pt);
    void drawLineCurrentColor(QPoint pt1, QPoint pt2);
    void drawLineTo(QPoint pt);

public slots:
    void clear();
    QColor drawColor();
    void setDrawColor(QColor color);
    void drawLine(QPoint pt1, QPoint pt2, const QColor& color);


signals:
    void linePainted(QPoint pt1, QPoint pt2, QColor color);

protected:
    void paintEvent(QPaintEvent *) override;
    void mousePressEvent(QMouseEvent *) override;
    void mouseMoveEvent(QMouseEvent *) override;
    void mouseReleaseEvent(QMouseEvent *) override;


private:
    QColor drawingColor;
    QImage canvas;
    bool painting;
    QPoint lastPoint;
};

#endif // DRAWWIDGET_H
