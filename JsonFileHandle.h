#ifndef JSONFILEHANDLE_H
#define JSONFILEHANDLE_H

#include <QObject>

class JsonFileHandle : public QObject
{
    Q_OBJECT
public:
    explicit JsonFileHandle(QObject *parent = nullptr);

public slots:
    /**
     * @brief writeObjectToFile         将一个json对象写入文件中
     * @param fileName                  文件名
     * @param ObjectStr                 json对象
     * @return
     */
    bool writeObjectToFile(QString fileName, QString ObjectStr);

    /**
     * @brief readObjectFromFile        从文件中读取json对象
     * @param fileName                  文件名
     * @return                          返回json字符串
     */
    QString readObjectFromFile(QString fileName);
signals:

};

#endif // JSONFILEHANDLE_H
