#include "CBiItemWnd.h"
#include "CDelegate.h"
#include <QHeaderView>
#include <QVBoxLayout>
#include "CBitSelectedDialog.h"

CBiItemWnd::CBiItemWnd(QWidget *parent) : CBaseItemWnd(parent)
{
    m_type = Enum_BiTable_Type_GeneralBi;
    m_table->clear();
    m_table->setColumnCount(8);
    QStringList headerList;
    headerList  << "描述" << "名称" << "属性" << "类型" << "关联常闭节点"
                << "关联模拟量节点" << "防抖时间" << "告警级别";
    m_table->setHorizontalHeaderLabels(headerList);

    m_comboBox->addItems(headerList);
    m_comboBox->setCurrentIndex(0);

    m_table->setItemDelegateForColumn(Enum_BiTable_HoldTime_Col,new CSpinBoxDelegate(this));

    QStringList typeList;
    typeList << "SPS" << "DPS" << "INS" << "FLS";
    m_table->setItemDelegateForColumn(Enum_BiTable_ChanType_Col,new CComboBoxDelegate(typeList,this));

    QStringList almList;
    almList << "A" << "B" << "C";
    m_table->setItemDelegateForColumn(Enum_BiTable_AlmLevel_Col,new CComboBoxDelegate(almList,this));
}

CBiItemWnd::~CBiItemWnd()
{

}

void CBiItemWnd::itemDoubleClickedSlot(QTableWidgetItem *item)
{
    if(item->column() == Enum_PubTable_Attr_Col)
    {
        BiItem * p = (BiItem *)item->data(Qt::UserRole+1).toULongLong();
        QStringList list;
        list << "DB0:可见标志" << "DB1:信号上送标志" << "DB2:录波标志" << "DB3:采样标志 "
             << "DB4:单点硬遥信" << "DB5:单点软遥信" << "DB6:备用" << "DB7:备用" << "DB8:备用"
             << "DB9:检修" << "DB10:复归" << "DB11:备用" << "DB12:备用" << "DB13:备用"
             << "DB14:远方控制压板";
        CBitSelectedDialog * dialog = new CBitSelectedDialog(p->dwAttr,list,this);
        dialog->exec();
        item->setData(Qt::EditRole,dialog->getVal());
        delete dialog;
    }
}

void CBiItemWnd::writeConfigVal(QTableWidgetItem *item)
{
    BiItem * p = (BiItem *)item->data(Qt::UserRole + 1).toULongLong();

    switch (item->column())
    {
    case Enum_PubTable_Desc_Col:
        p->sDesc = item->text();
        break;
    case Enum_PubTable_Name_Col:
        if(m_data->checkNameNoExit(item->text()) == false)
        {
            CMsgInfo msgInfo( CMsgInfo::Enum_Application_Verify_Mode, CMsgInfo::CN_ERROR_MSG,
                              QString("第%1行名称（%2 -> %3）存在重名，需重新命名!")
                              .arg(item->row()+1).arg(p->sName).arg(item->text()));
            outPutMsgInfo(msgInfo);
        }
        p->sName = item->text();
        break;
    case Enum_PubTable_Attr_Col:
        p->dwAttr = item->data(Qt::EditRole).toUInt();
        break;
    case Enum_BiTable_ChanType_Col:
        if(m_data->checkBiType(item->text()) == false)
        {
            CMsgInfo msgInfo( CMsgInfo::Enum_Application_Verify_Mode, CMsgInfo::CN_ERROR_MSG,
                              QString("第%1行类型（%2 -> %3）不属于标准类型，需修正!")
                              .arg(item->row()+1).arg(p->sChanType).arg(item->text()));
            outPutMsgInfo(msgInfo);
        }
        p->sChanType = item->text();
        break;
    case Enum_BiTable_AlmLevel_Col:
        if(m_data->checkAlmLevel(item->text()) == false)
        {
            CMsgInfo msgInfo( CMsgInfo::Enum_Application_Verify_Mode, CMsgInfo::CN_ERROR_MSG,
                              QString("第%1行告警级别（%2 -> %3）不属于标准级别，需修正!")
                              .arg(item->row()+1).arg(p->sAlmLevel).arg(item->text()));
            outPutMsgInfo(msgInfo);
        }
        p->sAlmLevel = item->text();
        break;
    case Enum_BiTable_HoldTime_Col:
        p->wHoldTime = item->text().toUInt();
        break;
    case Enum_BiTable_IndexDPS_Col:
        if(m_data->checkIndexDPS(item->text()) == false)
        {
            CMsgInfo msgInfo( CMsgInfo::Enum_Application_Verify_Mode, CMsgInfo::CN_ERROR_MSG,
                              QString("第%1行关联常闭节点（%2 -> %3）检验错误(格式错误、关键字错误、索引越限)，需要修正!")
                              .arg(item->row()+1).arg(p->sIndexDPS).arg(item->text()));
            outPutMsgInfo(msgInfo);
        }
        p->sIndexDPS = item->text();
        break;
    case Enum_BiTable_IndexAna_Col:
        if(m_data->checkIndexGSAna(item->text()) == false)
        {
            CMsgInfo msgInfo( CMsgInfo::Enum_Application_Verify_Mode, CMsgInfo::CN_ERROR_MSG,
                              QString("第%1行关联浮点值通道（%2 -> %3）检验错误(格式错误、关键字错误、索引越限)，需要修正!")
                              .arg(item->row()+1).arg(p->sIndexAna).arg(item->text()));
            outPutMsgInfo(msgInfo);
        }
        p->sIndexAna = item->text();
        break;
    default:
        break;
    }
}

bool CBiItemWnd::checkItemVal(QTableWidgetItem *item)
{
    if(item == NULL)
        return true;

    bool ok = true;
    BiItem * p = (BiItem *)item->data(Qt::UserRole+1).toULongLong();
    switch (item->column())
    {
    case Enum_PubTable_Name_Col:
        ok = m_data->checkNameNotDuplicate(p->sName);
        break;
    case Enum_BiTable_ChanType_Col:
        ok = m_data->checkBiType(p->sChanType);
        break;
    case Enum_BiTable_AlmLevel_Col:
        ok = m_data->checkAlmLevel(p->sAlmLevel);
        break;
    case Enum_BiTable_IndexDPS_Col:
        ok = m_data->checkIndexDPS(p->sIndexDPS);
        break;
    case Enum_BiTable_IndexAna_Col:
        ok = m_data->checkIndexGSAna(p->sIndexAna);
        break;
    default:
        break;
    }
    return ok;
}

void CBiItemWnd::createItem(int row, BaseItem *baseItem)
{
    BiItem * item = (BiItem *)baseItem;
    QVariant data = (qulonglong)item;

    QTableWidgetItem * desc = new QTableWidgetItem(item->sDesc);
    desc->setData(Qt::UserRole + 1 , data);
    m_table->setItem(row,Enum_PubTable_Desc_Col,desc);

    QTableWidgetItem * name = new QTableWidgetItem(item->sName);
    name->setData(Qt::UserRole + 1 , data);
    m_table->setItem(row,Enum_PubTable_Name_Col,name);

    AttrItem * attr = new AttrItem(QString::number(item->dwAttr));
    attr->setData(Qt::UserRole + 1 , data);
    //attr->setData(Qt::ToolTip, "二进制编辑：DB0:可见标志，DB1:信号上送标志，DB2:录波标志，DB3:采样标志");
    attr->setFlags(attr->flags() & (~Qt::ItemIsEditable));
    m_table->setItem(row,Enum_PubTable_Attr_Col,attr);

    QTableWidgetItem * type = new QTableWidgetItem(item->sChanType);
    type->setData(Qt::UserRole + 1 , data);
    type->setData(Qt::ToolTip,"SPS：单点，DPS：双点，INS：整型，FLS：浮点数");
    m_table->setItem(row,Enum_BiTable_ChanType_Col,type);

    QTableWidgetItem * indexDPS = new QTableWidgetItem(item->sIndexDPS);
    indexDPS->setData(Qt::UserRole + 1 , data);
    indexDPS->setData(Qt::ToolTip, "双点信号常闭节点关联，格式为 <子表前缀：序号> （序号从0开始），缺省为-1或不填");
    m_table->setItem(row,Enum_BiTable_IndexDPS_Col,indexDPS);

    QTableWidgetItem * indexAna = new QTableWidgetItem(item->sIndexAna);
    indexAna->setData(Qt::UserRole + 1 , data);
    indexAna->setData(Qt::ToolTip, "浮点值通道关联，格式为 <子表前缀：序号> （序号从0开始），缺省为-1或不填");
    m_table->setItem(row,Enum_BiTable_IndexAna_Col,indexAna);

    QTableWidgetItem * holdTime = new QTableWidgetItem(QString::number(item->wHoldTime));
    holdTime->setData(Qt::UserRole + 1 , data);
    holdTime->setData(Qt::ToolTip,"防抖时间（单位为ms）");
    m_table->setItem(row,Enum_BiTable_HoldTime_Col,holdTime);

    QTableWidgetItem * alm = new QTableWidgetItem(item->sAlmLevel);
    alm->setData(Qt::UserRole + 1 , data);
    alm->setData(Qt::ToolTip,"A类、B类、C类，缺省A类");
    m_table->setItem(row,Enum_BiTable_AlmLevel_Col,alm);
}

BaseTab *CBiItemWnd::getConfig()
{
    BaseTab * config = NULL;
    switch (m_type)
    {
    case Enum_BiTable_Type_GeneralBi:
        config = &m_data->GetConfig()->generalBiConfig;
        break;
    case Enum_BiTable_Type_Signal:
        config = &m_data->GetConfig()->signalConfig;
        break;
    case Enum_BiTable_Type_GooseBi:
        config = &m_data->GetConfig()->gooseBiConfig;
        break;
    }
    return config;
}

BaseItem *CBiItemWnd::CreateNewItem(int row)
{
    return new BiItem(row);
}

void CBiItemWnd::columnHidden()
{
    switch (m_type)
    {
    case Enum_BiTable_Type_GeneralBi:
        m_table->setColumnHidden(Enum_BiTable_IndexAna_Col,true);
        break;
    case Enum_BiTable_Type_Signal:
        m_table->setColumnHidden(Enum_BiTable_IndexAna_Col,true);
        m_table->setColumnHidden(Enum_BiTable_HoldTime_Col,true);
        break;
    case Enum_BiTable_Type_GooseBi:
        m_table->setColumnHidden(Enum_BiTable_HoldTime_Col,true);
        break;
    default:
        break;
    }
}
