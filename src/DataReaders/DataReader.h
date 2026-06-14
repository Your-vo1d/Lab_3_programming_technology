#pragma once
#include <QStringList>
#include <QDateTime>

class DataReader {
public:
    virtual ~DataReader() = default;
    virtual QList<QStringList>* read(const QString &path) = 0;
};
