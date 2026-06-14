#include <QSplitter>

#include "MainWindow.h"
#include "src/Chart/Style/StyleManager.h"
#include <includes/qcustomplot/qcustomplot.h>
#include <QLayout>
#include <src/IOC_Container.h>
#include <src/DataReaders/DataReader.h>
#include <src/Chart/IGraphFactory.h>
#include <src/Chart/ChartManager.h>
#include <src/Chart/Style/IChartStyle.h>
#include <src/PrintManager.h>
#include <QtConcurrent/QtConcurrent>
#include <src/Chart/GraphFactoryRegistry.h>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    this->showMaximized();
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    setWindowTitle("Chart Viewer");

    QSplitter *splitter = new QSplitter(centralWidget);

    QToolBar *toolbar = addToolBar("Панель инструментов");
    toolbar->setContextMenuPolicy(Qt::NoContextMenu);

    QAction *printAct = toolbar->addAction("Печать в PDF");
    connect(printAct, &QAction::triggered, this, &MainWindow::onPrint);

    QAction *propsAct = toolbar->addAction("Настройки осей");
    connect(propsAct, &QAction::triggered, this, &MainWindow::onEditGraphProperties);

    QAction *batchAct = toolbar->addAction("Пакетный экспорт");
    connect(batchAct, &QAction::triggered, this, &MainWindow::onBatchExport);

    QAction *colorAct = toolbar->addAction("Цветной");
    QAction *grayAct  = toolbar->addAction("Ч/Б");
    colorAct->setEnabled(false);
    grayAct->setEnabled(false);

    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->addWidget(splitter);

    graphArea_ = new GraphDisplayArea(centralWidget);
    new StyleManager(colorAct, grayAct, graphArea_, this);
    typeSwitcher_ = std::make_unique<GraphTypeManager>(toolbar, graphArea_, this);
    typeSwitcher_->renderCurrentType(graphArea_, nullptr);

    graphArea_->setMinimumSize(800, 600);

    // Файловый браузер
    fsModel_ = new QFileSystemModel(this);
    fsModel_->setRootPath(QDir::homePath());

    QTreeView *tree = new QTreeView;
    tree->setModel(fsModel_);
    tree->setRootIndex(fsModel_->index(QDir::homePath()));
    tree->setColumnWidth(0, 250);

    QLineEdit  *pathEdit = new QLineEdit(QDir::homePath());
    QPushButton *upBtn   = new QPushButton("↑", centralWidget);

    connect(pathEdit, &QLineEdit::returnPressed, [tree, pathEdit, this]() {
        const QString p = pathEdit->text();
        const QModelIndex idx = fsModel_->index(p);
        if (idx.isValid()) tree->setRootIndex(idx);
    });

    connect(upBtn, &QPushButton::pressed, [tree, pathEdit, this]() {
        QDir d(pathEdit->text());
        if (d.cdUp()) {
            const QString np = d.absolutePath();
            pathEdit->setText(np);
            const QModelIndex idx = fsModel_->index(np);
            if (idx.isValid()) tree->setRootIndex(idx);
        }
    });

    connect(tree, &QTreeView::clicked, [tree, pathEdit, this](const QModelIndex &idx) {
        const QString p = fsModel_->filePath(idx);
        if (QFileInfo(p).isDir()) {
            tree->setRootIndex(idx);
            pathEdit->setText(p);
        }
    });

    QWidget    *browserWidget = new QWidget;
    QVBoxLayout *bvLayout = new QVBoxLayout;
    QHBoxLayout *bhLayout = new QHBoxLayout;
    bhLayout->addWidget(upBtn);
    bhLayout->addWidget(pathEdit);
    bvLayout->addLayout(bhLayout);
    bvLayout->addWidget(tree);
    browserWidget->setLayout(bvLayout);

    splitter->addWidget(browserWidget);
    splitter->addWidget(graphArea_);

    connect(tree, &QTreeView::doubleClicked, this,
            [this, colorAct, grayAct](const QModelIndex &idx) {
        const QString path = fsModel_->filePath(idx);
        if (!QFileInfo(path).isFile()) return;

        auto reader = ChartManager::createReader(path);
        if (!reader) {
            QMessageBox::critical(this, "Ошибка",
                                  "Не удалось подобрать DataReader для\n" + path);
            return;
        }

        auto *progress = new QProgressDialog("Загрузка...", QString(), 0, 0, this);
        progress->setWindowModality(Qt::WindowModal);
        progress->setCancelButton(nullptr);
        progress->show();
        QApplication::processEvents();

        auto future = QtConcurrent::run([reader, path]() {
            return reader->read(path);
        });

        auto *watcher = new QFutureWatcher<QList<QStringList> *>(this);
        connect(watcher, &QFutureWatcher<QList<QStringList> *>::finished, this,
                [this, progress, watcher]() {
            progress->close();
            progress->deleteLater();

            QList<QStringList> *result = watcher->result();
            watcher->deleteLater();

            if (!result) {
                QMessageBox::critical(this, "Ошибка", "Не удалось прочитать данные");
                return;
            }

            dataRows_.reset(result);
            typeSwitcher_->renderCurrentType(graphArea_, result);
        });
        watcher->setFuture(future);

        colorAct->setEnabled(true);
        grayAct->setEnabled(true);
    });

    splitter->setCollapsible(0, false);
    splitter->setCollapsible(1, false);
}

void MainWindow::onPrint()
{
    IGraphWidget *w = graphArea_->graph();
    if (!w) {
        QMessageBox::warning(this, "Печать", "Нет графика для печати");
        return;
    }

    const QString defPath = QDir::homePath() + "/chart.pdf";
    QString file = QFileDialog::getSaveFileName(
        this, "Сохранить в PDF", defPath, "PDF (*.pdf)");
    if (file.isEmpty()) return;
    if (!file.endsWith(".pdf", Qt::CaseInsensitive)) file += ".pdf";

    const bool gray = (QMessageBox::question(
        this, "Режим", "Печатать в чёрно-белом?",
        QMessageBox::Yes | QMessageBox::No, QMessageBox::No) == QMessageBox::Yes);

    if (!PrintManager::exportToPdf(w, file, gray))
        QMessageBox::critical(this, "Ошибка", "Не удалось сохранить PDF:\n" + file);
    else
        statusBar()->showMessage("Сохранено: " + file, 3000);
}


void MainWindow::onEditGraphProperties()
{
    IGraphWidget *w = graphArea_->graph();
    if (!w) return;

    bool ok;
    const QString title = QInputDialog::getText(
        this, "Заголовок", "Введите заголовок графика:", QLineEdit::Normal, "", &ok);
    if (ok) w->setTitle(title);

    const QString xLabel = QInputDialog::getText(
        this, "Ось X", "Подпись оси X:", QLineEdit::Normal, "", &ok);
    const QString yLabel = QInputDialog::getText(
        this, "Ось Y", "Подпись оси Y:", QLineEdit::Normal, "", &ok);
    if (ok) w->setAxisLabels(xLabel, yLabel);

    const bool showLegend = (QMessageBox::question(
        this, "Легенда", "Показать легенду?",
        QMessageBox::Yes | QMessageBox::No, QMessageBox::No) == QMessageBox::Yes);
    w->setLegendVisible(showLegend);

    graphArea_->update();
}

void MainWindow::onBatchExport()
{
    const QString inDir = QFileDialog::getExistingDirectory(
        this, "Папка с данными", QDir::homePath());
    if (inDir.isEmpty()) return;

    const QString outDir = QFileDialog::getExistingDirectory(
        this, "Папка для результатов", QDir::homePath());
    if (outDir.isEmpty()) return;

    bool ok;

    QStringList types = GraphFactoryRegistry::instance().availableTypes();
    types.prepend("Auto");
    const QString gtype = QInputDialog::getItem(
        this, "Тип графика", "Тип (Auto = по данным):", types, 0, false, &ok);
    if (!ok) return;

    const bool gray = (QMessageBox::question(
        this, "Режим", "Экспортировать в ч/б?",
        QMessageBox::Yes | QMessageBox::No, QMessageBox::No) == QMessageBox::Yes);

    if (gtype.compare("Auto", Qt::CaseInsensitive) != 0)
        typeSwitcher_->setType(gtype);

    runBatchExport(inDir, outDir, gray);
}

void MainWindow::runBatchExport(const QString &inDir,
                                const QString &outDir,
                                bool grayScale)
{
    QStringList files;
    QDirIterator it(inDir, QDir::Files, QDirIterator::Subdirectories);
    while (it.hasNext()) files << it.next();

    if (files.isEmpty()) {
        QMessageBox::information(this, "Пакетный экспорт", "Файлы не найдены.");
        return;
    }

    QProgressDialog dlg("Экспорт в PDF...", "Отмена", 0, files.size(), this);
    dlg.setWindowModality(Qt::WindowModal);
    dlg.show();

    const QSize exportSize = graphArea_->size();

    for (int i = 0; i < files.size(); ++i) {
        if (dlg.wasCanceled()) break;

        const QString &fp = files.at(i);
        auto reader = ChartManager::createReader(fp);
        if (!reader) { dlg.setValue(i + 1); QApplication::processEvents(); continue; }

        auto *rows = reader->read(fp);
        if (!rows) { dlg.setValue(i + 1); QApplication::processEvents(); continue; }

        GraphDisplayArea tmpArea;
        tmpArea.resize(exportSize);
        typeSwitcher_->renderCurrentType(&tmpArea, rows);

        const QString outFile = outDir + "/" + QFileInfo(fp).completeBaseName() + ".pdf";
        PrintManager::exportToPdf(&tmpArea, outFile, grayScale);

        delete rows;
        dlg.setValue(i + 1);
        QApplication::processEvents();
    }

    dlg.close();
    QMessageBox::information(this, "Пакетный экспорт", "Экспорт завершён.");
}

MainWindow::~MainWindow() {}
