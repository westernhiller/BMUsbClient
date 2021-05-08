#ifndef PHOTOTHREAD_H
#define PHOTOTHREAD_H

#include <QThread>
#include <QImage>
#include <QMutex>
#include "global.h"

class PhotoThread : public QThread
{
    Q_OBJECT
public:
    explicit PhotoThread(QString path, QString prefix, QObject* parent = nullptr);
    ~PhotoThread();

signals:
    void            imageSaved(QString);

protected:
    void            run();

private:
    QMutex          m_mutex;
    QString         m_path;
    QString         m_prefix;
    QVector<cv::Mat> m_imageList;

public slots:
    void saveImage(cv::Mat);
};

#endif // PHOTOTHREAD_H
