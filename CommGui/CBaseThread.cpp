#include "CBaseThread.h"
#include "CMainWnd.h"
#include <QtGui>

CThread::CThread( CMainWnd	*pMainWnd  )
{
    m_pMainWnd = pMainWnd;
}

CThread::~CThread( )
{
    disconnect(this, 0, 0, 0);
}

void CThread::OutputMsgInfo( const CMsgInfo &msgInfo )
{
    CMsgEvent* pMyEvent = new CMsgEvent((QEvent::Type)CEvent::Enum_Msg_Event_Info);

    pMyEvent->m_MsgInfo = msgInfo;

    qApp->postEvent ( m_pMainWnd, pMyEvent );
}

void CThread::PutStringToSplashScreen( QString &strInfo, CEvent::Enum_Event_Code type )
{
    CStringEvent* pMyEvent = new CStringEvent((QEvent::Type)type);

    pMyEvent->m_strInfo = strInfo;

    qApp->postEvent ( m_pMainWnd, pMyEvent );
}

void CThread::OutputMsgInfoList( const QList<CMsgInfo> &MsgInfoList )
{
    for( int iLoop=0; iLoop < MsgInfoList.count(); ++iLoop )
    {
        const CMsgInfo & msgInfo = MsgInfoList.at(iLoop);
        OutputMsgInfo(msgInfo);
    }
}

void CThread::WaitThread()
{

    while( isRunning())
    {
        m_pMainWnd->SetFlagOfOtherThread(true);

        qApp->processEvents(QEventLoop::ExcludeUserInputEvents);

        msleep(500);
    }

    m_pMainWnd->SetFlagOfOtherThread(false);
}

void CThreadForOpenCfg::run()
{
    QString strInfo = QString("正在解析%1").arg(m_strXmlFilePathName);

    PutStringToSplashScreen ( strInfo, CStringEvent::Enum_Start_Event_Info);

    m_bOK = m_pXML->ParseConfig(m_strXmlFilePathName);

    if( m_bOK )
    {
        QString strInfo = QString("解析%1成功").arg(m_strXmlFilePathName);

        PutStringToSplashScreen ( strInfo, CStringEvent::Enum_End_Event_Info);
    }
    else
    {

        QString strInfo = QString("解析%1失败").arg(m_strXmlFilePathName);

        PutStringToSplashScreen ( strInfo, CStringEvent::Enum_Error_Event_Info);
    }

    //输出信息到输出框
    OutputMsgInfoList(m_pXML->GetMsgInfoList());
    m_pXML->ClearMsgInfoList();
}

CThreadForOpenCfg::CThreadForOpenCfg(CMainWnd *pMainWnd, CDataBase *pXML, QString strXmlFilePathName):CThread(pMainWnd)
{
    m_pXML = pXML;
    m_strXmlFilePathName = strXmlFilePathName;
}

void CThreadForSave::run()
{
    QString strInfo = QString("正在保存文件到%1").arg(m_strXmlFilePathName);

    PutStringToSplashScreen ( strInfo, CStringEvent::Enum_Start_Event_Info);

    QString strSaveInfo = m_pXML->SaveConfig(m_strXmlFilePathName);

    if( strSaveInfo.isEmpty()||strSaveInfo.isNull() )
    {
        m_bOK = true;

        QString strInfo = QString("文件保存成功");

        PutStringToSplashScreen ( strInfo, CStringEvent::Enum_End_Event_Info);
    }
    else
    {
        m_bOK = false;

        m_strSaveErrorInfo = strSaveInfo;

        QString strInfo = QString("文件保存失败");

        PutStringToSplashScreen ( strInfo, CStringEvent::Enum_Error_Event_Info);
    }
    //输出信息到输出框
    OutputMsgInfoList(m_pXML->GetMsgInfoList());
    m_pXML->ClearMsgInfoList();
}


CThreadForSave::CThreadForSave( CMainWnd	*pMainWnd, CDataBase *pXML, QString strFilePathName ):CThread(pMainWnd)
{
    m_pXML = pXML;
    m_strXmlFilePathName = strFilePathName;
}

void CThreadForSQL::run()
{
    QString strInfo = QString("正在保存数据库到%1").arg(m_strXmlFilePathName);

    PutStringToSplashScreen ( strInfo, CStringEvent::Enum_Start_Event_Info);

    QString strSaveInfo = m_pXML->SaveSQL(m_strXmlFilePathName);

    if( strSaveInfo.isEmpty()||strSaveInfo.isNull() )
    {
        m_bOK = true;

        QString strInfo = QString("数据库保存成功");

        PutStringToSplashScreen ( strInfo, CStringEvent::Enum_End_Event_Info);
    }
    else
    {
        m_bOK = false;

        m_strSQLErrorInfo = strSaveInfo;

        QString strInfo = QString("数据库保存失败");

        PutStringToSplashScreen ( strInfo, CStringEvent::Enum_Error_Event_Info);
    }
    //输出信息到输出框
//    OutputMsgInfoList(m_pXML->GetMsgInfoList());
//    m_pXML->ClearMsgInfoList();
}


CThreadForSQL::CThreadForSQL( CMainWnd	*pMainWnd, CDataBase *pXML, QString strFilePathName ):CThread(pMainWnd)
{
    m_pXML = pXML;
    m_strXmlFilePathName = strFilePathName;
}
