#ifndef CMAINWND_H
#define CMAINWND_H

#include <QMainWindow>
#include <QToolBar>
#include <QStatusBar>
#include "CommGui_global.h"
#include <QDebug>
#include "CDataBase.h"
#include "CMsgInfo.h"

class CDockWidget;
class CCommActions;
class COutputWnd;
class QLabel;

class COMMGUI_EXPORT CMainWnd: public QMainWindow
{
    Q_OBJECT
public:
    virtual CCommActions  *	GetCommAction()=0;

    void SetFlagOfOtherThread(bool);
    void OutputMsgInfo( const CMsgInfo &msgInfo );
    void OutputMsgInfoList( const QList<CMsgInfo>& msgInfoList );
    void SetEditToolBarVisible( bool bVisible );
protected:
    bool						m_bHasOtherThread;
    CDataBase       *           m_pProjectXml;
    QString						m_strToolName;
    QString						m_strProjectFilePathName;
    QString						m_strLastOpenPorjectFilePath;
    QLabel			*           m_pFileNameLable;

    QStatusBar		*			m_pStatusBar;
    CDockWidget		*   		m_pOutPutDockWidget;
    CDockWidget		*			m_pToolBoxDockWidget;
    COutputWnd      *           m_pOutputWnd;

    CMainWnd(QWidget * parent = 0, Qt::WindowFlags flags = 0);

    virtual bool IsModified()=0;
    virtual void WriteSettings()=0;
    virtual void ReadSettings()=0;
    virtual void CreateToolBox();
    virtual void CreateOutputWnd();
    virtual void CreateStatusBar();
    virtual bool UserReallyWantsToSave();
    virtual void UpdateToolBoxNode();
    virtual void ShowDefaultInfo();

    void closeEvent(QCloseEvent *event);
    void customEvent ( QEvent * event ) ;
    bool SaveXmlFile( QString strFilePathName, QString &strErrorInfo ) ;
protected slots:
    //文件菜单槽
    bool NewProjectFileSlot();
    virtual void OpenProjectFileSlot();
    void SaveProjectFileSlot();
    void SaveAsProjectFileSlot();
    virtual void CloseProjectFileSlot();
    //编辑菜单槽

    virtual void SubmitSlot(){};

    //视图工具条
    void ShowConsoleWndSlot();
    void ShowManageWndSlot();
    void ShowOutputWndSlot();
    void ShowFileToolBarSlot();
    void ShowEditToolBarSlot();
    void ShowVerifyToolBarSlot();
    void ShowAppToolBarSlot();
    //工具菜单槽

    //验证菜单槽
    void VerifySlot();
    void ClearOutputInfoSlot();

    //帮助
    void HelpSlot();
    void AboutSlot();
    //视图菜单选择状态
    void SetToolMenuCheckStateSlot(bool bShow);
    void SetOutPutMenuCheckStateSlot(bool bShow);

private:

};

#endif // CMAINWND_H
