#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QStatusBar>
#include <QMessageBox>
#include "turbojpeg.h"
#include "jpegdecode.h"

using namespace std;
using namespace cv;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_pDisplayer(nullptr)
    , m_pDecoder(nullptr)
    , m_bConnected(false)
    , m_data2read(0)
    , m_bReadingData(false)
{
    ui->setupUi(this);
    if(!loadConfig("config.json"))
    {
        ui->ipaddr->setText("192.168.1.108");
        ui->lineEditPort->setText("8080");
    }
    connect(ui->actionHelpAbout, SIGNAL(triggered()), this, SLOT(on_actionHelpAbout()));
    connect(ui->actionExit, SIGNAL(triggered()), this, SLOT(on_actionExit()));
    connect(ui->actionLoadConfig, SIGNAL(triggered()), this, SLOT(on_actionLoadConfig()));
    connect(ui->actionSaveConfig, SIGNAL(triggered()), this, SLOT(on_actionSaveConfig()));

    m_tcpSocket = new QTcpSocket();
    connect(m_tcpSocket, SIGNAL(readyRead()), this, SLOT(readData()));
    connect(m_tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(displayError(QAbstractSocket::SocketError)));
    connect(m_tcpSocket, SIGNAL(connected()), this, SLOT(connected()));
}

MainWindow::~MainWindow()
{
    if(m_pDecoder)
        delete m_pDecoder;
    if(m_pDisplayer)
        delete m_pDisplayer;

    delete ui;
    m_tcpSocket->abort();
    delete m_tcpSocket;
}

void MainWindow::connected()
{
    qDebug() << "MainWindow::connected";
    m_bConnected = true;
    if(!m_pDecoder)
    {
        m_pDecoder = new DecodeThread();
        connect(this, SIGNAL(decodeImage(QByteArray)), m_pDecoder, SLOT(decodeImage(QByteArray)));
        connect(m_pDecoder, SIGNAL(imageDecoded(cv::Mat)), this, SLOT(imageCaptured(cv::Mat)));
        m_pDecoder->start();
    }
    if(!m_pDisplayer)
    {
        m_pDisplayer = new DisplayDialog();
        connect(this, SIGNAL(display(cv::Mat)), m_pDisplayer, SLOT(display(cv::Mat)));
        m_pDisplayer->show();
    }
}

void MainWindow::imageCaptured(cv::Mat image)
{
    emit display(image);
}

void MainWindow::readData()
{
    QByteArray buffer = m_tcpSocket->readAll();
    m_receivedData.append(buffer);
    int len = m_receivedData.size();

    if(m_bReadingData)
    {
        if(m_data2read <= len)
        {
            QByteArray image = m_receivedData.left(m_data2read);
            m_receivedData = m_receivedData.right(len - m_data2read);
            m_bReadingData = false;
            m_data2read = 0;

//            qDebug() << "decoding image ";
            emit decodeImage(image);
        }
    }
    else // wait for head
    {
        int idx = m_receivedData.indexOf('#');
        if(-1 != idx)
        {
            int idx2 = m_receivedData.indexOf('#', idx + 1);
            if(-1 != idx2)
            {
                QString frameIndex = QString(m_receivedData.left(idx));
                int idxF = frameIndex.indexOf('F');
                frameIndex = frameIndex.mid(idxF + 5, idx - idxF);
//                qDebug() << "frame Index = " << frameIndex;
                statusBar()->showMessage(frameIndex);
                QString size = QString(m_receivedData.mid(idx+1, idx2-idx-1));
//                qDebug() << "Size = " << m_receivedData.mid(idx+1, idx2-idx-1) << ", size = " << size;
                m_data2read = size.toInt();
//                qDebug() << "idx = " << idx << ", idx2 = " << idx2 << ", m_data2read = " << m_data2read;
                m_bReadingData = true;
                m_receivedData = m_receivedData.right(len - idx2 - 1);
            }
        }
    }
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    delete m_pDisplayer;
    m_pDisplayer = nullptr;
}


void MainWindow::on_pushButtonConnect_clicked()
{
    m_tcpSocket->abort();
    m_bReadingData = false;
    if(m_bConnected)
    {
        m_bConnected = false;
        ui->pushButtonConnect->setText(QStringLiteral("连接"));
    }
    else
    {
        m_data2read = 10;
        ui->pushButtonConnect->setText(QStringLiteral("断开"));
        m_tcpSocket->connectToHost(ui->ipaddr->text(), ui->lineEditPort->text().toInt());
        qDebug() << "connecting to host " << ui->ipaddr->text() << ", at port " << ui->lineEditPort->text().toInt() ;
    }
}

void MainWindow::displayError(QAbstractSocket::SocketError)
{
    qDebug() << m_tcpSocket->errorString();
}

void MainWindow::on_actionExit()
{
    close();
}

void MainWindow::on_actionHelpAbout()
{
    QMessageBox::information(NULL, QStringLiteral("关于..."), QStringLiteral("定标工具软件\n上海超摩光电科技有限公司"));
}

void MainWindow::showMessage(QString message)
{
    statusBar()->showMessage(message);
}

void MainWindow::on_actionLoadConfig()
{
    QFileDialog *fileDialog = new QFileDialog(this);
    fileDialog->setWindowTitle(QStringLiteral("调入配置文件"));
    fileDialog->setDirectory(QCoreApplication::applicationDirPath());
    fileDialog->setAcceptMode(QFileDialog::AcceptOpen);
    fileDialog->setNameFilter(QStringLiteral("配置文件 (*.json)"));
    fileDialog->selectNameFilter(QStringLiteral("配置文件 (*.json)"));
    fileDialog->setViewMode(QFileDialog::Detail);
    if(fileDialog->exec() == QDialog::Accepted)
    {
        QString sFile = fileDialog->selectedFiles()[0];
        loadConfig(sFile);
   }
}

bool MainWindow::loadConfig(QString file)
{
    JsonFileHandle fileHandle;
    QString configJson = fileHandle.readObjectFromFile(file);
    QJsonObject configJsonObj = JsonTools::getObjectFromString(configJson);
    if(configJsonObj.empty())
    {
         emit showMessage(QStringLiteral("配置文件解析失败, 请检查配置文件json格式"));
         return false;
    }
    QJsonObject networkObj = configJsonObj["network"].toObject();

    QString ip = networkObj["ip"].toString();
    int port = networkObj["port"].toInt();
    ui->ipaddr->setText(ip);
    ui->lineEditPort->setText(QString::number(port));

    return true;
}

void MainWindow::on_actionSaveConfig()
{
    QFileDialog *fileDialog = new QFileDialog(this);
    fileDialog->setWindowTitle(QStringLiteral("保存配置文件"));
    fileDialog->setDirectory(QCoreApplication::applicationDirPath());
    fileDialog->setAcceptMode(QFileDialog::AcceptSave);
    fileDialog->setNameFilter(QStringLiteral("配置文件 (*.json)"));
    fileDialog->selectNameFilter(QStringLiteral("配置文件 (*.json)"));
    fileDialog->setViewMode(QFileDialog::Detail);
    if(fileDialog->exec() == QDialog::Accepted)
    {
        QString sFile = fileDialog->selectedFiles()[0];
        saveConfig(sFile);
    }
}

void MainWindow::saveConfig(QString file)
{
    QJsonObject networkObj;
    networkObj["ipaddr"] = ui->ipaddr->text();
    networkObj["port"] = ui->lineEditPort->text().toInt();

    QJsonObject configJson;
    configJson["network"] = networkObj;

    JsonFileHandle fileHandle;
    QString jsonStr = JsonTools::getStringFromQObject(configJson);
    fileHandle.writeObjectToFile(file, jsonStr);
    emit showMessage(QStringLiteral("配置文件保存成功！"));
}

