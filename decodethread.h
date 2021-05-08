#ifndef DECODETHREAD_H
#define DECODETHREAD_H

#include <QThread>
#include <QMutex>
#include "global.h"

class DecodeThread : public QThread
{
    Q_OBJECT
public:
    explicit DecodeThread(QObject* parent = nullptr);
    ~DecodeThread();

signals:
    void            imageDecoded(cv::Mat);

protected:
    void            run();

private:
    QMutex          m_mutex;
    QVector<QByteArray> m_imageList;

public slots:
    void decodeImage(QByteArray);
};

#endif // DECODETHREAD_H
