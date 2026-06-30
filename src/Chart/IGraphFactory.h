#pragma once
#include <src/Chart/IGraphWidget.h>
#include <QWidget>
#include <memory>

class IGraphFactory {
public:
    virtual ~IGraphFactory() = default;
    virtual std::unique_ptr<IGraphWidget> create(QWidget *parent = nullptr) = 0;
};
