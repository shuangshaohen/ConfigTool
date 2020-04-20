#include "CTripMatrixItemWnd.h"
#include "CDelegate.h"
#include <QHeaderView>
#include <QVBoxLayout>
#include "CBitSelectedDialog.h"

CTripMatrixItemWnd::CTripMatrixItemWnd(QWidget *parent) : CBaseItemWnd(parent)
{
    m_table->clear();
    m_table->setColumnCount(5);
    QStringList headerList;
    headerList  << "描述" << "名称" << "属性" << "出口槽位"
                << "出口置位";
    m_table->setHorizontalHeaderLabels(headerList);

    m_comboBox->addItems(headerList);
    m_comboBox->setCurrentIndex(0);

    m_table->setItemDelegateForColumn(Enum_TripMatrixTable_Baod_Col,new CSpinBoxDelegate(this,0,15,1,10));
}

CTripMatrixItemWnd::~CTripMatrixItemWnd()
{

}

void CTripMatrixItemWnd::itemDoubleClickedSlot(QTableWidgetItem *item)
{
    if(item->column() == Enum_PubTable_Attr_Col)
    {
        TripMatrixItem * p = (TripMatrixItem *)item->data(Qt::UserRole+1).toULongLong();
        QStringList list;
        list << "DB0:可见标志" << "DB1:信号上送标志" << "DB2:录波标志";
        CBitSelectedDialog * dialog = new CBitSelectedDialog(p->dwAttr,list,this);
        dialog->exec();
        item->setData(Qt::EditRole,dialog->getVal());
        delete dialog;
    }
    else if(item->column() == Enum_TripMatrixTable_Flag_Col)
    {
        TripMatrixItem * p = (TripMatrixItem *)item->data(Qt::UserRole+1).toULongLong();
        QStringList list;
        list << "DB0:出口1" << "DB1:出口2" << "DB2:出口3"<< "DB3:出口4"<< "DB4:出口5"
                << "DB5:出口6" << "DB6:出口7" << "DB7:出口8"<< "DB8:出口9"<< "DB9:出口10"
                <<"DB10:出口11";
        CBitSelectedDialog * dialog = new CBitSelectedDialog(p->dwAttr,list,this);
        dialog->exec();
        item->setData(Qt::EditRole,dialog->getVal());
        delete dialog;
    }
}

void CTripMatrixItemWnd::writeConfigVal(QTableWidgetItem *item)
{
    TripMatrixItem * p = (TripMatrixItem *)item->data(Qt::UserRole + 1).toULongLong();

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
    case Enum_TripMatrixTable_Baod_Col:
        p->wBoardIndex = item->data(Qt::EditRole).toUInt();
        break;
    case Enum_TripMatrixTable_Flag_Col:
        p->wBOFlag = item->data(Qt::EditRole).toUInt();
        break;
    default:
        break;
    }
}

bool CTripMatrixItemWnd::checkItemVal(QTableWidgetItem *item)
{
    if(item == NULL)
        return true;

    bool ok = true;
    TripMatrixItem * p = (TripMatrixItem *)item->data(Qt::UserRole+1).toULongLong();
    switch (item->column())
    {
    case Enum_PubTable_Name_Col:
        ok = m_data->checkNameNotDuplicate(p->sName);
        break;
    default:
        break;
    }
    return ok;
}

void CTripMatrixItemWnd::createItem(int row, BaseItem *baseItem)
{
    TripMatrixItem * item = (TripMatrixItem *)baseItem;
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

    QTableWidgetItem * boad = new QTableWidgetItem(QString::number(item->wBoardIndex));
    boad->setData(Qt::UserRole + 1 , data);
    boad->setData(Qt::ToolTip,"出口槽位号");
    m_table->setItem(row,Enum_TripMatrixTable_Baod_Col,boad);

    AttrItem * flag = new AttrItem(QString::number(item->wBOFlag));
    flag->setData(Qt::UserRole + 1 , data);
    flag->setFlags(attr->flags() & (~Qt::ItemIsEditable));
    m_table->setItem(row,Enum_TripMatrixTable_Flag_Col,flag);
}

BaseTab *CTripMatrixItemWnd::getConfig()
{
    return &m_data->GetConfig()->tripMaxtrixConfig;
}

BaseItem *CTripMatrixItemWnd::CreateNewItem(int row)
{
    return new TripMatrixItem(row);
}
