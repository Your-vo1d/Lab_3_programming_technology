// src/Chart/CustomGraphs/BarChartGraph.h
#pragma once

#include <src/Chart/IGraphWidget.h>
#include <includes/qcustomplot/qcustomplot.h>
#include <src/Chart/IGraphFactory.h>
#include <src/Chart/GraphFactoryRegistry.h>
#include <QVBoxLayout>

// Столбчатый граф с временной осью X.
class BarChartGraph : public IGraphWidget {
    Q_OBJECT
public:
    explicit BarChartGraph(QWidget *parent = nullptr)
        : IGraphWidget(parent)
    {
        plot_ = new QCustomPlot(this);
        setupTimeAxis();

        series_ = new QCPBars(plot_->xAxis, plot_->yAxis);
        series_->setWidth(0.8 * 3600);  // ~0.8 часа в секундах

        enableZoomPan(plot_);

        auto *lay = new QVBoxLayout(this);
        lay->setContentsMargins(0, 0, 0, 0);
        lay->addWidget(plot_);
    }

    void setData(const QList<QStringList> &rows) override {
        QVector<double> keys, vals;
        for (const auto &row : rows) {
            if (row.size() < 2) continue;
            const auto dt = parseDateTime(row.value(0));
            if (!dt.isValid()) continue;
            keys.append(dt.toSecsSinceEpoch());
            vals.append(row.value(1).toDouble());
        }
        series_->setData(keys, vals);
        plot_->rescaleAxes();
        plot_->replot();
    }

    QCustomPlot *plot() const override { return plot_; }

    void applyChartStyle(IChartStyle *style) override {
        setStyle(style);
        activeStyle_->configure(this);
        update();
        plot_->replot();
    }

    QCPBars *bars() const { return series_; }

private:
    QCPBars *series_ = nullptr;
};

class BarChartGraphFactory : public IGraphFactory {
public:
    std::unique_ptr<IGraphWidget> create(QWidget *parent = nullptr) override {
        return std::make_unique<BarChartGraph>(parent);
    }
};
