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

    void ShowGeneralBiTable(CDataBase * pData);
    void ShowSignalTable(CDataBase * pData);
    void ShowGooseBiTable(CDataBase * pData);
    void ShowSoftYBTable(CDataBase * pData);

    void ShowGooseBoTable(CDataBase * pData);
    void ShowTripMatrixTable(CDataBase * pData);
    void ShowYKTable(CDataBase * pData);

    void ShowSetSPTable(CDataBase * pData);
    void ShowSetSGTable(CDataBase * pData);

    void ShowEvtAlmTable(CDataBase * pData);
    void ShowEvtActTable(CDataBase * pData);
    void ShowEvtCheckTable(CDataBase * pData);
    void ShowRemoteTripTable(CDataBase * pData);

    void ShowParaTable();

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

    CDefaultWnd *   m_pGeneralBiInfoWnd;
    CDefaultWnd *   m_pSignalInfoWnd;
    CDefaultWnd *   m_pGooseBiInfoWnd;
    CDefaultWnd *   m_pSoftYBInfoWnd;

    CDefaultWnd *   m_pGooseBoInfoWnd;
    CDefaultWnd *   m_pTripMatrixInfoWnd;
    CDefaultWnd *   m_pYKInfoWnd;

    CDefaultWnd *   m_pSetSPInfoWnd;
    CDefaultWnd *   m_pSetSGInfoWnd;

    CDefaultWnd *   m_pEvtAlmInfoWnd;
    CDefaultWnd *   m_pEvtActInfoWnd;
    CDefaultWnd *   m_pEvtCheckInfoWnd;
    CDefaultWnd *   m_pRemoteTripInfoWnd;

    CDefaultWnd  *   m_pParaTable;
};

#endif // CCFGCENTERWND_H
