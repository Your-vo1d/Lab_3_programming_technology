#pragma once

#include "includes/qcustomplot/qcustomplot.h"
#include "src/Chart/Style/IChartStyle.h"
#include <QWidget>
#include <QDateTime>

class IGraphWidget : public QWidget {
    Q_OBJECT
public:
    using QWidget::QWidget;

    virtual void setData(const QList<QStringList> &rows) = 0;
    virtual ~IGraphWidget();

    virtual QDateTime      parseDateTime(const QString &raw);
    virtual void           enableZoomPan(QCustomPlot *plot);
    virtual void           setTitle(const QString &title);
    virtual void           setAxisLabels(const QString &x, const QString &y);
    virtual void           setLegendVisible(bool on);
    virtual void           setStyle(IChartStyle *st);
    virtual void           applyChartStyle(IChartStyle *) {}
    virtual QCustomPlot   *plot() const;
    virtual void           setupTimeAxis();

protected:
    QCustomPlot      *plot_          = nullptr;
    QCPTextElement   *titleNode_     = nullptr;
    IChartStyle      *activeStyle_   = nullptr;
};
