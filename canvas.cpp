#include "canvas.h"
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>

Canvas::Canvas(QWidget *parent)
    : QLabel(parent)
{
}

void Canvas::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.drawPixmap(rect(), m_img);
}

void Canvas::updateImage(QImage image)
{
    m_img = QPixmap::fromImage(image);
    update();
}

void Canvas::mouseReleaseEvent(QMouseEvent *event)
{
    QPoint pt= event->pos();
    emit clicked(pt.x(), pt.y());

    return QWidget::mouseReleaseEvent(event);
}

