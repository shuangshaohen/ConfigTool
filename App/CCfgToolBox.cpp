#include <QtGui>
#include <QGridLayout>
#include "CCfgToolBox.h"
#include "CResourceTreeWnd.h"
#include "CCfgMainWnd.h"
#include "ui_ToolBox.h"
#include "TopoPara.h"

void CCfgToolBox::CurrentPageChanged(int index)
{
    QWidget *pWidget = widget( index );

    if( pWidget==m_pUi_ToolBox->m_ResourceEditPage)
    {
        CCfgMainWnd::Instance()->SetEditToolBarVisible(true);
        CCfgMainWnd::Instance()->SetVerifyToolBarVisible(true);
        m_pResourceTreeWnd->ShowCurrentNodeInfoWnd();
    }
    else if( pWidget==m_pUi_ToolBox->m_LogicEditPage)
    {
        CCfgMainWnd::Instance()->SetEditToolBarVisible(false);
        CCfgMainWnd::Instance()->SetVerifyToolBarVisible(false);
        CCfgMainWnd::Instance()->ShowParaTable();
    }
    else if( pWidget==m_pUi_ToolBox->m_FunctionPage)
    {
        CCfgMainWnd::Instance()->SetEditToolBarVisible(false);
        CCfgMainWnd::Instance()->SetVerifyToolBarVisible(false);
        //CIcdMainWnd::Instance()->RaiseFnBrowsePageInCenterWnd();
    }
    else if( pWidget==m_pUi_ToolBox->m_TextViewPage)
    {
        CCfgMainWnd::Instance()->SetEditToolBarVisible(false);
        CCfgMainWnd::Instance()->SetVerifyToolBarVisible(false);
        //CIcdMainWnd::Instance()->RaiseDataTypeEditPageInCenterWnd();
    }
}

CCfgToolBox::CCfgToolBox(QWidget *parent):QTabWidget(parent)
{
    /**/
    m_pUi_ToolBox = new Ui_ToolBox();
    m_pUi_ToolBox->setupUi(this);

    m_pResourceTreeWnd = new CResourceTreeWnd(m_pUi_ToolBox->m_ResourceEditPage);
    QGridLayout    *pGridLayout = new QGridLayout(m_pUi_ToolBox->m_ResourceEditPage);
    pGridLayout->setMargin(0);
    pGridLayout->addWidget(m_pResourceTreeWnd, 0, 0, 1, 1);

    TopoPara::GetInstance()->setTreeView(m_pUi_ToolBox->treeView);

    connect( this, SIGNAL(currentChanged(int)), this, SLOT(CurrentPageChanged(int)));

    setMinimumWidth(200);
}

CCfgToolBox::~CCfgToolBox()
{

}

QSize CCfgToolBox::sizeHint() const
{
    return QSize(295,50);			//设置初始大小
}

void CCfgToolBox::CreateResourceTreeNodes(CDataBase *pXercesXML)
{
    if(NULL == pXercesXML)
        return;

    m_pResourceTreeWnd->CreateTreeNodes(pXercesXML);

}

void CCfgToolBox::RefreshResourceTreeNodes()
{
    m_pResourceTreeWnd->RefreshTreeNodes();
}

void CCfgToolBox::ShowCurrentNodeInfoWnd()
{
    m_pResourceTreeWnd->ShowCurrentNodeInfoWnd();
}

void CCfgToolBox::setIEDPage()
{
    setCurrentWidget(m_pUi_ToolBox->m_ResourceEditPage);
}

void CCfgToolBox::setParaPage()
{
    setCurrentWidget(m_pUi_ToolBox->m_LogicEditPage);
}

void CCfgToolBox::ClearTreeNode()
{
    m_pResourceTreeWnd->ClearTreeNode();
}
