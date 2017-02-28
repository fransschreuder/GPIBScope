#ifndef HP54111D_H
#define HP54111D_H
#include "gpib.h"
#include <QObject>

class HP54111d : public QThread
{
    Q_OBJECT
public:
    explicit HP54111d(QObject *parent = 0);
    ~HP54111d();

    bool connected();
    void setChannel(int ch);
    void run(void) Q_DECL_OVERRIDE;
    void abort(void)
    {
        m_abort = true;
    }

    /// system command
    /// *****************
    void Reset(void);  /// preset s to its void HP54111d::ault settings
    void Stop(void); /// stops acquire
    void Run(void); /// runs acquire
    void SetLocal(void); /// sets instrument in local mode
    void Autoscale(void);
    bool View(int str);  /// 1 to 4
    QString GetStatus(void);
    bool IsStopped(void);
    QString GetID(void);

    ///channel subsystem
    ///*****************
    double GetSensitivity(int channel);  /// '1' to '4'
    QString SetSensitivity(int ch, double sensitivity);  ///  '1' "0.01"
    double GetOffset(int ch);
    QString SetOffset(int ch, double str);

    /// Timebase subsystem
    ///*****************
    double GetTimebase(void);
    QString SetTimebase(double timebase);
    double GetDelay(void);
    QString SetDelay(double  delay);
    QString GetReference(void);
    QString SetReference(QString ref);  /// 'LEFT' 'CENTER' 'RIGHT'

    ///  Waveform subsystem
    ///*****************
    double GetXref(void);  ///0
    double GetYref(void);  ///128
    double GetXinc(void);  ///real 10ps to 20ms
    double GetYinc(void);  ///real
    double GetYorg(void);  ///real
    double GetXorg(void);  ///real
    QString WaveForm (void);
    void GetPreamble (void);
    QString GetCoupling(void);
    QString SetCoupling(QString coupling);  /// 'DC' 'AC' 'GND'
    QString GetValid(void);  ///0


    /// Acquire subsystem
    ///*****************
    QString Acquire ();
    QString Digitize();
    int GetPoints(void); ///integer  501 or 8192
    int GetResolution(void); ///integer OFF 6 7 or 8
    void SetResolution( int res); /// '6' '7' '8' or 'OFF'
signals:
    void dataReady(QVector<QVector<double> > data); //double vector of doubles, x and y
    void progress(int progress); //percentage of read points.
    void disconnected(void);

public slots:
    void onGpibDisconnected(void);

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
    int CHANNEL;

    bool m_abort;

};

#endif // HP54111D_H
