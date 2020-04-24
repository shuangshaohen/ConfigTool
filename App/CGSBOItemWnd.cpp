#include "CGSBOItemWnd.h"
#include "CDelegate.h"
#include <QHeaderView>
#include <QVBoxLayout>
#include "CBitSelectedDialog.h"

CGSBOItemWnd::CGSBOItemWnd(QWidget *parent) : CBaseItemWnd(parent)
{
    m_table->clear();
    m_table->setColumnCount(5);
    QStringList headerList;
    headerList  << "描述" << "名称" << "属性" << "合位通道索引" << "分位通道索引";
    m_table->setHorizontalHeaderLabels(headerList);

    m_comboBox->addItems(headerList);
    m_comboBox->setCurrentIndex(0);
}

CGSBOItemWnd::~CGSBOItemWnd()
{

}

void CGSBOItemWnd::itemDoubleClickedSlot(QTableWidgetItem *item)
{
    if(item->column() == Enum_PubTable_Attr_Col)
    {
        GooseBOItem * p = (GooseBOItem *)item->data(Qt::UserRole+1).toULongLong();
        QStringList list;
        list << "DB0:可见标志" << "DB1:信号上送标志";
        CBitSelectedDialog * dialog = new CBitSelectedDialog(p->dwAttr,list,this);
        dialog->exec();
        item->setData(Qt::EditRole,dialog->getVal());
        delete dialog;
    }
}

void CGSBOItemWnd::writeConfigVal(QTableWidgetItem *item)
{
    GooseBOItem * p = (GooseBOItem *)item->data(Qt::UserRole + 1).toULongLong();

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
    case Enum_GSBOTable_HWChan_Col:
        if(m_data->checkHWChan(item->text()) == false)
        {
            CMsgInfo msgInfo( CMsgInfo::Enum_Application_Verify_Mode, CMsgInfo::CN_ERROR_MSG,
                              QString("第%1行合位对应通道索引（%2 -> %3）检验错误(格式错误、关键字错误、索引越限)，需要修正!")
                              .arg(item->row()+1).arg(p->sHWChan).arg(item->text()));
            outPutMsgInfo(msgInfo);
        }
        p->sHWChan = item->text();
        break;
    case Enum_GSBOTable_FWChan_Col:
        if(m_data->checkFWChan(item->text()) == false)
        {
            CMsgInfo msgInfo( CMsgInfo::Enum_Application_Verify_Mode, CMsgInfo::CN_ERROR_MSG,
                              QString("第%1行分位对应通道索引（%2 -> %3）检验错误(格式错误、关键字错误、索引越限)，需要修正!")
                              .arg(item->row()+1).arg(p->sFWChan).arg(item->text()));
            outPutMsgInfo(msgInfo);
        }
        p->sFWChan = item->text();
        break;
    default:
        break;
    }
}

bool CGSBOItemWnd::checkItemVal(QTableWidgetItem *item)
{
    if(item == NULL)
        return true;

    bool ok = true;
    GooseBOItem * p = (GooseBOItem *)item->data(Qt::UserRole+1).toULongLong();
    switch (item->column())
    {
    case Enum_PubTable_Name_Col:
        ok = m_data->checkNameNotDuplicate(p->sName);
        break;
    case Enum_GSBOTable_HWChan_Col:
        ok = m_data->checkHWChan(p->sHWChan);
        break;
    case Enum_GSBOTable_FWChan_Col:
        ok = m_data->checkFWChan(p->sFWChan);
        break;
    default:
        break;
    }
    return ok;
}

void CGSBOItemWnd::createItem(int row, BaseItem *baseItem)
{
    GooseBOItem * item = (GooseBOItem *)baseItem;
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

    QTableWidgetItem * hwChan = new QTableWidgetItem(item->sHWChan);
    hwChan->setData(Qt::UserRole + 1 , data);
    hwChan->setData(Qt::ToolTip, "合位通道关联，格式为 <子表前缀：序号> （序号从0开始），缺省为-1或不填");
    m_table->setItem(row,Enum_GSBOTable_HWChan_Col,hwChan);

    QTableWidgetItem * fwChan = new QTableWidgetItem(item->sFWChan);
    fwChan->setData(Qt::UserRole + 1 , data);
    fwChan->setData(Qt::ToolTip, "分位通道关联，格式为 <子表前缀：序号> （序号从0开始），缺省为-1或不填");
    m_table->setItem(row,Enum_GSBOTable_FWChan_Col,fwChan);
}

BaseTab *CGSBOItemWnd::getConfig()
{
    return &m_data->GetConfig()->gooseBoConfig;
}

BaseItem *CGSBOItemWnd::CreateNewItem(int row)
{
    return new GooseBOItem(row);
}
