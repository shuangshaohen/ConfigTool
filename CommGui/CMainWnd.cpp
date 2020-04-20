#include "CMainWnd.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QCloseEvent>
#include <QLabel>
#include "CBaseThread.h"
#include "CSplashScreen.h"
#include "COutputWnd.h"
#include "CDockWidget.h"
#include "CCommActions.h"
#include <QDesktopServices>

void CMainWnd::SetFlagOfOtherThread(bool bHasOtherThread)
{
    m_bHasOtherThread = bHasOtherThread;
}

void CMainWnd::OutputMsgInfo(const CMsgInfo &msgInfo)
{
    m_pOutputWnd->OutPutMsg(msgInfo);
}

void CMainWnd::OutputMsgInfoList(const QList<CMsgInfo> &msgInfoList)
{
    for( int iLoop=0; iLoop < msgInfoList.count(); ++iLoop )
    {
        m_pOutputWnd->OutPutMsg(msgInfoList.at(iLoop));
    }
}

void CMainWnd::SetEditToolBarVisible(bool bVisible)
{
    GetCommAction()->m_pEditToolBar->setVisible(bVisible);

    GetCommAction()->m_pEditToolBarDockAction->setChecked(bVisible);
}

CMainWnd::CMainWnd(QWidget *parent, Qt::WindowFlags flags)
            :QMainWindow(parent,flags),m_bHasOtherThread(false),m_pProjectXml(NULL)
{
    m_strToolName = "MainWnd";
    m_strProjectFilePathName = "";
    m_strLastOpenPorjectFilePath = "./";

    setWindowTitle(m_strToolName);

    //////////////////////////////////////////////////////////////////////////
    //设置主窗口背景色
    this->setAutoFillBackground(true);
    QPalette palette = this->palette();
    //QColor backgroundcolor = QColor(180,210,230);
    QColor backgroundcolor = QColor(192,192,192);
    palette.setColor(QPalette::Window,backgroundcolor);
    this->setPalette(palette);
    //this->setBackgroundColor(backgroundcolor);

    QPalette palette2 = this->palette();
    QColor backgroundcolor2 = QColor(180,200,200);
    palette2.setColor(QPalette::Base,backgroundcolor2);
    //this->setPalette(palette2);

    //设置控件样式
    this->setStyleSheet(QString("QTextEdit{background:transparent;}"));
    //////////////////////////////////////////////////////////////////////////
}

void CMainWnd::CreateToolBox()
{

}

void CMainWnd::CreateOutputWnd()
{
    m_pOutPutDockWidget = new CDockWidget(this);
    m_pOutPutDockWidget->setWindowTitle("输出");

    m_pOutputWnd = new COutputWnd(m_pOutPutDockWidget);
    m_pOutPutDockWidget->setWidget(m_pOutputWnd);
    addDockWidget(Qt::BottomDockWidgetArea, m_pOutPutDockWidget);
}

void CMainWnd::CreateStatusBar()
{
    m_pStatusBar = new QStatusBar(this);
    setStatusBar(m_pStatusBar);

    m_pFileNameLable = new	QLabel("Cfg文件未打开");
    statusBar()->addPermanentWidget(m_pFileNameLable, 10);
}

bool CMainWnd::UserReallyWantsToSave()
{
    QString strInfo;

    strInfo = "Cfg文件已被修改，是否保存？";

    return ( QMessageBox::Save == QMessageBox::question ( this, "关闭应用程序", strInfo, QMessageBox::Save, QMessageBox::Cancel )) ;
}

void CMainWnd::UpdateToolBoxNode()
{

}

void CMainWnd::ShowDefaultInfo()
{

}

void CMainWnd::closeEvent(QCloseEvent *event)
{
    if( m_bHasOtherThread )
    {
        event->ignore();
        this->showMinimized();
    }
    else
    {
        WriteSettings();
        if( IsModified() )
        {
            if( UserReallyWantsToSave() )
            {
                SaveProjectFileSlot();
            }
        }
        event->accept();
    }
}

void CMainWnd::customEvent(QEvent *event)
{
    CMsgEvent		*	pMsgEvent = NULL;
    CStringEvent	*	pStringEvent = NULL;
    CEvent * pEvent = (CEvent*)event;
    switch( (CEvent::Enum_Event_Code) pEvent->type())
    {
    case CEvent::Enum_Start_Event_Info:
        pStringEvent = (CStringEvent*)event;
        CSplashScreen::Instance()->StartShow();
        CSplashScreen::Instance()->ShowMessage(pStringEvent->m_strInfo);
        break;
    case CEvent::Enum_End_Event_Info:
    case CEvent::Enum_Error_Event_Info:
        pStringEvent = (CStringEvent*)event;
        CSplashScreen::Instance()->ShowMessage(pStringEvent->m_strInfo);
        CSplashScreen::Instance()->EndShow();
        break;
    case CEvent::Enum_Msg_Event_Info:
        pMsgEvent  = (CMsgEvent*)event;
        OutputMsgInfo(pMsgEvent->m_MsgInfo);
        break;
    default:
        QMainWindow::customEvent(event);
        break;
    }
}

bool CMainWnd::SaveXmlFile(QString strFilePathName, QString &strErrorInfo)
{
    if( NULL==m_pProjectXml )
        return false;

    CThreadForSave *pThreadForSave = new CThreadForSave( this, m_pProjectXml, strFilePathName );

    pThreadForSave->start();

    pThreadForSave->WaitThread();

    bool bSave = pThreadForSave->IsOK();

    strErrorInfo = pThreadForSave->GetSaveErrorInfo();

    delete pThreadForSave;

    return bSave;
}

bool CMainWnd::NewProjectFileSlot()
{
    bool bNew;

    bNew = (QMessageBox::Ok == QMessageBox::question ( this, "新建Cfg文件", "真要新建Cfg文件", QMessageBox::Ok , QMessageBox::Cancel ));

    if( !bNew )
        return false;

    CloseProjectFileSlot();

    m_strProjectFilePathName = QFileDialog::getSaveFileName ( this, "新建Cfg文件", m_strLastOpenPorjectFilePath, "Cfg文件 (*.xml)" );

    if( m_strProjectFilePathName.length() <= 0 )
        return false;

    if( !m_strProjectFilePathName.endsWith(".xml",Qt::CaseInsensitive))
    {
        m_strProjectFilePathName.append(".xml");
    }

    m_pFileNameLable->setText("文件位置：" + m_strProjectFilePathName);

    ShowDefaultInfo();

    return true;
}

void CMainWnd::OpenProjectFileSlot()
{
    QString strFilePathName;

    strFilePathName = QFileDialog::getOpenFileName ( this, "打开Cfg文件", m_strLastOpenPorjectFilePath, "Cfg文件 (*.xml)" );

    if( strFilePathName.isEmpty() )
        return ;

    CloseProjectFileSlot();

    m_pProjectXml = new CDataBase();

    CThreadForOpenCfg *pThread = new CThreadForOpenCfg(this,m_pProjectXml,strFilePathName);

    pThread->start();

    pThread->WaitThread();

    if(pThread->IsOK())
    {
        m_strProjectFilePathName = strFilePathName;
        m_pFileNameLable->setText("文件位置：" + m_strProjectFilePathName);
        QFileInfo fileInfo(strFilePathName);
        m_strLastOpenPorjectFilePath = fileInfo.absoluteFilePath();
        //m_pProjectXml->SetModified(false);
        UpdateToolBoxNode();
        ShowDefaultInfo();
    }
    else
    {
        QString strInfo = "打开Cfg文件出错，文件不存在或者格式不正确";
        QMessageBox::critical(this,"打开Cfg文件",strInfo);

        delete m_pProjectXml;

        m_pProjectXml = NULL;
    }
    delete pThread;
}

void CMainWnd::SaveProjectFileSlot()
{
    QString strInfo;

    if( NULL==m_pProjectXml )
        return;

    SubmitSlot();
    strInfo = "Cfg文件保存成功。文件位置：";

    if( m_strProjectFilePathName.isEmpty() )
    {
        m_strProjectFilePathName = QFileDialog::getSaveFileName( this, "保存文件", m_strLastOpenPorjectFilePath, "Cfg文件 (*.xml)" );

        if( m_strProjectFilePathName.length() <= 0 )
        {
            return;
        }

        if( !m_strProjectFilePathName.endsWith(".xml", Qt::CaseInsensitive) )
        {
            m_strProjectFilePathName.append(".xml");
        }
    }

    QString strErrorInfo;

    if( SaveXmlFile( m_strProjectFilePathName, strErrorInfo ) )
    {
        m_pFileNameLable->setText("文件位置：" + m_strProjectFilePathName);

        QMessageBox::information(this,"文件保存成功", strInfo + m_strProjectFilePathName );
    }
    else
    {
        QMessageBox::critical(this,"文件保存失败", strErrorInfo );
    }

    QFileInfo fileInfo(m_strProjectFilePathName);

    m_strLastOpenPorjectFilePath = fileInfo.absoluteFilePath();
}

void CMainWnd::SaveAsProjectFileSlot()
{
    QString strInfo;

    QString strProjectFilePathName;

    if( NULL==m_pProjectXml )
        return;

    SubmitSlot();
    strProjectFilePathName = QFileDialog::getSaveFileName( this, "另存为...", m_strLastOpenPorjectFilePath, "Cfg文件 (*.xml)" );

    if( strProjectFilePathName.length() <= 0 )
    {
        return;
    }

    if( !strProjectFilePathName.endsWith(".xml", Qt::CaseInsensitive) )
    {
        strProjectFilePathName.append(".xml");
    }

    strInfo = "Cfg文件保存成功。文件位置：";

    QString strErrorInfo;

    if( SaveXmlFile( strProjectFilePathName, strErrorInfo ) )
    {
        m_pFileNameLable->setText("文件位置：" + strProjectFilePathName);

        strInfo = strInfo + strProjectFilePathName;

        QMessageBox::information(this,"文件保存成功",strInfo );
    }
    else
    {
        QMessageBox::critical(this,"文件保存失败", strErrorInfo );
    }

    m_strProjectFilePathName = strProjectFilePathName;
    QFileInfo fileInfo(strProjectFilePathName);

    m_strLastOpenPorjectFilePath = fileInfo.absoluteFilePath();
}

void CMainWnd::CloseProjectFileSlot()
{
    if(m_pProjectXml)
    {
        if( m_pProjectXml->GetModified() && UserReallyWantsToSave() )
        {
            SaveProjectFileSlot();
        }

        delete m_pProjectXml;
        m_pProjectXml = NULL;
    }

    m_strProjectFilePathName = "";

    m_pFileNameLable->setText("Cfg文件未打开");
}

void CMainWnd::ShowConsoleWndSlot()
{

}

void CMainWnd::ShowManageWndSlot()
{
    if( m_pToolBoxDockWidget->isHidden() )
    {
        m_pToolBoxDockWidget->show();
        //qDebug()<<"ShowManageWndSlot show!";
    }
    else
    {
        m_pToolBoxDockWidget->hide();
        //qDebug()<<"ShowManageWndSlot hide!";
    }
}

void CMainWnd::ShowOutputWndSlot()
{
    if(m_pOutPutDockWidget->isHidden())
    {
        m_pOutPutDockWidget->show();
        //qDebug()<<"ShowOutputWndSlot show!";
    }
    else
    {
        m_pOutPutDockWidget->hide();
        //qDebug()<<"ShowOutputWndSlot hide!";
    }
}

void CMainWnd::ShowFileToolBarSlot()
{
    if( GetCommAction()->m_pFileToolBar->isHidden() )
    {
        GetCommAction()->m_pFileToolBar->show();
        //qDebug()<<"ShowFileToolBarSlot show!";
    }
    else
    {
        GetCommAction()->m_pFileToolBar->hide();
        //qDebug()<<"ShowFileToolBarSlot hide!";
    }
}

void CMainWnd::ShowEditToolBarSlot()
{
    if( GetCommAction()->m_pEditToolBar->isHidden() )
    {
        GetCommAction()->m_pEditToolBar->show();
    }
    else
    {
        GetCommAction()->m_pEditToolBar->hide();
    }
}

void CMainWnd::ShowVerifyToolBarSlot()
{
    if( GetCommAction()->m_pVerifyToolBar->isHidden() )
    {
        GetCommAction()->m_pVerifyToolBar->show();
    }
    else
    {
        GetCommAction()->m_pVerifyToolBar->hide();
    }
}

void CMainWnd::ShowAppToolBarSlot()
{
    if( GetCommAction()->m_pAppToolBar->isHidden() )
    {
        GetCommAction()->m_pAppToolBar->show();
    }
    else
    {
        GetCommAction()->m_pAppToolBar->hide();
    }
}

void CMainWnd::VerifySlot()
{
    if( NULL==m_pProjectXml )
    {
        return;
    }
    m_pProjectXml->CheckConfig();
    //输出信息到输出框
    OutputMsgInfoList(m_pProjectXml->GetMsgInfoList());
    m_pProjectXml->ClearMsgInfoList();
}

void CMainWnd::ClearOutputInfoSlot()
{
    m_pOutputWnd->ClearMsgInfo();
}

void CMainWnd::HelpSlot()
{
    QString strDoc = "./help.pdf";
    //qDebug() << QDir::currentPath();
    QDesktopServices::openUrl(QUrl::fromLocalFile(strDoc));
}

void CMainWnd::AboutSlot()
{
    QMessageBox::about(this,"Config配置工具","用于新版本config配置文件的编辑");
}

void CMainWnd::SetToolMenuCheckStateSlot(bool bShow)
{
    GetCommAction()->m_pToolBoxDockAction = GetCommAction()->m_pToolBoxDockAction;
    GetCommAction()->m_pToolBoxDockAction->setChecked(bShow);
}

void CMainWnd::SetOutPutMenuCheckStateSlot(bool bShow)
{
    GetCommAction()->m_pOutputWndDockAction->setChecked(bShow);
}
