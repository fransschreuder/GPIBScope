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
            QString d = "";
            if(serial->waitForReadyRead(1000))
            {
                connected = true;
                d += QString(serial->readAll());
                while(d.length()<15)
                {
                      serial->waitForReadyRead(1000);
                      d +=  QString(serial->readAll());

                }
                qDebug()<<d.length();

                qDebug()<<"Got IDN data: "<<d;
                //connect(serial, SIGNAL(readyRead()), this, SLOT(readData()));

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
        //disconnect(serial, SIGNAL(readyRead()), this, SLOT(readData()));
        serial->close();
    }
    //delete serial;
}

void GPIB::readData(void)
{

    //disconnect(serial, SIGNAL(readyRead()), this, SLOT(readData()));
    QByteArray data = serial->readAll();

    qDebug()<<data;
    emit dataRead(data);
    //connect(serial, SIGNAL(readyRead()), this, SLOT(readData()));
}

void GPIB::write(QByteArray data)
{
    serial->write(data);
}

QByteArray GPIB::read(int minimumLength)
{
    QByteArray d;
    serial->waitForReadyRead(1000);
    d += QString(serial->readAll());
    while(d.length()<minimumLength||(minimumLength==-1&&!d.contains('\n')))
    {
          if(!serial->waitForReadyRead(1000))
              break;
          d +=  QString(serial->readAll());

    }

    return d;
}

QString GPIB::readLn()
{
    QString d;
    while(!serial->canReadLine())
        serial->waitForReadyRead(1000);
    return QString(serial->readLine());
    /*while(d.length()<minimumLength||(minimumLength==-1&&!d.contains('\n')))
    {
          if(!serial->waitForReadyRead(1000))
              break;
          d +=  QString(serial->readAll());

    }

    return d;*/
}
