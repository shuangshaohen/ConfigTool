#include "CEvtItemWnd.h"
#include "CDelegate.h"
#include <QHeaderView>
#include <QVBoxLayout>
#include "CBitSelectedDialog.h"

CEvtItemWnd::CEvtItemWnd(QWidget *parent) : CBaseItemWnd(parent)
{
    m_type = Enum_EvtTable_Type_Alm;
    m_table->clear();
    m_table->setColumnCount(8);
    QStringList headerList;
    headerList  << "描述" << "名称" << "属性" << "GOOSE开入" << "出口1"
                << "出口2" << "信号" << "灯";
    m_table->setHorizontalHeaderLabels(headerList);

    m_comboBox->addItems(headerList);
    m_comboBox->setCurrentIndex(0);
}

CEvtItemWnd::~CEvtItemWnd()
{

}

void CEvtItemWnd::itemDoubleClickedSlot(QTableWidgetItem *item)
{
    if(item->column() == Enum_PubTable_Attr_Col)
    {
        EvtItem * p = (EvtItem *)item->data(Qt::UserRole+1).toULongLong();
        QStringList list;
        list << "DB0:可见标志" << "DB1:信号上送标志" << "DB2:录波标志" << "DB3:采样标志 ";
        CBitSelectedDialog * dialog = new CBitSelectedDialog(p->dwAttr,list,this);
        dialog->exec();
        item->setData(Qt::EditRole,dialog->getVal());
        delete dialog;
    }
    else if(item->column() == Enum_EvtTable_BO1_Col)
    {
        EvtItem * p = (EvtItem *)item->data(Qt::UserRole+1).toULongLong();
        QStringList list;
        int tripMatrixNum = m_data->GetConfig()->tripMaxtrixConfig.items.size();
        for (int i = 0; i < 32; i++)
        {
            if(i < tripMatrixNum)
                list << QString("DB%1:").arg(i) + m_data->GetConfig()->tripMaxtrixConfig.items[i]->sDesc;
        }
        CBitSelectedDialog * dialog = new CBitSelectedDialog(p->boConfig1,list,this);
        dialog->exec();
        item->setData(Qt::EditRole,dialog->getVal());
        delete dialog;
    }
    else if(item->column() == Enum_EvtTable_BO2_Col)
    {
        EvtItem * p = (EvtItem *)item->data(Qt::UserRole+1).toULongLong();
        QStringList list;
        int tripMatrixNum = m_data->GetConfig()->tripMaxtrixConfig.items.size();
        for (int i = 0; i < 32; i++)
        {
            if(i+32 < tripMatrixNum)
                list << QString("DB%1:").arg(i) + m_data->GetConfig()->tripMaxtrixConfig.items[i+32]->sDesc;
        }
        CBitSelectedDialog * dialog = new CBitSelectedDialog(p->boConfig2,list,this);
        dialog->exec();
        item->setData(Qt::EditRole,dialog->getVal());
        delete dialog;
    }
    else if(item->column() == Enum_EvtTable_SIGNAL_Col)
    {
        EvtItem * p = (EvtItem *)item->data(Qt::UserRole+1).toULongLong();
        QStringList list;
        for (int i = 0; i < 32; i++)
        {
            list << QString("DB%1:信号%2").arg(i).arg(i);
        }
        CBitSelectedDialog * dialog = new CBitSelectedDialog(p->boSignal,list,this);
        dialog->exec();
        item->setData(Qt::EditRole,dialog->getVal());
        delete dialog;
    }
    else if(item->column() == Enum_EvtTable_Light_Col)
    {
        EvtItem * p = (EvtItem *)item->data(Qt::UserRole+1).toULongLong();
        QStringList list;
        for (int i = 0; i < 32; i++)
        {
            list << QString("DB%1:灯%2").arg(i).arg(i);
        }
        CBitSelectedDialog * dialog = new CBitSelectedDialog(p->boLight,list,this);
        dialog->exec();
        item->setData(Qt::EditRole,dialog->getVal());
        delete dialog;
    }
}

void CEvtItemWnd::writeConfigVal(QTableWidgetItem *item)
{
    EvtItem * p = (EvtItem *)item->data(Qt::UserRole + 1).toULongLong();

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
    case Enum_EvtTable_GSIN_Col:
        if(m_data->checkGSINCnnInfo(item->text()) == false)
        {
            CMsgInfo msgInfo( CMsgInfo::Enum_Application_Verify_Mode, CMsgInfo::CN_ERROR_MSG,
                              QString("第%1行关联GOOSE开入（%2 -> %3）检验错误(格式错误、关键字错误、索引越限)，需要修正!")
                              .arg(item->row()+1).arg(p->sCnnGSIn).arg(item->text()));
            outPutMsgInfo(msgInfo);
        }
        p->sCnnGSIn = item->text();
        break;
    case Enum_EvtTable_BO1_Col:
        p->boConfig1 = item->data(Qt::EditRole).toUInt();
        break;
    case Enum_EvtTable_BO2_Col:
        p->boConfig2 = item->data(Qt::EditRole).toUInt();
        break;
    case Enum_EvtTable_SIGNAL_Col:
        p->boSignal = item->data(Qt::EditRole).toUInt();
        break;
    case Enum_EvtTable_Light_Col:
        p->boLight = item->data(Qt::EditRole).toUInt();
        break;
    default:
        break;
    }
}

bool CEvtItemWnd::checkItemVal(QTableWidgetItem *item)
{
    if(item == NULL)
        return true;

    bool ok = true;
    EvtItem * p = (EvtItem *)item->data(Qt::UserRole+1).toULongLong();
    switch (item->column())
    {
    case Enum_PubTable_Name_Col:
        ok = m_data->checkNameNotDuplicate(p->sName);
        break;
    case Enum_EvtTable_GSIN_Col:
        ok = m_data->checkGSINCnnInfo(p->sCnnGSIn);
        break;
    default:
        break;
    }
    return ok;
}

void CEvtItemWnd::createItem(int row, BaseItem *baseItem)
{
    EvtItem * item = (EvtItem *)baseItem;
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

    QTableWidgetItem * gsIn = new QTableWidgetItem(item->sCnnGSIn);
    gsIn->setData(Qt::UserRole + 1 , data);
    gsIn->setData(Qt::ToolTip, "Goose开入关联，格式为 <子表前缀：序号> （序号从0开始），缺省为-1或不填");
    m_table->setItem(row,Enum_EvtTable_GSIN_Col,gsIn);

    AttrItem * bo1 = new AttrItem(QString::number(item->boConfig1));
    bo1->setData(Qt::UserRole + 1 , data);
    bo1->setFlags(attr->flags() & (~Qt::ItemIsEditable));
    m_table->setItem(row,Enum_EvtTable_BO1_Col,bo1);

    AttrItem * bo2 = new AttrItem(QString::number(item->boConfig2));
    bo2->setData(Qt::UserRole + 1 , data);
    bo2->setFlags(attr->flags() & (~Qt::ItemIsEditable));
    m_table->setItem(row,Enum_EvtTable_BO2_Col,bo2);

    AttrItem * signal = new AttrItem(QString::number(item->boSignal));
    signal->setData(Qt::UserRole + 1 , data);
    signal->setFlags(attr->flags() & (~Qt::ItemIsEditable));
    m_table->setItem(row,Enum_EvtTable_SIGNAL_Col,signal);

    AttrItem * light = new AttrItem(QString::number(item->boLight));
    light->setData(Qt::UserRole + 1 , data);
    light->setFlags(attr->flags() & (~Qt::ItemIsEditable));
    m_table->setItem(row,Enum_EvtTable_Light_Col,light);
}

BaseTab *CEvtItemWnd::getConfig()
{
    BaseTab * config = NULL;
    switch (m_type)
    {
    case Enum_EvtTable_Type_Alm:
        config = &m_data->GetConfig()->evtAlmConfig;
        break;
    case Enum_EvtTable_Type_Act:
        config = &m_data->GetConfig()->evtActConfig;
        break;
    case Enum_EvtTable_Type_Check:
        config = &m_data->GetConfig()->evtCheckConfig;
        break;
    case Enum_EvtTable_Type_Remote:
        config = &m_data->GetConfig()->remoteTripConfig;
        break;
    default:
        break;
    }
    return config;
}

BaseItem *CEvtItemWnd::CreateNewItem(int row)
{
    return new EvtItem(row);
}

void CEvtItemWnd::columnHidden()
{
    if(Enum_EvtTable_Type_Remote != m_type)
        m_table->setColumnHidden(Enum_EvtTable_GSIN_Col,true);
}
