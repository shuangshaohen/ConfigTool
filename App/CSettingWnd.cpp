#include "CSettingWnd.h"
#include "CDataFun.h"
#include "CAttrDialog.h"
#include "CTypeDialog.h"
#include "CTableWidgetDialog.h"
#include <QMessageBox>
#include <QInputDialog>
#include "ui_SettingWnd.h"

CSettingWnd::CSettingWnd(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CSettingWnd)
{
    ui->setupUi(this);

    connect(ui->m_TableWidget, SIGNAL(cellChanged( int, int )), this, SLOT(TableWidgetCellChangedSlot( int, int )) );
    connect(ui->m_TableWidget, SIGNAL(cellDoubleClicked(int,int)),this,SLOT(TableWidgetCellDoubleClickedSlot(int,int)));

    connect(ui->m_SelectComboBox,SIGNAL(currentIndexChanged(int)), this, SLOT(SelectComboBoxCurrentIndexChangedSlot(int)));
    connect(ui->m_TypeComboBox,SIGNAL(currentIndexChanged(int)), this, SLOT(TypeComboBoxCurrentIndexChangedSlot(int)));
    connect(ui->m_KeyLineEdit,SIGNAL(textChanged(const QString &)), this, SLOT(InfoLineEditTextChanged(const QString &)));

    connect(ui->m_SettingGroups,SIGNAL(textChanged(const QString &)), this, SLOT(SettingGroupLineEditTextChanged(const QString &)));
}

CSettingWnd::~CSettingWnd()
{
    delete ui;
}

void CSettingWnd::ClearTableModel()
{
    ui->m_TableWidget->clearContents();
    m_pGseConfig = NULL;
}

void CSettingWnd::ShowSettingInfo(CDataBase *pCDataBase)
{
    m_pGseConfig = pCDataBase->GetConfig();

    ui->m_TableWidget->clearContents();
    ui->m_TableWidget->setRowCount(m_pGseConfig->settingConfig.size());

    ui->m_SettingGroups->setText(QString::number(m_pGseConfig->setGroup));

    disconnect(ui->m_TableWidget, SIGNAL(cellChanged( int, int )), 0, 0);

    for (int i = 0; i < m_pGseConfig->settingConfig.size() ; i++)
    {
        SettingConfig * pNode = m_pGseConfig->settingConfig[i];
        CreateRowOfTable(pNode,i);
    }

    ui->m_TableWidget->resizeColumnsToContents();

    HideSomeRowOfTableWidget();

    connect( ui->m_TableWidget, SIGNAL(cellChanged( int, int )), this, SLOT(TableWidgetCellChangedSlot( int, int )) );
}

void CSettingWnd::AddOper()
{
    QTableWidget * pTable = ui->m_TableWidget;
    QTableWidgetItem * pItem = NULL;
    int row = 0;
    pItem = pTable->currentItem();
    if(pItem != NULL)
        row = pItem->row() + 1;

    QString newName = InputNewName("NewSet_" + QString::number(m_pGseConfig->settingConfig.size()));

    if(newName == "NULL")
        return;

    SettingConfig * pNewNode = new SettingConfig();

    pNewNode->sName         = newName;

    pNewNode->sDesc         = newName;
    pNewNode->sUnit         = "N";
    pNewNode->sKiloUnit     = "N";
    pNewNode->sAddr         = "";

    pNewNode->wIndex        = row;
    pNewNode->dwAttr        = 0;
    pNewNode->dwValMin      = 1;
    pNewNode->dwValMax      = 0xFFFF;
    pNewNode->dwValDft      = 0xFFFF;
    pNewNode->wSecIn        = 0xFFFF;
    pNewNode->wCoeRet       = 0xFF;
    pNewNode->wType         = 0;
    pNewNode->byGroup      = 0;
    pNewNode->byWidth      = 6;
    pNewNode->byDotBit     = 0;


    pNewNode->sSecIn      = "NULL";

    disconnect(pTable, SIGNAL(cellChanged( int, int )), 0, 0);
    m_pGseConfig->settingConfig.insert(row,pNewNode);
    pTable->insertRow(row);
    CreateRowOfTable(pNewNode,row);
    connect( pTable, SIGNAL(cellChanged( int, int )), this, SLOT(TableWidgetCellChangedSlot( int, int )) );
}

void CSettingWnd::DeleteOper()
{
    QTableWidgetItem * pItem = NULL;
    pItem = ui->m_TableWidget->currentItem();
    if(pItem == NULL)
        return;

    int row = pItem->row();
    QString oldName = m_pGseConfig->settingConfig[row]->sName;
    ui->m_TableWidget->removeRow(row);
    m_pGseConfig->settingConfig.remove(row);
    RefreshCnnSetName(oldName, "NULL");
}

void CSettingWnd::CopyOper()
{
    QTableWidgetItem * pItem = NULL;
    pItem = ui->m_TableWidget->currentItem();
    if(pItem == NULL)
        return;

    SettingConfig * pNode = GetNodeDataPoint(pItem);
    QString newName = InputNewName(pNode->sName+"_Copy");

    if(newName == "NULL")
        return;

    int row = pItem->row();
    SettingConfig * pNewNode = new SettingConfig();

    *pNewNode = *pNode;

    pNewNode->sName  = newName;

    disconnect(ui->m_TableWidget, SIGNAL(cellChanged( int, int )), 0, 0);
    m_pGseConfig->settingConfig.insert(row+1,pNewNode);
    ui->m_TableWidget->insertRow(row+1);
    CreateRowOfTable(pNewNode,row+1);
    connect( ui->m_TableWidget, SIGNAL(cellChanged( int, int )), this, SLOT(TableWidgetCellChangedSlot( int, int )) );
}

void CSettingWnd::UpOper()
{
    QTableWidgetItem * pItem = NULL;
    pItem = ui->m_TableWidget->currentItem();
    if(pItem == NULL)
        return;

    int row = pItem->row();
    disconnect(ui->m_TableWidget, SIGNAL(cellChanged( int, int )), 0, 0);

    if(row != 0)
    {
        CDataFun::GetInstance()->moveRow(ui->m_TableWidget,row,row-1);
        m_pGseConfig->settingConfig.swapItemsAt(row,row-1);
    }
    connect( ui->m_TableWidget, SIGNAL(cellChanged( int, int )), this, SLOT(TableWidgetCellChangedSlot( int, int )) );
}

void CSettingWnd::DownOper()
{
    QTableWidgetItem * pItem = NULL;
    pItem = ui->m_TableWidget->currentItem();
    if(pItem == NULL)
        return;

    int row = pItem->row();
    disconnect(ui->m_TableWidget, SIGNAL(cellChanged( int, int )), 0, 0);

    if(row != ui->m_TableWidget->rowCount()-1)
    {
        CDataFun::GetInstance()->moveRow(ui->m_TableWidget,row,row+2);
        m_pGseConfig->settingConfig.swapItemsAt(row,row+1);
    }
    connect( ui->m_TableWidget, SIGNAL(cellChanged( int, int )), this, SLOT(TableWidgetCellChangedSlot( int, int )) );
}

void CSettingWnd::SelectComboBoxCurrentIndexChangedSlot(int index)
{
    Q_UNUSED(index);
    HideSomeRowOfTableWidget();
}

void CSettingWnd::TypeComboBoxCurrentIndexChangedSlot(int index)
{
    Q_UNUSED(index);
    HideSomeRowOfTableWidget();
}

void CSettingWnd::InfoLineEditTextChanged(const QString &text)
{
    Q_UNUSED(text);
    HideSomeRowOfTableWidget();
}

void CSettingWnd::TableWidgetCellChangedSlot(int row, int column)
{
    disconnect(ui->m_TableWidget,SIGNAL(cellChanged(int,int)),0,0);
    QTableWidgetItem * pItem = NULL;
    switch (column)
    {
    case Enum_SettingTable_Name_Col:
        pItem = ui->m_TableWidget->item(row,column);
        ChangeName(pItem);
        break;
    case Enum_SettingTable_Attr_Col:
        pItem = ui->m_TableWidget->item(row,column);
        ChangeAttr(pItem);
        break;
    case Enum_SettingTable_Desc_Col:
        pItem = ui->m_TableWidget->item(row,column);
        ChangeDesc(pItem);
        break;
    case Enum_SettingTable_CoeRet_Col:
        pItem = ui->m_TableWidget->item(row,column);
        ChangeCoeRet(pItem);
        break;
    case Enum_SettingTable_MaxVal_Col:
        pItem = ui->m_TableWidget->item(row,column);
        ChangeMaxVal(pItem);
        break;
    case Enum_SettingTable_MinVal_Col:
        pItem = ui->m_TableWidget->item(row,column);
        ChangeMinVal(pItem);
        break;
    case Enum_SettingTable_DftVal_Col:
        pItem = ui->m_TableWidget->item(row,column);
        ChangeDftVal(pItem);
        break;
    case Enum_SettingTable_Width_Col:
        pItem = ui->m_TableWidget->item(row,column);
        ChangeWidth(pItem);
        break;
    case Enum_SettingTable_Dot_Col:
        pItem = ui->m_TableWidget->item(row,column);
        ChangeDotBit(pItem);
        break;
    case Enum_SettingTable_Unit_Col:
        pItem = ui->m_TableWidget->item(row,column);
        ChangeUnit(pItem);
        break;
    case Enum_SettingTable_KUnit_Col:
        pItem = ui->m_TableWidget->item(row,column);
        ChangeKunit(pItem);
        break;
    case Enum_SettingTable_Type_Col:
        pItem = ui->m_TableWidget->item(row,column);
        ChangeType(pItem);
        break;
    case Enum_SettingTable_Group_Col:
        pItem = ui->m_TableWidget->item(row,column);
        ChangeGroup(pItem);
        break;
    case Enum_SettingTable_SecIn_Col:
        pItem = ui->m_TableWidget->item(row,column);
        ChangeSecIn(pItem);
        break;
    default:
        break;
    }
    connect( ui->m_TableWidget, SIGNAL(cellChanged( int, int )), this, SLOT(TableWidgetCellChangedSlot( int, int )) );
}

void CSettingWnd::TableWidgetCellDoubleClickedSlot(int row, int column)
{
    if(column == Enum_SettingTable_Attr_Col)
    {
        QTableWidgetItem * pItem = ui->m_TableWidget->item(row,column);
        CAttrDialog * pDialog = new CAttrDialog(ui->m_TableWidget,pItem);
        pDialog->SetCheckBoxText(0,"DB0:可见标志");
        pDialog->SetCheckBoxText(1,"DB1:信号上送标志");
        pDialog->SetCheckBoxText(2,"DB2:录波");
        pDialog->SetCheckBoxText(3,"DB3:不分组定值");
        pDialog->SetCheckBoxText(4,"DB4:出口控制字偏移");
        pDialog->SetCheckBox(CDataFun::GetInstance()->ChangeQStringToUInt(pItem->text()));
        pDialog->show();
    }
    if(column == Enum_SettingTable_Type_Col)
    {
        QTableWidgetItem * pItem = ui->m_TableWidget->item(row,column);
        SettingConfig * pNode = GetNodeDataPoint(pItem);
        CTypeDialog * pDialog = new CTypeDialog(ui->m_TableWidget,pItem);
        for (int i = 0; i < CN_MAX_TYPE_NUM; i++)
        {
            QString text = CDataFun::GetInstance()->GetQStringfromSetType(i);
            if(text != "ERROR")
                pDialog->SetRadioButtonText(i,text);
        }
        pDialog->setWindowTitle(pNode->sName + "定值类型设置");
        pDialog->SetType(pNode->wType);
        pDialog->show();
    }
    if(column == Enum_SettingTable_Group_Col)
    {
        QTableWidgetItem * pItem = ui->m_TableWidget->item(row,column);
        SettingConfig * pNode = GetNodeDataPoint(pItem);
        CTypeDialog * pDialog = new CTypeDialog(ui->m_TableWidget,pItem);
        for (int i = 0; i < CN_MAX_TYPE_NUM; i++)
        {
            QString text = CDataFun::GetInstance()->GetQStringfromSetGroup(i);
            if(text != "ERROR")
                pDialog->SetRadioButtonText(i,text);
        }
        pDialog->setWindowTitle(pNode->sName + "组别设置");
        pDialog->SetType(pNode->byGroup);
        pDialog->show();
    }
    if(column == Enum_SettingTable_SecIn_Col)
    {
        QTableWidgetItem * pItem = ui->m_TableWidget->item(row,column);
        SettingConfig * pNode = GetNodeDataPoint(pItem);
        if((pNode->wType != EN_CURRENT_TYPE)
                &&(pNode->wType != EN_IMPEDANCE_TYPE))
        {
            if(pItem->text() != "NULL")
            {
                pItem->setText("NULL");
                pNode->wSecIn = 0xFFFF;
            }
            return;
        }

        CTableWidgetDialog * pDialog = new CTableWidgetDialog(ui->m_TableWidget,pItem);
        pDialog->SetConfig(m_pGseConfig);
        pDialog->ShowSetWithSecInType();
        pDialog->setWindowTitle(pNode->sName + "二次额定值关联");
        pDialog->show();
    }
}

void CSettingWnd::SettingGroupLineEditTextChanged(const QString &text)
{
    bool ok = false;
    int group = text.toInt(&ok,10);
    if(ok)
    {
        m_pGseConfig->setGroup = group;
    }
    else
    {
        ui->m_SettingGroups->setText(QString::number(m_pGseConfig->setGroup));
    }
}

void CSettingWnd::SetNodeDataPoint(QTableWidgetItem *pItem, SettingConfig *pNode)
{
    if(NULL!=pItem)
    {
        pItem->setData( Qt::UserRole, QVariant((uint)pNode) );
    }
}

SettingConfig *CSettingWnd::GetNodeDataPoint(QTableWidgetItem *pItem)
{
    SettingConfig* pNode = NULL;

    if(NULL!=pItem)
    {
        pNode = (SettingConfig*)(pItem->data( Qt::UserRole ).toUInt());
    }

    return pNode;
}

void CSettingWnd::ChangeName(QTableWidgetItem *pItem)
{
    SettingConfig * p = GetNodeDataPoint(pItem);

    QString oldName = p->sName;
    QString name = pItem->text();
    bool bFind = false;
    int indexFind = -1;

    for (int i = 0; i < m_pGseConfig->settingConfig.size(); i++)
    {
        if((p != m_pGseConfig->settingConfig[i])
            &&(name == m_pGseConfig->settingConfig[i]->sName))
        {
            bFind = true;
            indexFind = i;
            break;
        }
    }

    if(bFind)
    {
        QString strInfo = QString("修改失败，%1 存在重名，详见全部定值表第%2行!").arg(name).arg(QString::number(indexFind+1));
        QMessageBox::critical(this,"修改定值名称",strInfo);
        pItem->setText(p->sName);
    }
    else
    {
        p->sName = name;
        RefreshCnnSetName(oldName,name);
    }
}

void CSettingWnd::ChangeDesc(QTableWidgetItem *pItem)
{
    SettingConfig * p = GetNodeDataPoint(pItem);
    p->sDesc = pItem->text();
}

void CSettingWnd::ChangeAttr(QTableWidgetItem *pItem)
{
    SettingConfig * p = GetNodeDataPoint(pItem);
    p->dwAttr = CDataFun::GetInstance()->ChangeQStringToUInt(pItem->text());
}

void CSettingWnd::ChangeCoeRet(QTableWidgetItem *pItem)
{
    SettingConfig * p = GetNodeDataPoint(pItem);
    p->wCoeRet = CDataFun::GetInstance()->ChangeQStringToUInt(pItem->text());
}

void CSettingWnd::ChangeMaxVal(QTableWidgetItem *pItem)
{
    SettingConfig * p = GetNodeDataPoint(pItem);
    unsigned int val = CDataFun::GetInstance()->ChangeQStringToUInt(pItem->text());
    if(val < p->dwValMin)
    {
        QMessageBox::critical(this,"修改校验","最大值小于最小值，请重新输入！");
        pItem->setText(CDataFun::GetInstance()->ChangeDecToDec(p->dwValMax));
    }
    else
        p->dwValMax = val;
}

void CSettingWnd::ChangeMinVal(QTableWidgetItem *pItem)
{
    SettingConfig * p = GetNodeDataPoint(pItem);
    unsigned int val = CDataFun::GetInstance()->ChangeQStringToUInt(pItem->text());
    if(val > p->dwValMax)
    {
        QMessageBox::critical(this,"修改校验","最小值大于最大值，请重新输入！");
        pItem->setText(CDataFun::GetInstance()->ChangeDecToDec(p->dwValMin));
    }
    else
        p->dwValMin = val;
}

void CSettingWnd::ChangeDftVal(QTableWidgetItem *pItem)
{
    SettingConfig * p = GetNodeDataPoint(pItem);
    unsigned int val = CDataFun::GetInstance()->ChangeQStringToUInt(pItem->text());
    if(val > p->dwValMax)
    {
        QMessageBox::critical(this,"修改校验","缺省值大于最大值，请重新输入！");
        pItem->setText(CDataFun::GetInstance()->ChangeDecToDec(p->dwValDft));
    }
    else if(val < p->dwValMin)
    {
        QMessageBox::critical(this,"修改校验","缺省值小于最小值，请重新输入！");
        pItem->setText(CDataFun::GetInstance()->ChangeDecToDec(p->dwValDft));
    }
    else
        p->dwValDft = val;
}

void CSettingWnd::ChangeWidth(QTableWidgetItem *pItem)
{
    SettingConfig * p = GetNodeDataPoint(pItem);
    p->byWidth = CDataFun::GetInstance()->ChangeQStringToUInt(pItem->text());
}

void CSettingWnd::ChangeDotBit(QTableWidgetItem *pItem)
{
    SettingConfig * p = GetNodeDataPoint(pItem);
    p->byDotBit = CDataFun::GetInstance()->ChangeQStringToUInt(pItem->text());
}

void CSettingWnd::ChangeUnit(QTableWidgetItem *pItem)
{
    SettingConfig * p = GetNodeDataPoint(pItem);
    p->sUnit = pItem->text();
}

void CSettingWnd::ChangeKunit(QTableWidgetItem *pItem)
{
    SettingConfig * p = GetNodeDataPoint(pItem);
    p->sKiloUnit = pItem->text();
}

void CSettingWnd::ChangeType(QTableWidgetItem *pItem)
{
    unsigned int type = pItem->text().toUInt();
    SettingConfig * p = GetNodeDataPoint(pItem);
    p->wType = type;
    pItem->setText(CDataFun::GetInstance()->GetQStringfromSetType(type));

    QStringList unitList;
    unitList  << "N" << "N" << "A" << "V" << "Ω" << "°" << "s" << "N"
              << "A/V" << "V" << "A" << "A" << "A" << "N" << "V/s" << "N" << "N"
              << "N" << "Km" << "HZ" << "HZ/s" << "N" << "°" << "A";
    QStringList kUnitList;
    kUnitList  << "N" << "N" << "A" << "kV" << "Ω" << "°" << "s" << "N"
               << "A/kV" << "kV" << "A" << "A" << "A" << "N" << "kV/s" << "N" << "N"
               << "N" << "Km" << "HZ" << "HZ/s" << "N" << "°" << "A";

    p->sUnit = unitList[type];
    ui->m_TableWidget->item(pItem->row(),Enum_SettingTable_Unit_Col)->setText(p->sUnit);
    p->sKiloUnit = kUnitList[type];
    ui->m_TableWidget->item(pItem->row(),Enum_SettingTable_KUnit_Col)->setText(p->sUnit);
}

void CSettingWnd::ChangeGroup(QTableWidgetItem *pItem)
{
    unsigned int group = pItem->text().toUInt();
    SettingConfig * p = GetNodeDataPoint(pItem);
    p->byGroup = group;
    pItem->setText(CDataFun::GetInstance()->GetQStringfromSetGroup(group));
}

void CSettingWnd::ChangeSecIn(QTableWidgetItem *pItem)
{
    SettingConfig * p = GetNodeDataPoint(pItem);
    p->sSecIn = pItem->text();
}

void CSettingWnd::HideSomeRowOfTableWidget()
{
    int seletedAttr = ui->m_SelectComboBox->currentIndex();
    int seletedType = ui->m_TypeComboBox->currentIndex();
    QString keyStr = ui->m_KeyLineEdit->text();

    QTableWidget * pTable = ui->m_TableWidget;
    for (int i = 0; i < pTable->rowCount(); i++)
    {
        SettingConfig * pNode = GetNodeDataPoint(pTable->item(i,0));

        pTable->setRowHidden(i,true);

        if((keyStr == "")
                ||(pNode->sName.contains(keyStr)))
        {
            if((seletedAttr == 0)
                    ||((seletedAttr == 1)&& ((pNode->dwAttr & CN_NOGROUP_SET_BIT) != 0))
                    ||((seletedAttr == 2)&& ((pNode->dwAttr & CN_NOGROUP_SET_BIT) == 0)))
            {
                unsigned int type = (unsigned int) seletedType;
                if((type == 0)
                        ||(type == pNode->wType + 1))
                {
                    pTable->setRowHidden(i,false);
                }
            }
        }
    }
}

void CSettingWnd::CreateRowOfTable(SettingConfig *pNode, int row)
{
    QTableWidget * pTableWidget = ui->m_TableWidget;
    QTableWidgetItem * pTableWidgetItem = NULL;

    pTableWidgetItem = new QTableWidgetItem(pNode->sName);
    SetNodeDataPoint(pTableWidgetItem,pNode);
    pTableWidget->setItem(row,Enum_SettingTable_Name_Col,pTableWidgetItem);

    pTableWidgetItem = new QTableWidgetItem(pNode->sDesc);
    SetNodeDataPoint(pTableWidgetItem,pNode);
    pTableWidget->setItem(row,Enum_SettingTable_Desc_Col,pTableWidgetItem);

    pTableWidgetItem = new QTableWidgetItem(CDataFun::GetInstance()->GetQStringfromSetType(pNode->wType));
    SetNodeDataPoint(pTableWidgetItem,pNode);
    pTableWidgetItem->setFlags(pTableWidgetItem->flags()&(~Qt::ItemIsEditable));
    pTableWidget->setItem(row,Enum_SettingTable_Type_Col,pTableWidgetItem);

    pTableWidgetItem = new QTableWidgetItem(CDataFun::GetInstance()->ChangeDECToHEX(pNode->dwAttr));
    SetNodeDataPoint(pTableWidgetItem,pNode);
    pTableWidgetItem->setFlags(pTableWidgetItem->flags()&(~Qt::ItemIsEditable));
    pTableWidget->setItem(row,Enum_SettingTable_Attr_Col,pTableWidgetItem);

    pTableWidgetItem = new QTableWidgetItem(pNode->sSecIn);
    pTableWidgetItem->setFlags(pTableWidgetItem->flags()&(~Qt::ItemIsEditable));
    SetNodeDataPoint(pTableWidgetItem,pNode);
    pTableWidget->setItem(row,Enum_SettingTable_SecIn_Col,pTableWidgetItem);

    pTableWidgetItem = new QTableWidgetItem(CDataFun::GetInstance()->ChangeShortToDec(pNode->wCoeRet));
    SetNodeDataPoint(pTableWidgetItem,pNode);
    pTableWidget->setItem(row,Enum_SettingTable_CoeRet_Col,pTableWidgetItem);

    pTableWidgetItem = new QTableWidgetItem(CDataFun::GetInstance()->ChangeDecToDec(pNode->dwValMax));
    SetNodeDataPoint(pTableWidgetItem,pNode);
    pTableWidget->setItem(row,Enum_SettingTable_MaxVal_Col,pTableWidgetItem);

    pTableWidgetItem = new QTableWidgetItem(CDataFun::GetInstance()->ChangeDecToDec(pNode->dwValMin));
    SetNodeDataPoint(pTableWidgetItem,pNode);
    pTableWidget->setItem(row,Enum_SettingTable_MinVal_Col,pTableWidgetItem);

    pTableWidgetItem = new QTableWidgetItem(CDataFun::GetInstance()->ChangeDecToDec(pNode->dwValDft));
    SetNodeDataPoint(pTableWidgetItem,pNode);
    pTableWidget->setItem(row,Enum_SettingTable_DftVal_Col,pTableWidgetItem);

    pTableWidgetItem = new QTableWidgetItem(QString::number(pNode->byWidth));
    SetNodeDataPoint(pTableWidgetItem,pNode);
    pTableWidget->setItem(row,Enum_SettingTable_Width_Col,pTableWidgetItem);

    pTableWidgetItem = new QTableWidgetItem(QString::number(pNode->byDotBit));
    SetNodeDataPoint(pTableWidgetItem,pNode);
    pTableWidget->setItem(row,Enum_SettingTable_Dot_Col,pTableWidgetItem);

    pTableWidgetItem = new QTableWidgetItem(CDataFun::GetInstance()->GetQStringfromSetGroup(pNode->byGroup));
    SetNodeDataPoint(pTableWidgetItem,pNode);
    pTableWidgetItem->setFlags(pTableWidgetItem->flags()&(~Qt::ItemIsEditable));
    pTableWidget->setItem(row,Enum_SettingTable_Group_Col,pTableWidgetItem);

    pTableWidgetItem = new QTableWidgetItem(pNode->sUnit);
    SetNodeDataPoint(pTableWidgetItem,pNode);
    pTableWidget->setItem(row,Enum_SettingTable_Unit_Col,pTableWidgetItem);

    pTableWidgetItem = new QTableWidgetItem(pNode->sKiloUnit);
    SetNodeDataPoint(pTableWidgetItem,pNode);
    pTableWidget->setItem(row,Enum_SettingTable_KUnit_Col,pTableWidgetItem);
}

QString CSettingWnd::InputNewName(QString old)
{
    bool ok;
    QString text = QInputDialog::getText(this, tr("名称修改"),tr("请输入不重复的通道名称"), QLineEdit::Normal,old, &ok);
    if (ok && !text.isEmpty())
    {
        bool bFindSame = false;
        for (int i = 0; i < m_pGseConfig->settingConfig.size(); i++)
        {
            if(text == m_pGseConfig->settingConfig[i]->sName)
            {
                bFindSame = true;
                QString strInfo = QString("%1与表中的条目%2重复!").arg(text).arg(QString::number(i+1));
                QMessageBox::critical(this,"名称重复",strInfo);
                break;
            }
        }

        if(bFindSame)
            return InputNewName(old);
        else
            return text;
    }
    else
        return "NULL";
}

void CSettingWnd::RefreshCnnSetName(QString oldName, QString newName)
{
    for (int i = 0 ; i < m_pGseConfig->anaConfig.size(); i ++)
    {
        AnaConfig * p = m_pGseConfig->anaConfig.at(i);
        if(p->sSecInName == oldName)
            p->sSecInName = newName;

        if(p->sRFVName == oldName)
            p->sRFVName = newName;
    }

    QTableWidget * pTable = ui->m_TableWidget;
    for (int i = 0 ; i < pTable->rowCount(); i ++)
    {
        QTableWidgetItem * pItem = pTable->item(i,Enum_SettingTable_SecIn_Col);
        if(pItem->text() == oldName)
        {
            pItem->setText(newName);
            SettingConfig * p = GetNodeDataPoint(pItem);
            p->sSecIn = newName;
        }
    }
}
