#include "CCfgCenterWnd.h"
#include "CDefaultWnd.h"

CCfgCenterWnd::CCfgCenterWnd(QWidget *parent) : QStackedWidget(parent)
{
    m_pDefaultWnd = new CDefaultWnd(this);
    addWidget(m_pDefaultWnd);

    m_pCAnaWnd = new CDefaultWnd(this);
    addWidget(m_pCAnaWnd);

    m_pBIWnd = new CDefaultWnd(this);
    addWidget(m_pBIWnd);

    m_pGSBIWnd = new CDefaultWnd(this);
    addWidget(m_pGSBIWnd);

    m_pGSBOWnd = new CDefaultWnd(this);
    addWidget(m_pGSBOWnd);

    m_pTripMatrixWnd = new CDefaultWnd(this);
    addWidget(m_pTripMatrixWnd);

    m_pAlmRecWnd = new CDefaultWnd(this);
    addWidget(m_pAlmRecWnd);

    m_pActRecWnd = new CDefaultWnd(this);
    addWidget(m_pActRecWnd);

    m_pSoftYBWnd = new CDefaultWnd(this);
    addWidget(m_pSoftYBWnd);

    m_pSettingWnd = new CDefaultWnd(this);
    addWidget(m_pSettingWnd);
}

CCfgCenterWnd::~CCfgCenterWnd()
{

}

void CCfgCenterWnd::ShowDefaultInfo(CDataBase *pData)
{
    Q_UNUSED(pData);
    this->setCurrentWidget(m_pDefaultWnd);
}

void CCfgCenterWnd::ShowAnaInfo(CDataBase *pData)
{
    m_pCAnaWnd->showInfo(pData);
    this->setCurrentWidget(m_pCAnaWnd);
}

void CCfgCenterWnd::ShowBIInfo(CDataBase *pData)
{
    m_pBIWnd->showInfo(pData);
    this->setCurrentWidget(m_pBIWnd);
}

void CCfgCenterWnd::ShowGSBIInfo(CDataBase *pData)
{
    m_pGSBIWnd->showInfo(pData);
    this->setCurrentWidget(m_pGSBIWnd);
}

void CCfgCenterWnd::ShowGSBOInfo(CDataBase *pData)
{
    m_pGSBOWnd->showInfo(pData);
    this->setCurrentWidget(m_pGSBOWnd);
}

void CCfgCenterWnd::ShowTripMatrixInfo(CDataBase *pData)
{
    m_pTripMatrixWnd->showInfo(pData);
    this->setCurrentWidget(m_pTripMatrixWnd);
}

void CCfgCenterWnd::ShowAlmRecInfo(CDataBase *pData)
{
    m_pAlmRecWnd->showInfo(pData);
    this->setCurrentWidget(m_pAlmRecWnd);
}

void CCfgCenterWnd::ShowActRecInfo(CDataBase *pData)
{
    m_pActRecWnd->showInfo(pData);
    this->setCurrentWidget(m_pActRecWnd);
}

void CCfgCenterWnd::ShowSoftYBInfo(CDataBase *pData)
{
    m_pSoftYBWnd->showInfo(pData);
    this->setCurrentWidget(m_pSoftYBWnd);
}

void CCfgCenterWnd::ShowSettingInfo(CDataBase *pData)
{
    m_pSettingWnd->showInfo(pData);
    this->setCurrentWidget(m_pSettingWnd);
}

void CCfgCenterWnd::AddInfo()
{
    QWidget* pWidget = currentWidget ();
    if(pWidget == m_pCAnaWnd)
    {
        m_pCAnaWnd->AddOper();
    }
    if(pWidget == m_pBIWnd)
    {
        m_pBIWnd->AddOper();
    }
    if(pWidget == m_pGSBIWnd)
    {
        m_pGSBIWnd->AddOper();
    }
    if(pWidget == m_pGSBOWnd)
    {
        m_pGSBOWnd->AddOper();
    }
    if(pWidget == m_pTripMatrixWnd)
    {
        m_pTripMatrixWnd->AddOper();
    }
    if(pWidget == m_pAlmRecWnd)
    {
        m_pAlmRecWnd->AddOper();
    }
    if(pWidget == m_pActRecWnd)
    {
        m_pActRecWnd->AddOper();
    }
    if(pWidget == m_pSoftYBWnd)
    {
        m_pSoftYBWnd->AddOper();
    }
    if(pWidget == m_pSettingWnd)
    {
        m_pSettingWnd->AddOper();
    }
}

void CCfgCenterWnd::DeleteInfo()
{
    QWidget* pWidget = currentWidget ();
    if(pWidget == m_pCAnaWnd)
    {
        m_pCAnaWnd->DeleteOper();
    }
    if(pWidget == m_pBIWnd)
    {
        m_pBIWnd->DeleteOper();
    }
    if(pWidget == m_pGSBIWnd)
    {
        m_pGSBIWnd->DeleteOper();
    }
    if(pWidget == m_pGSBOWnd)
    {
        m_pGSBOWnd->DeleteOper();
    }
    if(pWidget == m_pTripMatrixWnd)
    {
        m_pTripMatrixWnd->DeleteOper();
    }
    if(pWidget == m_pAlmRecWnd)
    {
        m_pAlmRecWnd->DeleteOper();
    }
    if(pWidget == m_pActRecWnd)
    {
        m_pActRecWnd->DeleteOper();
    }
    if(pWidget == m_pSoftYBWnd)
    {
        m_pSoftYBWnd->DeleteOper();
    }
    if(pWidget == m_pSettingWnd)
    {
        m_pSettingWnd->DeleteOper();
    }
}

void CCfgCenterWnd::CopyInfo()
{
    QWidget* pWidget = currentWidget ();
    if(pWidget == m_pCAnaWnd)
    {
        m_pCAnaWnd->CopyOper();
    }
    if(pWidget == m_pBIWnd)
    {
        m_pBIWnd->CopyOper();
    }
    if(pWidget == m_pGSBIWnd)
    {
        m_pGSBIWnd->CopyOper();
    }
    if(pWidget == m_pGSBOWnd)
    {
        m_pGSBOWnd->CopyOper();
    }
    if(pWidget == m_pTripMatrixWnd)
    {
        m_pTripMatrixWnd->CopyOper();
    }
    if(pWidget == m_pAlmRecWnd)
    {
        m_pAlmRecWnd->CopyOper();
    }
    if(pWidget == m_pActRecWnd)
    {
        m_pActRecWnd->CopyOper();
    }
    if(pWidget == m_pSoftYBWnd)
    {
        m_pSoftYBWnd->CopyOper();
    }
    if(pWidget == m_pSettingWnd)
    {
        m_pSettingWnd->CopyOper();
    }
}

void CCfgCenterWnd::UpInfo()
{
    QWidget* pWidget = currentWidget ();
    if(pWidget == m_pCAnaWnd)
    {
        m_pCAnaWnd->UpOper();
    }
    if(pWidget == m_pBIWnd)
    {
        m_pBIWnd->UpOper();
    }
    if(pWidget == m_pGSBIWnd)
    {
        m_pGSBIWnd->UpOper();
    }
    if(pWidget == m_pGSBOWnd)
    {
        m_pGSBOWnd->UpOper();
    }
    if(pWidget == m_pTripMatrixWnd)
    {
        m_pTripMatrixWnd->UpOper();
    }
    if(pWidget == m_pAlmRecWnd)
    {
        m_pAlmRecWnd->UpOper();
    }
    if(pWidget == m_pActRecWnd)
    {
        m_pActRecWnd->UpOper();
    }
    if(pWidget == m_pSoftYBWnd)
    {
        m_pSoftYBWnd->UpOper();
    }
    if(pWidget == m_pSettingWnd)
    {
        m_pSettingWnd->UpOper();
    }
}

void CCfgCenterWnd::DownInfo()
{
    QWidget* pWidget = currentWidget ();
    if(pWidget == m_pCAnaWnd)
    {
        m_pCAnaWnd->DownOper();
    }
    if(pWidget == m_pBIWnd)
    {
        m_pBIWnd->DownOper();
    }
    if(pWidget == m_pGSBIWnd)
    {
        m_pGSBIWnd->DownOper();
    }
    if(pWidget == m_pGSBOWnd)
    {
        m_pGSBOWnd->DownOper();
    }
    if(pWidget == m_pTripMatrixWnd)
    {
        m_pTripMatrixWnd->DownOper();
    }
    if(pWidget == m_pAlmRecWnd)
    {
        m_pAlmRecWnd->DownOper();
    }
    if(pWidget == m_pActRecWnd)
    {
        m_pActRecWnd->DownOper();
    }
    if(pWidget == m_pSoftYBWnd)
    {
        m_pSoftYBWnd->DownOper();
    }
    if(pWidget == m_pSettingWnd)
    {
        m_pSettingWnd->DownOper();
    }
}

void CCfgCenterWnd::ClearTableModel()
{
    m_pCAnaWnd->Clear();
    m_pBIWnd->Clear();
    m_pGSBIWnd->Clear();
    m_pGSBOWnd->Clear();
    m_pTripMatrixWnd->Clear();
    m_pAlmRecWnd->Clear();
    m_pActRecWnd->Clear();
    m_pSoftYBWnd->Clear();
    m_pSettingWnd->Clear();
}
