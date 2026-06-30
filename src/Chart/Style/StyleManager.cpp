// src/Chart/Style/StyleManager.cpp
#include "StyleManager.h"

#include <QDebug>
#include "src/Chart/GraphDisplayArea.h"
#include "ColourChartStyle.h"
#include "GrayscaleChartStyle.h"

StyleManager::StyleManager(QAction *actColor, QAction *actGray,
                           GraphDisplayArea *display,
                           QObject *parent)
    : QObject(parent)
{
    connect(actColor, &QAction::triggered, this, [display]() {
        IGraphWidget *w = display->graph();
        if (!w) { qDebug() << "[StyleManager] No active graph"; return; }
        w->applyChartStyle(new ColourChartStyle);
    });

    connect(actGray, &QAction::triggered, this, [display]() {
        IGraphWidget *w = display->graph();
        if (!w) { qDebug() << "[StyleManager] No active graph"; return; }
        w->applyChartStyle(new GrayScaleChartStyle);
    });
}
