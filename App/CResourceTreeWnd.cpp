#include "CResourceTreeWnd.h"
#include "CCfgMainWnd.h"
#include <QHeaderView>

CResourceTreeWnd::CResourceTreeWnd(QWidget *parent): QTreeWidget(parent)
{
    setAnimated(true);
    QTreeWidgetItem * pTreeWidgetItem  = this->headerItem();
    pTreeWidgetItem->setText( 0, "" );
    pTreeWidgetItem->setHidden(true);
    pTreeWidgetItem->setText( 1, "" );
    pTreeWidgetItem->setHidden(true);
    setColumnHidden(1, true);

    header()->setSectionResizeMode(QHeaderView::ResizeToContents );
    header()->setStretchLastSection( false);

    //CreateIEDTreeNodes();

    connect( this, SIGNAL(currentItemChanged( QTreeWidgetItem *, QTreeWidgetItem * )), this, SLOT(currentItemChangedSlot( QTreeWidgetItem *, QTreeWidgetItem * )));
}

void CResourceTreeWnd::CreateIEDTreeNodes()
{
    m_pRootTreeWidgetItem  = new QTreeWidgetItem(this);
    m_pRootTreeWidgetItem->setText( 0, "装置配置" );
    SetNodeType(m_pRootTreeWidgetItem, Enum_Root_Node);
    this->addTopLevelItem(m_pRootTreeWidgetItem);

    m_pAnaTreeWidgetItem  = new QTreeWidgetItem(m_pRootTreeWidgetItem);
    m_pAnaTreeWidgetItem->setText( 0, "模拟量" );
    SetNodeType(m_pAnaTreeWidgetItem, Enum_Ana_Node);

    m_pBITreeWidgetItem  = new QTreeWidgetItem(m_pRootTreeWidgetItem);
    m_pBITreeWidgetItem->setText( 0, "开关量" );
    SetNodeType(m_pBITreeWidgetItem, Enum_BI_Node);

    m_pGSBITreeWidgetItem   = new QTreeWidgetItem(m_pRootTreeWidgetItem);
    m_pGSBITreeWidgetItem->setText( 0, "GS开入" );
    SetNodeType(m_pGSBITreeWidgetItem, Enum_GSBI_Node);

    m_pGSBOTreeWidgetItem   = new QTreeWidgetItem(m_pRootTreeWidgetItem);
    m_pGSBOTreeWidgetItem->setText( 0, "GS开出" );
    SetNodeType(m_pGSBOTreeWidgetItem, Enum_GSBO_Node);

    m_pTripMatrixTreeWidgetItem   = new QTreeWidgetItem(m_pRootTreeWidgetItem);
    m_pTripMatrixTreeWidgetItem->setText( 0, "跳闸矩阵" );
    SetNodeType(m_pTripMatrixTreeWidgetItem, Enum_TripMatrix_Node);

    m_pSettingTreeWidgetItem   = new QTreeWidgetItem(m_pRootTreeWidgetItem);
    m_pSettingTreeWidgetItem->setText( 0, "定值" );
    SetNodeType(m_pSettingTreeWidgetItem, Enum_Setting_Node);

    m_pSoftYBTreeWidgetItem   = new QTreeWidgetItem(m_pRootTreeWidgetItem);
    m_pSoftYBTreeWidgetItem->setText( 0, "软压板" );
    SetNodeType(m_pSoftYBTreeWidgetItem, Enum_SoftYB_Node);

    m_pAlarmTreeWidgetItem   = new QTreeWidgetItem(m_pRootTreeWidgetItem);
    m_pAlarmTreeWidgetItem->setText( 0, "告警记录" );
    SetNodeType(m_pAlarmTreeWidgetItem, Enum_Alarm_Node);

    m_pActTreeWidgetItem   = new QTreeWidgetItem(m_pRootTreeWidgetItem);
    m_pActTreeWidgetItem->setText( 0, "动作记录" );
    SetNodeType(m_pActTreeWidgetItem, Enum_Act_Node);

//    m_pRemoteTreeWidgetItem   = new QTreeWidgetItem(m_pRootTreeWidgetItem);
//    m_pRemoteTreeWidgetItem->setText( 0, "远方跳闸" );
//    SetNodeType(m_pRemoteTreeWidgetItem, Enum_Remote_Node);

    expandItem(m_pRootTreeWidgetItem);
}

void CResourceTreeWnd::ClearTreeNode()
{
    clear();
}

void CResourceTreeWnd::SetNodeType(QTreeWidgetItem *pTreeWidgetItem, CResourceTreeWnd::Enum_Node_Type type)
{
    if(NULL!=pTreeWidgetItem)
    {
        pTreeWidgetItem->setData( 0, Qt::UserRole, QVariant(type) );
    }
}

CResourceTreeWnd::Enum_Node_Type CResourceTreeWnd::GetNodeType(QTreeWidgetItem *pTreeWidgetItem)
{
    Enum_Node_Type nNodeType = Enum_Invalid_Node;

    if(NULL!=pTreeWidgetItem)
    {
        nNodeType = (Enum_Node_Type)(pTreeWidgetItem->data( 0, Qt::UserRole ).toUInt());
    }
    return nNodeType;
}

void CResourceTreeWnd::ShowCurrentNodeInfoWnd()
{
    QTreeWidgetItem* pCurrentItem = currentItem();
    Enum_Node_Type nNodeType = GetNodeType(pCurrentItem);

    if (nNodeType == Enum_Root_Node)
    {
        CCfgMainWnd::Instance()->ShowRootInfo();
        return;
    }

    switch(nNodeType)
    {
    case Enum_Ana_Node:
        CCfgMainWnd::Instance()->ShowAnaInfo();
        break;
    case Enum_BI_Node:
        CCfgMainWnd::Instance()->ShowBIInfo();
        break;
    case Enum_GSBI_Node:
        CCfgMainWnd::Instance()->ShowGSBIInfo();
        break;
    case Enum_GSBO_Node:
        CCfgMainWnd::Instance()->ShowGSBOInfo();
        break;
    case Enum_TripMatrix_Node:
        CCfgMainWnd::Instance()->ShowTripMatrixInfo();
        break;
    case Enum_Setting_Node:
        CCfgMainWnd::Instance()->ShowSettingInfo();
        break;
    case Enum_SoftYB_Node:
        CCfgMainWnd::Instance()->ShowSoftYBInfo();
        break;
    case Enum_Alarm_Node:
        CCfgMainWnd::Instance()->ShowAlarmInfo();
        break;
    case Enum_Act_Node:
        CCfgMainWnd::Instance()->ShowActInfo();
        break;
    case Enum_Remote_Node:
        CCfgMainWnd::Instance()->ShowRemoteInfo();
        break;
    default:
        CCfgMainWnd::Instance()->ShowDefaultInfo();
        break;
    }
}

void CResourceTreeWnd::currentItemChangedSlot(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{
    Q_UNUSED(previous);
    Q_UNUSED(current);

    ShowCurrentNodeInfoWnd();
}


