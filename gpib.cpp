#include "gpib.h"


GPIB::GPIB(int address, QObject *parent) :
    QObject(parent)
{
    serial = new QSerialPort(this);
    m_connected = false;
    QList<QSerialPortInfo> availablePorts = QSerialPortInfo::availablePorts();
    for(int i=0; i<availablePorts.size(); i++)
    {
        serial->setPortName(availablePorts[i].portName());
        serial->setBaudRate(460800);
        if (serial->open(QIODevice::ReadWrite)) {
            qDebug()<<"Opening port "<<availablePorts[i].portName();
            QString init = QString("++addr %1\n*IDN?\n").arg(address);
            serial->write(init.toUtf8());
            QString d = "";
            if(serial->waitForReadyRead(1000))
            {
                m_connected = true;
                d += QString(serial->readAll());
                while(d.length()<15)
                {
                      serial->waitForReadyRead(1000);
                      d +=  QString(serial->readAll());

                }
                qDebug()<<d.length();

                qDebug()<<"Got IDN data: "<<d;
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
    if(m_connected)
    {
        serial->close();
    }
}

void GPIB::write(QByteArray data)
{
    if(!connected())
        return;
    serial->write(data);
}

/*QByteArray GPIB::read(int minimumLength)
{
    QByteArray d;
    serial->waitForReadyRead(1000);
    d += QString(serial->readAll());
    while((d.length()<minimumLength||(minimumLength==-1&&!d.contains('\n')))&&!abort)
    {
          if(!serial->waitForReadyRead(1000))
              break;
          d +=  QString(serial->readAll());

    }

    return d;
}*/

QString GPIB::readLn(bool* abort)
{
    if(!connected())
        return QString("");
    while(!serial->canReadLine()&&!(*abort))
        serial->waitForReadyRead(1000);
    return QString(serial->readLine());
}

void GPIB::flush()
{
    if(!connected())
        return;
    serial->readAll();
}

bool GPIB::connected()
{
    QSerialPortInfo p(serial->portName());
    if(p.isNull())
    {
        qDebug()<<"Port is NULL, disconnecting";
        m_connected = false;
        emit disconnected();
    }
    return m_connected;
}
