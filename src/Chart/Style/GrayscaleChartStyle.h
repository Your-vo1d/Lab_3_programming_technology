#pragma once

#include <QPainter>
#include <QBrush>
#include <QPen>
#include <QColor>
#include <src/Chart/Style/IChartStyle.h>
#include <src/Chart/CustomGraphs/PieChartGraph.h>
#include <src/Chart/CustomGraphs/CustomPlotGraph.h>
#include <src/Chart/CustomGraphs/ScatterGraph.h>
#include <src/Chart/CustomGraphs/BarChartGraph.h>
#include <includes/qcustomplot/qcustomplot.h>

// Монохромный стиль: оттенки серого для всех типов графиков.
class GrayScaleChartStyle : public IChartStyle {
public:
    QColor sliceColor(int idx, const QString &, double) const override {
        const int gray = 220 - (idx * 12) % 170;
        return QColor(gray, gray, gray);
    }

    void apply(QPainter &painter, IGraphWidget *) const override {
        painter.setPen(QPen(Qt::black, 1));
    }

    void configure(IGraphWidget *w) const override {
        auto setGridAxes = [](QCustomPlot *p) {
            p->xAxis->grid()->setVisible(true);
            p->yAxis->grid()->setVisible(true);
            p->xAxis->grid()->setPen(QPen(Qt::gray, 1, Qt::DashLine));
            p->yAxis->grid()->setPen(QPen(Qt::gray, 1, Qt::DashLine));
            p->xAxis->setBasePen(QPen(Qt::darkGray, 1));
            p->yAxis->setBasePen(QPen(Qt::darkGray, 1));
            p->xAxis->setTickPen(QPen(Qt::darkGray, 1));
            p->yAxis->setTickPen(QPen(Qt::darkGray, 1));
            p->xAxis->setSubTickPen(QPen(Qt::lightGray, 1));
            p->yAxis->setSubTickPen(QPen(Qt::lightGray, 1));
        };

        if (auto *g = dynamic_cast<CustomPlotGraph *>(w)) {
            auto *p = g->plot();
            p->setBackground(QBrush(QColor(240, 240, 240)));
            p->graph(0)->setPen(QPen(Qt::darkGray, 2));
            setGridAxes(p);
            g->enableZoomPan(p);
        } else if (auto *s = dynamic_cast<ScatterGraph *>(w)) {
            auto *p = s->plot();
            p->setBackground(QBrush(QColor(240, 240, 240)));
            QCPScatterStyle sc(QCPScatterStyle::ssCircle, 8);
            sc.setPen(QPen(Qt::darkGray, 2));
            sc.setBrush(QBrush(Qt::lightGray));
            p->graph(0)->setLineStyle(QCPGraph::lsNone);
            p->graph(0)->setScatterStyle(sc);
            setGridAxes(p);
            s->enableZoomPan(p);
        } else if (auto *b = dynamic_cast<BarChartGraph *>(w)) {
            auto *p = b->plot();
            p->setBackground(QBrush(QColor(240, 240, 240)));
            b->bars()->setBrush(QBrush(QColor(200, 200, 200)));
            b->bars()->setPen(QPen(Qt::darkGray, 1));
            setGridAxes(p);
            b->enableZoomPan(p);
        } else if (auto *pie = dynamic_cast<PieChartGraph *>(w)) {
            pie->setAutoFillBackground(true);
            QPalette pal = pie->palette();
            pal.setColor(QPalette::Window, QColor(250, 250, 250));
            pie->setPalette(pal);
            pie->setContentsMargins(15, 15, 15, 15);
        }
    }
};
