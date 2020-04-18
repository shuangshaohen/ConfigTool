#include "CCfgCenterWnd.h"
#include "CDefaultWnd.h"
#include "CDeviceInfoWnd.h"
#include "CDeviceParasWnd.h"
#include "CAnaItemWnd.h"

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
}
