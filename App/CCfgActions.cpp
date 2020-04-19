#include "CCfgActions.h"
#include "CCfgMainWnd.h"

CCfgActions::CCfgActions(CCfgMainWnd *pCfgMainWnd) : CCommActions(pCfgMainWnd), m_pCfgMainWnd(pCfgMainWnd)
{
    CreateActions();
}

void CCfgActions::CreateActions()
{

    CreateMenuAndToolBar();
}

void CCfgActions::CreateMenuAndToolBar()
{
    CreateMenuBar();
    CreateToolBar();
}

//文件菜单
void CCfgActions::CreateFileMenu()
{
    m_pFileMenu = m_pCfgMainWnd->menuBar()->addMenu("文件(&F)");
    m_pFileMenu->addAction(QIcon(":/png/Icon/FileNew.png"),"新建配置文件", m_pCfgMainWnd, SLOT(NewCfgProjectFileSlot()), QKeySequence(tr("Ctrl+N")));
    m_pFileMenu->addAction(QIcon(":/png/Icon/FileOpen.png"),"打开配置文件", m_pCfgMainWnd, SLOT(OpenProjectFileSlot()), QKeySequence(tr("Ctrl+O")));
    m_pFileMenu->addAction(QIcon(":/png/Icon/FileSave.png"),"保存配置文件", m_pCfgMainWnd, SLOT(SaveProjectFileSlot()), QKeySequence(tr("Ctrl+Shift+S")));

    m_pFileMenu->addAction("配置另存为...", m_pCfgMainWnd, SLOT(SaveAsProjectFileSlot()) );
    m_pFileMenu->addSeparator();
    m_pFileMenu->addAction("关闭配置文件", m_pCfgMainWnd, SLOT(CloseProjectFileSlot()));
    m_pFileMenu->addSeparator();
    m_pFileMenu->addAction("退出", m_pCfgMainWnd, SLOT(close()));
}

void CCfgActions::CreateFileToolBar()
{
    m_pFileToolBar = new QToolBar("文件工具条",m_pCfgMainWnd);
    //新建
    QAction *pNewFileAction = m_pFileToolBar->addAction ( QIcon(":/png/Icon/FileNew.png"), "新建" );
    pNewFileAction->setStatusTip(tr("新建配置文件"));
    connect(pNewFileAction, SIGNAL(triggered()), m_pCfgMainWnd, SLOT(NewCfgProjectFileSlot()));

    //打开
    QAction *pOpenFileAction = m_pFileToolBar->addAction ( QIcon(":/png/Icon/FileOpen.png"), "打开" );
    pOpenFileAction->setStatusTip(tr("打开配置文件"));
    connect(pOpenFileAction, SIGNAL(triggered()), m_pCfgMainWnd, SLOT(OpenProjectFileSlot()));

    //保存
    QAction *pSaveFileAction = m_pFileToolBar->addAction ( QIcon(":/png/Icon/FileSave.png"), "保存" );
    pSaveFileAction->setStatusTip(tr("保存配置文件"));
    connect(pSaveFileAction, SIGNAL(triggered()), m_pCfgMainWnd, SLOT(SaveProjectFileSlot()));

    m_pCfgMainWnd->addToolBar(Qt::TopToolBarArea, m_pFileToolBar);
}

//编辑菜单
void CCfgActions::CreateEditMenu()
{
    m_pEditMenu = m_pCfgMainWnd->menuBar()->addMenu("编辑(&E)");
    //m_pEditMenu->addAction("编辑ICD数据类型模板", m_pCfgMainWnd, SLOT(EditIcdLNSlot()));
    //m_pEditMenu->addSeparator();
    //m_pEditMenu->addAction("编辑通用数据类型词典", m_pCfgMainWnd, SLOT(EditTplLNSlot()));
    //m_pEditMenu->addSeparator();
    //m_pEditMenu->addAction("整理ICD文件", m_pCfgMainWnd, SLOT(CleanIcdLNSlot()));
}

void CCfgActions::CreateEditFileToolBar()
{
    m_pEditToolBar = new QToolBar("编辑工具条",m_pCfgMainWnd);
    //新增
    QAction *pAddAction = m_pEditToolBar->addAction ( QIcon(":/png/Icon/plus.png"), "新增" );
    pAddAction->setStatusTip(tr("新增"));
    connect(pAddAction, SIGNAL(triggered()), m_pCfgMainWnd, SLOT(AddSlot()));

    //删除
    QAction *pDeleteAction = m_pEditToolBar->addAction ( QIcon(":/png/Icon/minus.png"), "删除" );
    pDeleteAction->setStatusTip(tr("删除"));
    connect(pDeleteAction, SIGNAL(triggered()), m_pCfgMainWnd, SLOT(DeleteSlot()));

    //上移
    QAction *pUpAction = m_pEditToolBar->addAction ( QIcon(":/png/Icon/up.png"), "上移" );
    pUpAction->setStatusTip(tr("上移"));
    connect(pUpAction, SIGNAL(triggered()), m_pCfgMainWnd, SLOT(UpSlot()));

    //下移
    QAction *pDownAction = m_pEditToolBar->addAction ( QIcon(":/png/Icon/down.png"), "下移" );
    pDownAction->setStatusTip(tr("下移"));
    connect(pDownAction, SIGNAL(triggered()), m_pCfgMainWnd, SLOT(DownSlot()));


    m_pCfgMainWnd->addToolBar(Qt::TopToolBarArea, m_pEditToolBar);
    m_pEditToolBar->setEnabled(false);
}

void CCfgActions::CreateSaveToolBar()
{
    m_pSaveToolBar = new QToolBar("保存工具条",m_pCfgMainWnd);
    //确认更改
    QAction *pSubmitAction = m_pSaveToolBar->addAction ( QIcon(":/png/Icon/submit.png"), "确认更改" );
    pSubmitAction->setStatusTip(tr("确认更改"));
    pSubmitAction->setShortcut(QKeySequence(tr("Ctrl+S")));
    connect(pSubmitAction, SIGNAL(triggered()), m_pCfgMainWnd, SLOT(SubmitSlot()));

    //撤销更改
    QAction *pRevertAction = m_pSaveToolBar->addAction ( QIcon(":/png/Icon/revert.png"), "撤销更改" );
    pRevertAction->setStatusTip(tr("撤销更改"));
    pRevertAction->setShortcut(QKeySequence(tr("Ctrl+Z")));
    connect(pRevertAction, SIGNAL(triggered()), m_pCfgMainWnd, SLOT(RevertSlot()));
    m_pCfgMainWnd->addToolBar(Qt::TopToolBarArea, m_pSaveToolBar);
    m_pSaveToolBar->setEnabled(false);
}

void CCfgActions::CreateAppFileToolBar()
{

}

 //工具菜单
void CCfgActions::CreateAppToolMenu()
{

}
