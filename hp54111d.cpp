///include "hp54111d.h"

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
    qDebug()<<"HP54111d received: " << data;
    /**
      "STOPPED"
    if ( (i == 42) or (i ==0 ) ):
        return 1
    else:
        return -1

      "STATUS"
      "ID"
        */
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
           gpib->write(msg);
           //st=self.GPIB.read()
           dataExpected.push_back("STATUS");
           return true;
        }
        else
           return false;
    }

    void HP54111d::GetStatus(void)
    {

        gpib->write("STA?\n");
        dataExpected.push_back("STATUS");
    }

    void HP54111d::IsStopped(void)
    {
        gpib->write("KEY?\n");

        dataExpected.push_back("STOPPED");
    }

    void HP54111d::GetID(void)
    {
        gpib->write("ID?\n");
        dataExpected.push_back("ID");
        //st=self.GPIB.read()

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
           gpib->write(msg);
           expectedData.push_back("STATUS");
           //st=self.GPIB.read()
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
           gpib->write(msg);
           expectedData.push_back("STATUS");
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
           gpib->write(msg);
           expectedData.push_back("STATUS");
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

    void HP54111d::SetTimebase(self,str):
        msg = "TIM SENS %s\n" %(str)
        gpib->write(msg)
        st=self.GPIB.read()
        return st

    void HP54111d::GetDelay(void):
        gpib->write("TIM DEL?\n")
        st=self.GPIB.read()
        return st

    void HP54111d::SetDelay(self, str):
        msg = "TIM DEL %s\n" %(str)
        gpib->write(msg)
        st=self.GPIB.read()
        return st

    void HP54111d::GetReference(void):
        gpib->write("TIM REF?\n")
        st=self.GPIB.read()
        return st

    void HP54111d::SetReference(self, str):  /// 'LEFT' 'CENTER' 'RIGHT'
        msg = "TIM REF %s\n" %(str)
        gpib->write(msg)
        st=self.GPIB.read()
        return st

///  Waveform subsystem
///*****************
    void HP54111d::GetXref(void):  ///0
        ///gpib->write("WAV XREFERENCE?\n")
        ///st=self.GPIB.read()
        return self.XREF

    void HP54111d::GetYref(void):  ///128
        ///gpib->write("WAV YREFERENCE?\n")
        ///st=self.GPIB.read()
        return self.YREF

    void HP54111d::GetXinc(void):  ///real 10ps to 20ms
        ///gpib->write("XINCREMENT?\n")
        ///st=self.GPIB.read()
        ///bug oscilloscope !!!
        if self.XINC=='  1.00000E-09':
            ///print 'self.XINC=',self.XINC,' Oscillo error !!'
            self.XINC=str(float(self.XDIV)/50.0)
        ///print self.XINC
        self.XINC=str(float(self.XDIV)/50.0)
        return self.XINC

    void HP54111d::GetYinc(void):  ///real
        ///gpib->write("WAV YINCREMENT?\n")
        ///st=self.GPIB.read()
        return self.YINC

    void HP54111d::GetYorg(self,str):  ///real
        ///gpib->write("WAV YORIGIN?\n")
        ///st=self.GPIB.read()
        return self.YORG1

    void HP54111d::GetXorg(void):  ///real
        ///gpib->write("WAV XORIGIN?\n")
        ///st=self.GPIB.read()
        return self.XORG

    void HP54111d::WaveForm (void):
        gpib->write("WAV?\n")
        st=self.GPIB.read()
        return st

    void HP54111d::GetPreamble (void):
        gpib->write("WAV SRC MEM1 PRE?\n")
        st=self.GPIB.read()
        pos=st.find(',')+1
        st=st[pos:]
        pos=st.find(',')+1
        st=st[pos:]

        pos=st.find(',')
        self.POINTS=st[:pos]
        ///print self.POINTS
        st=st[pos+1:]

        ///print st
        pos=st.find(',')+1
        st=st[pos:]

        pos=st.find(',')
        self.XINC=st[:pos]
        ///print 'self.XINC=',self.XINC

        st=st[pos+1:]

        pos=st.find(',')
        self.XORG=st[:pos]
        ///print 'self.XORG=',self.XORG
        st=st[pos+1:]

        pos=st.find(',')
        self.XREF=st[:pos]
        ///print 'self.XREF=',self.XREF
        st=st[pos+1:]

        pos=st.find(',')
        self.YINC=st[:pos]
        ///print 'self.YINC=',self.YINC
        st=st[pos+1:]

        pos=st.find(',')
        self.YORG1=self.YORG2=st[:pos]
        ///print 'self.YORG1=',self.YORG1
        st=st[pos+1:]

        pos=st.find(',')
        self.YREF=st[:pos]
        ///print 'self.YREF=',self.YREF
        st=st[pos+1:]

        gpib->write("CH 1 SENS?\n")
        self.Y1DIV=self.GPIB.read()

        gpib->write("CH 2 SENS?\n")
        self.Y2DIV=self.GPIB.read()

        gpib->write("TIM SENS?\n")
        self.XDIV=self.GPIB.read()

        return st

    void HP54111d::GetCoupling(void):
        gpib->write("WAV COUPLING?\n")
        st=self.GPIB.read()
        return st

    void HP54111d::SetCoupling(void):  /// 'DC' 'AC' 'GND'
        msg = "WAV COUPLING %s\n" %(str)
        gpib->write(msg)
        st=self.GPIB.read()
        return st

    void HP54111d::GetValid(void):  ///0
        gpib->write("WAV VALID?\n")
        st=self.GPIB.read()
        return st

    void HP54111d::GetData(self,str,len=8*Ko):
        msg = "WAV SRC MEM%s FORMAT BYTE\n" %(str)
        gpib->write(msg)
        gpib->write("DATA?\n")
        self.GPIB.readbyte(4)
        ///print "///2A + len (2bytes)"
        YP=self.GPIB.readbyte(len)   ///data
        data= zeros (len, Float)
        y1div=float(self.GetSensitivity('1'))
        y2div=float(self.GetSensitivity('2'))
        yref=float(self.YREF)
        yinc=float(self.YINC)
        yorg=float(self.GetYorg(str))
        if (str=='1'):
           for XP in range (0,len,1):
               data[XP]= ( (ord(YP[XP]) - yref)*yinc + yorg )
        else:
           for XP in range (0,len,1):
               data[XP]= ( (ord(YP[XP]) - yref)*yinc + yorg ) * y1div/ y2div
        return data


/// Acquire subsystem
///*****************

    void HP54111d::Acquire (self,str):
        msg='ACQ '+str+'\n'
        gpib->write(msg)
        st=self.GPIB.read()
        return st

    void HP54111d::Digitize(self,str):
        msg = "DIG %s\n" %(str)
        gpib->write(msg)

    void HP54111d::GetPoints(void) ///integer  501 or 8192
        gpib->write("ACQ POINTS?\n")
        st=self.GPIB.read()
        try:
          test=int(st)
        except:
          return "No acquisition made!\nFirst Acquire Data"
        return st

    void HP54111d::GetResolution(void) ///integer OFF 6 7 or 8
        gpib->write("ACQ RESO?\n")
        st=self.GPIB.read()
        try:
          test=int(st)
        except:
          return "No acquisition made!\nFirst Acquire Data"
        return st

    void HP54111d::SetResolution(self, str): /// '6' '7' '8' or 'OFF'
        msg = "ACQ RESO %s\n" %(str)
        gpib->write(msg)
