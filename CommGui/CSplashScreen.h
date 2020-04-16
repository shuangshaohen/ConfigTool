#ifndef CSPLASHSCREEN_H
#define CSPLASHSCREEN_H

#include <QObject>
#include <QTimer>
#include <qelapsedtimer.h>
#include <QSplashScreen>
#include "CommGui_global.h"

class COMMGUI_EXPORT CSplashScreen : public QSplashScreen
{
    Q_OBJECT
private:
    bool                m_bDebug;
    QElapsedTimer		m_Time;
    QString             m_strTime;
    QString             m_strContent;
    int                 m_nMilliseconds;
    QTimer              *m_pTimer;
private:
    CSplashScreen();
    CSplashScreen( CSplashScreen&);
    CSplashScreen & operator=(CSplashScreen&);
public slots:
    void update();
protected:
    virtual void drawContents ( QPainter * painter );
public:
    void StartShow();
    void EndShow();
    static CSplashScreen *Instance();
    void ShowMessage ( const QString & message );

};

#endif // CSPLASHSCREEN_H
