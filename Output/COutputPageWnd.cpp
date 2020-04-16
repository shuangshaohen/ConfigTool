#include <QHeaderView>
#include <QMenu>
#include <QContextMenuEvent>
#include "COutputPageWnd.h"
#include "COutputWnd.h"
#include "CMsgInfo.h"
#include <QTime>

COutputPageWnd::COutputPageWnd( COutputWnd *pOutputWnd, QWidget * parent ):QTextEdit(parent)
{
    m_pOutputWnd = pOutputWnd;
    nInfoNum = 0;
    bShowWarnningInfo = true;
    bShowErrorInfo = true;
    CreateContextMenu();
}

COutputPageWnd::~COutputPageWnd(void)
{
}

void COutputPageWnd::OutPutMsg(const CMsgInfo & MsgInfo)
{
    CMsgInfo *pMsgInfo = new CMsgInfo(MsgInfo);

    m_MsgInfoList.append(pMsgInfo);


    ShowMsg(pMsgInfo);
}
void COutputPageWnd::ShowMsg(CMsgInfo* pMsgInfo)
{
    QString strshow = "";

    switch( pMsgInfo->m_Enum_Msg_Level)
    {
    case CMsgInfo::CN_HINT_MSG:
        strshow = QString("%1").arg(nInfoNum);
        nInfoNum++;		//Output输出显示的编号自增1
        strshow += "&nbsp;&nbsp;&nbsp;&nbsp;";
        strshow += pMsgInfo->m_strInfo;
        strshow = "<font color=\"green\">" + strshow;
        strshow += "</font>";
        append(strshow);

        break;
    case CMsgInfo::CN_WARNNING_MSG:
        if (bShowWarnningInfo)
        {
            strshow = QString("%1").arg(nInfoNum);
            nInfoNum++;		//Output输出显示的编号自增1
            strshow += "&nbsp;&nbsp;&nbsp;&nbsp;";
            strshow += pMsgInfo->m_strInfo;
            strshow = "<font color=\"blue\">" + strshow;
            strshow += "</font>";
            append(strshow);
        }

        break;
    case CMsgInfo::CN_ERROR_MSG:
        if (bShowErrorInfo)
        {
            /*
            QTime timenow;
            timenow = QTime::currentTime();
            QString strtimenow = "00-00-00-0000    ";
            strtimenow = timenow.toString("hh:mm:ss.zzz");
            strtimenow += "    ";
            */
            strshow = QString("%1").arg(nInfoNum);
            nInfoNum++;		//Output输出显示的编号自增1
            strshow += "&nbsp;&nbsp;&nbsp;&nbsp;";
            strshow += pMsgInfo->m_strInfo;
            strshow = "<font color=\"red\">" + strshow;
            strshow += "</font>";
            append(strshow);
        }

        break;
    default:
        break;

    }

}


void COutputPageWnd::UpdateMsgInfo()
{
    this->clear();
    for (int i = 0; i < m_MsgInfoList.size(); ++i)
    {
        ShowMsg(m_MsgInfoList.at(i));
    }
}


void COutputPageWnd::ClearInfoNum()
{
    nInfoNum = 0;
}

void COutputPageWnd::ClearAllInfo()			//清除所有信息
{
    ClearInfoNum();
    m_pOutputWnd->ClearMsgInfo();
    m_MsgInfoList.clear();

    m_pOutputWnd->SetShowFlag(false,false);
    m_pOutputWnd->SetActionCheckedFlag(true,false,false,false);//设置动作选择的标志
    m_pOutputWnd->UpdateMsgPageWnd();
}

void COutputPageWnd::ShowAllInfo()			//显示所有信息
{
    ClearInfoNum();
    m_pOutputWnd->ClearMsgInfo();

    //bShowWarnningInfo = true;
    //bShowErrorInfo = true;
    m_pOutputWnd->SetShowFlag(true,true);
    m_pOutputWnd->SetActionCheckedFlag(false,true,false,false);//设置动作选择的标志
    m_pOutputWnd->UpdateMsgPageWnd();
}

void COutputPageWnd::ShowWarnningInfo()		//显示警告信息
{
    ClearInfoNum();
    m_pOutputWnd->ClearMsgInfo();

    //bShowWarnningInfo = true;
    //bShowErrorInfo = false;
    m_pOutputWnd->SetShowFlag(true,false);
    m_pOutputWnd->SetActionCheckedFlag(false,false,true,false);//设置动作选择的标志
    m_pOutputWnd->UpdateMsgPageWnd();
}

void COutputPageWnd::ShowErrorInfo()		//显示错误信息
{
    ClearInfoNum();
    m_pOutputWnd->ClearMsgInfo();

    //bShowErrorInfo = true;
    //bShowWarnningInfo = false;
    m_pOutputWnd->SetShowFlag(false,true);
    m_pOutputWnd->SetActionCheckedFlag(false,false,false,true);//设置动作选择的标志
    m_pOutputWnd->UpdateMsgPageWnd();
}


//设置显示的标志
void COutputPageWnd::SetShowFlag(bool bwarning,bool berror)
{
    bShowWarnningInfo = bwarning;
    bShowErrorInfo    = berror;
}


//设置动作选择的标志
void COutputPageWnd::SetActionCheckedFlag(bool bClear,bool bAll,bool bWarnning,bool bError)
{
    pClearAction->setChecked(bClear);
    pShowAllAction->setChecked(bAll);
    pShowWarnningAction->setChecked(bWarnning);
    pShowErrorAction->setChecked(bError);
}


//创建上下文菜单
void COutputPageWnd::CreateContextMenu()
{

    pClearAction = new QAction("清除信息",this);
    connect(pClearAction,SIGNAL(triggered()),this,SLOT(ClearAllInfo()));
    pClearAction->setCheckable(true);
    pClearAction->setChecked(false);

    pShowAllAction = new QAction("所有信息",this);
    connect(pShowAllAction,SIGNAL(triggered()),this,SLOT(ShowAllInfo()));
    pShowAllAction->setCheckable(true);
    pShowAllAction->setChecked(true);

    pShowWarnningAction = new QAction("告警信息",this);
    connect(pShowWarnningAction,SIGNAL(triggered()),this,SLOT(ShowWarnningInfo()));
    pShowWarnningAction->setCheckable(true);
    pShowWarnningAction->setChecked(false);

    pShowErrorAction = new QAction("错误信息",this);
    connect(pShowErrorAction,SIGNAL(triggered()),this,SLOT(ShowErrorInfo()));
    pShowErrorAction->setCheckable(true);
    pShowErrorAction->setChecked(false);

    this->addAction(pClearAction);
    this->addAction(pShowAllAction);
    this->addAction(pShowWarnningAction);
    this->addAction(pShowErrorAction);
    this->setContextMenuPolicy(Qt::ActionsContextMenu);

}
