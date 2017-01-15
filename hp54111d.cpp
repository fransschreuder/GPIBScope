#include "hp54111d.h"

HP54111d::HP54111d(QObject *parent) :
    QObject(parent)
{
    gpib=new GPIB(7);
    connect(gpib, SIGNAL(dataRead(QByteArray)), this, SLOT(dataRead(QByteArray)));
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

void HP54111d::dataRead(QByteArray data)
{
    bool success;
    QString dataS(unfinishedData+data);
    qDebug()<<"HP54111d received: " << dataS;
    unfinishedData = "";
    DataType type = expectedData.back();
    expectedData.pop_back();
    int i;
    QStringList sl;
    switch(type)
    {
        case STOPPED:
            i=dataS.toLong();
            if ( (i == 42) || (i ==0 ) )
                emit stopped(true);
            else
                emit stopped(false);
            break;
        case STATUS:
            qDebug()<<"Status: "<<data;
            emit status(dataS);
            break;
        case ID:
            emit id(dataS);
        case PREAMBLE:
            sl=dataS.split(',');
            qDebug()<<"Preamle: "<<sl;

            if(sl.size()<11)
            {
                qDebug()<<"Preamble does not have enough points";
                unfinishedData = dataS;
                break;
            }
            POINTS = sl[3].toLong();
            XINC   = sl[5].toDouble();
            XORG   = sl[6].toDouble();
            XREF   = sl[7].toDouble();
            YINC   = sl[8].toDouble();
            Y1ORG  = sl[9].toDouble();
            Y2ORG  = Y1ORG;
            YREF   = sl[10].toDouble();
            break;
        case CH1SENS:
            Y1DIV=dataS.toDouble();
            gpib->write("CH 2 SENS?\n");
            expectedData.push_back(CH1SENS);
            break;
        case CH2SENS:
            Y1DIV=dataS.toDouble();
            gpib->write("TIM SENS?\n");
            expectedData.push_back(TIMESENS);
            break;
        case TIMESENS:
            XDIV = dataS.toDouble();
            emit preamble(POINTS,XINC,XORG,XREF,YINC,Y1ORG,Y2ORG);
            break;

        case DATA:
            qDebug()<<"DATA:" << dataS;
            /**
             * TODO: Interprete dataS like python code below:
             * GPIB.readbyte(4)
            ///print "///2A + len (2bytes)"
            YP=GPIB.readbyte(len)   ///data
            data= zeros (len, Float)
            y1div=float(GetSensitivity('1'))
            y2div=float(GetSensitivity('2'))
            yref=float(YREF)
            yinc=float(YINC)
            yorg=float(GetYorg(str))
            if (str=='1'):
               for XP in range (0,len,1):
                   data[XP]= ( (ord(YP[XP]) - yref)*yinc + yorg )
            else:
               for XP in range (0,len,1):
                   data[XP]= ( (ord(YP[XP]) - yref)*yinc + yorg ) * y1div/ y2div
            return data
            */
            emit dataPoints();
            break;
        case ACQPOINTS:
            acqPoints = dataS.toDouble(&success);
            if(!success)qDebug()<<"No acquisition made!\nFirst Acquire Data";
            break;
        case ACQRES:
            acqRes = dataS.toDouble(&success);
            if(!success)qDebug()<<"No acquisition made!\nFirst Acquire Data";
            break;
        default:
            qDebug()<<type<<" not implemented, invalid return value from scope";
            break;
    }
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

bool HP54111d::View(int str)  /// 1 to 4
{

    if ((str>0) && (str<5))
    {
       QString msg = QString("VIEW CH %1\n").arg(str);
       gpib->write(msg.toUtf8());
       //st=GPIB.read()
       expectedData.push_back(STATUS);
       return true;
    }
    else
       return false;
}

void HP54111d::GetStatus(void)
{

    gpib->write("STA?\n");
    expectedData.push_back(STATUS);
}

void HP54111d::IsStopped(void)
{
    gpib->write("KEY?\n");

    expectedData.push_back(STOPPED);
}

void HP54111d::GetID(void)
{
    gpib->write("ID?\n");
    expectedData.push_back(ID);
    //st=GPIB.read()

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

bool HP54111d::SetSensitivity(int ch, double sensitivity)  ///  '1' "0.01"
{
    if ((ch>'0') &&(ch<'5'))
    {
       QString msg = QString("CH %1 SENS %2\n").arg(ch).arg(sensitivity);
       gpib->write(msg.toUtf8());
       expectedData.push_back(STATUS);
       //st=GPIB.read()
       return true;
    }
    else
       return false;
}

bool HP54111d::GetOffset(int ch)
{
    if (ch>0&&ch<5)
    {
       QString msg = QString("CH %1 OFFS?\n").arg(ch);
       gpib->write(msg.toUtf8());
       expectedData.push_back(OFFSET);
       return true;
    }
    else
       return false;
}

bool HP54111d::SetOffset(int ch, double str)
{
   if (ch>0&&ch<5)
   {
       QString msg = QString("CH %1 OFFS %2\n").arg(ch).arg(str);
       gpib->write(msg.toUtf8());
       expectedData.push_back(STATUS);
       return true;
   }
   else
       return false;
}

/// Timebase subsystem
///*****************
double HP54111d::GetTimebase(void)
{
    return XDIV;
}

void HP54111d::SetTimebase(double timebase)
{
    QString msg = QString("TIM SENS %1\n").arg(timebase);
    gpib->write(msg.toUtf8());
    expectedData.push_back(STATUS);
}

void HP54111d::GetDelay(void)
{
    gpib->write("TIM DEL?\n");
    expectedData.push_back(DELAY);
}

void HP54111d::SetDelay(double  delay)
{
    QString msg = QString("TIM DEL %1\n").arg(delay);
    gpib->write(msg.toUtf8());
    expectedData.push_back(STATUS);
}

void HP54111d::GetReference(void)
{
    gpib->write("TIM REF?\n");
    expectedData.push_back(REFERENCE);
}

void HP54111d::SetReference(QString ref)  /// 'LEFT' 'CENTER' 'RIGHT'
{
    QString msg = QString("TIM REF %1\n").arg(ref);
    gpib->write(msg.toUtf8());
    expectedData.push_back(STATUS);
}

///  Waveform subsystem
///*****************
double HP54111d::GetXref(void)  ///0
{
    ///gpib->write("WAV XREFERENCE?\n")
    ///st=GPIB.read()
    return XREF;
}

double HP54111d::GetYref(void)  ///128
{
    ///gpib->write("WAV YREFERENCE?\n")
    ///st=GPIB.read()
    return YREF;
}

double HP54111d::GetXinc(void)  ///real 10ps to 20ms
{
    ///gpib->write("XINCREMENT?\n")
    ///st=GPIB.read()
    XINC=XDIV/50.0;
    return XINC;
}

double HP54111d::GetYinc(void)  ///real
{
    ///gpib->write("WAV YINCREMENT?\n")
    ///st=GPIB.read()
    return YINC;
}

double HP54111d::GetYorg(void)  ///real
{
    return Y1ORG;
}

double HP54111d::GetXorg(void)  ///real
{
    ///gpib->write("WAV XORIGIN?\n")
    ///st=GPIB.read()
    return XORG;
}

void HP54111d::WaveForm (void)
{
    gpib->write("WAV?\n");
    expectedData.push_back(WAVE);
}

void HP54111d::GetPreamble (void)
{

    gpib->write("WAV SRC MEM1 PRE?\n");
    expectedData.push_back(PREAMBLE);
}

void HP54111d::GetCoupling(void)
{
    gpib->write("WAV COUPLING?\n");
    expectedData.push_back(COUPLING);
}

void HP54111d::SetCoupling(QString coupling)  /// 'DC' 'AC' 'GND'
{
    QString msg = QString("WAV COUPLING %1\n").arg(coupling);
    gpib->write(msg.toUtf8());
    expectedData.push_back(STATUS);
}

void HP54111d::GetValid(void)  ///0
{
    gpib->write("WAV VALID?\n");
    expectedData.push_back(WAVEVALID);
}

void HP54111d::GetData(int ch)
{
    //len=8*Ko
    QString msg = QString("WAV SRC MEM%1 FORMAT BYTE\n").arg(ch);
    gpib->write(msg.toUtf8());
    gpib->write("DATA?\n");
    expectedData.push_back(DATA);
}
/// Acquire subsystem
///*****************

void HP54111d::Acquire (QString str)
{
    QString msg=QString("ACQ %1\n").arg(str);
    gpib->write(msg.toUtf8());
    expectedData.push_back(STATUS);
}

void HP54111d::Digitize(QString str)
{
    QString msg = QString("DIG %1\n").arg(str);
    gpib->write(msg.toUtf8());
}


void HP54111d::GetPoints(void) ///integer  501 or 8192
{
    gpib->write("ACQ POINTS?\n");
    expectedData.push_back(ACQPOINTS);
}

void HP54111d::GetResolution(void) ///integer OFF 6 7 or 8
{
    gpib->write("ACQ RESO?\n");
    expectedData.push_back(ACQRES);
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
