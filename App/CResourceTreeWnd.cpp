#include "CResourceTreeWnd.h"
#include "CCfgMainWnd.h"
#include "CDataBase.h"
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

CResourceTreeWnd::~CResourceTreeWnd()
{
    delete m_pRootTreeWidgetItem;
    delete m_pInfoTreeWidgetItem;
    delete m_pParasTreeWidgetItem;
    delete m_pSmpTreeWidgetItem;
    delete m_pAnaTreeWidgetItem;
    delete m_pDerivedTreeWidgetItem;
    delete m_pSvTreeWidgetItem;
    delete m_pGsAnaTreeWidgetItem;
    delete m_pOherAnaTreeWidgetItem;
    delete m_pBITreeWidgetItem;
    delete m_pGeneralBITreeWidgetItem;
    delete m_pSignalTreeWidgetItem;
    delete m_pGSBITreeWidgetItem;
    delete m_pSoftYBTreeWidgetItem;
    delete m_pBOTreeWidgetItem;
    delete m_pGSBOTreeWidgetItem;
    delete m_pTripMatrixTreeWidgetItem;
    delete m_pYKTreeWidgetItem;
    delete m_pSetTreeWidgetItem;
    delete m_pSPSetTreeWidgetItem;
    delete m_pSGSetTreeWidgetItem;
    delete m_pEvtTreeWidgetItem;
    delete m_pAlmEvtTreeWidgetItem;
    delete m_pActEvtTreeWidgetItem;
    delete m_pRemoteTripTreeWidgetItem;
    delete m_pCheckEvtTreeWidgetItem;
}

void CResourceTreeWnd::CreateTreeNodes(CDataBase *pXercesXML)
{
    m_pXerceXML = pXercesXML;
    if(NULL == m_pXerceXML)
        return;

    m_pRootTreeWidgetItem  = new QTreeWidgetItem(this);
    m_pRootTreeWidgetItem->setText( 0, "装置配置" );
    SetNodeType(m_pRootTreeWidgetItem, Enum_Root_Node);
    this->addTopLevelItem(m_pRootTreeWidgetItem);

    m_pInfoTreeWidgetItem  = new QTreeWidgetItem(m_pRootTreeWidgetItem);
    m_pInfoTreeWidgetItem->setText( 0, "装置信息" );
    SetNodeType(m_pInfoTreeWidgetItem, Enum_DeviceInfo_Node);

    m_pParasTreeWidgetItem  = new QTreeWidgetItem(m_pRootTreeWidgetItem);
    m_pParasTreeWidgetItem->setText( 0, "装置参数" );
    SetNodeType(m_pParasTreeWidgetItem, Enum_DeviceParas_Node);

    m_pSmpTreeWidgetItem  = new QTreeWidgetItem(m_pRootTreeWidgetItem);
    m_pSmpTreeWidgetItem->setText( 0, "模拟量" );
    SetNodeType(m_pSmpTreeWidgetItem, Enum_Ana_Node);

    m_pAnaTreeWidgetItem  = new QTreeWidgetItem(m_pSmpTreeWidgetItem);
    m_pAnaTreeWidgetItem->setText( 0, "AD模拟量");
    SetNodeType(m_pAnaTreeWidgetItem, Enum_Ana_AD_Node);

    m_pDerivedTreeWidgetItem  = new QTreeWidgetItem(m_pSmpTreeWidgetItem);
    m_pDerivedTreeWidgetItem->setText( 0, "衍生模拟量" );
    SetNodeType(m_pDerivedTreeWidgetItem, Enum_Ana_Derived_Node);

    m_pSvTreeWidgetItem  = new QTreeWidgetItem(m_pSmpTreeWidgetItem);
    m_pSvTreeWidgetItem->setText( 0, "SV模拟量" );
    SetNodeType(m_pSvTreeWidgetItem, Enum_Ana_SV_Node);

    m_pGsAnaTreeWidgetItem  = new QTreeWidgetItem(m_pSmpTreeWidgetItem);
    m_pGsAnaTreeWidgetItem->setText( 0, "GS模拟量" );
    SetNodeType(m_pGsAnaTreeWidgetItem, Enum_Ana_GS_Node);

    m_pOherAnaTreeWidgetItem  = new QTreeWidgetItem(m_pSmpTreeWidgetItem);
    m_pOherAnaTreeWidgetItem->setText( 0, "其他模拟量" );
    SetNodeType(m_pOherAnaTreeWidgetItem, Enum_Ana_Other_Node);

    m_pBITreeWidgetItem  = new QTreeWidgetItem(m_pRootTreeWidgetItem);
    m_pBITreeWidgetItem->setText( 0, "开关量" );
    SetNodeType(m_pBITreeWidgetItem, Enum_BI_Node);

    m_pGeneralBITreeWidgetItem   = new QTreeWidgetItem(m_pBITreeWidgetItem);
    m_pGeneralBITreeWidgetItem->setText( 0, "硬开入" );
    SetNodeType(m_pGeneralBITreeWidgetItem, Enum_BI_General_Node);

    m_pSignalTreeWidgetItem   = new QTreeWidgetItem(m_pBITreeWidgetItem);
    m_pSignalTreeWidgetItem->setText( 0, "软遥信" );
    SetNodeType(m_pSignalTreeWidgetItem, Enum_BI_Signal_Node);

    m_pGSBITreeWidgetItem   = new QTreeWidgetItem(m_pBITreeWidgetItem);
    m_pGSBITreeWidgetItem->setText( 0, "GS开入" );
    SetNodeType(m_pGSBITreeWidgetItem, Enum_GSBI_Node);

    m_pSoftYBTreeWidgetItem   = new QTreeWidgetItem(m_pBITreeWidgetItem);
    m_pSoftYBTreeWidgetItem->setText( 0, "软压板" );
    SetNodeType(m_pSoftYBTreeWidgetItem, Enum_SoftYB_Node);

    m_pBOTreeWidgetItem   = new QTreeWidgetItem(m_pRootTreeWidgetItem);
    m_pBOTreeWidgetItem->setText( 0, "开出" );
    SetNodeType(m_pBOTreeWidgetItem, Enum_BO_Node);

    m_pGSBOTreeWidgetItem   = new QTreeWidgetItem(m_pBOTreeWidgetItem);
    m_pGSBOTreeWidgetItem->setText( 0, "GS开出" );
    SetNodeType(m_pGSBOTreeWidgetItem, Enum_GSBO_Node);

    m_pTripMatrixTreeWidgetItem   = new QTreeWidgetItem(m_pBOTreeWidgetItem);
    m_pTripMatrixTreeWidgetItem->setText( 0, "跳闸矩阵" );
    SetNodeType(m_pTripMatrixTreeWidgetItem, Enum_TripMatrix_Node);

    m_pYKTreeWidgetItem   = new QTreeWidgetItem(m_pBOTreeWidgetItem);
    m_pYKTreeWidgetItem->setText( 0, "遥控" );
    SetNodeType(m_pYKTreeWidgetItem, Enum_BO_YK_Node);

    m_pSetTreeWidgetItem   = new QTreeWidgetItem(m_pRootTreeWidgetItem);
    m_pSetTreeWidgetItem->setText( 0, "定值" );
    SetNodeType(m_pSetTreeWidgetItem, Enum_Setting_Node);

    m_pSPSetTreeWidgetItem   = new QTreeWidgetItem(m_pSetTreeWidgetItem);
    m_pSPSetTreeWidgetItem->setText( 0, "不分组定值" );
    SetNodeType(m_pSPSetTreeWidgetItem, Enum_SPSet_Node);

    m_pSGSetTreeWidgetItem   = new QTreeWidgetItem(m_pSetTreeWidgetItem);
    m_pSGSetTreeWidgetItem->setText( 0, "分组定值" );
    SetNodeType(m_pSGSetTreeWidgetItem, Enum_SGSet_Node);

    m_pEvtTreeWidgetItem   = new QTreeWidgetItem(m_pRootTreeWidgetItem);
    m_pEvtTreeWidgetItem->setText( 0, "记录" );
    SetNodeType(m_pEvtTreeWidgetItem, Enum_Evt_Node);

    m_pAlmEvtTreeWidgetItem   = new QTreeWidgetItem(m_pEvtTreeWidgetItem);
    m_pAlmEvtTreeWidgetItem->setText( 0, "告警记录" );
    SetNodeType(m_pAlmEvtTreeWidgetItem, Enum_Alarm_Node);

    m_pActEvtTreeWidgetItem   = new QTreeWidgetItem(m_pEvtTreeWidgetItem);
    m_pActEvtTreeWidgetItem->setText( 0, "动作记录" );
    SetNodeType(m_pActEvtTreeWidgetItem, Enum_Act_Node);

    m_pRemoteTripTreeWidgetItem   = new QTreeWidgetItem(m_pEvtTreeWidgetItem);
    m_pRemoteTripTreeWidgetItem->setText( 0, "远方跳闸" );
    SetNodeType(m_pRemoteTripTreeWidgetItem, Enum_Remote_Node);

    m_pCheckEvtTreeWidgetItem   = new QTreeWidgetItem(m_pEvtTreeWidgetItem);
    m_pCheckEvtTreeWidgetItem->setText( 0, "自检记录" );
    SetNodeType(m_pCheckEvtTreeWidgetItem, Enum_Check_Node);

    expandItem(m_pRootTreeWidgetItem);

    RefreshTreeNodes();
}

void CResourceTreeWnd::RefreshTreeNodes()
{
    if(NULL == m_pXerceXML)
    {
        m_pAnaTreeWidgetItem->setText( 0, "AD模拟量");
        m_pDerivedTreeWidgetItem->setText( 0, "衍生模拟量" );
        m_pSvTreeWidgetItem->setText( 0, "SV模拟量" );
        m_pGsAnaTreeWidgetItem->setText( 0, "GS模拟量" );
        m_pOherAnaTreeWidgetItem->setText( 0, "其他模拟量" );
        m_pGeneralBITreeWidgetItem->setText( 0, "硬开入" );
        m_pSignalTreeWidgetItem->setText( 0, "软遥信" );
        m_pGSBITreeWidgetItem->setText( 0, "GS开入" );
        m_pSoftYBTreeWidgetItem->setText( 0, "软压板" );
        m_pGSBOTreeWidgetItem->setText( 0, "GS开出" );
        m_pTripMatrixTreeWidgetItem->setText( 0, "跳闸矩阵" );
        m_pYKTreeWidgetItem->setText( 0, "遥控" );
        m_pSPSetTreeWidgetItem->setText( 0, "不分组定值" );
        m_pSGSetTreeWidgetItem->setText( 0, "分组定值" );
        m_pAlmEvtTreeWidgetItem->setText( 0, "告警记录" );
        m_pActEvtTreeWidgetItem->setText( 0, "动作记录" );
        m_pRemoteTripTreeWidgetItem->setText( 0, "远方跳闸" );
        m_pCheckEvtTreeWidgetItem->setText( 0, "自检记录" );
        return;
    }

    GseConfig * config = m_pXerceXML->GetConfig();

    m_pAnaTreeWidgetItem->setText( 0, /*config->adAnaConfig.sDesc +*/ QString("AD模拟量:%1").arg(config->adAnaConfig.items.size()));
    m_pDerivedTreeWidgetItem->setText( 0, /*config->derivedConfig.sDesc +*/ QString("衍生模拟量:%1").arg(config->derivedConfig.items.size()) );
    m_pSvTreeWidgetItem->setText( 0, /*config->svConfig.sDesc + */QString("SV模拟量:%1").arg(config->svConfig.items.size()) );
    m_pGsAnaTreeWidgetItem->setText( 0, /*config->gsAnaConfig.sDesc +*/ QString("GS模拟量:%1").arg(config->gsAnaConfig.items.size()) );
    m_pOherAnaTreeWidgetItem->setText( 0, /*config->otherAnaConfig.sDesc +*/ QString("其他模拟量:%1").arg(config->otherAnaConfig.items.size()) );

    m_pGeneralBITreeWidgetItem->setText( 0, /*config->generalBiConfig.sDesc +*/ QString("硬开入:%1").arg(config->generalBiConfig.items.size()) );
    m_pSignalTreeWidgetItem->setText( 0, /*config->signalConfig.sDesc +*/ QString("软遥信:%1").arg(config->signalConfig.items.size()) );
    m_pGSBITreeWidgetItem->setText( 0, /*config->gooseBiConfig.sDesc +*/ QString("GS开入:%1").arg(config->gooseBiConfig.items.size()) );
    m_pSoftYBTreeWidgetItem->setText( 0, /*config->softYBConfig.sDesc +*/ QString("软压板:%1").arg(config->softYBConfig.items.size()) );

    m_pGSBOTreeWidgetItem->setText( 0, /*config->gooseBoConfig.sDesc +*/ QString("GS开出:%1").arg(config->gooseBoConfig.items.size()) );
    m_pTripMatrixTreeWidgetItem->setText( 0, /*config->tripMaxtrixConfig.sDesc + */QString("跳闸矩阵:%1").arg(config->tripMaxtrixConfig.items.size()) );
    m_pYKTreeWidgetItem->setText( 0, /*config->ykConfig.sDesc +*/ QString("遥控:%1").arg(config->ykConfig.items.size()) );

    m_pSPSetTreeWidgetItem->setText( 0, /*config->settingSPConifg.sDesc +*/ QString("不分组定值:%1").arg(config->settingSPConifg.items.size()) );
    m_pSGSetTreeWidgetItem->setText( 0, /*config->settingSGConifg.sDesc +*/ QString("分组定值:%1").arg(config->settingSGConifg.items.size()) );

    m_pAlmEvtTreeWidgetItem->setText( 0, /*config->evtAlmConfig.sDesc +*/ QString("告警记录:%1").arg(config->evtAlmConfig.items.size()) );
    m_pActEvtTreeWidgetItem->setText( 0, /*config->evtActConfig.sDesc +*/ QString("动作记录:%1").arg(config->evtActConfig.items.size()) );
    m_pRemoteTripTreeWidgetItem->setText( 0, /*config->remoteTripConfig.sDesc +*/ QString("远方跳闸:%1").arg(config->remoteTripConfig.items.size()) );
    m_pCheckEvtTreeWidgetItem->setText( 0, /*config->evtCheckConfig.sDesc +*/ QString("自检记录:%1").arg(config->evtCheckConfig.items.size()) );
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

    switch(nNodeType)
    {
    case Enum_DeviceInfo_Node:
        CCfgMainWnd::Instance()->ShowDeviceInfo();
        break;
    case Enum_DeviceParas_Node:
        CCfgMainWnd::Instance()->ShowDeviceParas();
        break;
    case Enum_Ana_AD_Node:
        CCfgMainWnd::Instance()->ShowADAnaTable();
        break;
    case Enum_Ana_Derived_Node:
        CCfgMainWnd::Instance()->ShowDerivedInfoTable();
        break;
    case Enum_Ana_SV_Node:
        CCfgMainWnd::Instance()->ShowSVInfoTable();
        break;
    case Enum_Ana_GS_Node:
        CCfgMainWnd::Instance()->ShowGSAnaInfoTable();
        break;
    case Enum_Ana_Other_Node:
        CCfgMainWnd::Instance()->ShowOtherAnaTable();
        break;
    case Enum_BI_General_Node:
        CCfgMainWnd::Instance()->ShowGeneralBiTable();
        break;
    case Enum_BI_Signal_Node:
        CCfgMainWnd::Instance()->ShowSignalTable();
        break;
    case Enum_GSBI_Node:
        CCfgMainWnd::Instance()->ShowGooseBiTable();
        break;
    case Enum_SoftYB_Node:
        CCfgMainWnd::Instance()->ShowSoftYBTable();
        break;
    case Enum_GSBO_Node:
        CCfgMainWnd::Instance()->ShowGooseBoTable();
        break;
    case Enum_TripMatrix_Node:
        CCfgMainWnd::Instance()->ShowTripMatrixTable();
        break;
    case Enum_BO_YK_Node:
        CCfgMainWnd::Instance()->ShowYKTable();
        break;
    case Enum_SPSet_Node:
        CCfgMainWnd::Instance()->ShowSetSPTable();
        break;
    case Enum_SGSet_Node:
        CCfgMainWnd::Instance()->ShowSetSGTable();
        break;
    case Enum_Alarm_Node:
        CCfgMainWnd::Instance()->ShowEvtAlmTable();
        break;
    case Enum_Act_Node:
        CCfgMainWnd::Instance()->ShowEvtActTable();
        break;
    case Enum_Remote_Node:
        CCfgMainWnd::Instance()->ShowRemoteTripTable();
        break;
    case Enum_Check_Node:
        CCfgMainWnd::Instance()->ShowEvtCheckTable();
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


