#include "displaydialog.h"
#include <QLayout>

DisplayDialog::DisplayDialog(QWidget *parent)
    : QDialog(parent)
{
    QHBoxLayout* playout = new QHBoxLayout();
    m_pCanvas = new Canvas();
    playout->addWidget(m_pCanvas);
    setLayout(playout);

    connect(this, SIGNAL(displayImage(QImage)), m_pCanvas, SLOT(updateImage(QImage)));
}

void DisplayDialog::display(cv::Mat image)
{
    resize(image.cols, image.rows);
    emit displayImage(mat2qimage(image));
}
