#ifndef VIDEOTHREAD_H
#define VIDEOTHREAD_H

#include <QObject>
#include <QThread>
#include <QMutex>

#include "global.h"

class VideoThread : public QThread
{
    Q_OBJECT

public:
    explicit VideoThread(QString path, int width, int height, QObject* parent = nullptr);
    ~VideoThread() override;

signals:
    void videoSaved(QString);

protected:
    void run() override;

private:
    cv::VideoWriter* m_pVideoSaver;
    int             m_framecnt;
    QString         m_videofile;
    QMutex          m_mutex;
    QString         m_path;
    int             m_nWidth;
    int             m_nHeight;
    QVector<cv::Mat> m_imagelist;

    void startRecording();
    void stopRecording();

public slots:
    void addImage(cv::Mat);
};

#endif // VIDEOTHREAD_H
