#pragma once
#include "includes/qcustomplot/qcustomplot.h"
#include <src/Chart/IGraphWidget.h>
#include <src/Chart/IGraphFactory.h>
#include <src/Chart/GraphFactoryRegistry.h>

// Линейный временной граф на основе QCustomPlot.
class CustomPlotGraph : public IGraphWidget {
    Q_OBJECT
public:
    explicit CustomPlotGraph(QWidget *parent = nullptr);

    QCustomPlot *plot() const override;
    void setData(const QList<QStringList> &rows) override;

    void applyChartStyle(IChartStyle *style) override {
        setStyle(style);
        activeStyle_->configure(this);
        update();
        plot_->replot();
    }
};

class CustomGraphWidgetFactory : public IGraphFactory {
public:
    std::unique_ptr<IGraphWidget> create(QWidget *parent = nullptr) override {
        return std::make_unique<CustomPlotGraph>(parent);
    }
};
