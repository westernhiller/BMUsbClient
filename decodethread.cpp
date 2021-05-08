#include "decodethread.h"
#include <QDebug>
#include "jpegdecode.h"

using namespace std;
using namespace cv;

DecodeThread::DecodeThread(QObject* parent)
    : QThread(parent)
{
}

DecodeThread::~DecodeThread()
{
    requestInterruption();
    m_mutex.lock();
    m_imageList.clear();
    m_mutex.unlock();
    quit();
    wait();
}

void DecodeThread::run()
{
    while (!isInterruptionRequested())
    {
        if(m_imageList.empty())
        {
            usleep(10000);
            continue;
        }

        m_mutex.lock();
        QByteArray buffer = m_imageList.front();
        m_imageList.pop_front();
        m_mutex.unlock();

        cv::Mat image = Jpeg2Mat((uint8_t *)buffer.data(), buffer.size());
        if(image.data)
        {
            emit imageDecoded(image);
//            qDebug() << "DecodeThread::decoded a frame";
        }
    }
}

void DecodeThread::decodeImage(QByteArray image)
{
    m_mutex.lock();
    m_imageList.append(image);
    m_mutex.unlock();
}


