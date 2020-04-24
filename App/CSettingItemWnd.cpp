#include "CSettingItemWnd.h"
#include "CDelegate.h"
#include <QHeaderView>
#include <QVBoxLayout>
#include "CBitSelectedDialog.h"

CSettingItemWnd::CSettingItemWnd(QWidget *parent) : CBaseItemWnd(parent)
{
    m_type = Enum_SetTable_Type_SP;
    m_table->clear();
    m_table->setColumnCount(14);
    QStringList headerList;
    headerList  << "描述" << "名称" << "属性" << "最小值" << "最大值"
                << "缺省值" << "步长" << "二次额定值" << "返回系数" << "定值类型"
                << "位宽" << "精度" << "一次单位" << "二次单位";
    m_table->setHorizontalHeaderLabels(headerList);

    m_comboBox->addItems(headerList);
    m_comboBox->setCurrentIndex(0);

    m_table->setItemDelegateForColumn(Enum_SetTable_CoeRet_Col,new CSpinBoxDelegate(this));
    m_table->setItemDelegateForColumn(Enum_SetTable_Min_Col,new CSpinBoxDelegate(this,0,9999999,1,10));
    m_table->setItemDelegateForColumn(Enum_SetTable_Max_Col,new CSpinBoxDelegate(this,0,9999999,1,10));
    m_table->setItemDelegateForColumn(Enum_SetTable_Dft_Col,new CSpinBoxDelegate(this,0,9999999,1,10));
    m_table->setItemDelegateForColumn(Enum_SetTable_Step_Col,new CSpinBoxDelegate(this,0,9999999,1,10));
    m_table->setItemDelegateForColumn(Enum_SetTable_Width_Col,new CSpinBoxDelegate(this,0,63));
    m_table->setItemDelegateForColumn(Enum_SetTable_Dotbit_Col,new CSpinBoxDelegate(this,0,63));

    QStringList typeList;
    typeList << "NULL" << "CTL" << "CUR" << "VOL"  << "IMP" << "ANG" << "TIME"
             << "PTCT" << "STR";
    m_table->setItemDelegateForColumn(Enum_SetTable_Type_Col,new CComboBoxDelegate(typeList,this));

    QStringList unitList;
    unitList << "A" << "V" << "kA" << "kV" << "s" << "Ω" << "Hz" << "W"  << "V/s"  << "Hz/s" << "N";
    m_table->setItemDelegateForColumn(Enum_SetTable_Unit_Col,new CComboBoxDelegate(unitList,this));

    QStringList kiloUnitList;
    kiloUnitList << "A" << "V" << "kA" << "kV" << "s" << "Ω" << "Hz" << "mW"  << "kV/s"  << "Hz/s" << "N";
    m_table->setItemDelegateForColumn(Enum_SetTable_KUnit_Col,new CComboBoxDelegate(kiloUnitList,this));
}

CSettingItemWnd::~CSettingItemWnd()
{

}

void CSettingItemWnd::itemDoubleClickedSlot(QTableWidgetItem *item)
{
    if(item->column() == Enum_PubTable_Attr_Col)
    {
        SettingItem * p = (SettingItem *)item->data(Qt::UserRole+1).toULongLong();
        QStringList list;
        list << "DB0:可见标志" << "DB1:信号上送标志" << "DB2:录波标志" << "DB3:不分组定值 ";
        CBitSelectedDialog * dialog = new CBitSelectedDialog(p->dwAttr,list,this);
        dialog->exec();
        item->setData(Qt::EditRole,dialog->getVal());
        delete dialog;
    }
}

void CSettingItemWnd::writeConfigVal(QTableWidgetItem *item)
{
    SettingItem * p = (SettingItem *)item->data(Qt::UserRole + 1).toULongLong();

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
    case Enum_SetTable_Min_Col:
        p->wValMin = item->text().toUInt();
        break;
    case Enum_SetTable_Max_Col:
        p->wValMax = item->text().toUInt();
        break;
    case Enum_SetTable_Dft_Col:
        p->wValDft = item->text().toUInt();
        break;
    case Enum_SetTable_Step_Col:
        p->wValStep = item->text().toUInt();
        break;
    case Enum_SetTable_SecIn_Col:
        if(m_data->checkSPSetCnnInfo(item->text()) == false)
        {
            CMsgInfo msgInfo( CMsgInfo::Enum_Application_Verify_Mode, CMsgInfo::CN_ERROR_MSG,
                              QString("第%1行关联二次额定值（%2 -> %3）检验错误(格式错误、关键字错误、索引越限)，需要修正!")
                              .arg(item->row()+1).arg(p->sSecIn).arg(item->text()));
            outPutMsgInfo(msgInfo);
        }
        p->sSecIn = item->text();
        break;
    case Enum_SetTable_CoeRet_Col:
        p->wCoeRet = item->text().toUInt();
        break;
    case Enum_SetTable_Type_Col:
        if(m_data->checkSetType(item->text()) == false)
        {
            CMsgInfo msgInfo( CMsgInfo::Enum_Application_Verify_Mode, CMsgInfo::CN_ERROR_MSG,
                              QString("第%1行类型（%2 -> %3）不属于标准类型，需修正!")
                              .arg(item->row()+1).arg(p->sType).arg(item->text()));
            outPutMsgInfo(msgInfo);
        }
        p->sType = item->text();
        break;
    case Enum_SetTable_Width_Col:
        if(item->text().toUInt() <= p->byDotBit)
        {
            CMsgInfo msgInfo( CMsgInfo::Enum_Application_Verify_Mode, CMsgInfo::CN_ERROR_MSG,
                              QString("第%1行位宽（%2 -> %3）设置小于或等于精度（%4），需要修正!")
                              .arg(item->row()+1).arg(p->byWidth).arg(item->text()).arg(p->byDotBit) );
            outPutMsgInfo(msgInfo);
        }
        p->byWidth = item->text().toUInt();
        break;
    case Enum_SetTable_Dotbit_Col:
        if(item->text().toUInt() >= p->byWidth)
        {
            CMsgInfo msgInfo( CMsgInfo::Enum_Application_Verify_Mode, CMsgInfo::CN_ERROR_MSG,
                              QString("第%1行精度（%2 -> %3）设置大于或等于位宽（%4），需要修正!")
                              .arg(item->row()+1).arg(p->byDotBit).arg(item->text()).arg(p->byWidth) );
            outPutMsgInfo(msgInfo);
        }
        p->byDotBit = item->text().toUInt();
        break;
    case Enum_SetTable_Unit_Col:
        p->sUnit = item->text();
        break;
    case Enum_SetTable_KUnit_Col:
        p->sKiloUnit = item->text();
        break;
    default:
        break;
    }
}

bool CSettingItemWnd::checkItemVal(QTableWidgetItem *item)
{
    if(item == NULL)
        return true;

    bool ok = true;
    SettingItem * p = (SettingItem *)item->data(Qt::UserRole+1).toULongLong();
    switch (item->column())
    {
    case Enum_PubTable_Name_Col:
        ok = m_data->checkNameNotDuplicate(p->sName);
        break;
    case Enum_SetTable_Min_Col:
        if((p->wValMin > p->wValMax)||(p->wValMin > p->wValDft))
            ok = false;
        break;
    case Enum_SetTable_Max_Col:
        if((p->wValMax < p->wValMin)||(p->wValMax < p->wValDft))
            ok = false;
        break;
    case Enum_SetTable_Dft_Col:
        if((p->wValDft > p->wValMax)||(p->wValMin > p->wValDft))
            ok = false;
        break;
    case Enum_SetTable_SecIn_Col:
        ok = m_data->checkSPSetCnnInfo(p->sSecIn);
        break;
    case Enum_SetTable_Width_Col:
    case Enum_SetTable_Dotbit_Col:
        if(p->byDotBit >= p->byWidth)
            ok = false;
        break;
    default:
        break;
    }
    return ok;
}

void CSettingItemWnd::createItem(int row, BaseItem *baseItem)
{
    SettingItem * item = (SettingItem *)baseItem;
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

    QTableWidgetItem * min = new QTableWidgetItem(QString::number(item->wValMin));
    min->setData(Qt::UserRole + 1 , data);
    min->setData(Qt::ToolTip,"最小值 = 真实最小值*10^精度");
    m_table->setItem(row,Enum_SetTable_Min_Col,min);

    QTableWidgetItem * max = new QTableWidgetItem(QString::number(item->wValMax));
    max->setData(Qt::UserRole + 1 , data);
    max->setData(Qt::ToolTip,"最大值 = 真实最大值*10^精度");
    m_table->setItem(row,Enum_SetTable_Max_Col,max);

    QTableWidgetItem * dft = new QTableWidgetItem(QString::number(item->wValDft));
    dft->setData(Qt::UserRole + 1 , data);
    dft->setData(Qt::ToolTip,"缺省值 = 真实缺省值*10^精度");
    m_table->setItem(row,Enum_SetTable_Dft_Col,dft);

    QTableWidgetItem * step = new QTableWidgetItem(QString::number(item->wValStep));
    step->setData(Qt::UserRole + 1 , data);
    step->setData(Qt::ToolTip,"步长 = 真实步长*10^精度");
    m_table->setItem(row,Enum_SetTable_Step_Col,step);

    QTableWidgetItem * setS = new QTableWidgetItem(item->sSecIn);
    setS->setData(Qt::UserRole + 1 , data);
    setS->setData(Qt::ToolTip, "二次额定值关联，格式为 <子表前缀：序号> （序号从0开始），缺省为-1或不填");
    m_table->setItem(row,Enum_SetTable_SecIn_Col,setS);

    QTableWidgetItem * coeRet = new QTableWidgetItem(QString::number(item->wCoeRet));
    coeRet->setData(Qt::UserRole + 1 , data);
    coeRet->setData(Qt::ToolTip,"返回系数 = 真实返回系数*100");
    m_table->setItem(row,Enum_SetTable_CoeRet_Col,coeRet);

    QTableWidgetItem * type = new QTableWidgetItem(item->sType);
    type->setData(Qt::UserRole + 1 , data);
    type->setData(Qt::ToolTip,"定值类型：缺省（NULL）、控制字(CTL)、电流型(CUR)、电压型(VOL)、阻抗型(IMP)、角度型(ANG)、时间型(TIME)、PTCT一二次额定值(PTCT)、字符串(STR)");
    m_table->setItem(row,Enum_SetTable_Type_Col,type);

    QTableWidgetItem * width = new QTableWidgetItem(QString::number(item->byWidth));
    width->setData(Qt::UserRole + 1 , data);
    width->setData(Qt::ToolTip,"小数点也算一个位宽");
    m_table->setItem(row,Enum_SetTable_Width_Col,width);

    QTableWidgetItem * dot = new QTableWidgetItem(QString::number(item->byDotBit));
    dot->setData(Qt::UserRole + 1 , data);
    m_table->setItem(row,Enum_SetTable_Dotbit_Col,dot);

    QTableWidgetItem * unit = new QTableWidgetItem(item->sUnit);
    unit->setData(Qt::UserRole + 1 , data);
    m_table->setItem(row,Enum_SetTable_Unit_Col,unit);

    QTableWidgetItem * kUnit = new QTableWidgetItem(item->sKiloUnit);
    kUnit->setData(Qt::UserRole + 1 , data);
    m_table->setItem(row,Enum_SetTable_KUnit_Col,kUnit);
}

BaseTab *CSettingItemWnd::getConfig()
{
    BaseTab * config = NULL;
    switch (m_type)
    {
    case Enum_SetTable_Type_SP:
        config = &m_data->GetConfig()->settingSPConifg;
        break;
    case Enum_SetTable_Type_SG:
        config = &m_data->GetConfig()->settingSGConifg;
        break;
    default:
        break;
    }
    return config;
}

BaseItem *CSettingItemWnd::CreateNewItem(int row)
{
    return new SettingItem(row);
}
