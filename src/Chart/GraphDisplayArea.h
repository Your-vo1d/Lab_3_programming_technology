#pragma once

#include <QWidget>
#include <QVBoxLayout>
#include <src/Chart/IGraphWidget.h>

// Контейнер-виджет, владеющий текущим графиком.
class GraphDisplayArea : public QWidget {
    Q_OBJECT
public:
    explicit GraphDisplayArea(QWidget *parent = nullptr)
        : QWidget(parent), box_(new QVBoxLayout(this)) {}

    void setGraph(std::unique_ptr<IGraphWidget> g) {
        if (widget_) {
            box_->removeWidget(widget_.get());
            widget_.reset();
        }
        widget_ = std::move(g);
        box_->addWidget(widget_.get());
    }

    void feedData(const QList<QStringList> &rows) {
        if (widget_) widget_->setData(rows);
    }

    IGraphWidget *graph() { return widget_.get(); }

private:
    QVBoxLayout                   *box_;
    std::unique_ptr<IGraphWidget>  widget_;
};
