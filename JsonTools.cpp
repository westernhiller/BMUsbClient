/** *******************************************************
    * @brief		:
    * @author		:zhongyl
    * @date			:2020-10-22
    * @version		:v1.0
    * @copyright	:Copyright (C) 2019 - All Rights Reserved
    * @warning		:
    * @note			:

******************************************************** **/
#include "JsonTools.h"

#include <QJsonObject>
#include <QJsonDocument>
#include <QDebug>

QString JsonTools::getStringFromQObject(const QJsonObject &object)
{
    return QString((QJsonDocument(object).toJson(QJsonDocument::Indented)));
}

QJsonObject JsonTools::getObjectFromString(const QString &str)
{
    QJsonDocument jsonDocument = QJsonDocument::fromJson(str.toLocal8Bit().data());
    if( jsonDocument.isNull() ){
//        qInfo()<< "string is null ===> please check the string "<< str.toLocal8Bit().data();
    }
    QJsonObject jsonObject = jsonDocument.object();
    return jsonObject;
}
