#ifndef CCFGCENTERWND_H
#define CCFGCENTERWND_H

#include <QObject>
#include <QStackedWidget>
#include "CDataBase.h"

class CDefaultWnd;

class CCfgCenterWnd : public QStackedWidget
{
    Q_OBJECT
public:
    CCfgCenterWnd(QWidget *parent = nullptr);

    virtual ~CCfgCenterWnd(void);

    void ShowDefaultInfo(CDataBase * pData);
    void ShowDeviceInfo(CDataBase * pData);
    void ShowDeviceParas(CDataBase * pData);
    void ShowADAnaTable(CDataBase * pData);
    void ShowDerivedInfoTable(CDataBase * pData);
    void ShowSVInfoTable(CDataBase * pData);
    void ShowGSAnaInfoTable(CDataBase * pData);
    void ShowOtherAnaTable(CDataBase * pData);

    void AddInfo();
    void DeleteInfo();
    void UpInfo();
    void DownInfo();

    void ClearTableModel();
private:
    CDefaultWnd *   m_pDefaultWnd;
    CDefaultWnd *   m_pDeviceInfoWnd;
    CDefaultWnd *   m_pDeviceParasWnd;
    CDefaultWnd *   m_pADAnaInfoWnd;
    CDefaultWnd *   m_pDerivedInfoWnd;
    CDefaultWnd *   m_pSVInfoWnd;
    CDefaultWnd *   m_pGSAnaInfoWnd;
    CDefaultWnd *   m_pOtherAnaInfoWnd;
};

#endif // CCFGCENTERWND_H
