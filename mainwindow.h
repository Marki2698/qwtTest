#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <vector>
#include <algorithm>
#include <memory>

#include "qwt_plot.h"
#include "qwt_plot_grid.h"
#include "qwt_legend.h"
#include "qwt_plot_curve.h"
#include "qwt_symbol.h"
#include "qwt_plot_marker.h"
#include "qwt_plot_panner.h"
#include "qwt_plot_picker.h"
#include "qwt_picker_machine.h"

#include "drag_and_delete_point_machine.h"

struct PointMarker {
    using PointIter = std::vector<QPointF>::iterator;
    using MarkerIter = std::vector<std::shared_ptr<QwtPlotMarker>>::iterator;
    PointIter point;
    MarkerIter marker;
    PointMarker(PointIter p, MarkerIter m): point(p), marker(m) {}
    ~PointMarker() {}
};


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void onSelect(const QPointF &pos);
    void onMove(const QPoint& pos);
    void onRemove(const QPoint& pos);
private:
    Ui::MainWindow *ui;
    std::shared_ptr<QwtPlot> plotPtr = nullptr;
    std::unique_ptr<QwtPlotGrid> gridPtr = nullptr;
    std::unique_ptr<QwtPlotPicker> pickerPtr = nullptr;
    std::vector<std::shared_ptr<QwtPlotMarker>> markers;
    std::vector<QPointF> points;
    QPointF initialPointToMove = QPointF();

    PointMarker findAndGetPointAndMarker(QPointF posToFind, bool invert = false, double percision = 0.2);
    void removePointAndMarker(PointMarker pm);
    void addAndDrawPoint(const QPointF& pos);
};


#endif // MAINWINDOW_H
