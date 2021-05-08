/** *******************************************************
    * @brief		:
    * @author		:zhongyl
    * @date			:2020-10-22
    * @version		:v1.0
    * @copyright	:Copyright (C) 2019 - All Rights Reserved
    * @warning		:
    * @note			:

******************************************************** **/
#ifndef JSONTOOLS_H
#define JSONTOOLS_H

#include <QObject>

namespace JsonTools {
/**
     * @brief getStringFromQObject QJsonObject 转为 QString
     * @param object
     * @return
     */
QString getStringFromQObject(const QJsonObject &object);

/**
     * @brief getObjectFromString QString转为QJsonObject
     * @param str
     * @return
     */
QJsonObject getObjectFromString(const QString &str);
}

#endif // JSONTOOLS_H
