#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <QVector>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    d_plot = new QwtPlot( this );

    setCentralWidget(d_plot);
    d_plot->setTitle( "Qwt demonstration" );
        d_plot->setCanvasBackground( Qt::white );

        d_plot->setAxisTitle(QwtPlot::yLeft, "Y");
        d_plot->setAxisTitle(QwtPlot::xBottom, "X");
        d_plot->setAxisScale(QwtPlot::yLeft, 0, 10, 5);
        d_plot->setAxisScale(QwtPlot::xBottom, 0, 10, 5);
        d_plot->insertLegend( new QwtLegend() );

        grid = new QwtPlotGrid();
        grid->setMajorPen(QPen( Qt::gray, 2 ));
        grid->attach( d_plot );

        d_picker = new QwtPlotPicker(
                                    QwtPlot::xBottom,
                                    QwtPlot::yLeft,
                                    QwtPlotPicker::CrossRubberBand,
                                    QwtPicker::ActiveOnly,
                                    d_plot->canvas() );

            // changing default patterns for MouseSelect2 and MouseSelect5; for MyMachine
            //d_picker->setMousePattern(QwtEventPattern::MouseSelect2, Qt::RightButton);
            //d_picker->setMousePattern(QwtEventPattern::MouseSelect5, Qt::LeftButton, Qt::ControlModifier);

            // should call MyMachine() but unfortunately I have not done necessary tasks except first two steps
            //d_picker->setStateMachine( new MyMachine());
            d_picker->setStateMachine( new QwtPickerDragPointMachine());

            connect(d_picker, SIGNAL(selected(const QPointF)), this, SLOT(onSelect(const QPointF)));
            connect(d_picker, SIGNAL(appended(const QPoint)), this, SLOT(onAppend(const QPoint)));
            connect(d_picker, SIGNAL(moved(const QPoint)), this, SLOT(onMove(const QPoint)));
            connect(d_picker, SIGNAL(removed(const QPoint)), this, SLOT(onRemove(const QPoint)));

}

void MainWindow::onSelect(const QPointF &pos) {
    points.push_back({pos.x(), pos.y()});
    auto marker = new QwtPlotMarker();
    marker->setValue(points.back().first, points.back().second);
    marker->setSymbol(new QwtSymbol( QwtSymbol::Diamond, Qt::red, Qt::NoPen, QSize( 10, 10 ) ));
    marker->attach(d_plot);
    markers.push_back(std::move(marker));
    markers.back()->attach(d_plot);

    d_plot->replot();
    d_plot->update();
}

void MainWindow::onMove(const QPoint& pos) {}

void MainWindow::onAppend(const QPoint& pos) {}

void MainWindow::onRemove(const QPoint& pos) {}


MainWindow::~MainWindow()
{
    if (!markers.empty()) for (auto i : markers) delete i;
    delete grid;
    delete d_picker;
    delete d_plot;
    delete ui;
}

