#pragma once

#include <QString>
#include <QStringList>
#include <vector>
#include <functional>
#include <memory>
#include <algorithm>
#include "IGraphWidget.h"
#include <QWidget>

// Реестр фабричных функций для всех типов графиков.
class GraphFactoryRegistry {
public:
    using BuildFn = std::function<std::unique_ptr<IGraphWidget>(QWidget *)>;

    struct Entry {
        QString name;
        BuildFn fn;
    };

    static GraphFactoryRegistry &instance() {
        static GraphFactoryRegistry inst;
        return inst;
    }

    void add(const QString &name, BuildFn fn) {
        catalog_.push_back({ name, std::move(fn) });
    }

    QStringList availableTypes() const {
        QStringList list;
        list.reserve(static_cast<int>(catalog_.size()));
        for (const auto &e : catalog_)
            list << e.name;
        std::sort(list.begin(), list.end());
        return list;
    }

    std::unique_ptr<IGraphWidget> build(const QString &name, QWidget *parent = nullptr) const {
        for (const auto &e : catalog_)
            if (e.name.toLower() == name.toLower())
                return e.fn(parent);
        return nullptr;
    }

    const std::vector<Entry> &entries() const { return catalog_; }

private:
    std::vector<Entry> catalog_;
};

#define REGISTER_GRAPH_FACTORY(NAME, WIDGET_CLASS)                      \
namespace {                                                             \
    const bool _greg_##WIDGET_CLASS = []() {                            \
        GraphFactoryRegistry::instance().add(                           \
            NAME,                                                       \
            [](QWidget *p) {                                            \
                return std::make_unique<WIDGET_CLASS>(p);               \
            });                                                         \
        return true;                                                    \
    }();                                                                \
}
