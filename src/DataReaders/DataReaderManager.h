// src/DataReaders/DataReaderManager.h
#pragma once

#include <QString>
#include <vector>
#include <functional>
#include <memory>
#include "DataReader.h"
#include <src/DataReaders/JsonReader.h>
#include <src/DataReaders/SqliteReader.h>

// Реестр фабрик для DataReader; каждая запись связывает расширение файла с фабрикой.
class DataReaderRegistry {
public:
    using FactoryFn = std::function<std::shared_ptr<DataReader>()>;

    struct Entry {
        QString   ext;
        FactoryFn fn;
    };

    static DataReaderRegistry& instance() {
        static DataReaderRegistry self;
        return self;
    }

    void add(const QString &extension, FactoryFn fn) {
        entries_.push_back({ extension.toLower(), std::move(fn) });
    }

    const std::vector<Entry>& entries() const { return entries_; }

private:
    std::vector<Entry> entries_;
};

// Макрос для авто-регистрации ридера через статическую инициализацию
#define REGISTER_DATA_READER(EXT, READER_CLASS)                              \
namespace {                                                                  \
    const bool _dreg_##READER_CLASS = []() {                                 \
        DataReaderRegistry::instance()                                       \
            .add(EXT, [] { return std::make_shared<READER_CLASS>(); });      \
        return true;                                                         \
    }();                                                                     \
}
