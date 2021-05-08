#ifndef CANVAS_H
#define CANVAS_H

#include <QLabel>
#include <QPixmap>
#include <QImage>

class Canvas : public QLabel
{
    Q_OBJECT

public:
    explicit Canvas(QWidget* parent = nullptr);

signals:
    void clicked(int, int);

protected:
    virtual void paintEvent(QPaintEvent *ev);
    void mouseReleaseEvent(QMouseEvent *event);

private:
    QPixmap m_img;

public slots:
    void updateImage(QImage);
};

#endif // IMAGEBUTTON_H
