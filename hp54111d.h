#ifndef HP54111D_H
#define HP54111D_H
#include "gpib.h"
#include <QObject>

class HP54111d : public QObject
{
    Q_OBJECT
public:
    explicit HP54111d(QObject *parent = 0);
    ~HP54111d();

signals:

public slots:
    void dataRead(QByteArray data);
private:
    GPIB* gpib;

    double YINC;
    double XINC;
    double XREF;
    double YREF;
    double XORG;
    double Y1ORG;
    double Y2ORG;

    double POINTS;
    double XDIV ;
    double Y1DIV;
    double Y2DIV;
    QStringList dataExpected;

};

#endif // HP54111D_H
