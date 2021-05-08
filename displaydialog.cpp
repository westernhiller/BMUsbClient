#include "displaydialog.h"
#include <QLayout>

DisplayDialog::DisplayDialog(QWidget *parent)
    : QDialog(parent)
    , m_nFrames(0)
    , m_fps("FPS:0")
{
    QHBoxLayout* playout = new QHBoxLayout();
    m_pCanvas = new Canvas();
    playout->addWidget(m_pCanvas);
    setLayout(playout);

    connect(this, SIGNAL(displayImage(QImage)), m_pCanvas, SLOT(updateImage(QImage)));
    m_pTimer = new QTimer();
    connect(m_pTimer, SIGNAL(timeout()), this, SLOT(onTimer()));
    m_pTimer->start(1000);
}

void DisplayDialog::display(cv::Mat image)
{
    resize(image.cols, image.rows);
    m_nFrames++;
    cv::putText(image, m_fps.toStdString(), cv::Point(50, 50), cv::FONT_HERSHEY_COMPLEX, 2, cv::Scalar(0, 255, 255), 2);
    emit displayImage(mat2qimage(image));
}

void DisplayDialog::onTimer()
{
    m_fps = QString("FPS:%1").arg(m_nFrames);
    m_nFrames = 0;
}
