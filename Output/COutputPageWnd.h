#ifndef COUTPUTPAGEWND_H
#define COUTPUTPAGEWND_H

#include <QList>
//#include <QTreeWidget>
#include <QTextEdit>
#include <QMenu>
class CMsgInfo;
class COutputWnd;

class COutputPageWnd :	public QTextEdit
{
    Q_OBJECT
public:
    COutputPageWnd( COutputWnd *pOutputWnd, QWidget * parent );
    void OutPutMsg( const CMsgInfo &MsgInfo);
    void UpdateMsgInfo();
    virtual ~COutputPageWnd(void);
    void ClearInfoNum();	//清空Output输出显示的编号
    void SetShowFlag(bool bwarning,bool berror);		//设置显示标志
    void SetActionCheckedFlag(bool bClear,bool bAll,bool bWarnning,bool bError);	//设置动作选择的标志

protected:
    void CreateContextMenu();

private slots:
    void ShowAllInfo();			//显示所有信息
    void ShowWarnningInfo();	//显示警告信息
    void ShowErrorInfo();		//显示错误信息
    void ClearAllInfo();		//清除所有信息

private:
    void ShowMsg(CMsgInfo* pMsgInfo);
private:
    COutputWnd		*m_pOutputWnd;
    QList<CMsgInfo*> m_MsgInfoList;

    int nInfoNum;		//Output输出显示的编号
    bool bShowAllInfo;
    bool bShowWarnningInfo;
    bool bShowErrorInfo;

    QAction *pClearAction;
    QAction *pShowAllAction;
    QAction *pShowWarnningAction;
    QAction *pShowErrorAction;
};

#endif // COUTPUTPAGEWND_H
