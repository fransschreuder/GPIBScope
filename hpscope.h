#ifndef HPSCOPE_H
#define HPSCOPE_H
#include "gpib.h"
#include <QMainWindow>
#include <QProgressBar>
#include "hp54111d.h"

namespace Ui {
class HpScope;
}

class HpScope : public QMainWindow
{
    Q_OBJECT

public:
    explicit HpScope(QWidget *parent = 0);
    ~HpScope();

private slots:


    void on_actionTake_data_triggered();
public slots:
    void onScopeDataComplete(QVector<QVector<double> > data);
    void onScopeProgress(int progress);
    void OnScopeDisconnected(void);

private:
    Ui::HpScope *ui;
    HP54111d* hp54111d;
    void Plot(QVector<QVector<double> > dataPoints);
    QProgressBar *progressBar;

};

#endif // HPSCOPE_H
