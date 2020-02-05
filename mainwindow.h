#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <vector>

#include "qwt_plot.h"
#include "qwt_plot_grid.h"
#include "qwt_legend.h"
#include "qwt_plot_curve.h"
#include "qwt_symbol.h"
#include "qwt_plot_marker.h"
#include "qwt_plot_panner.h"
#include "qwt_plot_picker.h"
#include "qwt_picker_machine.h"

#include "mymachine.h"

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
    void onAppend(const QPoint& pos);
    void onMove(const QPoint& pos);
    void onRemove(const QPoint& pos);
private:
    Ui::MainWindow *ui;
    QwtPlot *d_plot = nullptr;
    QwtPlotGrid *grid = nullptr;
    QwtPlotPicker *d_picker = nullptr;
    QwtPlotCurve *curve = nullptr;
    QwtPlotMarker *marker = nullptr;
    std::vector<QwtPlotMarker*> markers;
    std::vector<std::pair<double, double>> points;
};


#endif // MAINWINDOW_H
