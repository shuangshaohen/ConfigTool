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

    void ShowAnaInfo(CDataBase * pData);

    void ShowBIInfo(CDataBase * pData);

    void ShowGSBIInfo(CDataBase * pData);

    void ShowGSBOInfo(CDataBase * pData);

    void ShowTripMatrixInfo(CDataBase * pData);

    void ShowAlmRecInfo(CDataBase * pData);

    void ShowActRecInfo(CDataBase * pData);

    void ShowSoftYBInfo(CDataBase * pData);

    void ShowSettingInfo(CDataBase * pData);

    void AddInfo();
    void DeleteInfo();
    void CopyInfo();
    void UpInfo();
    void DownInfo();

    void ClearTableModel();
private:
    CDefaultWnd *   m_pDefaultWnd;
    CDefaultWnd *   m_pCAnaWnd;
    CDefaultWnd *   m_pBIWnd;
    CDefaultWnd *   m_pGSBIWnd;
    CDefaultWnd *   m_pGSBOWnd;
    CDefaultWnd *   m_pTripMatrixWnd;
    CDefaultWnd *   m_pAlmRecWnd;
    CDefaultWnd *   m_pActRecWnd;
    CDefaultWnd *   m_pSoftYBWnd;
    CDefaultWnd *   m_pSettingWnd;
};

#endif // CCFGCENTERWND_H
