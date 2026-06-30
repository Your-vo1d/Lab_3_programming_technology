#pragma once

#include <QUuid>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QDebug>
#include "DataReader.h"

class SqliteReader : public DataReader {
public:
    QList<QStringList>* read(const QString &dbPath) override
    {
        const QString connId = QUuid::createUuid().toString();

        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", connId);
        db.setDatabaseName(dbPath);
        if (!db.open()) {
            qCritical() << "[SqliteReader] Failed to open DB:" << db.lastError().text();
            QSqlDatabase::removeDatabase(connId);
            return nullptr;
        }

        const QStringList tbls = db.tables();
        if (tbls.isEmpty()) {
            qWarning() << "[SqliteReader] No tables in:" << dbPath;
            db.close();
            QSqlDatabase::removeDatabase(connId);
            return nullptr;
        }

        const QString tbl = tbls.first();
        QSqlQuery q(db);
        if (!q.exec(QStringLiteral("SELECT * FROM \"%1\"").arg(tbl))) {
            qCritical() << "[SqliteReader] Query error:" << q.lastError().text();
            db.close();
            QSqlDatabase::removeDatabase(connId);
            return nullptr;
        }

        auto *rows = new QList<QStringList>();
        const int cols = q.record().count();
        while (q.next()) {
            QStringList row;
            row.reserve(cols);
            for (int i = 0; i < cols; ++i)
                row << q.value(i).toString();
            rows->append(row);
        }

        db.close();
        QSqlDatabase::removeDatabase(connId);
        return rows;
    }
};
