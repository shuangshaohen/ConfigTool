#include "CComboBox.h"

CComboBox::CComboBox(QWidget *parent, _ENum_ComboBox_Type type, void *p) : QComboBox(parent)
{
    m_pAna = (AnaConfig*)p;
    switch (type) {
    case Enum_DefaultComboBox_Type:
        break;
    case Enum_AnaChanTypeComboBox_Type:
        SetAnaList();
        ConnectAnaTypeChanged(true);
        break;
    default:
        break;
    }
}

CComboBox::~CComboBox()
{
    m_pAna = NULL;
    this->clear();
    disconnect(this,SIGNAL(currentIndexChanged(int)),0,0);
}

void CComboBox::SetAnaList()
{
    this->clear();
    QStringList strList;
    strList << "未定义" << "电流" << "电压" << "站用变电压" << "有功功率" << "无功功率" << "视在功率" << "功率因素"
            << "直流电压" << "直流电流" << "档位" << "频率" << "温度" << "直流" << "脉冲" << "振幅" << "设备态"
            << "阻抗" << "故障电压" << "故障电流" << "零序电流" << "差动电流" << "故障测距" << "零序差压";
    this->addItems(strList);
}

void CComboBox::SetAnaNodePoint(AnaConfig *pAna)
{
    m_pAna = pAna;
}

void CComboBox::ConnectAnaTypeChanged(bool b)
{
    if(b)
        connect(this,SIGNAL(currentIndexChanged(int)),this,SLOT(AnaTypeCurrentIndexChangedSlot(int)));
    else
        disconnect(this,SIGNAL(currentIndexChanged(int)),this,SLOT(AnaTypeCurrentIndexChangedSlot(int)));
}

void CComboBox::AnaTypeCurrentIndexChangedSlot(int index)
{
    if(m_pAna != NULL)
        m_pAna->wChanType = index;
}
