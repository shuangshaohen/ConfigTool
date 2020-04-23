#include "CCommActions.h"
#include "CMainWnd.h"
#include <QtGui>

CCommActions::CCommActions(CMainWnd *pMainWnd) : m_pMainWnd(pMainWnd)
{

}


//帮助菜单
void CCommActions::CreateHelpMenu()
{
    m_pHelpMenu = m_pMainWnd->menuBar()->addMenu("帮助(&H)");
    m_pHelpMenu->addAction("使用说明", m_pMainWnd, SLOT(HelpSlot()));
    m_pHelpMenu->addAction("关于", m_pMainWnd, SLOT(AboutSlot()));
}

//验证菜单
void CCommActions::CreateVerifyMenu()
{
    m_pVerifyMenu = m_pMainWnd->menuBar()->addMenu("验证(&V)");
    m_pVerifyMenu->addAction( QIcon(":/Icon/verigy .png"), "配置校验", m_pMainWnd, SLOT(VerifySlot()));	//添加了菜单图标

    m_pVerifyMenu->addSeparator();

    //清除信息
    m_pVerifyMenu->addAction(QIcon(":/Icon/clear.png"), "清除信息", m_pMainWnd, SLOT(ClearOutputInfoSlot()));

}

//验证工具条
void CCommActions::CreateVerifyFileToolBar()
{
    m_pVerifyToolBar = new QToolBar("验证工具条",m_pMainWnd);
    //验证
    QAction *pSchemaVerifyAction = m_pVerifyToolBar->addAction ( QIcon(":/Icon/verigy .png"), "配置校验" );
    pSchemaVerifyAction->setStatusTip(tr("配置校验"));
    connect(pSchemaVerifyAction, SIGNAL(triggered()), m_pMainWnd, SLOT(VerifySlot()));

    m_pVerifyToolBar->addSeparator();

    //清除信息
    QAction *pClearOutPutInfoAction = m_pVerifyToolBar->addAction ( QIcon(":/Icon/clear.png"), "清除信息" );
    pClearOutPutInfoAction->setStatusTip(tr("清除信息"));
    connect(pClearOutPutInfoAction, SIGNAL(triggered()), m_pMainWnd, SLOT(ClearOutputInfoSlot()));

    m_pMainWnd->addToolBar(Qt::TopToolBarArea, m_pVerifyToolBar);
}

//视图菜单
void CCommActions::CreateViewMenu()
{
    m_pViewMenu = m_pMainWnd->menuBar()->addMenu("视图(&V)");
    m_pToolBoxDockAction = m_pViewMenu->addAction("工具箱", m_pMainWnd, SLOT(ShowManageWndSlot()));
    m_pToolBoxDockAction->setCheckable(true);
    m_pToolBoxDockAction->setChecked(true);

    m_pOutputWndDockAction = m_pViewMenu->addAction("输出框", m_pMainWnd, SLOT(ShowOutputWndSlot()));
    m_pOutputWndDockAction->setCheckable(true);
    m_pOutputWndDockAction->setChecked(true);

    m_pViewMenu->addSeparator();

    m_pFileToolBarDockAction = m_pViewMenu->addAction("文件工具条", m_pMainWnd, SLOT(ShowFileToolBarSlot()));
    m_pFileToolBarDockAction->setCheckable(true);
    m_pFileToolBarDockAction->setChecked(true);

    m_pEditToolBarDockAction = m_pViewMenu->addAction("编辑工具条", m_pMainWnd, SLOT(ShowEditToolBarSlot()));
    m_pEditToolBarDockAction->setCheckable(true);
    m_pEditToolBarDockAction->setChecked(true);

    m_pVerifyToolBarDockAction = m_pViewMenu->addAction("验证工具条", m_pMainWnd, SLOT(ShowVerifyToolBarSlot()));
    m_pVerifyToolBarDockAction->setCheckable(true);
    m_pVerifyToolBarDockAction->setChecked(true);
}

//菜单条
void CCommActions::CreateMenuBar()
{
    m_pMenuBar = new QMenuBar(m_pMainWnd);
    m_pMenuBar->setGeometry(QRect(0, 0, 800, 20));
    m_pMainWnd->setMenuBar(m_pMenuBar);

    //文件菜单
    CreateFileMenu();

    //编辑菜单
    CreateEditMenu();

    //视图菜单
    CreateViewMenu();

    //验证菜单
    CreateVerifyMenu();

    //工具菜单
    CreateAppToolMenu();

    //帮助菜单
    CreateHelpMenu();

}


//工具条
void CCommActions::CreateToolBar()
{
    CreateFileToolBar();
    CreateSaveToolBar();
    CreateEditFileToolBar();
    CreateVerifyFileToolBar();
    CreateAppFileToolBar();
}
