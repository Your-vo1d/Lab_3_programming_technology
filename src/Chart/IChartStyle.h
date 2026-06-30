#pragma once

#include <QPainter>
class IGraphWidget;

// Стратегия визуального оформления графика.
class IChartStyle {
public:
    virtual ~IChartStyle() = default;

    // Вызывается в paintEvent для настройки QPainter
    virtual void apply(QPainter &painter, IGraphWidget *widget) const = 0;

    // Вызывается при смене/создании графика для его первоначальной настройки
    virtual void configure(IGraphWidget *widget) const = 0;

    // Цвет сектора для круговой диаграммы
    virtual QColor sliceColor(int idx, const QString &label, double val) const {
        Q_UNUSED(label); Q_UNUSED(val);
        return QColor::fromHsv((idx * 47) % 360, 255, 200);
    }
};
