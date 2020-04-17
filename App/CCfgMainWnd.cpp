#include "CCfgMainWnd.h"
#include "CCfgActions.h"
#include "CDockWidget.h"
#include "CCfgToolBox.h"
#include "CSplashScreen.h"
#include "CCfgCenterWnd.h"
#include <QGridLayout>
#include <QSettings>

CCfgMainWnd::CCfgMainWnd(QWidget *parent, Qt::WindowFlags flags):CMainWnd(parent,flags)
{
    SetToolTile();

    CreateActions();
    CreateToolBox();
    CreateOutputWnd();
    CreateStatusBar();
    CreateCenterWnd();

    connect(m_pToolBoxDockWidget,SIGNAL(visibilityChanged(bool)),this,SLOT(SetToolMenuCheckStateSlot(bool)));
    connect(m_pOutPutDockWidget,SIGNAL(visibilityChanged(bool)),this,SLOT(SetOutPutMenuCheckStateSlot(bool)));
}

void CCfgMainWnd::SetToolTile()
{
    m_strToolName = "Config配置工具V1.00";
    setWindowIcon( QIcon(":/png/Icon/CfgTitleIcon.png" ));
    setWindowTitle(m_strToolName);
}

CCfgMainWnd *CCfgMainWnd::Instance()
{
    static  CCfgMainWnd *_pMainWndForCfg = NULL;
    if( NULL==_pMainWndForCfg)
    {
        _pMainWndForCfg = new CCfgMainWnd();

    }
    return _pMainWndForCfg;
}

void CCfgMainWnd::ShowDefaultInfo()
{
    GetCommAction()->m_pEditToolBar->setEnabled(false);
    m_pCenterWnd->ShowDefaultInfo(m_pProjectXml);
}

void CCfgMainWnd::ShowRootInfo()
{
    GetCommAction()->m_pEditToolBar->setEnabled(false);
    m_pCenterWnd->ShowDefaultInfo(m_pProjectXml);
}

void CCfgMainWnd::ShowAnaInfo()
{
    GetCommAction()->m_pEditToolBar->setEnabled(true);
    m_pCenterWnd->ShowAnaInfo(m_pProjectXml);
}

void CCfgMainWnd::ShowBIInfo()
{
    GetCommAction()->m_pEditToolBar->setEnabled(true);
    m_pCenterWnd->ShowBIInfo(m_pProjectXml);
}

void CCfgMainWnd::ShowGSBIInfo()
{
    GetCommAction()->m_pEditToolBar->setEnabled(true);
    m_pCenterWnd->ShowGSBIInfo(m_pProjectXml);
}

void CCfgMainWnd::ShowGSBOInfo()
{
    GetCommAction()->m_pEditToolBar->setEnabled(true);
    m_pCenterWnd->ShowGSBOInfo(m_pProjectXml);
}

void CCfgMainWnd::ShowTripMatrixInfo()
{
    GetCommAction()->m_pEditToolBar->setEnabled(true);
    m_pCenterWnd->ShowTripMatrixInfo(m_pProjectXml);
}

void CCfgMainWnd::ShowSettingInfo()
{
    GetCommAction()->m_pEditToolBar->setEnabled(true);
    m_pCenterWnd->ShowSettingInfo(m_pProjectXml);
}

void CCfgMainWnd::ShowSoftYBInfo()
{
    GetCommAction()->m_pEditToolBar->setEnabled(true);
    m_pCenterWnd->ShowSoftYBInfo(m_pProjectXml);
}

void CCfgMainWnd::ShowAlarmInfo()
{
    GetCommAction()->m_pEditToolBar->setEnabled(true);
    m_pCenterWnd->ShowAlmRecInfo(m_pProjectXml);
}

void CCfgMainWnd::ShowActInfo()
{
    GetCommAction()->m_pEditToolBar->setEnabled(true);
    m_pCenterWnd->ShowActRecInfo(m_pProjectXml);
}

void CCfgMainWnd::ShowRemoteInfo()
{

}

void CCfgMainWnd::RefreshResourceInfo()
{
    m_pCfgToolBox->RefreshResourceTreeNodes();
}

bool CCfgMainWnd::IsModified()
{
    if( NULL==m_pProjectXml )
    {
        return false;
    }

    return m_pProjectXml->GetModified();
}

void CCfgMainWnd::CreateActions()
{
    m_pCfgActions = new CCfgActions(this);
}

void CCfgMainWnd::CreateToolBox()
{
    m_pToolBoxDockWidget = new CDockWidget(this);
    m_pToolBoxDockWidget->setWindowTitle("工具箱");

    m_pToolBoxDockWidget->setAllowedAreas ( Qt::LeftDockWidgetArea );
    m_pCfgToolBox = new CCfgToolBox(m_pToolBoxDockWidget);
    m_pToolBoxDockWidget->setWidget(m_pCfgToolBox);
    addDockWidget(Qt::LeftDockWidgetArea, m_pToolBoxDockWidget);
}

void CCfgMainWnd::CreateCenterWnd()
{
    QWidget *pCentralWidget = new QWidget(this);
    m_pCenterWnd = new CCfgCenterWnd(pCentralWidget);
    QGridLayout *pGridLayout = new QGridLayout(pCentralWidget);
    //pGridLayout->setContentsMargins(0, 2, 0,0);
    pGridLayout->addWidget(m_pCenterWnd, 0, 0, 1, 1);
    setCentralWidget(pCentralWidget);
}

void CCfgMainWnd::WriteSettings()
{
    QSettings settings("GZSH", "CFG");

    settings.beginGroup("FilePathName");
    settings.setValue("LastOpenIcdFilePath", m_strLastOpenPorjectFilePath );
    settings.endGroup();
}

void CCfgMainWnd::ReadSettings()
{
    QSettings settings("GZSH", "CFG");

    settings.beginGroup("FilePathName");
    m_strLastOpenPorjectFilePath = settings.value("LastOpenIcdFilePath", "./").toString();
    settings.endGroup();
}

CCommActions *CCfgMainWnd::GetCommAction()
{
    return m_pCfgActions;
}

void CCfgMainWnd::CloseProjectFileSlot()
{
    m_pCfgToolBox->ClearTreeNode();
    m_pCenterWnd->ClearTableModel();

    CMainWnd::CloseProjectFileSlot();
}

void CCfgMainWnd::AddSlot()
{
    m_pCenterWnd->AddInfo();
}

void CCfgMainWnd::DeleteSlot()
{
    m_pCenterWnd->DeleteInfo();
}

void CCfgMainWnd::CopySlot()
{
    m_pCenterWnd->CopyInfo();
}

void CCfgMainWnd::UpSlot()
{
    m_pCenterWnd->UpInfo();
}

void CCfgMainWnd::DownSlot()
{
    m_pCenterWnd->DownInfo();
}

void CCfgMainWnd::NewCfgProjectFileSlot()
{
    //新建Cfg文件名称
    if( !NewProjectFileSlot() )
    {
        return;
    }

    if (m_strProjectFilePathName.length() <=0 )
    {
        return;
    }
    //新建基本信息
    m_pProjectXml = new CDataBase();

    //刷新树节点
    UpdateToolBoxNode();
}

void CCfgMainWnd::UpdateToolBoxNode()
{
    CSplashScreen::Instance()->StartShow();
    CSplashScreen::Instance()->ShowMessage("创建浏览节点");
    m_pCfgToolBox->CreateResourceTreeNodes(m_pProjectXml);
    CSplashScreen::Instance()->EndShow();
}
