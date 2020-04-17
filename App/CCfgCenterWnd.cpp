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

    m_pADAnaParasWnd = new CAnaItemWnd(this);
    m_pADAnaParasWnd->setTableType(Enum_AnaTable_Type_AD);
    addWidget(m_pADAnaParasWnd);
}

CCfgCenterWnd::~CCfgCenterWnd()
{
    delete m_pDefaultWnd;
    delete m_pDeviceInfoWnd;
    delete m_pDeviceParasWnd;
    delete m_pADAnaParasWnd;
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
    m_pADAnaParasWnd->showInfo(pData);
    this->setCurrentWidget(m_pADAnaParasWnd);
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

void CCfgCenterWnd::CopyInfo()
{
    QWidget* pWidget = currentWidget ();
    CDefaultWnd * pWnd = (CDefaultWnd *) pWidget;
    pWnd->CopyOper();
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
    m_pADAnaParasWnd->Clear();
}
