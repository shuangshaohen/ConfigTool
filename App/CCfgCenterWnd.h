#ifndef CCFGCENTERWND_H
#define CCFGCENTERWND_H

#include <QObject>
#include <QStackedWidget>
#include "CDataBase.h"

class CDefaultWnd;
class CAnaWnd;
class CBIWnd;
class CGSBIWnd;
class CGSBOWnd;
class CTripMatrixWnd;
class CAlmRecWnd;
class CActRecWnd;
class CSoftYBWnd;
class CSettingWnd;

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
    CAnaWnd     *   m_pCAnaWnd;
    CBIWnd      *   m_pBIWnd;
    CGSBIWnd    *   m_pGSBIWnd;
    CGSBOWnd    *   m_pGSBOWnd;
    CTripMatrixWnd  *   m_pTripMatrixWnd;
    CAlmRecWnd  *   m_pAlmRecWnd;
    CActRecWnd  *   m_pActRecWnd;
    CSoftYBWnd  *   m_pSoftYBWnd;
    CSettingWnd *   m_pSettingWnd;
};

#endif // CCFGCENTERWND_H
