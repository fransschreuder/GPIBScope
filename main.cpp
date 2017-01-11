#include "hpscope.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    HpScope w;
    w.show();

    return a.exec();
}
