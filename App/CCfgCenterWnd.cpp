#include "CCfgCenterWnd.h"
#include "CDefaultWnd.h"
#include "CDeviceInfoWnd.h"
#include "CDeviceParasWnd.h"
#include "CAnaItemWnd.h"
#include "CBiItemWnd.h"
#include "CSoftYBItemWnd.h"
#include "CGSBOItemWnd.h"
#include "CTripMatrixItemWnd.h"
#include "CSettingItemWnd.h"
#include "CEvtItemWnd.h"

CCfgCenterWnd::CCfgCenterWnd(QWidget *parent) : QStackedWidget(parent)
{
    m_pDefaultWnd = new CDefaultWnd(this);
    addWidget(m_pDefaultWnd);

    m_pDeviceInfoWnd = new CDeviceInfoWnd(this);
    addWidget(m_pDeviceInfoWnd);

    m_pDeviceParasWnd = new CDeviceParasWnd(this);
    addWidget(m_pDeviceParasWnd);

    m_pADAnaInfoWnd = new CAnaItemWnd(this);
    m_pADAnaInfoWnd->setTableType(Enum_AnaTable_Type_AD);
    addWidget(m_pADAnaInfoWnd);

    m_pDerivedInfoWnd = new CAnaItemWnd(this);
    m_pDerivedInfoWnd->setTableType(Enum_AnaTable_Type_Derived);
    addWidget(m_pDerivedInfoWnd);

    m_pSVInfoWnd = new CAnaItemWnd(this);
    m_pSVInfoWnd->setTableType(Enum_AnaTable_Type_SV);
    addWidget(m_pSVInfoWnd);

    m_pGSAnaInfoWnd = new CAnaItemWnd(this);
    m_pGSAnaInfoWnd->setTableType(Enum_AnaTable_Type_GS);
    addWidget(m_pGSAnaInfoWnd);

    m_pOtherAnaInfoWnd = new CAnaItemWnd(this);
    m_pOtherAnaInfoWnd->setTableType(Enum_AnaTable_Type_Other);
    addWidget(m_pOtherAnaInfoWnd);

    m_pGeneralBiInfoWnd = new CBiItemWnd(this);
    m_pGeneralBiInfoWnd->setTableType(Enum_BiTable_Type_GeneralBi);
    addWidget(m_pGeneralBiInfoWnd);

    m_pSignalInfoWnd = new CBiItemWnd(this);
    m_pSignalInfoWnd->setTableType(Enum_BiTable_Type_Signal);
    addWidget(m_pSignalInfoWnd);

    m_pGooseBiInfoWnd = new CBiItemWnd(this);
    m_pGooseBiInfoWnd->setTableType(Enum_BiTable_Type_GooseBi);
    addWidget(m_pGooseBiInfoWnd);

    m_pSoftYBInfoWnd = new CSoftYBItemWnd(this);
    addWidget(m_pSoftYBInfoWnd);

    m_pGooseBoInfoWnd = new CGSBOItemWnd(this);
    addWidget(m_pGooseBoInfoWnd);

    m_pTripMatrixInfoWnd = new CTripMatrixItemWnd(this);
    addWidget(m_pTripMatrixInfoWnd);

    m_pYKInfoWnd = new CDefaultWnd(this);
    addWidget(m_pYKInfoWnd);

    m_pSetSPInfoWnd = new CSettingItemWnd(this);
    m_pSetSPInfoWnd->setTableType(Enum_SetTable_Type_SP);
    addWidget(m_pSetSPInfoWnd);

    m_pSetSGInfoWnd = new CSettingItemWnd(this);
    m_pSetSGInfoWnd->setTableType(Enum_SetTable_Type_SG);
    addWidget(m_pSetSGInfoWnd);

    m_pEvtAlmInfoWnd = new CEvtItemWnd(this);
    m_pEvtAlmInfoWnd->setTableType(Enum_EvtTable_Type_Alm);
    addWidget(m_pEvtAlmInfoWnd);

    m_pEvtActInfoWnd = new CEvtItemWnd(this);
    m_pEvtActInfoWnd->setTableType(Enum_EvtTable_Type_Act);
    addWidget(m_pEvtActInfoWnd);

    m_pEvtCheckInfoWnd = new CEvtItemWnd(this);
    m_pEvtCheckInfoWnd->setTableType(Enum_EvtTable_Type_Check);
    addWidget(m_pEvtCheckInfoWnd);

    m_pRemoteTripInfoWnd = new CEvtItemWnd(this);
    m_pRemoteTripInfoWnd->setTableType(Enum_EvtTable_Type_Remote);
    addWidget(m_pRemoteTripInfoWnd);
}

CCfgCenterWnd::~CCfgCenterWnd()
{
    delete m_pDefaultWnd;
    delete m_pDeviceInfoWnd;
    delete m_pDeviceParasWnd;
    delete m_pADAnaInfoWnd;
    delete m_pDerivedInfoWnd;
    delete m_pSVInfoWnd;
    delete m_pGSAnaInfoWnd;
    delete m_pOtherAnaInfoWnd;
    delete m_pGeneralBiInfoWnd;
    delete m_pSignalInfoWnd;
    delete m_pGooseBiInfoWnd;
    delete m_pSoftYBInfoWnd;

    delete m_pGooseBoInfoWnd;
    delete m_pTripMatrixInfoWnd;
    delete m_pYKInfoWnd;

    delete m_pSetSPInfoWnd;
    delete m_pSetSGInfoWnd;

    delete m_pEvtAlmInfoWnd;
    delete m_pEvtActInfoWnd;
    delete m_pEvtCheckInfoWnd;
    delete m_pRemoteTripInfoWnd;
}

void CCfgCenterWnd::ShowDefaultInfo(CDataBase *pData)
{
    Q_UNUSED(pData);
    this->setCurrentWidget(m_pDefaultWnd);
}

void CCfgCenterWnd::ShowDeviceInfo(CDataBase *pData)
{
    m_pDeviceInfoWnd->showInfo(pData);
    this->setCurrentWidget(m_pDeviceInfoWnd);
}

void CCfgCenterWnd::ShowDeviceParas(CDataBase *pData)
{
    m_pDeviceParasWnd->showInfo(pData);
    this->setCurrentWidget(m_pDeviceParasWnd);
}

void CCfgCenterWnd::ShowADAnaTable(CDataBase *pData)
{
    m_pADAnaInfoWnd->showInfo(pData);
    this->setCurrentWidget(m_pADAnaInfoWnd);
}

void CCfgCenterWnd::ShowDerivedInfoTable(CDataBase *pData)
{
    m_pDerivedInfoWnd->showInfo(pData);
    this->setCurrentWidget(m_pDerivedInfoWnd);
}

void CCfgCenterWnd::ShowSVInfoTable(CDataBase *pData)
{
    m_pSVInfoWnd->showInfo(pData);
    this->setCurrentWidget(m_pSVInfoWnd);
}

void CCfgCenterWnd::ShowGSAnaInfoTable(CDataBase *pData)
{
    m_pGSAnaInfoWnd->showInfo(pData);
    this->setCurrentWidget(m_pGSAnaInfoWnd);
}

void CCfgCenterWnd::ShowOtherAnaTable(CDataBase *pData)
{
    m_pOtherAnaInfoWnd->showInfo(pData);
    this->setCurrentWidget(m_pOtherAnaInfoWnd);
}

void CCfgCenterWnd::ShowGeneralBiTable(CDataBase *pData)
{
    m_pGeneralBiInfoWnd->showInfo(pData);
    this->setCurrentWidget(m_pGeneralBiInfoWnd);
}

void CCfgCenterWnd::ShowSignalTable(CDataBase *pData)
{
    m_pSignalInfoWnd->showInfo(pData);
    this->setCurrentWidget(m_pSignalInfoWnd);
}

void CCfgCenterWnd::ShowGooseBiTable(CDataBase *pData)
{
    m_pGooseBiInfoWnd->showInfo(pData);
    this->setCurrentWidget(m_pGooseBiInfoWnd);
}

void CCfgCenterWnd::ShowSoftYBTable(CDataBase *pData)
{
    m_pSoftYBInfoWnd->showInfo(pData);
    this->setCurrentWidget(m_pSoftYBInfoWnd);
}

void CCfgCenterWnd::ShowGooseBoTable(CDataBase *pData)
{
    m_pGooseBoInfoWnd->showInfo(pData);
    this->setCurrentWidget(m_pGooseBoInfoWnd);
}

void CCfgCenterWnd::ShowTripMatrixTable(CDataBase *pData)
{
    m_pTripMatrixInfoWnd->showInfo(pData);
    this->setCurrentWidget(m_pTripMatrixInfoWnd);
}

void CCfgCenterWnd::ShowYKTable(CDataBase *pData)
{
    m_pYKInfoWnd->showInfo(pData);
    this->setCurrentWidget(m_pYKInfoWnd);
}

void CCfgCenterWnd::ShowSetSPTable(CDataBase *pData)
{
    m_pSetSPInfoWnd->showInfo(pData);
    this->setCurrentWidget(m_pSetSPInfoWnd);
}

void CCfgCenterWnd::ShowSetSGTable(CDataBase *pData)
{
    m_pSetSGInfoWnd->showInfo(pData);
    this->setCurrentWidget(m_pSetSGInfoWnd);
}

void CCfgCenterWnd::ShowEvtAlmTable(CDataBase *pData)
{
    m_pEvtAlmInfoWnd->showInfo(pData);
    this->setCurrentWidget(m_pEvtAlmInfoWnd);
}

void CCfgCenterWnd::ShowEvtActTable(CDataBase *pData)
{
    m_pEvtActInfoWnd->showInfo(pData);
    this->setCurrentWidget(m_pEvtActInfoWnd);
}

void CCfgCenterWnd::ShowEvtCheckTable(CDataBase *pData)
{
    m_pEvtCheckInfoWnd->showInfo(pData);
    this->setCurrentWidget(m_pEvtCheckInfoWnd);
}

void CCfgCenterWnd::ShowRemoteTripTable(CDataBase *pData)
{
    m_pRemoteTripInfoWnd->showInfo(pData);
    this->setCurrentWidget(m_pRemoteTripInfoWnd);
}

void CCfgCenterWnd::AddInfo()
{
    QWidget* pWidget = currentWidget ();
    CDefaultWnd * pWnd = (CDefaultWnd *) pWidget;
    pWnd->AddOper();
}

void CCfgCenterWnd::DeleteInfo()
{
    QWidget* pWidget = currentWidget ();
    CDefaultWnd * pWnd = (CDefaultWnd *) pWidget;
    pWnd->DeleteOper();
}

void CCfgCenterWnd::UpInfo()
{
    QWidget* pWidget = currentWidget ();
    CDefaultWnd * pWnd = (CDefaultWnd *) pWidget;
    pWnd->UpOper();
}

void CCfgCenterWnd::DownInfo()
{
    QWidget* pWidget = currentWidget ();
    CDefaultWnd * pWnd = (CDefaultWnd *) pWidget;
    pWnd->DownOper();
}

void CCfgCenterWnd::ClearTableModel()
{
    m_pDefaultWnd->Clear();
    m_pDeviceInfoWnd->Clear();
    m_pDeviceParasWnd->Clear();
    m_pADAnaInfoWnd->Clear();
    m_pDerivedInfoWnd->Clear();
    m_pSVInfoWnd->Clear();
    m_pGSAnaInfoWnd->Clear();
    m_pOtherAnaInfoWnd->Clear();
    m_pGeneralBiInfoWnd->Clear();
    m_pSignalInfoWnd->Clear();
    m_pGooseBiInfoWnd->Clear();
    m_pSoftYBInfoWnd->Clear();

    m_pGooseBoInfoWnd->Clear();
    m_pTripMatrixInfoWnd->Clear();
    m_pYKInfoWnd->Clear();

    m_pSetSPInfoWnd->Clear();
    m_pSetSGInfoWnd->Clear();

    m_pEvtAlmInfoWnd->Clear();
    m_pEvtActInfoWnd->Clear();
    m_pEvtCheckInfoWnd->Clear();
    m_pRemoteTripInfoWnd->Clear();
}
