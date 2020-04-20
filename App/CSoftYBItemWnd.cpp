#include "CSoftYBItemWnd.h"
#include "CDelegate.h"
#include <QHeaderView>
#include <QVBoxLayout>
#include "CBitSelectedDialog.h"

CSoftYBItemWnd::CSoftYBItemWnd(QWidget *parent) : CBaseItemWnd(parent)
{
    m_table->clear();
    m_table->setColumnCount(4);
    QStringList headerList;
    headerList  << "描述" << "名称" << "属性" << "缺省默认值";
    m_table->setHorizontalHeaderLabels(headerList);

    m_comboBox->addItems(headerList);
    m_comboBox->setCurrentIndex(0);

    m_table->setItemDelegateForColumn(Enum_SoftYBTable_ValDft_Col,new CSpinBoxDelegate(this,0,1,1,10));
}

CSoftYBItemWnd::~CSoftYBItemWnd()
{

}

void CSoftYBItemWnd::itemDoubleClickedSlot(QTableWidgetItem *item)
{
    if(item->column() == Enum_PubTable_Attr_Col)
    {
        SoftYBItem * p = (SoftYBItem *)item->data(Qt::UserRole+1).toULongLong();
        QStringList list;
        list << "DB0:可见标志" << "DB1:信号上送标志" << "DB2:录波标志" << "DB3:远方不可控"
             << "DB4:远方修改定值" << "DB5:远方切换定值区" << "DB6:远方控制压板";
        CBitSelectedDialog * dialog = new CBitSelectedDialog(p->dwAttr,list,this);
        dialog->exec();
        item->setData(Qt::EditRole,dialog->getVal());
        delete dialog;
    }
}

void CSoftYBItemWnd::writeConfigVal(QTableWidgetItem *item)
{
    SoftYBItem * p = (SoftYBItem *)item->data(Qt::UserRole + 1).toULongLong();

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
    case Enum_SoftYBTable_ValDft_Col:
        p->wDftVal = item->text().toUInt();
        break;
    default:
        break;
    }
}

bool CSoftYBItemWnd::checkItemVal(QTableWidgetItem *item)
{
    if(item == NULL)
        return true;

    bool ok = true;
    SoftYBItem * p = (SoftYBItem *)item->data(Qt::UserRole+1).toULongLong();
    switch (item->column())
    {
    case Enum_PubTable_Name_Col:
        ok = m_data->checkNameNotDuplicate(p->sName);
        break;
    case Enum_SoftYBTable_ValDft_Col:
        ok = m_data->checkCtlVal(p->wDftVal);
        break;
    default:
        break;
    }
    return ok;
}

void CSoftYBItemWnd::createItem(int row, BaseItem *baseItem)
{
    SoftYBItem * item = (SoftYBItem *)baseItem;
    QVariant data = (qulonglong)item;

    QTableWidgetItem * desc = new QTableWidgetItem(item->sDesc);
    desc->setData(Qt::UserRole + 1 , data);
    m_table->setItem(row,Enum_PubTable_Desc_Col,desc);

    QTableWidgetItem * name = new QTableWidgetItem(item->sName);
    name->setData(Qt::UserRole + 1 , data);
    m_table->setItem(row,Enum_PubTable_Name_Col,name);

    AttrItem * attr = new AttrItem(QString::number(item->dwAttr));
    attr->setData(Qt::UserRole + 1 , data);
    //attr->setData(Qt::ToolTip, "二进制编辑：DB0:可见标志，DB1:信号上送标志，DB2:录波标志，DB3:远方不可控");
    attr->setFlags(attr->flags() & (~Qt::ItemIsEditable));
    m_table->setItem(row,Enum_PubTable_Attr_Col,attr);


    QTableWidgetItem * holdTime = new QTableWidgetItem(QString::number(item->wDftVal));
    holdTime->setData(Qt::UserRole + 1 , data);
    holdTime->setData(Qt::ToolTip,"缺省值：0或者1");
    m_table->setItem(row,Enum_SoftYBTable_ValDft_Col,holdTime);
}

BaseTab *CSoftYBItemWnd::getConfig()
{
    return &m_data->GetConfig()->softYBConfig;
}

BaseItem *CSoftYBItemWnd::CreateNewItem(int row)
{
    return new SoftYBItem(row);
}
