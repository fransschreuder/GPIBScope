#include "hpscope.h"
#include "ui_hpscope.h"

HpScope::HpScope(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::HpScope)
{
    ui->setupUi(this);
    hp54111d = new HP54111d();

}

HpScope::~HpScope()
{

    delete ui;
    delete hp54111d;
}
