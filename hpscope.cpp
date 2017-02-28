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
    progressBar = new QProgressBar(this);

    progressBar->setMaximumHeight(16);
    progressBar->setMaximumWidth(200);
    progressBar->setTextVisible(false);
    ui->statusBar->addPermanentWidget(progressBar,0);

    //((QMainWindow*)parent())->statusBar()->addPermanentWidget(progressBar, 0);
    //((QMainWindow*)parent())->statusBar()->showMessage(QString("Loading"));
    progressBar->setValue(0);
    progressBar->setMinimum(0);
    progressBar->setMaximum(100);
    hp54111d = new HP54111d();
    connect(hp54111d, SIGNAL(dataReady(QVector<QVector<double> >)), this, SLOT(onScopeDataComplete(QVector<QVector<double> >)));
    connect(hp54111d, SIGNAL(progress(int)), this, SLOT(onScopeProgress(int)));

    if(hp54111d->connected())
    {
        hp54111d->Stop();
        hp54111d->GetPreamble();
    }
    else
        ui->statusBar->showMessage("Not connected");
}


HpScope::~HpScope()
{
    ui->statusBar->clearMessage();
    ui->statusBar->removeWidget(progressBar);
    disconnect(hp54111d, SIGNAL(dataReady(QVector<QVector<double> >)), this, SLOT(onScopeDataComplete(QVector<QVector<double> >)));
    disconnect(hp54111d, SIGNAL(progress(int)), this, SLOT(onScopeProgress(int)));
    delete ui;
    hp54111d->abort();
    hp54111d->wait();
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
    progressBar->setValue(0);
    if(hp54111d->connected())
    {
        ui->statusBar->showMessage("Measuring");
        hp54111d->Stop();
        hp54111d->setChannel(1);

        hp54111d->Digitize();

        hp54111d->start();

    }
    else
        ui->statusBar->showMessage("Not connected");
}


void HpScope::onScopeDataComplete(QVector<QVector<double> > data)
{
    ui->statusBar->showMessage("Data taking complete");
    Plot(data);
    hp54111d->Run();
    hp54111d->SetLocal();
}

void HpScope::onScopeProgress(int progress)
{
    progressBar->setValue(progress);
}

void HpScope::OnScopeDisconnected()
{
    ui->statusBar->showMessage("Not connected");
}
