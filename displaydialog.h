#ifndef DISPLAYDIALOG_H
#define DISPLAYDIALOG_H

#include <QDialog>
#include <QTimer>
#include "global.h"
#include "canvas.h"

class DisplayDialog : public QDialog
{
    Q_OBJECT
public:
    explicit DisplayDialog(QWidget *parent = nullptr);

signals:
    void displayImage(QImage);

private:
    Canvas*     m_pCanvas;
    QTimer*     m_pTimer;
    int         m_nFrames;
    QString     m_fps;

public slots:
    void    display(cv::Mat);
    void    onTimer();
};

#endif // DISPLAYDIALOG_H
