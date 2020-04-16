#ifndef CTHREAD_H
#define CTHREAD_H

#include <QObject>
#include <QThread>
#include <QEvent>
#include "CMsgInfo.h"
#include "CDataBase.h"
#include "CommGui_global.h"

class CMainWnd;

class COMMGUI_EXPORT CEvent : public QEvent
{
public:
    enum Enum_Event_Code
    {
        Enum_Start_Event_Info = QEvent::User +1,
        Enum_Error_Event_Info,
        Enum_End_Event_Info,

        Enum_Msg_Event_Info,

    };
    CEvent( Type type ) :QEvent(type){};
};

class COMMGUI_EXPORT CStringEvent : public CEvent
{
public:
    CStringEvent( Type type ) :CEvent(type){};
public:
    QString m_strInfo;
};

class COMMGUI_EXPORT CMsgEvent : public CEvent
{
public:
    CMsgEvent( Type type ) :CEvent(type){};
public:
    CMsgInfo m_MsgInfo;
};

class COMMGUI_EXPORT CThread : public QThread
{
    Q_OBJECT
protected:
    CMainWnd	*m_pMainWnd;
public:
    CThread( CMainWnd	*pMainWnd );
    ~CThread( );
public:
    void WaitThread();

    void OutputMsgInfo( const CMsgInfo &msgInfo );

    void OutputMsgInfoList( const QList<CMsgInfo> &MsgInfoList );

    void PutStringToSplashScreen( QString &strInfo, CEvent::Enum_Event_Code type );

signals:
    void SendMsgInfo( const CMsgInfo &msgInfo );
};

class COMMGUI_EXPORT CThreadForOpenCfg : public CThread
{
private:
    bool		m_bOK;
    CDataBase  *m_pXML;
    QString		m_strXmlFilePathName;
private:
    CThreadForOpenCfg( const CThreadForOpenCfg &);
    CThreadForOpenCfg & operator= ( const CThreadForOpenCfg &);
protected:
    void run();
public:
    bool IsOK(){ return m_bOK;};
    CThreadForOpenCfg( CMainWnd	*pMainWnd, CDataBase *pXML, QString strXmlFilePathName);

};

class COMMGUI_EXPORT CThreadForSave : public CThread
{
private:
    bool		m_bOK;
    CDataBase *m_pXML;
    QString		m_strSaveErrorInfo;
    QString		m_strXmlFilePathName;
private:
    CThreadForSave( const CThreadForSave &);
    CThreadForSave & operator= ( const CThreadForSave &);
protected:
    void run();
public:
    QString GetSaveErrorInfo(){ return m_strSaveErrorInfo; };
    bool IsOK(){ return m_bOK;};
    CThreadForSave( CMainWnd	*ppMainWnd, CDataBase *pXML, QString strXmlFilePathName );

};

#endif // CTHREAD_H
