// ExportManager.h
#pragma once

#include <QWidget>
#include <QPixmap>

// Сохраняет содержимое виджета в растровый файл (PNG, JPEG и т.д.).
class ExportManager {
public:
    static bool exportToImage(QWidget *widget,
                              const QString &filePath,
                              const char *format = "PNG")
    {
        if (!widget) return false;

        QPixmap px(widget->size());
        widget->render(&px);
        return px.save(filePath, format);
    }
};
