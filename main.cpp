#include "hpscope.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qRegisterMetaType<QVector<QVector<double> > >("QVectorQVectorDouble");
    HpScope w;
    w.show();

    return a.exec();
}
