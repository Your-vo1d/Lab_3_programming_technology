#pragma once

#include <QObject>
#include <QAction>
#include "src/Chart/GraphDisplayArea.h"

// Слушает экшены тулбара и применяет соответствующий стиль к активному графику.
class StyleManager : public QObject {
    Q_OBJECT
public:
    StyleManager(QAction *actColor, QAction *actGray,
                 GraphDisplayArea *display,
                 QObject *parent = nullptr);
};
