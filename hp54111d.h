#ifndef HP54111D_H
#define HP54111D_H
#include "gpib.h"
#include <QObject>

typedef enum{
    STOPPED, STATUS, ID, PREAMBLE, CH1SENS, CH2SENS, TIMESENS,DATA, ACQPOINTS, ACQRES, OFFSET, DELAY, REFERENCE, WAVE, WAVEVALID, COUPLING,
    UNKNOWN

}DataType;

class HP54111d : public QObject
{
    Q_OBJECT
public:
    explicit HP54111d(QObject *parent = 0);
    ~HP54111d();

    /// system command
    /// *****************
    void Reset(void);  /// preset s to its void HP54111d::ault settings
    void Stop(void); /// stops acquire
    void Run(void); /// runs acquire
    void SetLocal(void); /// sets instrument in local mode
    void Autoscale(void);
    bool View(int str);  /// 1 to 4
    void GetStatus(void);
    void IsStopped(void);
    void GetID(void);

    ///channel subsystem
    ///*****************
    double GetSensitivity(int channel);  /// '1' to '4'
    bool SetSensitivity(int ch, double sensitivity);  ///  '1' "0.01"
    bool GetOffset(int ch);
    bool SetOffset(int ch, double str);

    /// Timebase subsystem
    ///*****************
    double GetTimebase(void);
    void SetTimebase(double timebase);
    void GetDelay(void);
    void SetDelay(double  delay);
    void GetReference(void);
    void SetReference(QString ref);  /// 'LEFT' 'CENTER' 'RIGHT'

    ///  Waveform subsystem
    ///*****************
    double GetXref(void);  ///0
    double GetYref(void);  ///128
    double GetXinc(void);  ///real 10ps to 20ms
    double GetYinc(void);  ///real
    double GetYorg(void);  ///real
    double GetXorg(void);  ///real
    void WaveForm (void);
    void GetPreamble (void);
    void GetCoupling(void);
    void SetCoupling(QString coupling);  /// 'DC' 'AC' 'GND'
    void GetValid(void);  ///0
    QVector<QVector<double> > GetData(int ch);

    /// Acquire subsystem
    ///*****************
    void Acquire (QString str);
    void Digitize(QString str);
    void GetPoints(void); ///integer  501 or 8192
    void GetResolution(void); ///integer OFF 6 7 or 8
    void SetResolution( int res); /// '6' '7' '8' or 'OFF'

signals:
    void stopped(bool);
    void status(QString);
    void id(QString);
    void preamble(long int POINTS,double XINC,double XORG,double XREF,double YINC,double Y1ORG,double Y2ORG);
    void dataPoints(void);


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

    long int POINTS;
    double XDIV ;
    double Y1DIV;
    double Y2DIV;
    int acqPoints;
    int acqRes;
    QList<DataType> expectedData;
    QString unfinishedData;
    int expectedDataBytes;
    QString receivedData;

};

#endif // HP54111D_H
