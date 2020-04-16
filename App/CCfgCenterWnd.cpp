#include "CCfgCenterWnd.h"
#include "CDefaultWnd.h"
#include "CAnaWnd.h"
#include "CBIWnd.h"
#include "CGSBIWnd.h"
#include "CGSBOWnd.h"
#include "CTripMatrixWnd.h"
#include "CAlmRecWnd.h"
#include "CActRecWnd.h"
#include "CSoftYBWnd.h"
#include "CSettingWnd.h"

CCfgCenterWnd::CCfgCenterWnd(QWidget *parent) : QStackedWidget(parent)
{
    m_pDefaultWnd = new CDefaultWnd(this);
    addWidget(m_pDefaultWnd);

    m_pCAnaWnd = new CAnaWnd(this);
    addWidget(m_pCAnaWnd);

    m_pBIWnd = new CBIWnd(this);
    addWidget(m_pBIWnd);

    m_pGSBIWnd = new CGSBIWnd(this);
    addWidget(m_pGSBIWnd);

    m_pGSBOWnd = new CGSBOWnd(this);
    addWidget(m_pGSBOWnd);

    m_pTripMatrixWnd = new CTripMatrixWnd(this);
    addWidget(m_pTripMatrixWnd);

    m_pAlmRecWnd = new CAlmRecWnd(this);
    addWidget(m_pAlmRecWnd);

    m_pActRecWnd = new CActRecWnd(this);
    addWidget(m_pActRecWnd);

    m_pSoftYBWnd = new CSoftYBWnd(this);
    addWidget(m_pSoftYBWnd);

    m_pSettingWnd = new CSettingWnd(this);
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
    m_pCAnaWnd->ShowAnaInfo(pData);
    this->setCurrentWidget(m_pCAnaWnd);
}

void CCfgCenterWnd::ShowBIInfo(CDataBase *pData)
{
    m_pBIWnd->ShowBIInfo(pData);
    this->setCurrentWidget(m_pBIWnd);
}

void CCfgCenterWnd::ShowGSBIInfo(CDataBase *pData)
{
    m_pGSBIWnd->ShowGSBIInfo(pData);
    this->setCurrentWidget(m_pGSBIWnd);
}

void CCfgCenterWnd::ShowGSBOInfo(CDataBase *pData)
{
    m_pGSBOWnd->ShowGSBOInfo(pData);
    this->setCurrentWidget(m_pGSBOWnd);
}

void CCfgCenterWnd::ShowTripMatrixInfo(CDataBase *pData)
{
    m_pTripMatrixWnd->ShowTripMatrixInfo(pData);
    this->setCurrentWidget(m_pTripMatrixWnd);
}

void CCfgCenterWnd::ShowAlmRecInfo(CDataBase *pData)
{
    m_pAlmRecWnd->ShowAlmInfo(pData);
    this->setCurrentWidget(m_pAlmRecWnd);
}

void CCfgCenterWnd::ShowActRecInfo(CDataBase *pData)
{
    m_pActRecWnd->ShowActInfo(pData);
    this->setCurrentWidget(m_pActRecWnd);
}

void CCfgCenterWnd::ShowSoftYBInfo(CDataBase *pData)
{
    m_pSoftYBWnd->ShowSoftYBInfo(pData);
    this->setCurrentWidget(m_pSoftYBWnd);
}

void CCfgCenterWnd::ShowSettingInfo(CDataBase *pData)
{
    m_pSettingWnd->ShowSettingInfo(pData);
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
    m_pCAnaWnd->ClearTableModel();
    m_pBIWnd->ClearTableModel();
    m_pGSBIWnd->ClearTableModel();
    m_pGSBOWnd->ClearTableModel();
    m_pTripMatrixWnd->ClearTableModel();
    m_pAlmRecWnd->ClearTableModel();
    m_pActRecWnd->ClearTableModel();
    m_pSoftYBWnd->ClearTableModel();
    m_pSettingWnd->ClearTableModel();
}
