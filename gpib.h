#ifndef GPIB_H
#define GPIB_H
#include <QtSerialPort/QtSerialPort>
#include <QObject>

class GPIB : public QObject
{
    Q_OBJECT
public:
    explicit GPIB(int address, QObject *parent = 0);
    ~GPIB();

    //QByteArray read(int minimumLength);
    QString readLn(bool* abort);
    void write(QByteArray data);
    void flush();
    bool connected();
private:
    bool m_connected;
    QSerialPort *serial;
signals:
    void disconnected(void);
};

#endif // GPIB_H
