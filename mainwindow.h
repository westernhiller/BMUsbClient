#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QtNetwork>
#include <QAbstractSocket>
#include <QJsonParseError>
#include <QJsonObject>

class QTcpSocket;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

#include "ipctrl.h"
#include "displaydialog.h"
#include "JsonFileHandle.h"
#include "JsonTools.h"
#include "decodethread.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void display(cv::Mat);
    void decodeImage(QByteArray);

protected:
    virtual void closeEvent(QCloseEvent* event) override;

private:
    Ui::MainWindow *ui;
    QTcpSocket* m_tcpSocket;
    DisplayDialog* m_pDisplayer;
    DecodeThread* m_pDecoder;
    bool m_bConnected;
    QByteArray m_receivedData;
    int m_data2read;
    bool m_bReadingData;

    bool    loadConfig(QString);
    void    saveConfig(QString);

private slots:
    void on_pushButtonConnect_clicked();
    void connected();
    void readData();
    void displayError(QAbstractSocket::SocketError);
    void imageCaptured(cv::Mat);

    void showMessage(QString message);
    void on_actionLoadConfig();
    void on_actionSaveConfig();
    void on_actionHelpAbout();
    void on_actionExit();
};
#endif // MAINWINDOW_H
