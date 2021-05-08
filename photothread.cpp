#include "photothread.h"

using namespace std;
using namespace cv;

PhotoThread::PhotoThread(QString path, QString prefix, QObject* parent)
    : QThread(parent)
    , m_path(path)
    , m_prefix(prefix)
{
}

PhotoThread::~PhotoThread()
{
    requestInterruption();
    m_mutex.lock();
    m_imageList.clear();
    m_mutex.unlock();
    quit();
    wait();
}

void PhotoThread::run()
{
    static int index = 1;
    while (!isInterruptionRequested())
    {
        m_mutex.lock();
        if(m_imageList.empty())
        {
            m_mutex.unlock();
            usleep(10000);
            continue;
        }
        Mat image = m_imageList.front();
        m_imageList.pop_front();
        m_mutex.unlock();

        QString filename = m_path + "/" + m_prefix + QString::number(index) + ".png";
        imwrite(filename.toStdString(), image);
        emit imageSaved(filename);

        index++;
    }
}

void PhotoThread::saveImage(cv::Mat image)
{
    m_mutex.lock();
    m_imageList.append(image);
    m_mutex.unlock();
}


