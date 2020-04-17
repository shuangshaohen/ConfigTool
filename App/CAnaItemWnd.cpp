#include "CAnaItemWnd.h"
#include "CDelegate.h"
#include <QHeaderView>
#include <QVBoxLayout>

CAnaItemWnd::CAnaItemWnd(QWidget *parent)
    : CBaseItemWnd(parent)
{
    m_type = Enum_AnaTable_Type_AD;
    m_table->clear();
    m_table->setColumnCount(15);
    QStringList headerList;
    headerList  << "描述" << "名称" << "属性" << "类型" << "标识"
                << "缺省" << "系数" << "一次额定定值" << "二次额定定值" << "接收软压板"
                << "接收硬压板" << "位宽" << "精度" << "二次单位" << "一次单位";
    m_table->setHorizontalHeaderLabels(headerList);

    connect(m_table,SIGNAL(itemChanged(QTableWidgetItem *)),this,SLOT(itemChangedSlot(QTableWidgetItem *)));

    m_table->setItemDelegateForColumn(Enum_AnaTable_Coe_Col,new CSpinBoxDelegate(this));
    m_table->setItemDelegateForColumn(Enum_PubTable_Attr_Col,new CSpinBoxDelegate(this));
    m_table->setItemDelegateForColumn(Enum_AnaTable_XuYCDft_Col,new CSpinBoxDelegate(this,-1,9999999));
    m_table->setItemDelegateForColumn(Enum_AnaTable_DataWidth_Col,new CSpinBoxDelegate(this,0,63));
    m_table->setItemDelegateForColumn(Enum_AnaTable_DataDot_Col,new CSpinBoxDelegate(this,0,63));
    m_data = NULL;
}

CAnaItemWnd::~CAnaItemWnd()
{

}

void CAnaItemWnd::setTableType(int type)
{
    m_type = type;
}

void CAnaItemWnd::showInfo(void *pData)
{
    m_table->clearContents();
    if(NULL == pData)
    {
        m_data = NULL;
        return;
    }

    m_table->disconnect(SIGNAL(itemChanged(QTableWidgetItem *)));
    m_data = (CDataBase *)pData;

    AnaConfig * config = NULL;
    switch (m_type)
    {
    case Enum_AnaTable_Type_AD:
        config = &m_data->GetConfig()->adAnaConfig;
        break;
    case Enum_AnaTable_Type_Derived:
        config = &m_data->GetConfig()->derivedConfig;
        break;
    case Enum_AnaTable_Type_SV:
        config = &m_data->GetConfig()->svConfig;
        break;
    case Enum_AnaTable_Type_GS:
        config = &m_data->GetConfig()->gsAnaConfig;
        break;
    case Enum_AnaTable_Type_Other:
        config = &m_data->GetConfig()->otherAnaConfig;
        break;
    }

    if(NULL == config)
        return;

    m_table->setRowCount(config->items.size());
    for (int row = 0; row < config->items.size(); row++)
    {
        createItem(row,config->items[row]);
    }

    setAlignment(m_table, Qt::AlignHCenter|Qt::AlignVCenter);
    connect(m_table,SIGNAL(itemChanged(QTableWidgetItem *)),this,SLOT(itemChangedSlot(QTableWidgetItem *)));
}

void CAnaItemWnd::itemChangedSlot(QTableWidgetItem *item)
{
    m_data->SetModified(true);

    AnaItem * ana = (AnaItem *)item->data(Qt::UserRole + 1).toULongLong();

    switch (item->column())
    {
    case Enum_PubTable_Desc_Col:
        ana->sDesc = item->text();
        break;
    case Enum_PubTable_Name_Col:
        if((ana->sName != item->text())
                &&(m_data->checkName(item->text())))
        {
            ana->sName = item->text();
        }
        else
        {
            item->setText(ana->sName);
        }
        break;
    case Enum_PubTable_Attr_Col:
        ana->dwAttr = item->data(Qt::EditRole).toInt();
        break;
    case Enum_AnaTable_ChanType_Col:
        ana->sChanType = item->text();
        break;
    case Enum_AnaTable_ChanFlag_Col:
        ana->sChanFlag = item->text();
        break;
    case Enum_AnaTable_XuYCDft_Col:
        ana->iXuYCDft = item->text().toInt();
        break;
    case Enum_AnaTable_Coe_Col:
        ana->wCoe = item->text().toUInt();
        break;
    case Enum_AnaTable_RateSetP_Col:
        if(m_data->checkSPSetCnnInfo(item->text()))
        {
            ana->sRateSetP = item->text();
        }
        else
        {
            item->setText(ana->sRateSetP);
        }
        break;
    case Enum_AnaTable_RateSetS_Col:
        if(m_data->checkSPSetCnnInfo(item->text()))
        {
            ana->sRateSetS = item->text();
        }
        else
        {
            item->setText(ana->sRateSetS);
        }
        break;

    case Enum_AnaTable_RecSYb_Col:
        if(m_data->checkSoftYBCnnInfo(item->text()))
        {
            ana->sRecSYb = item->text();
        }
        else
        {
            item->setText(ana->sRecSYb);
        }
        break;
    case Enum_AnaTable_RecHYb_Col:
        if(m_data->checkBICnnInfo(item->text()))
        {
            ana->sRecHYb = item->text();
        }
        else
        {
            item->setText(ana->sRecHYb);
        }
        break;
    case Enum_AnaTable_DataWidth_Col:
        ana->byWidth = item->text().toUInt();
        break;
    case Enum_AnaTable_DataDot_Col:
        if(item->text().toUInt() < ana->byWidth)
        {
            ana->byDotBit = item->text().toUInt();
        }
        else
        {
            item->setText(QString::number(ana->byDotBit));
        }
        break;
    case Enum_AnaTable_Unit_Col:
        ana->sUnit = item->text();
        break;
    case Enum_AnaTable_KUnit_Col:
        ana->sKiloUnit = item->text();
        break;
    default:
        break;
    }
}

void CAnaItemWnd::createItem(int row, AnaItem *item)
{
    QVariant data = (qulonglong)item;

    QTableWidgetItem * desc = new QTableWidgetItem(item->sDesc);
    desc->setData(Qt::UserRole + 1 , data);
    m_table->setItem(row,Enum_PubTable_Desc_Col,desc);

    QTableWidgetItem * name = new QTableWidgetItem(item->sName);
    name->setData(Qt::UserRole + 1 , data);
    m_table->setItem(row,Enum_PubTable_Name_Col,name);

    AttrItem * attr = new AttrItem(QString::number(item->dwAttr));
    attr->setData(Qt::UserRole + 1 , data);
    attr->setData(Qt::ToolTip, "0x01:可见标志，0x02:信号上送标志，0x04:录波标志，0x08:采样标志");
    m_table->setItem(row,Enum_PubTable_Attr_Col,attr);

    QTableWidgetItem * type = new QTableWidgetItem(item->sChanType);
    type->setData(Qt::UserRole + 1 , data);
    m_table->setItem(row,Enum_AnaTable_ChanType_Col,type);

    QTableWidgetItem * flag = new QTableWidgetItem(item->sChanFlag);
    flag->setData(Qt::UserRole + 1 , data);
    m_table->setItem(row,Enum_AnaTable_ChanFlag_Col,flag);

    QTableWidgetItem * dft = new QTableWidgetItem(QString::number(item->iXuYCDft));
    dft->setData(Qt::UserRole + 1 , data);
    m_table->setItem(row,Enum_AnaTable_XuYCDft_Col,dft);

    QTableWidgetItem * coe = new QTableWidgetItem(QString::number(item->wCoe));
    coe->setData(Qt::UserRole + 1 , data);
    m_table->setItem(row,Enum_AnaTable_Coe_Col,coe);

    QTableWidgetItem * setP = new QTableWidgetItem(item->sRateSetP);
    setP->setData(Qt::UserRole + 1 , data);
    setP->setData(Qt::ToolTip, "一次额定值关联，格式为 子表名：序号 （序号从0开始），缺省为-1或不填");
    m_table->setItem(row,Enum_AnaTable_RateSetP_Col,setP);

    QTableWidgetItem * setS = new QTableWidgetItem(item->sRateSetS);
    setS->setData(Qt::UserRole + 1 , data);
    setS->setData(Qt::ToolTip, "二次额定值关联，格式为 子表名：序号 （序号从0开始），缺省为-1或不填");
    m_table->setItem(row,Enum_AnaTable_RateSetS_Col,setS);

    QTableWidgetItem * sYB = new QTableWidgetItem(item->sRecSYb);
    sYB->setData(Qt::UserRole + 1 , data);
    sYB->setData(Qt::ToolTip, "接收软压板关联，格式为 子表名：序号 （序号从0开始），缺省为-1或不填");
    m_table->setItem(row,Enum_AnaTable_RecSYb_Col,sYB);

    QTableWidgetItem * hYB = new QTableWidgetItem(item->sRecHYb);
    hYB->setData(Qt::UserRole + 1 , data);
    hYB->setData(Qt::ToolTip, "接收硬压板关联，格式为 子表名：序号 （序号从0开始），缺省为-1或不填");
    m_table->setItem(row,Enum_AnaTable_RecHYb_Col,hYB);

    QTableWidgetItem * width = new QTableWidgetItem(QString::number(item->byWidth));
    width->setData(Qt::UserRole + 1 , data);
    m_table->setItem(row,Enum_AnaTable_DataWidth_Col,width);

    QTableWidgetItem * dot = new QTableWidgetItem(QString::number(item->byDotBit));
    dot->setData(Qt::UserRole + 1 , data);
    m_table->setItem(row,Enum_AnaTable_DataDot_Col,dot);

    QTableWidgetItem * unit = new QTableWidgetItem(item->sUnit);
    unit->setData(Qt::UserRole + 1 , data);
    m_table->setItem(row,Enum_AnaTable_Unit_Col,unit);

    QTableWidgetItem * kUnit = new QTableWidgetItem(item->sKiloUnit);
    kUnit->setData(Qt::UserRole + 1 , data);
    m_table->setItem(row,Enum_AnaTable_KUnit_Col,kUnit);
}
