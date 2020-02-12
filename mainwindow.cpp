#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "utilities.h"

#include <iostream>
#include <QVector>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    plotPtr.reset(new QwtPlot(this));

    setCentralWidget(plotPtr.get());
    plotPtr->setTitle( "Qwt demonstration" );
    plotPtr->setCanvasBackground( Qt::white );

    plotPtr->setAxisTitle(QwtPlot::yLeft, "Y");
    plotPtr->setAxisTitle(QwtPlot::xBottom, "X");
    plotPtr->setAxisScale(QwtPlot::yLeft, 0, 10, 5);
    plotPtr->setAxisScale(QwtPlot::xBottom, 0, 10, 5);
    plotPtr->insertLegend( new QwtLegend() );

    gridPtr.reset(new QwtPlotGrid());

    gridPtr->setMajorPen(QPen( Qt::gray, 2 ));
    gridPtr->attach( plotPtr.get() );

    pickerPtr.reset(new QwtPlotPicker(
                                QwtPlot::xBottom,
                                QwtPlot::yLeft,
                                QwtPlotPicker::CrossRubberBand,
                                QwtPicker::ActiveOnly,
                                plotPtr->canvas())
                    );

    // changing default patterns for MouseSelect2 and MouseSelect5; for DragAndDeletePointMachine
    pickerPtr->setMousePattern(QwtEventPattern::MouseSelect2, Qt::RightButton);
    pickerPtr->setMousePattern(QwtEventPattern::MouseSelect5, Qt::LeftButton, Qt::ControlModifier);

    pickerPtr->setStateMachine( new DragAndDeletePointMachine());

    connect(pickerPtr.get(), SIGNAL(selected(const QPointF)), this, SLOT(onSelect(const QPointF)));
    connect(pickerPtr.get(), SIGNAL(moved(const QPoint)), this, SLOT(onMove(const QPoint)));
    connect(pickerPtr.get(), SIGNAL(removed(const QPoint)), this, SLOT(onRemove(const QPoint)));

}

void MainWindow::onSelect(const QPointF &pos) {
    addAndDrawPoint(pos);

    if (!initialPointToMove.isNull()) {
        auto pm = findAndGetPointAndMarker(initialPointToMove);
        removePointAndMarker(pm);
        Utilities::setNullPoint(initialPointToMove);
        plotPtr->replot();
    }
}

void MainWindow::onMove(const QPoint& pos) {
    if (initialPointToMove.isNull()) {
        initialPointToMove = QPointF(plotPtr->invTransform(QwtPlot::Axis::xBottom, pos.x()),
                              plotPtr->invTransform(QwtPlot::Axis::yLeft, pos.y()));
    }
}

void MainWindow::onRemove(const QPoint& pos) {
    PointMarker found = findAndGetPointAndMarker(pos, true);
    removePointAndMarker(found);

    plotPtr->replot();
}

PointMarker MainWindow::findAndGetPointAndMarker(QPointF posToFind, bool invert, double percision) {
    QPointF invertedPoint;
    if (invert) {
        invertedPoint = QPointF(plotPtr->invTransform(QwtPlot::Axis::xBottom, posToFind.x()),
                                plotPtr->invTransform(QwtPlot::Axis::yLeft, posToFind.y()));
    } else {
        invertedPoint = posToFind;
    }
    // find closest point from points
    qreal upperY = invertedPoint.y() + percision;
    qreal lowerY = invertedPoint.y() - percision;
    qreal upperX = invertedPoint.x() + percision;
    qreal lowerX = invertedPoint.x() - percision;

    auto pointPredicate = [&] (const QPointF point) -> bool {
        auto pX = point.x();
        auto pY = point.y();
        return (pX < upperX && pX > lowerX && pY < upperY && pY > lowerY);
    };

    auto foundPointIter = std::find_if(points.begin(),
                                  points.end(),
                                  pointPredicate);


    auto markerPredicate = [&] (const std::shared_ptr<QwtPlotMarker> m) -> bool {
        return (foundPointIter != points.end() && m->value() == *foundPointIter);
    };

    auto foundMarkerIter = std::find_if(markers.begin(),
                                        markers.end(),
                                        markerPredicate);

    return PointMarker(foundPointIter, foundMarkerIter);
}

void MainWindow::removePointAndMarker(PointMarker pm) {
    if (!points.empty() && pm.point != points.end()) points.erase(pm.point);
    if (!markers.empty() && pm.marker != markers.end()) {
        (*pm.marker)->detach();
        markers.erase(pm.marker);
    }
}

void MainWindow::addAndDrawPoint(const QPointF &pos) {
    points.push_back(pos);
    std::shared_ptr<QwtPlotMarker> marker(new QwtPlotMarker());
    marker->setValue(pos);
    marker->setSymbol(new QwtSymbol( QwtSymbol::Diamond, Qt::red, Qt::NoPen, QSize( 10, 10 ) ));
    marker->attach(plotPtr.get());
    markers.push_back(std::move(marker));


    plotPtr->replot();
    plotPtr->update();
}


MainWindow::~MainWindow()
{
    delete ui;
}

