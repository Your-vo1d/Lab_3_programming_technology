QT = core gui sql printsupport concurrent

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17 console
TEMPLATE += app


DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0  

SOURCES += \
        src/Chart/ChartManager.cpp \
        src/Chart/CustomGraphs/BarChartGraph.cpp \
        src/Chart/CustomGraphs/CustomPlotGraph.cpp \
        src/Chart/CustomGraphs/PieChartGraph.cpp \
        src/Chart/CustomGraphs/ScatterGraph.cpp \
        src/Chart/GraphTypeManager.cpp \
    src/Chart/IGraphWidget.cpp \
        src/Chart/Style/ColourChartStyle.cpp \
        src/Chart/Style/StyleManager.cpp \
        src/DataReaders/CsvReader.cpp \
        src/DataReaders/JsonReader.cpp \
        src/DataReaders/SqliteReader.cpp \
        src/IOC_Container.cpp \
        src/MainWindow.cpp \
        src/main.cpp \
        includes/qcustomplot/qcustomplot.cpp \

HEADERS += \
    src/Chart/ChartManager.h \
    src/Chart/CustomGraphs/BarChartGraph.h \
    src/Chart/CustomGraphs/ScatterGraph.h \
    src/Chart/CustomGraphs/CustomPlotGraph.h \
    src/Chart/CustomGraphs/PieChartGraph.h \
    src/Chart/GraphDisplayArea.h \
    src/Chart/GraphFactoryRegistry.h \
    src/Chart/GraphTypeManager.h \
    src/Chart/IGraphFactory.h \
    src/Chart/IGraphWidget.h \
    src/Chart/Style/ColourChartStyle.h \
    src/Chart/Style/GrayscaleChartStyle.h \
    src/Chart/Style/IChartStyle.h \
    src/Chart/Style/StyleManager.h \
    src/DataReaders/CsvReader.h \
    src/DataReaders/DataReader.h \
    src/DataReaders/DataReaderManager.h \
    src/ExportManager.h \
    src/IOC_Container.h \
    src/MainWindow.h \
    src/PrintManager.h \
    includes/qcustomplot/qcustomplot.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
