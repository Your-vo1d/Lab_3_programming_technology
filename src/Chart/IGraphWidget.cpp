#pragma once
#include "src/Chart/IGraphWidget.h"

IGraphWidget::~IGraphWidget() {}

QDateTime IGraphWidget::parseDateTime(const QString &raw)
{
    // Полный формат «дата время»
    QDateTime dt = QDateTime::fromString(raw, "dd.MM.yyyy HH:mm");
    dt.setTimeSpec(Qt::UTC);
    if (dt.isValid())
        return dt;

    // «дата минуты» — вторая часть может быть числом минут
    const QStringList parts = raw.trimmed().split(QRegExp("\\s+"));
    if (parts.size() >= 2) {
        const QDate date = QDate::fromString(parts.at(0), "dd.MM.yyyy");
        bool ok = false;
        const int mins = parts.at(1).toInt(&ok);
        if (ok && date.isValid())
            return QDateTime(date, QTime(0, 0).addSecs(mins * 60), Qt::UTC);
    }

    // Только дата
    const QDate dateOnly = QDate::fromString(raw.left(10), "dd.MM.yyyy");
    if (dateOnly.isValid())
        return QDateTime(dateOnly, QTime(0, 0), Qt::UTC);

    return {};
}

void IGraphWidget::enableZoomPan(QCustomPlot *plot)
{
    plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    plot->axisRect()->setRangeDrag(Qt::Horizontal | Qt::Vertical);
    plot->axisRect()->setRangeZoom(Qt::Horizontal | Qt::Vertical);

    connect(plot, &QCustomPlot::mouseWheel, this, [=](QWheelEvent *ev) {
        const auto mods = ev->modifiers();
        if (mods & Qt::ShiftModifier)
            plot->axisRect()->setRangeZoomAxes(plot->xAxis, nullptr);
        else if (mods & Qt::ControlModifier)
            plot->axisRect()->setRangeZoomAxes(nullptr, plot->yAxis);
        else
            plot->axisRect()->setRangeZoomAxes(plot->xAxis, plot->yAxis);
    });
}

void IGraphWidget::setTitle(const QString &title)
{
    if (!titleNode_) {
        plot_->plotLayout()->insertRow(0);
        titleNode_ = new QCPTextElement(plot_, title, QFont("sans", 12, QFont::Bold));
        plot_->plotLayout()->addElement(0, 0, titleNode_);
    } else {
        titleNode_->setText(title);
    }
    plot_->replot();
}

void IGraphWidget::setAxisLabels(const QString &x, const QString &y)
{
    plot_->xAxis->setLabel(x);
    plot_->yAxis->setLabel(y);
}

void IGraphWidget::setLegendVisible(bool on)
{
    plot_->legend->setVisible(on);
    if (on)
        plot_->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop | Qt::AlignRight);
}

void IGraphWidget::setStyle(IChartStyle *st)
{
    activeStyle_ = st;
}

QCustomPlot *IGraphWidget::plot() const { return plot_; }

void IGraphWidget::setupTimeAxis()
{
    QSharedPointer<QCPAxisTickerDateTime> ticker(new QCPAxisTickerDateTime);
    ticker->setDateTimeFormat("dd.MM.yyyy HH:mm");
    plot_->xAxis->setTicker(ticker);
    plot_->xAxis->setLabel("Дата и время");
    plot_->yAxis->setLabel("Значение");
}
