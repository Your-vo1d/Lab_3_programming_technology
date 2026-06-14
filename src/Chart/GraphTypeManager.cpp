// src/Chart/GraphTypeManager.cpp
#include "src/Chart/GraphTypeManager.h"

#include <QComboBox>
#include <QToolBar>
#include <QDebug>

#include "src/Chart/GraphFactoryRegistry.h"
#include "src/Chart/GraphDisplayArea.h"
#include "src/MainWindow.h"

GraphTypeManager::GraphTypeManager(QToolBar *toolbar,
                                   GraphDisplayArea *display,
                                   MainWindow *win,
                                   QObject *parent)
    : QObject(parent)
    , canvas_(display)
    , win_(win)
{
    picker_ = new QComboBox;
    toolbar->addWidget(picker_);

    for (const auto &e : GraphFactoryRegistry::instance().entries()) {
        picker_->addItem(e.name);
        builders_.push_back(e.fn);
    }

    connect(picker_, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &GraphTypeManager::onComboChanged);
}

void GraphTypeManager::setType(const QString &name)
{
    const int idx = picker_->findText(name, Qt::MatchFixedString);
    if (idx >= 0) {
        picker_->blockSignals(true);
        picker_->setCurrentIndex(idx);
        picker_->blockSignals(false);
        renderCurrentType(canvas_, win_->currentRows());
    }
}

void GraphTypeManager::onComboChanged(int)
{
    renderCurrentType(canvas_, win_->currentRows());
}

void GraphTypeManager::renderCurrentType(GraphDisplayArea *display,
                                         const QList<QStringList> *rows)
{
    const int idx = picker_->currentIndex();
    auto widget = builders_[idx](display);
    if (!widget) {
        qWarning() << "[GraphTypeManager] No builder for index" << idx;
        return;
    }

    if (rows && !rows->isEmpty())
        widget->setData(*rows);

    display->setGraph(std::move(widget));
}
