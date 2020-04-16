#include <QToolButton>
#include "CMsgInfo.h"
#include "COutputWnd.h"
#include "ui_OutputWnd.h"
#include "COutputPageWnd.h"

COutputWnd::COutputWnd(QWidget * parent):QTabWidget(parent)
{
    QGridLayout *pGridLayout;
    this->setTabPosition( QTabWidget::South);
    m_pUi_OutputWnd = new Ui_OutputWnd();
    m_pUi_OutputWnd->setupUi(this);

    m_pAllOutputWnd = new COutputPageWnd( this,m_pUi_OutputWnd->m_AllInfoPage);
    pGridLayout = new QGridLayout(m_pUi_OutputWnd->m_AllInfoPage);
    pGridLayout->setContentsMargins(0, 0, 0,0);
    pGridLayout->addWidget(m_pAllOutputWnd, 0, 0, 1, 1);

    m_pOpenOutputWnd = new COutputPageWnd(this,m_pUi_OutputWnd->m_OpenInfoPage);
    pGridLayout = new QGridLayout(m_pUi_OutputWnd->m_OpenInfoPage);
    pGridLayout->setContentsMargins(0, 0, 0,0);
    pGridLayout->addWidget(m_pOpenOutputWnd, 0, 0, 1, 1);

    m_pSaveOutputWnd = new COutputPageWnd(this,m_pUi_OutputWnd->m_SaveInfoPage);
    pGridLayout = new QGridLayout(m_pUi_OutputWnd->m_SaveInfoPage);
    pGridLayout->setContentsMargins(0, 0, 0,0);
    pGridLayout->addWidget(m_pSaveOutputWnd, 0, 0, 1, 1);

    m_pAppOutputWnd = new COutputPageWnd(this,m_pUi_OutputWnd->m_AppInfoPage);
    pGridLayout = new QGridLayout(m_pUi_OutputWnd->m_AppInfoPage);
    pGridLayout->setContentsMargins(0, 0, 0,0);
    pGridLayout->addWidget(m_pAppOutputWnd, 0, 0, 1, 1);

    setMinimumHeight(150);
}

COutputWnd::~COutputWnd(void)
{
}

QSize COutputWnd::sizeHint() const
{
    return QSize(1000, 200); /* 在这里定义dock的初始大小 */
}

void COutputWnd::OutPutMsg(const CMsgInfo &MsgInfo)
{
    switch(MsgInfo.m_Enum_Verify_Mode)
    {
    case CMsgInfo::Enum_Comm_Mode:
        m_pAllOutputWnd->OutPutMsg(MsgInfo);
        break;
    case CMsgInfo::Enum_Application_Parse_Mode:
        m_pAllOutputWnd->OutPutMsg(MsgInfo);
        m_pOpenOutputWnd->OutPutMsg(MsgInfo);
        break;
    case CMsgInfo::Enum_Application_Save_Mode:
        m_pAllOutputWnd->OutPutMsg(MsgInfo);
        m_pSaveOutputWnd->OutPutMsg(MsgInfo);
        break;
    case CMsgInfo::Enum_Application_Verify_Mode:
        m_pAllOutputWnd->OutPutMsg(MsgInfo);
        m_pAppOutputWnd->OutPutMsg(MsgInfo);
        break;
    }
}


void COutputWnd::UpdateMsgPageWnd()
{
    ClearAllInfoNum();

    m_pAllOutputWnd->UpdateMsgInfo();
    m_pOpenOutputWnd->UpdateMsgInfo();
    m_pSaveOutputWnd->UpdateMsgInfo();
    m_pAppOutputWnd->UpdateMsgInfo();
}

void COutputWnd::ClearMsgInfo()
{
    ClearAllInfoNum();

    m_pAllOutputWnd->clear();
    m_pOpenOutputWnd->clear();
    m_pSaveOutputWnd->clear();
    m_pAppOutputWnd->clear();
}

//清空所有计数标志
void COutputWnd::ClearAllInfoNum()
{
    //清空Output计数标志
    m_pAllOutputWnd->ClearInfoNum();
    m_pOpenOutputWnd->ClearInfoNum();
    m_pSaveOutputWnd->ClearInfoNum();
    m_pAppOutputWnd->ClearInfoNum();
}

//设置显示的标志
void COutputWnd::SetShowFlag(bool bwarning,bool berror)
{
    m_pAllOutputWnd->SetShowFlag(bwarning,berror);
    m_pOpenOutputWnd->SetShowFlag(bwarning,berror);
    m_pSaveOutputWnd->SetShowFlag(bwarning,berror);
    m_pAppOutputWnd->SetShowFlag(bwarning,berror);
}


//设置动作选择的标志
void COutputWnd::SetActionCheckedFlag(bool bClear,bool bAll,bool bWarnning,bool bError)
{
    m_pAllOutputWnd->SetActionCheckedFlag(bClear,bAll,bWarnning,bError);
    m_pOpenOutputWnd->SetActionCheckedFlag(bClear,bAll,bWarnning,bError);
    m_pSaveOutputWnd->SetActionCheckedFlag(bClear,bAll,bWarnning,bError);
    m_pAppOutputWnd->SetActionCheckedFlag(bClear,bAll,bWarnning,bError);
}
