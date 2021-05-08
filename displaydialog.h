#ifndef DISPLAYDIALOG_H
#define DISPLAYDIALOG_H

#include <QDialog>
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

public slots:
    void    display(cv::Mat);
};

#endif // DISPLAYDIALOG_H
