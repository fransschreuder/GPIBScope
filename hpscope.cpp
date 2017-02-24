#include "hpscope.h"
#include "ui_hpscope.h"
#include <QThread>

HpScope::HpScope(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::HpScope)
{
    ui->setupUi(this);

    //demoName = "Simple Demo";

    // add two new graphs and set their look:
    ui->plot->addGraph();
    ui->plot->graph(0)->setPen(QPen(Qt::blue)); // line color blue for first graph
    ui->plot->graph(0)->setBrush(QBrush(QColor(0, 0, 255, 20))); // first graph will be filled with translucent blue
    // generate some points of data (y0 for first, y1 for second graph):
    // configure right and top axis to show ticks but no labels:
    // (see QCPAxisRect::setupFullAxesBox for a quicker method to do this)

    // make left and bottom axes always transfer their ranges to right and top axes:
    //connect(ui->plot->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->plot->xAxis2, SLOT(setRange(QCPRange)));
    //connect(ui->plot->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->plot->yAxis2, SLOT(setRange(QCPRange)));
    ui->plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);


    hp54111d = new HP54111d();

    hp54111d->Stop();

    hp54111d->GetPreamble();
}


HpScope::~HpScope()
{

    delete ui;
    delete hp54111d;
}

void HpScope::Plot(QVector<QVector<double> > dataPoints)
{
      ui->plot->graph(0)->setData(dataPoints[0], dataPoints[1], true);
      ui->plot->graph(0)->rescaleAxes();
      QCPRange range = ui->plot->xAxis->range();
      range /= 10;
      ui->plot->xAxis->setRange(range);
      ui->plot->replot();
}


void HpScope::on_actionTake_data_triggered()
{
    hp54111d->Stop();
    hp54111d->Digitize("1");
    QVector<QVector<double> > dataPoints = hp54111d->GetData(1);
    Plot(dataPoints);
}
