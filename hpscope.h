#ifndef HPSCOPE_H
#define HPSCOPE_H
#include "gpib.h"
#include <QMainWindow>
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

private:
    Ui::HpScope *ui;
    HP54111d* hp54111d;
    void Plot(QVector<QVector<double> > dataPoints);

};

#endif // HPSCOPE_H
