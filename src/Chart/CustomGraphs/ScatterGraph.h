#pragma once

#include <src/Chart/IGraphWidget.h>
#include <includes/qcustomplot/qcustomplot.h>
#include <src/Chart/IGraphFactory.h>
#include <src/Chart/GraphFactoryRegistry.h>
#include <QVBoxLayout>

// Точечный (scatter) граф с временной осью X.
class ScatterGraph : public IGraphWidget {
    Q_OBJECT
public:
    explicit ScatterGraph(QWidget *parent = nullptr)
        : IGraphWidget(parent)
    {
        plot_ = new QCustomPlot(this);
        plot_->addGraph();
        setupTimeAxis();

        plot_->graph(0)->setLineStyle(QCPGraph::lsNone);
        plot_->graph(0)->setScatterStyle(QCPScatterStyle::ssCircle);

        enableZoomPan(plot_);

        auto *lay = new QVBoxLayout(this);
        lay->setContentsMargins(0, 0, 0, 0);
        lay->addWidget(plot_);
    }

    void setData(const QList<QStringList> &rows) override {
        QVector<double> xs, ys;
        for (const auto &row : rows) {
            if (row.size() < 2) continue;
            const auto dt = parseDateTime(row.value(0));
            if (!dt.isValid()) continue;
            xs.append(dt.toSecsSinceEpoch());
            ys.append(row.value(1).toDouble());
        }
        plot_->graph(0)->setData(xs, ys);
        plot_->rescaleAxes();
        plot_->replot();
    }

    void applyChartStyle(IChartStyle *style) override {
        setStyle(style);
        activeStyle_->configure(this);
        update();
        plot_->replot();
    }

    QCustomPlot *plot() const override { return plot_; }
};

class ScatterGraphFactory : public IGraphFactory {
public:
    std::unique_ptr<IGraphWidget> create(QWidget *parent = nullptr) override {
        return std::make_unique<ScatterGraph>(parent);
    }
};
