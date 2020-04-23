#include "CCfgMainWnd.h"
#include "CCfgActions.h"
#include "CDockWidget.h"
#include "CCfgToolBox.h"
#include "CSplashScreen.h"
#include "CCfgCenterWnd.h"
#include <QGridLayout>
#include <QSettings>
#include "TopoPara.h"

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
    m_strToolName = "Config配置工具V1.10";
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

void CCfgMainWnd::ShowDeviceInfo()
{
    GetCommAction()->m_pEditToolBar->setEnabled(false);
    m_pCenterWnd->ShowDeviceInfo(m_pProjectXml);
}

void CCfgMainWnd::ShowDeviceParas()
{
    GetCommAction()->m_pEditToolBar->setEnabled(false);
    m_pCenterWnd->ShowDeviceParas(m_pProjectXml);
}

void CCfgMainWnd::ShowADAnaTable()
{
    GetCommAction()->m_pEditToolBar->setEnabled(true);
    m_pCenterWnd->ShowADAnaTable(m_pProjectXml);
}

void CCfgMainWnd::ShowDerivedInfoTable()
{
    GetCommAction()->m_pEditToolBar->setEnabled(true);
    m_pCenterWnd->ShowDerivedInfoTable(m_pProjectXml);
}

void CCfgMainWnd::ShowSVInfoTable()
{
    GetCommAction()->m_pEditToolBar->setEnabled(true);
    m_pCenterWnd->ShowSVInfoTable(m_pProjectXml);
}

void CCfgMainWnd::ShowGSAnaInfoTable()
{
    GetCommAction()->m_pEditToolBar->setEnabled(true);
    m_pCenterWnd->ShowGSAnaInfoTable(m_pProjectXml);
}

void CCfgMainWnd::ShowOtherAnaTable()
{
    GetCommAction()->m_pEditToolBar->setEnabled(true);
    m_pCenterWnd->ShowOtherAnaTable(m_pProjectXml);
}

void CCfgMainWnd::ShowGeneralBiTable()
{
    GetCommAction()->m_pEditToolBar->setEnabled(true);
    m_pCenterWnd->ShowGeneralBiTable(m_pProjectXml);
}

void CCfgMainWnd::ShowSignalTable()
{
    GetCommAction()->m_pEditToolBar->setEnabled(true);
    m_pCenterWnd->ShowSignalTable(m_pProjectXml);
}

void CCfgMainWnd::ShowGooseBiTable()
{
    GetCommAction()->m_pEditToolBar->setEnabled(true);
    m_pCenterWnd->ShowGooseBiTable(m_pProjectXml);
}

void CCfgMainWnd::ShowSoftYBTable()
{
    GetCommAction()->m_pEditToolBar->setEnabled(true);
    m_pCenterWnd->ShowSoftYBTable(m_pProjectXml);
}

void CCfgMainWnd::ShowGooseBoTable()
{
    GetCommAction()->m_pEditToolBar->setEnabled(true);
    m_pCenterWnd->ShowGooseBoTable(m_pProjectXml);
}

void CCfgMainWnd::ShowTripMatrixTable()
{
    GetCommAction()->m_pEditToolBar->setEnabled(true);
    m_pCenterWnd->ShowTripMatrixTable(m_pProjectXml);
}

void CCfgMainWnd::ShowYKTable()
{
    GetCommAction()->m_pEditToolBar->setEnabled(false);
    m_pCenterWnd->ShowYKTable(m_pProjectXml);
}

void CCfgMainWnd::ShowSetSPTable()
{
    GetCommAction()->m_pEditToolBar->setEnabled(true);
    m_pCenterWnd->ShowSetSPTable(m_pProjectXml);
}

void CCfgMainWnd::ShowSetSGTable()
{
    GetCommAction()->m_pEditToolBar->setEnabled(true);
    m_pCenterWnd->ShowSetSGTable(m_pProjectXml);
}

void CCfgMainWnd::ShowEvtAlmTable()
{
    GetCommAction()->m_pEditToolBar->setEnabled(true);
    m_pCenterWnd->ShowEvtAlmTable(m_pProjectXml);
}

void CCfgMainWnd::ShowEvtActTable()
{
    GetCommAction()->m_pEditToolBar->setEnabled(true);
    m_pCenterWnd->ShowEvtActTable(m_pProjectXml);
}

void CCfgMainWnd::ShowEvtCheckTable()
{
    GetCommAction()->m_pEditToolBar->setEnabled(true);
    m_pCenterWnd->ShowEvtCheckTable(m_pProjectXml);
}

void CCfgMainWnd::ShowRemoteTripTable()
{
    GetCommAction()->m_pEditToolBar->setEnabled(true);
    m_pCenterWnd->ShowRemoteTripTable(m_pProjectXml);
}

void CCfgMainWnd::ShowParaTable()
{
    GetCommAction()->m_pEditToolBar->setEnabled(false);
    m_pCenterWnd->ShowParaTable();
}

void CCfgMainWnd::RefreshResourceInfo()
{
    m_pCfgToolBox->RefreshResourceTreeNodes();
}

void CCfgMainWnd::SetModified(bool bModified)
{
    GetCommAction()->m_pSaveToolBar->setEnabled(bModified);
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

void CCfgMainWnd::UpSlot()
{
    m_pCenterWnd->UpInfo();
}

void CCfgMainWnd::DownSlot()
{
    m_pCenterWnd->DownInfo();
}

void CCfgMainWnd::SubmitSlot()
{
    SetModified(false);
    m_pProjectXml->Submit();
    RefreshResourceInfo();
}

void CCfgMainWnd::RevertSlot()
{
    SetModified(false);
    m_pProjectXml->Revert();
    RefreshResourceInfo();
    m_pCfgToolBox->ShowCurrentNodeInfoWnd();
}

void CCfgMainWnd::NewTopoPara()
{
    TopoPara::GetInstance()->NewPara();
    m_pCfgToolBox->setParaPage();
}

void CCfgMainWnd::OpenTopoPara()
{
    TopoPara::GetInstance()->OpenPara();
    m_pCfgToolBox->setParaPage();
}

void CCfgMainWnd::SaveTopoPara()
{
    TopoPara::GetInstance()->SavePara();
    m_pCfgToolBox->setParaPage();
}

void CCfgMainWnd::CloseTopoPara()
{
    TopoPara::GetInstance()->ClearPara();
    m_pCfgToolBox->setIEDPage();
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


