#pragma once

#include <QString>
#include <memory>
#include "src/DataReaders/DataReader.h"
#include "src/Chart/IGraphWidget.h"
#include <QWidget>

// Фасад: создаёт ридер или виджет графика по расширению файла.
class ChartManager {
public:
    static std::shared_ptr<DataReader> createReader(const QString &path);
    static std::unique_ptr<IGraphWidget> createGraphWidget(const QString &path,
                                                           QWidget *parent = nullptr);
};
