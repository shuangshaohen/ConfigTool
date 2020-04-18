#ifndef CCFGMAINWND_H
#define CCFGMAINWND_H

#include "CMainWnd.h"
#include <QDebug>

class CCfgActions;
class CCfgToolBox;
class CCfgCenterWnd;

class CCfgMainWnd : public CMainWnd
{
    Q_OBJECT
public:
    static CCfgMainWnd* Instance();

    void ShowDefaultInfo();
    void ShowDeviceInfo();
    void ShowDeviceParas();
    void ShowADAnaTable();
    void ShowDerivedInfoTable();
    void ShowSVInfoTable();
    void ShowGSAnaInfoTable();
    void ShowOtherAnaTable();

    void RefreshResourceInfo();

    void SetModified(bool bModified);
protected:
    bool            IsModified();
    void            CreateActions();
    void            CreateToolBox();
    void            CreateCenterWnd();
    void            WriteSettings();
    void            ReadSettings();

        //动作类
    CCommActions  * GetCommAction();

public slots:
    //文件菜单槽
    void CloseProjectFileSlot();
    //编辑菜单槽
    void AddSlot();
    void DeleteSlot();
    void UpSlot();
    void DownSlot();

    void SubmitSlot();
    void RevertSlot();
    //void EditTplLNSlot();
    //void CleanIcdLNSlot();
    //void EditIcdLNSlot();

    //模板编辑后显示
    //void ShowSlot();

    //新建文件
    void NewCfgProjectFileSlot();

    void UpdateToolBoxNode();

private:
    CCfgActions				*		m_pCfgActions;
    CCfgToolBox				*		m_pCfgToolBox;
    CCfgCenterWnd			*		m_pCenterWnd;

    CCfgMainWnd(QWidget * parent = 0, Qt::WindowFlags flags = 0);

    void SetToolTile();
signals:

};

#endif // CCFGMAINWND_H
