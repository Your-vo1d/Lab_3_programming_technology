#pragma once

#include <QPainter>
#include <QBrush>
#include <QPen>
#include <QColor>
#include <QPalette>
#include "src/Chart/Style/IChartStyle.h"

class CustomPlotGraph;
class ScatterGraph;
class BarChartGraph;
class PieChartGraph;

#include <includes/qcustomplot/qcustomplot.h>

// Цветной стиль: яркие цвета для всех типов графиков.
class ColourChartStyle : public IChartStyle {
public:
    QColor sliceColor(int idx, const QString &, double) const override;
    void apply(QPainter &painter, IGraphWidget *) const override;
    void configure(IGraphWidget *w) const override;
};
