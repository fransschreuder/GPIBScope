#ifndef GPIB_H
#define GPIB_H
#include <QtSerialPort/QtSerialPort>
#include <QObject>

class GPIB : public QObject
{
    Q_OBJECT
public:
    explicit GPIB(int channel, QObject *parent = 0);
    ~GPIB();
    void write(QByteArray data);
    bool connected;
signals:
    void dataRead(QByteArray data);
public slots:
    void readData(void);
private:
    QSerialPort *serial;

};

#endif // GPIB_H
