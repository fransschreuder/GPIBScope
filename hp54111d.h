#ifndef HP54111D_H
#define HP54111D_H
#include "gpib.h"
#include <QObject>

typedef enum
{
    CONNECT, MEASURE
}command_t;

class HP54111d : public QThread
{
    Q_OBJECT
public:
    explicit HP54111d(QObject *parent = 0);
    ~HP54111d();

    bool connected();
    void setChannel(int ch);
    void setCommand(command_t Command);
    void run(void) Q_DECL_OVERRIDE;
    void abort(void)
    {
        m_abort = true;
    }

    void connectGpib();

    /// system command
    /// *****************
    void Reset(void);  /// preset s to its void HP54111d::ault settings
    void Stop(void); /// stops acquire
    void Run(void); /// runs acquire
    void SetLocal(void); /// sets instrument in local mode
    void Autoscale(void);
    void View(int str);  /// 1 to 4
    void GetStatus(void);
    void IsStopped(void);
    void GetID(void);

    void measure(void);

signals:
    void status(QString status);
    void stopped(bool isStopped);
    void iD(QString ID);

public:
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
    QVector<void*> FunctionQueue;
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
    bool m_connected;
    command_t m_Command;

};

#endif // HP54111D_H
