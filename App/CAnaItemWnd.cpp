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

    m_comboBox->addItems(headerList);
    m_comboBox->setCurrentIndex(0);

    m_table->setItemDelegateForColumn(Enum_AnaTable_Coe_Col,new CSpinBoxDelegate(this));
    m_table->setItemDelegateForColumn(Enum_PubTable_Attr_Col,new CSpinBoxDelegate(this,0,65535,1,2));
    m_table->setItemDelegateForColumn(Enum_AnaTable_XuYCDft_Col,new CSpinBoxDelegate(this,-1,9999999));
    m_table->setItemDelegateForColumn(Enum_AnaTable_DataWidth_Col,new CSpinBoxDelegate(this,0,63));
    m_table->setItemDelegateForColumn(Enum_AnaTable_DataDot_Col,new CSpinBoxDelegate(this,0,63));

    QStringList typeList;
    typeList << "AC" << "DC" << "Other";
    m_table->setItemDelegateForColumn(Enum_AnaTable_ChanType_Col,new CComboBoxDelegate(typeList,this));

    QStringList flagList;
    flagList << "UA" << "UB" << "UC" << "UX"  << "UAB" << "UBC" << "UCA"
             << "3U0" << "U1" << "U2" << "IA" << "IB" << "IC" << "3I0"
             << "P" << "Q" << "S" << "COS" << "FR" << "Other";
    m_table->setItemDelegateForColumn(Enum_AnaTable_ChanFlag_Col,new CComboBoxDelegate(flagList,this));

    QStringList unitList;
    unitList << "A" << "V" << "s" << "Ω" << "Hz" << "W"  << "V/s"  << "Hz/s" << "N";
    m_table->setItemDelegateForColumn(Enum_AnaTable_Unit_Col,new CComboBoxDelegate(unitList,this));

    QStringList kiloUnitList;
    kiloUnitList << "kA" << "kV" << "s" << "Ω" << "Hz" << "mW"  << "kV/s"  << "Hz/s" << "N";
    m_table->setItemDelegateForColumn(Enum_AnaTable_KUnit_Col,new CComboBoxDelegate(kiloUnitList,this));
}

CAnaItemWnd::~CAnaItemWnd()
{

}

bool CAnaItemWnd::checkItemVal(QTableWidgetItem *item)
{
    if(item == NULL)
        return true;

    bool ok = true;
    AnaItem * ana = (AnaItem *)item->data(Qt::UserRole+1).toULongLong();
    switch (item->column())
    {
    case Enum_PubTable_Name_Col:
        ok = m_data->checkNameNotDuplicate(ana->sName);
        break;
    case Enum_AnaTable_RateSetP_Col:
        ok = m_data->checkSPSetCnnInfo(ana->sRateSetP);
        break;
    case Enum_AnaTable_RateSetS_Col:
        ok = m_data->checkSPSetCnnInfo(ana->sRateSetS);
        break;
    case Enum_AnaTable_RecSYb_Col:
        ok = m_data->checkSoftYBCnnInfo(ana->sRecSYb);
        break;
    case Enum_AnaTable_RecHYb_Col:
        ok = m_data->checkHardBICnnInfo(ana->sRecHYb);
        break;
    case Enum_AnaTable_DataWidth_Col:
    case Enum_AnaTable_DataDot_Col:
        if(ana->byDotBit >= ana->byWidth)
            ok = false;
        break;
    default:
        break;
    }
    return ok;
}

void CAnaItemWnd::writeConfigVal(QTableWidgetItem *item)
{
    AnaItem * ana = (AnaItem *)item->data(Qt::UserRole + 1).toULongLong();

    switch (item->column())
    {
    case Enum_PubTable_Desc_Col:
        ana->sDesc = item->text();
        break;
    case Enum_PubTable_Name_Col:
        if(m_data->checkNameNoExit(item->text()) == false)
        {
            CMsgInfo msgInfo( CMsgInfo::Enum_Application_Verify_Mode, CMsgInfo::CN_ERROR_MSG,
                              QString("第%1行名称（%2 -> %3）存在重名，需重新命名!")
                              .arg(item->row()+1).arg(ana->sName).arg(item->text()));
            outPutMsgInfo(msgInfo);
        }
        ana->sName = item->text();
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
        if(m_data->checkSPSetCnnInfo(item->text()) == false)
        {
            CMsgInfo msgInfo( CMsgInfo::Enum_Application_Verify_Mode, CMsgInfo::CN_ERROR_MSG,
                              QString("第%1行关联一次额定值（%2 -> %3）检验错误(格式错误、关键字错误、索引越限)，需要修正!")
                              .arg(item->row()+1).arg(ana->sRateSetP).arg(item->text()));
            outPutMsgInfo(msgInfo);
        }
        ana->sRateSetP = item->text();
        break;
    case Enum_AnaTable_RateSetS_Col:
        if(m_data->checkSPSetCnnInfo(item->text()) == false)
        {
            CMsgInfo msgInfo( CMsgInfo::Enum_Application_Verify_Mode, CMsgInfo::CN_ERROR_MSG,
                              QString("第%1行关联二次额定值（%2 -> %3）检验错误(格式错误、关键字错误、索引越限)，需要修正!")
                              .arg(item->row()+1).arg(ana->sRateSetS).arg(item->text()));
            outPutMsgInfo(msgInfo);
        }
        ana->sRateSetS = item->text();
        break;

    case Enum_AnaTable_RecSYb_Col:
        if(m_data->checkSoftYBCnnInfo(item->text()) == false)
        {
            CMsgInfo msgInfo( CMsgInfo::Enum_Application_Verify_Mode, CMsgInfo::CN_ERROR_MSG,
                              QString("第%1行关联软压板（%2 -> %3）检验错误(格式错误、关键字错误、索引越限)，需要修正!")
                              .arg(item->row()+1).arg(ana->sRecSYb).arg(item->text()));
            outPutMsgInfo(msgInfo);
        }
        ana->sRecSYb = item->text();
        break;
    case Enum_AnaTable_RecHYb_Col:
        if(m_data->checkHardBICnnInfo(item->text()) == false)
        {
            CMsgInfo msgInfo( CMsgInfo::Enum_Application_Verify_Mode, CMsgInfo::CN_ERROR_MSG,
                              QString("第%1行关联硬压板（%2 -> %3）检验错误(格式错误、关键字错误、索引越限)，需要修正!")
                              .arg(item->row()+1).arg(ana->sRecHYb).arg(item->text()));
            outPutMsgInfo(msgInfo);
        }
        ana->sRecHYb = item->text();
        break;
    case Enum_AnaTable_DataWidth_Col:
        if(item->text().toUInt() <= ana->byDotBit)
        {
            CMsgInfo msgInfo( CMsgInfo::Enum_Application_Verify_Mode, CMsgInfo::CN_ERROR_MSG,
                              QString("第%1行位宽（%2 -> %3）设置小于或等于精度（%4），需要修正!")
                              .arg(item->row()+1).arg(ana->byWidth).arg(item->text()).arg(ana->byDotBit) );
            outPutMsgInfo(msgInfo);
        }
        ana->byWidth = item->text().toUInt();
        break;
    case Enum_AnaTable_DataDot_Col:
        if(item->text().toUInt() >= ana->byWidth)
        {
            CMsgInfo msgInfo( CMsgInfo::Enum_Application_Verify_Mode, CMsgInfo::CN_ERROR_MSG,
                              QString("第%1行精度（%2 -> %3）设置大于或等于位宽（%4），需要修正!")
                              .arg(item->row()+1).arg(ana->byDotBit).arg(item->text()).arg(ana->byWidth) );
            outPutMsgInfo(msgInfo);
        }
        ana->byDotBit = item->text().toUInt();
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

void CAnaItemWnd::createItem(int row, BaseItem *baseItem)
{
    AnaItem * item = (AnaItem *)baseItem;
    QVariant data = (qulonglong)item;

    QTableWidgetItem * desc = new QTableWidgetItem(item->sDesc);
    desc->setData(Qt::UserRole + 1 , data);
    m_table->setItem(row,Enum_PubTable_Desc_Col,desc);

    QTableWidgetItem * name = new QTableWidgetItem(item->sName);
    name->setData(Qt::UserRole + 1 , data);
    m_table->setItem(row,Enum_PubTable_Name_Col,name);

    AttrItem * attr = new AttrItem(QString::number(item->dwAttr));
    attr->setData(Qt::UserRole + 1 , data);
    attr->setData(Qt::ToolTip, "二进制编辑：DB0:可见标志，DB1:信号上送标志，DB2:录波标志，DB3:采样标志");
    m_table->setItem(row,Enum_PubTable_Attr_Col,attr);

    QTableWidgetItem * type = new QTableWidgetItem(item->sChanType);
    type->setData(Qt::UserRole + 1 , data);
    m_table->setItem(row,Enum_AnaTable_ChanType_Col,type);

    QTableWidgetItem * flag = new QTableWidgetItem(item->sChanFlag);
    flag->setData(Qt::UserRole + 1 , data);
    m_table->setItem(row,Enum_AnaTable_ChanFlag_Col,flag);

    QTableWidgetItem * dft = new QTableWidgetItem(QString::number(item->iXuYCDft));
    dft->setData(Qt::UserRole + 1 , data);
    dft->setData(Qt::ToolTip,"缺省值 = 真实缺省值*10^精度");
    m_table->setItem(row,Enum_AnaTable_XuYCDft_Col,dft);

    QTableWidgetItem * coe = new QTableWidgetItem(QString::number(item->wCoe));
    coe->setData(Qt::UserRole + 1 , data);
    coe->setData(Qt::ToolTip,"数字量：I:1000,U：100， 模拟量：PT=400/5,COE=58; PT=150/5,COE=154; CT=10/5,COE=2306; CT=150/5,COE=153");
    m_table->setItem(row,Enum_AnaTable_Coe_Col,coe);

    QTableWidgetItem * setP = new QTableWidgetItem(item->sRateSetP);
    setP->setData(Qt::UserRole + 1 , data);
    setP->setData(Qt::ToolTip, "一次额定值关联，格式为 <子表前缀：序号> （序号从0开始），缺省为-1或不填");
    m_table->setItem(row,Enum_AnaTable_RateSetP_Col,setP);

    QTableWidgetItem * setS = new QTableWidgetItem(item->sRateSetS);
    setS->setData(Qt::UserRole + 1 , data);
    setS->setData(Qt::ToolTip, "二次额定值关联，格式为 <子表前缀：序号> （序号从0开始），缺省为-1或不填");
    m_table->setItem(row,Enum_AnaTable_RateSetS_Col,setS);

    QTableWidgetItem * sYB = new QTableWidgetItem(item->sRecSYb);
    sYB->setData(Qt::UserRole + 1 , data);
    sYB->setData(Qt::ToolTip, "接收软压板关联，格式为 <子表前缀：序号> （序号从0开始），缺省为-1或不填");
    m_table->setItem(row,Enum_AnaTable_RecSYb_Col,sYB);

    QTableWidgetItem * hYB = new QTableWidgetItem(item->sRecHYb);
    hYB->setData(Qt::UserRole + 1 , data);
    hYB->setData(Qt::ToolTip, "接收硬压板关联，格式为 <子表前缀：序号> （序号从0开始），缺省为-1或不填");
    m_table->setItem(row,Enum_AnaTable_RecHYb_Col,hYB);

    QTableWidgetItem * width = new QTableWidgetItem(QString::number(item->byWidth));
    width->setData(Qt::UserRole + 1 , data);
    width->setData(Qt::ToolTip,"小数点也算一个位宽");
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

BaseTab *CAnaItemWnd::getConfig()
{
    BaseTab * config = NULL;
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
    return config;
}

BaseItem *CAnaItemWnd::CreateNewItem(int row)
{
    return new AnaItem(row);
}
