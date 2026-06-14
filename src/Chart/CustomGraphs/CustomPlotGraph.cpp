#pragma once
#include <src/Chart/CustomGraphs/CustomPlotGraph.h>
#include <QVBoxLayout>

CustomPlotGraph::CustomPlotGraph(QWidget *parent)
    : IGraphWidget(parent)
{
    plot_ = new QCustomPlot(this);
    plot_->addGraph();
    setupTimeAxis();

    auto *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(plot_);

    enableZoomPan(plot_);
}

QCustomPlot *CustomPlotGraph::plot() const { return plot_; }

void CustomPlotGraph::setData(const QList<QStringList> &rows)
{
    QVector<double> xs, ys;
    xs.reserve(rows.size());
    ys.reserve(rows.size());

    for (const auto &row : rows) {
        if (row.size() < 2) continue;
        const QDateTime dt = parseDateTime(row.value(0));
        if (!dt.isValid()) continue;
        xs.append(dt.toSecsSinceEpoch());
        ys.append(row.value(1).toDouble());
    }

    plot_->graph(0)->setData(xs, ys);
    plot_->rescaleAxes();
    plot_->replot();
}

REGISTER_GRAPH_FACTORY("Line", CustomPlotGraph)
