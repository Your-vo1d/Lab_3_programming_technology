#pragma once

#include <QPainter>
#include <QPaintEvent>
#include <QMap>
#include <QString>
#include "src/Chart/IGraphWidget.h"
#include "src/Chart/IGraphFactory.h"
#include "src/Chart/Style/ColourChartStyle.h"

// Круговая диаграмма с настраиваемой цветовой стратегией.
class PieChartGraph : public IGraphWidget {
public:
    explicit PieChartGraph(QWidget *parent = nullptr)
        : IGraphWidget(parent)
    {
        auto *def = new ColourChartStyle;
        setStyle(def);
        def->configure(this);
    }

    void setData(const QList<QStringList> &rows) override
    {
        segments_.clear();
        for (const auto &row : rows) {
            if (row.size() < 2) continue;
            bool ok = false;
            double v = row.value(1).toDouble(&ok);
            if (ok) segments_[row.value(0)] += v;
        }
        update();
    }

    void applyChartStyle(IChartStyle *style) override {
        setStyle(style);
        style->configure(this);
        update();
    }

protected:
    void paintEvent(QPaintEvent *event) override
    {
        QPainter p(this);
        p.setRenderHint(QPainter::Antialiasing);
        if (activeStyle_) activeStyle_->apply(p, this);

        const QRectF area = rect().adjusted(10, 10, -10, -10);
        double total = 0.0;
        for (double v : segments_.values()) total += v;

        if (total <= 0.0) {
            QWidget::paintEvent(event);
            return;
        }

        double angle = 0.0;
        int idx = 0;
        for (auto it = segments_.constBegin(); it != segments_.constEnd(); ++it, ++idx) {
            const double span = 360.0 * it.value() / total;
            const QColor c = activeStyle_
                ? activeStyle_->sliceColor(idx, it.key(), it.value())
                : QColor::fromHsv((idx * 47) % 360, 255, 200);
            p.setBrush(c);
            p.drawPie(area, static_cast<int>(angle * 16), static_cast<int>(span * 16));
            angle += span;
        }

        QWidget::paintEvent(event);
    }

private:
    QMap<QString, double> segments_;
};

class PieChartGraphFactory : public IGraphFactory {
public:
    std::unique_ptr<IGraphWidget> create(QWidget *parent = nullptr) override {
        return std::make_unique<PieChartGraph>(parent);
    }
};
