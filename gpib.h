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

    QByteArray read(int minimumLength);
    QString readLn();
    void write(QByteArray data);
    bool connected;
    QSerialPort *serial;
signals:
    void dataRead(QByteArray data);
public slots:
    void readData(void);
private:


};

#endif // GPIB_H
