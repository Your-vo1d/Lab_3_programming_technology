// GraphTypeManager.h
#pragma once

#include <QObject>
#include <vector>
#include <functional>
#include <memory>

class QToolBar;
class QComboBox;
class GraphDisplayArea;
class IGraphWidget;
class MainWindow;

// Управляет выбором типа графика через комбо-бокс на тулбаре.
class GraphTypeManager : public QObject {
    Q_OBJECT
public:
    GraphTypeManager(QToolBar *toolbar,
                     GraphDisplayArea *display,
                     MainWindow *win,
                     QObject *parent = nullptr);

    // Перерисовывает граф в display, используя текущий тип и переданные данные
    void renderCurrentType(GraphDisplayArea *display,
                           const QList<QStringList> *rows);

    void setType(const QString &name);

private slots:
    void onComboChanged(int index);

private:
    GraphDisplayArea *canvas_   = nullptr;
    MainWindow       *win_      = nullptr;
    QComboBox        *picker_   = nullptr;

    std::vector<std::function<std::unique_ptr<IGraphWidget>(QWidget *)>> builders_;
};
