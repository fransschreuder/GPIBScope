#include "gpib.h"


GPIB::GPIB(int channel, QObject *parent) :
    QObject(parent)
{
    serial = new QSerialPort(this);
    connected = false;
    QList<QSerialPortInfo> availablePorts = QSerialPortInfo::availablePorts();
    for(int i=0; i<availablePorts.size(); i++)
    {
        serial->setPortName(availablePorts[i].portName());
        serial->setBaudRate(460800);
        if (serial->open(QIODevice::ReadWrite)) {
            qDebug()<<"Connected to "<<availablePorts[i].portName();
            QString init = QString("++addr %1\n*IDN?\n").arg(channel);
            serial->write(init.toUtf8());
            if(serial->waitForReadyRead(100))
            {
                connected = true;
                qDebug()<<"Got IDN data: "<<serial->readAll();
                connect(serial, SIGNAL(readyRead()), this, SLOT(readData()));

                break;
            }
            else
            {
                serial->close();
            }

        } else {
            qDebug()<<"Could not connect";
        }

    }
}

GPIB::~GPIB()
{
    if(connected)
    {
        disconnect(serial, SIGNAL(readyRead()), this, SLOT(readData()));
        serial->close();
    }
    //delete serial;
}

void GPIB::readData(void)
{
    QByteArray data = serial->readAll();
    qDebug()<<data;
    emit dataRead(data);
}

void GPIB::write(QByteArray data)
{
    serial->write(data);
}
