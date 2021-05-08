#include "JsonFileHandle.h"
#include <QFile>
#include <QDebug>
#include <QCoreApplication>


JsonFileHandle::JsonFileHandle(QObject *parent) : QObject(parent)
{
//    qDebug()<<"JsonFileHandle 构造成功";
}

bool JsonFileHandle::writeObjectToFile(QString fileName, QString ObjectStr)
{
    if(fileName.isEmpty())
    {
        qWarning()<<"写入失败， 文件名为空";
        return  false;
    }

    if(ObjectStr.isEmpty())
    {
        qWarning()<<"待写入的字符串为空，写入文件失败";
        return false;
    }

    QFile file(fileName);
    if(!file.open(QIODevice::WriteOnly |QIODevice::Truncate))
    {
        qCritical()<<"读取json文件失败 文件名:"<<fileName;
        return false;
    }

    file.write(ObjectStr.toStdString().data());
//    qCritical()<<"json文件 文件名:"<<fileName;
    file.flush();
    file.close();

    return true;
}

QString JsonFileHandle::readObjectFromFile(QString fileName)
{
    if(fileName.isEmpty())
    {
        qWarning()<<"读取失败， 文件名为空";
        return  QString();
    }

    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly))
    {
        qCritical()<<"读取json文件失败 文件名:"<<fileName;
        return QString();
    }

    QByteArray ba = file.readAll();
    file.close();

    QString objectStr(ba);

    return objectStr;
}
