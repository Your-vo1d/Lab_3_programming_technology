#pragma once

#include "qpdfwriter.h"
#include "qwidget.h"
#include <QPainter>
#include <QPageLayout>
#include <QPageSize>
#include <QFileInfo>
#include <QDir>

// Экспортирует виджет графика в PDF-файл с масштабированием под страницу.
class PrintManager {
public:
    static bool exportToPdf(QWidget *widget,
                            const QString &filePath,
                            bool grayScale = false,
                            int  dpi       = 3000)
    {
        if (!widget || filePath.isEmpty())
            return false;

        QFileInfo fi(filePath);
        QDir().mkpath(fi.path());

        QPdfWriter writer(filePath);
        writer.setResolution(dpi);
        writer.setPageSize(QPagedPaintDevice::A4);

        // Альбомная ориентация
        {
            const QSizeF mmSize = writer.pageSizeMM();
            writer.setPageSizeMM(QSizeF(mmSize.height(), mmSize.width()));
        }

        const int pageW = writer.width();
        const int pageH = writer.height();
        if (pageW <= 0 || pageH <= 0)
            return false;

        QPainter painter(&writer);

        const QSize wSz = widget->size();
        if (wSz.isEmpty()) { painter.end(); return false; }

        const qreal sx    = qreal(pageW) / wSz.width();
        const qreal sy    = qreal(pageH) / wSz.height();
        const qreal scale = qMin(sx, sy);

        // Центрирование на странице
        const qreal ox = (pageW - wSz.width()  * scale) / 2.0;
        const qreal oy = (pageH - wSz.height() * scale) / 2.0;
        painter.translate(ox, oy);
        painter.scale(scale, scale);

        if (grayScale) {
            painter.setPen(Qt::black);
            painter.setBrush(Qt::NoBrush);
        }

        widget->render(&painter);
        painter.end();

        const QFileInfo out(filePath);
        return out.exists() && out.size() > 0;
    }
};
