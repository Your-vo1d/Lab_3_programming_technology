#include "ChartManager.h"
#include <QFileInfo>
#include <src/DataReaders/DataReaderManager.h>
#include <src/Chart/GraphFactoryRegistry.h>

std::shared_ptr<DataReader> ChartManager::createReader(const QString &path)
{
    const QString ext = QFileInfo(path).suffix().toLower();
    for (const auto &e : DataReaderRegistry::instance().entries()) {
        if (e.ext == ext)
            return e.fn();
    }
    return nullptr;
}

std::unique_ptr<IGraphWidget>
ChartManager::createGraphWidget(const QString &path, QWidget *parent)
{
    const QString ext = QFileInfo(path).suffix().toLower();
    for (const auto &e : GraphFactoryRegistry::instance().entries()) {
        if (e.name.toLower() == ext)
            return e.fn(parent);
    }
    const auto &all = GraphFactoryRegistry::instance().entries();
    if (!all.empty())
        return all.front().fn(parent);
    return nullptr;
}
