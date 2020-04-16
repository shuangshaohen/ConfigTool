#ifndef COUTPUTWND_H
#define COUTPUTWND_H

#include <QTabWidget>
#include "Output_global.h"

class CMsgInfo;
class Ui_OutputWnd;
class COutputPageWnd;

class OUTPUT_EXPORT COutputWnd :	public QTabWidget
{
    Q_OBJECT
private:
    Ui_OutputWnd			*m_pUi_OutputWnd;
    COutputPageWnd			*m_pAllOutputWnd;			//总览
    COutputPageWnd			*m_pAppOutputWnd;			//应用
    COutputPageWnd			*m_pOpenOutputWnd;          //Open
    COutputPageWnd			*m_pSaveOutputWnd;          //Save
private slots:
protected:
    //void UpdateMsgPageWnd();
public:
    COutputWnd( QWidget * parent );
    void OutPutMsg( const CMsgInfo & MsgInfo );
    QSize sizeHint() const;
    void ClearMsgInfo();
    void UpdateMsgPageWnd();
    void ClearAllInfoNum();	//清空所有计数标志
    void SetShowFlag(bool bwarning,bool berror);		//设置显示的标志
    void SetActionCheckedFlag(bool bClear,bool bAll,bool bWarnning,bool bError);	//设置动作选择的标志
public:
    virtual ~COutputWnd(void);
};

#endif // COUTPUTWND_H
