#include "hp54111d.h"
#include <QDebug>
#include <stdio.h>
HP54111d::HP54111d(QObject *parent) :
    QThread(parent)
{
    YINC=.0;
    XINC=.0;
    XREF=.0;
    YREF=128;
    XORG=.0;
    Y1ORG=.0;
    Y2ORG=.0;

    POINTS=8192;
    XDIV =100E-3;
    Y1DIV=1.0;
    Y2DIV=1.0;

}


HP54111d::~HP54111d()
{
    //delete gpib;
}

void HP54111d::connectGpib(void)
{
    gpib=new GPIB(7);
    if(gpib->connected())
    {
        m_connected = true;
        connect(gpib, SIGNAL(disconnected()), this, SLOT(onGpibDisconnected()));
        GetPreamble();
    }
}

bool HP54111d::connected()
{
    return m_connected;
}

/// system command
/// *****************
void HP54111d::Reset(void)  /// preset s to its void HP54111d::ault settings
{
    gpib->write("RESET\n");
}

void HP54111d::Stop(void) /// stops acquire
{
    gpib->write("KEY 42\n");
    bool abort;
    gpib->readLn(&abort);

}

void HP54111d::Run(void) /// runs acquire
{
    gpib->write("RUN\n");
}

void HP54111d::SetLocal(void) /// sets instrument in local mode
{
    gpib->write("LOCAL\n");
}

void HP54111d::Autoscale(void)
{
    gpib->write("AUTOSCALE\n");
}

void HP54111d::View(int str)  /// 1 to 4
{
    bool abort = false;
    if ((str>0) && (str<5))
    {
       QString msg = QString("VIEW CH %1\n").arg(str);
       gpib->write(msg.toUtf8());
       QString st=gpib->readLn(&abort);
       qDebug()<<st;
    }
}

void HP54111d::GetStatus(void)
{
    bool abort = false;

    gpib->write("STA?\n");
    QString st=gpib->readLn(&abort);
    emit status(st);
}

void HP54111d::IsStopped(void)
{
    bool abort = false;
    gpib->write("KEY?\n");
    QString s = gpib->readLn(&abort);

    int i=s.toLong();
    if ( (i == 42) || (i ==0 ) )
        emit stopped(true);
    else
        emit stopped(false);

}

void HP54111d::GetID(void)
{
    bool abort = false;
    gpib->write("ID?\n");
    QString st=gpib->readLn(&abort);
    emit iD(st);
}


///channel subsystem
///*****************
double HP54111d::GetSensitivity(int channel)  /// '1' to '4'
{
    if (channel==1)
       return Y1DIV;
    else
       return Y2DIV;
}

QString HP54111d::SetSensitivity(int ch, double sensitivity)  ///  '1' "0.01"
{
    bool abort = false;
    if ((ch>0) &&(ch<5))
    {
       QString msg = QString("CH %1 SENS %2\n").arg(ch).arg(sensitivity);
       gpib->write(msg.toUtf8());
       QString st=gpib->readLn(&abort);
       qDebug()<<st;
       return st;
    }
    else
       return "-1";
}

double HP54111d::GetOffset(int ch)
{
    bool abort = false;
    if (ch>0&&ch<5)
    {
       QString msg = QString("CH %1 OFFS?\n").arg(ch);
       gpib->write(msg.toUtf8());
       QString st=gpib->readLn(&abort);
       bool ok;
       double offset = st.toDouble(&ok);
       if(ok)return offset;
       else return -1;
    }
    else
       return -1;
}

QString HP54111d::SetOffset(int ch, double str)
{
    bool abort = false;
   if (ch>0&&ch<5)
   {
       QString msg = QString("CH %1 OFFS %2\n").arg(ch).arg(str);
       gpib->write(msg.toUtf8());
       QString st=gpib->readLn(&abort);
       return st;
   }
   else
       return "-1";
}

/// Timebase subsystem
///*****************
double HP54111d::GetTimebase(void)
{
    return XDIV;
}

QString HP54111d::SetTimebase(double timebase)
{
    bool abort = false;
    QString msg = QString("TIM SENS %1\n").arg(timebase);
    gpib->write(msg.toUtf8());
    QString st=gpib->readLn(&abort);
    return st;
}

double HP54111d::GetDelay(void)
{
    bool abort = false;
    gpib->write("TIM DEL?\n");
    QString st=gpib->readLn(&abort);
    bool ok;
    double delay = st.toDouble(&ok);
    if(ok)return delay;
    else return -1;
}

QString HP54111d::SetDelay(double  delay)
{
    bool abort = false;
    QString msg = QString("TIM DEL %1\n").arg(delay);
    gpib->write(msg.toUtf8());
    QString st=gpib->readLn(&abort);
    return st;
}

QString HP54111d::GetReference(void)
{
    bool abort = false;
    gpib->write("TIM REF?\n");
    QString st=gpib->readLn(&abort);
    return st;
}

QString HP54111d::SetReference(QString ref)  /// 'LEFT' 'CENTER' 'RIGHT'
{
    bool abort = false;
    QString msg = QString("TIM REF %1\n").arg(ref);
    gpib->write(msg.toUtf8());
    QString st=gpib->readLn(&abort);
    return st;
}

///  Waveform subsystem
///*****************
double HP54111d::GetXref(void)  ///0
{
    /*gpib->write("WAV XREFERENCE?\n");
    QString d = gpib->read(-1);
    qDebug()<<"Xref: "<<d;
    ///st=GPIB.read()*/
    return XREF;
}

double HP54111d::GetYref(void)  ///128
{
    /*gpib->write("WAV YREFERENCE?\n");
    QString d = gpib->read(-1);
    qDebug()<<"Yref: "<<d;
    ///st=GPIB.read()*/
    return YREF;
}

double HP54111d::GetXinc(void)  ///real 10ps to 20ms
{
    /*gpib->write("XINCREMENT?\n");
    QString d = gpib->read(-1);
    qDebug()<<"Xinc: "<<d;

    ///st=GPIB.read()
    XINC=XDIV/50.0;*/
    return XINC;
}

double HP54111d::GetYinc(void)  ///real
{
    /*gpib->write("WAV YINCREMENT?\n");
    QString d = gpib->read(-1);
    qDebug()<<"Yinc:" <<d;
    ///st=GPIB.read()*/
    return YINC;
}

double HP54111d::GetYorg(void)  ///real
{
    /*gpib->write("WAV YORIGIN?\n");
    QString d = gpib->read(-1);
    qDebug()<<"Yorg: "<<d;
    ///st=GPIB.read()*/
    return Y1ORG;
}

double HP54111d::GetXorg(void)  ///real
{
    /*gpib->write("WAV XORIGIN?\n");
    QString d = gpib->read(-1);
    qDebug()<<"Xorg: "<<d;
    ///st=GPIB.read()*/
    return XORG;
}

QString HP54111d::WaveForm (void)
{
    bool abort = false;
    gpib->write("WAV?\n");
    QString st=gpib->readLn(&abort);
    return st;
}

void HP54111d::GetPreamble (void)
{
    bool abort = false;

    gpib->write("WAV SRC MEM1 PRE?\n");
    QString d = gpib->readLn(&abort);
    QStringList sl;
    sl=d.split(',');

    if(sl.size()<11)
    {
        qDebug()<<"Preamble does not have enough points";
        return;
    }
    POINTS = sl[2].toLong();
    XINC   = sl[4].toDouble();
    XORG   = sl[5].toDouble();
    XREF   = sl[6].toDouble();
    YINC   = sl[7].toDouble();
    Y1ORG  = sl[8].toDouble();
    Y2ORG  = Y1ORG;
    YREF   = sl[9].toDouble();
    qDebug()<<d;
    qDebug()<<"Preamble: "<<POINTS <<" "<< XINC <<" "<<XORG<<" "<< XREF<< " "<<YINC<<" "<< Y1ORG<<" "<< Y2ORG<<" "<< YREF;

    gpib->write("CH 1 SENS?\n");
    d = gpib->readLn(&abort);
    Y1DIV=d.toDouble();
    gpib->write("CH 2 SENS?\n");
    d= gpib->readLn(&abort);
    Y2DIV=d.toDouble();
    gpib->write("TIM SENS?\n");
    d = gpib->readLn(&abort);
    XDIV = d.toDouble();


}

QString HP54111d::GetCoupling(void)
{
    bool abort = false;
    gpib->write("WAV COUPLING?\n");
    QString st=gpib->readLn(&abort);
    return st;
}

QString HP54111d::SetCoupling(QString coupling)  /// 'DC' 'AC' 'GND'
{
    bool abort = false;
    QString msg = QString("WAV COUPLING %1\n").arg(coupling);
    gpib->write(msg.toUtf8());
    QString st=gpib->readLn(&abort);
    return st;
}

QString HP54111d::GetValid(void)  ///0
{
    bool abort = false;
    gpib->write("WAV VALID?\n");
    QString st=gpib->readLn(&abort);
    return st;
}

/**
 * @brief HP54111d::GetData
 * @param ch
 *
WAV SRC MEM1 FORMAT ASCII
DATA?

 */
void HP54111d::setChannel(int ch)
{
    CHANNEL=ch;
}

void HP54111d::measure(void)
{
    QTime timer;
    timer.start();
    Stop();
    Digitize();

    m_abort = false;
    gpib->flush();
    QString msg = QString("WAV SRC MEM%1 FORMAT ASCII\n").arg(CHANNEL);
    gpib->write(msg.toUtf8());
    gpib->write("DATA?\n");
    gpib->readLn(&m_abort); //"DATA\n"
    QVector< QVector<double> > dataPoints;
    dataPoints.resize(2);
    dataPoints[0].resize(8192);
    dataPoints[1].resize(8192);
    for (int i=0; i<8192; i++)
    {
        if(m_abort)return;
        QString data = gpib->readLn(&m_abort);
        dataPoints[0][(i)]=(((double)i)*XINC)-XREF;
        bool ok;
        int d = (int) data.toInt(&ok);
        if (!ok)
            qDebug()<<i<<": "<<data;
        dataPoints[1][(i)]=(((double) (d))-YREF)*YINC;
        emit progress((100*i)/8192);

    }
    int nMilliseconds = timer.elapsed();
    qDebug()<<"Taking a scope trace took "<<nMilliseconds<<" ms.";
    Run();
    emit dataReady(dataPoints);


}
/// Acquire subsystem
///*****************

QString HP54111d::Acquire ()
{
    bool abort = false;
    QString msg=QString("ACQ %1\n").arg(CHANNEL);
    gpib->write(msg.toUtf8());
    QString st=gpib->readLn(&abort);
    return st;
}

QString HP54111d::Digitize()
{
    bool abort = false;
    QString msg = QString("DIG %1\n").arg(CHANNEL);
    gpib->write(msg.toUtf8());
    QString st = gpib->readLn(&abort);
    return st;
}


int HP54111d::GetPoints(void) ///integer  501 or 8192
{
    bool abort = false;
    gpib->write("ACQ POINTS?\n");
    QString st=gpib->readLn(&abort);
    bool ok;
    int points = st.toInt(&ok);
    if(ok)return points;
    else return -1;
}

int HP54111d::GetResolution(void) ///integer OFF 6 7 or 8
{
    bool abort = false;
    gpib->write("ACQ RESO?\n");
    QString st=gpib->readLn(&abort);
    bool ok;
    int res = st.toInt(&ok);
    if(ok)return res;
    else return -1;
}

void HP54111d::SetResolution( int res) /// '6' '7' '8' or 'OFF'
{
    QString sRes;
    if(res>=6 &&res <=8)
        sRes = QString("%1").arg(res);
    else
        sRes = QString("OFF");
    QString msg = QString("ACQ RESO %1\n").arg(sRes);
    gpib->write(msg.toUtf8());
}

void HP54111d::onGpibDisconnected()
{
    m_connected = false;
    emit disconnected();
}

void HP54111d::setCommand(command_t Command)
{
    m_Command = Command;
}

void HP54111d::run(void)
{
    switch(m_Command)
    {
    case CONNECT:
        connectGpib();
        break;
    case MEASURE:
        measure();
        break;

    }
}
