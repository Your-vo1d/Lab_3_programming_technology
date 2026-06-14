#pragma once

#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonParseError>
#include <QDebug>
#include <QStringList>
#include <cmath>
#include "DataReader.h"
#include <src/DataReaders/DataReaderManager.h>

class JsonReader : public DataReader {
public:
    QList<QStringList>* read(const QString &filePath) override
    {
        QFile f(filePath);
        if (!f.open(QIODevice::ReadOnly | QIODevice::Text)) {
            qWarning() << "[JsonReader] Cannot open" << filePath;
            return nullptr;
        }
        const QByteArray raw = f.readAll();
        f.close();

        QJsonParseError parseErr;
        auto doc = QJsonDocument::fromJson(raw, &parseErr);
        if (parseErr.error != QJsonParseError::NoError) {
            qWarning() << "[JsonReader] Parse error:" << parseErr.errorString();
            return nullptr;
        }

        QJsonArray arr = extractArray(doc);
        if (arr.isEmpty()) {
            qWarning() << "[JsonReader] No JSON array found in" << filePath;
            return nullptr;
        }

        auto *rows = new QList<QStringList>();
        rows->reserve(arr.size());

        for (const auto &element : arr) {
            QString dateStr;
            double  numVal = std::numeric_limits<double>::quiet_NaN();

            if (element.isArray()) {
                auto sub = element.toArray();
                if (sub.size() >= 2 && sub[0].isString() && sub[1].isDouble()) {
                    dateStr = sub[0].toString();
                    numVal  = sub[1].toDouble();
                }
            } else if (element.isObject()) {
                auto obj = element.toObject();
                for (const char *k : {"Time", "time", "Date", "date"}) {
                    if (obj.contains(k) && obj[k].isString()) {
                        dateStr = obj[k].toString();
                        break;
                    }
                }
                for (const char *k : {"Value", "value", "Val", "val"}) {
                    if (obj.contains(k) && obj[k].isDouble()) {
                        numVal = obj[k].toDouble();
                        break;
                    }
                }
            }

            if (!dateStr.isEmpty() && !qIsNaN(numVal))
                rows->append(QStringList{ dateStr, QString::number(numVal) });
        }

        if (rows->isEmpty())
            qWarning() << "[JsonReader] No valid records found in" << filePath;

        return rows;
    }

private:
    static QJsonArray extractArray(const QJsonDocument &doc)
    {
        if (doc.isArray())
            return doc.array();

        if (doc.isObject()) {
            const auto obj = doc.object();
            if (obj.contains("data") && obj["data"].isArray())
                return obj["data"].toArray();
            for (auto it = obj.begin(); it != obj.end(); ++it)
                if (it.value().isArray())
                    return it.value().toArray();
        }
        return {};
    }
};
