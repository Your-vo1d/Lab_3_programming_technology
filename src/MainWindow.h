#pragma once

#include "src/Chart/GraphTypeManager.h"
#include <QMainWindow>
#include <QTextCodec>
#include <QFrame>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QWidget>
#include <src/Chart/GraphDisplayArea.h>
#include <src/DataReaders/DataReader.h>

QT_BEGIN_NAMESPACE
    namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT
protected:
    QWidget *centralWidget;

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    const QList<QStringList> *currentRows() const { return dataRows_.get(); }

private slots:
    void onPrint();
    void onEditGraphProperties();
    void onBatchExport();

private:
    void runBatchExport(const QString &inDir,
                        const QString &outDir,
                        bool grayScale);

    GraphDisplayArea                   *graphArea_;
    QFileSystemModel                   *fsModel_;
    std::unique_ptr<QList<QStringList>> dataRows_;
    std::unique_ptr<GraphTypeManager>   typeSwitcher_;
};
