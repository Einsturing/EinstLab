#ifndef WIDGET_H
#define WIDGET_H

#include <string>

#include <QDebug>
#include <QtMath>
#include <QVector>
#include <QWidget>

#include <qwt_plot.h>
#include <qwt_legend.h>
#include <qwt_plot_grid.h>
#include <qwt_plot_curve.h>
#include <qwt_point_data.h>
#include <qwt_plot_panner.h>
#include <qwt_plot_marker.h>
#include <qwt_plot_magnifier.h>

using namespace std;

namespace Ui
{
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

    bool priority(char lhs,char rhs);
    string inPrefix2postPrefix(string str);
    double operate(double first, double second, char op);
    double operate(double x, char op);
    double calculateByPostPrefix(string input,double x);

private slots:
    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

private:
    Ui::Widget *ui;

    QVector<double> xs;
    QVector<double> ys;

    QwtPlotCurve curve;
    QwtPlotGrid* grid;
    QwtPlotMarker* mk1;
    QwtPlotMarker* mk2;

    int flag=1;
    int isgrid=1;
};

#endif // WIDGET_H
