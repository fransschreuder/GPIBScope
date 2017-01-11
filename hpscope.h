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

private:
    Ui::HpScope *ui;
    HP54111d* hp54111d;

};

#endif // HPSCOPE_H
