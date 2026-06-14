#pragma once

#include "src/Chart/Style/ColourChartStyle.h"
#include "src/Chart/CustomGraphs/CustomPlotGraph.h"
#include "src/Chart/CustomGraphs/ScatterGraph.h"
#include "src/Chart/CustomGraphs/BarChartGraph.h"
#include "src/Chart/CustomGraphs/PieChartGraph.h"

QColor ColourChartStyle::sliceColor(int idx, const QString &, double) const
{
    static const QVector<int> palette = { 0, 30, 60, 120, 180, 210, 270, 300 };
    return QColor::fromHsv(palette.at(idx % palette.size()), 200, 220);
}

void ColourChartStyle::apply(QPainter &painter, IGraphWidget *) const
{
    painter.setPen(QPen(Qt::black, 1));
}

void ColourChartStyle::configure(IGraphWidget *w) const
{
    if (auto *g = dynamic_cast<CustomPlotGraph *>(w)) {
        auto *p = g->plot();
        p->setBackground(QBrush(Qt::white));
        p->graph(0)->setPen(QPen(Qt::blue, 1));
        p->graph(0)->setBrush(Qt::NoBrush);
        p->xAxis->setBasePen(QPen(Qt::black));
        p->yAxis->setBasePen(QPen(Qt::black));
        g->enableZoomPan(p);
    } else if (auto *s = dynamic_cast<ScatterGraph *>(w)) {
        auto *p = s->plot();
        p->setBackground(QBrush(Qt::white));
        QCPScatterStyle sc(QCPScatterStyle::ssCircle, Qt::blue, Qt::blue, 6);
        sc.setBrush(Qt::NoBrush);
        sc.setPen(QPen(Qt::blue, 1));
        p->graph(0)->setLineStyle(QCPGraph::lsNone);
        p->graph(0)->setScatterStyle(sc);
        p->xAxis->setBasePen(QPen(Qt::black));
        p->yAxis->setBasePen(QPen(Qt::black));
        s->enableZoomPan(p);
    } else if (auto *b = dynamic_cast<BarChartGraph *>(w)) {
        auto *p = b->plot();
        p->setBackground(QBrush(Qt::white));
        b->bars()->setBrush(QBrush(Qt::blue));
        b->bars()->setPen(QPen(Qt::black));
        p->xAxis->setBasePen(QPen(Qt::black));
        p->yAxis->setBasePen(QPen(Qt::black));
        b->enableZoomPan(p);
    } else if (auto *pie = dynamic_cast<PieChartGraph *>(w)) {
        pie->setAutoFillBackground(true);
        QPalette pal = pie->palette();
        pal.setColor(QPalette::Window, Qt::white);
        pie->setPalette(pal);
        pie->setContentsMargins(10, 10, 10, 10);
    }
}
