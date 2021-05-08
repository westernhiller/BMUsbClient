#include "videothread.h"
#include <QDateTime>

using namespace std;
using namespace cv;

VideoThread::VideoThread(QString path, int width, int height, QObject* parent)
    : QThread(parent)
    , m_pVideoSaver(nullptr)
    , m_framecnt(0)
    , m_path(path)
    , m_nWidth(width)
    , m_nHeight(height)
{
}

VideoThread::~VideoThread()
{
    requestInterruption();
    stopRecording();
    quit();
    wait();
}

void VideoThread::run()
{
    startRecording();
    while(!isInterruptionRequested())
    {
        if(!m_imagelist.empty())
        {
            m_mutex.lock();
            Mat image = m_imagelist.front();
            m_imagelist.pop_front();
            m_mutex.unlock();
            if(m_pVideoSaver)
                m_pVideoSaver->write(image);
            if(m_framecnt++ == 15000)
            {
                stopRecording();
                startRecording();
            }
        }
        usleep(10000);
    }
}

void VideoThread::startRecording()
{
    m_videofile = m_path + tr("/") + QDateTime::currentDateTime().toString("yyyy-MM-dd-hh-mm-ss") + tr(".avi");
    m_mutex.lock();
    m_pVideoSaver = new VideoWriter();
    m_pVideoSaver->open(m_videofile.toStdString(),
//                                    CV_FOURCC('M', 'J', 'P', 'G'),
                                    CV_FOURCC('M', 'P', '4','2'),
                                    25,
                                    Size(m_nWidth, m_nHeight),
                                    true);

    m_framecnt = 0;
    m_mutex.unlock();
}

void VideoThread::stopRecording()
{
    bool bfinished = false;
    m_mutex.lock();
    if(m_pVideoSaver)
    {
        delete m_pVideoSaver;
        m_pVideoSaver = nullptr;
        bfinished = true;
    }
    m_mutex.unlock();

    if(bfinished)
        emit videoSaved(m_videofile);
}

void VideoThread::addImage(Mat image)
{
    m_mutex.lock();
    m_imagelist.append(image);
    m_mutex.unlock();
}
