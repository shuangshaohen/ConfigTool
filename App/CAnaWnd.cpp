#include "CAnaWnd.h"
#include "CDataFun.h"
#include "CComboBox.h"
#include "CAnaAttrDialog.h"
#include "CAnaTypeDialog.h"
#include "CAnaPhaseDialog.h"
#include "ui_AnaWnd.h"
#include <QMessageBox>
#include <QInputDialog>
#include <QLineEdit>
#include <QtDebug>

CAnaWnd::CAnaWnd(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CAnaWnd)
{
    ui->setupUi(this);

    ShowYCPropertyCheckBoxStateChangedSlot(ui->m_ShowYCPropertyCheckBox->checkState());

    connect(ui->m_ShowYCPropertyCheckBox,SIGNAL(stateChanged(int)), this, SLOT(ShowYCPropertyCheckBoxStateChangedSlot(int)));
    connect(ui->m_AnaTableWidget, SIGNAL(cellChanged( int, int )), this, SLOT(AnaTableWidgetCellChangedSlot( int, int )) );
    connect(ui->m_AnaTableWidget, SIGNAL(cellDoubleClicked(int,int)),this,SLOT(AnaTableWidgetCellDoubleClickedSlot(int,int)));

    connect(ui->m_AnaSelectComboBox,SIGNAL(currentIndexChanged(int)), this, SLOT(AnaSelectComboBoxCurrentIndexChangedSlot(int)));
    connect(ui->m_AnaPropertyComboBox,SIGNAL(currentIndexChanged(int)), this, SLOT(AnaPropertyComboBoxCurrentIndexChangedSlot(int)));
    connect(ui->m_AnaInfoLineEdit,SIGNAL(textChanged(const QString &)), this, SLOT(AnaInfoLineEditTextChanged(const QString &)));

    connect(ui->m_AnaTableWidget, SIGNAL(currentItemChanged( QTableWidgetItem *, QTableWidgetItem * )), this, SLOT(AnaTableWidgetCurrentItemChangedSlot( QTableWidgetItem *, QTableWidgetItem * )) );

    connect(ui->m_OtherSelectComboBox,SIGNAL(currentIndexChanged(int)), this, SLOT(AnaOtherComboBoxCurrentIndexChangedSlot(int)));
    connect(ui->m_OtherInfoLineEdit,SIGNAL(textChanged(const QString &)), this, SLOT(OtherInfoLineEditTextChanged(const QString &)));

    connect(ui->m_AnaCnnPushButton,SIGNAL(clicked(bool)), this, SLOT(AnaCnnPushButtonClicked(bool)));
    connect(ui->m_AnaDisPushButton,SIGNAL(clicked(bool)), this, SLOT(AnaDisPushButtonClicked(bool)));
}

CAnaWnd::~CAnaWnd()
{
    delete ui;
}

void CAnaWnd::ClearTableModel()
{
    m_pGseConfig = NULL;
    ui->m_AnaTableWidget->clearContents();
    ui->m_AnaInfoLineEdit->clear();
    ui->m_OtherInfoLineEdit->clear();
    ui->m_AnaCnnTableWidget->clearContents();
    ui->m_AnaOtherTableWidget->clearContents();
}

void CAnaWnd::CreateRowOfAnaTableWidget(AnaConfig *pNode, int row)
{
    QTableWidget * pTableWidget = ui->m_AnaTableWidget;
    QTableWidgetItem * pTableWidgetItem = NULL;

    pTableWidgetItem = new QTableWidgetItem(pNode->sName);
    SetNodeDataPoint(pTableWidgetItem,pNode);
    pTableWidget->setItem(row,Enum_AnaTable_Name_Col,pTableWidgetItem);

    pTableWidgetItem = new QTableWidgetItem(pNode->sDesc);
    SetNodeDataPoint(pTableWidgetItem,pNode);
    pTableWidget->setItem(row,Enum_AnaTable_Desc_Col,pTableWidgetItem);

//        CComboBox * pTypeComboBox = new CComboBox(pTableWidget,Enum_AnaChanTypeComboBox_Type,pNode);
//        pTypeComboBox->setCurrentIndex(pNode->wChanType);
//        pTableWidget->setCellWidget(i,Enum_Type_Col,pTypeComboBox);

    pTableWidgetItem = new QTableWidgetItem(CDataFun::GetInstance()->GetQStringfromAnaType(pNode->wChanType));
    SetNodeDataPoint(pTableWidgetItem,pNode);
    pTableWidgetItem->setFlags(pTableWidgetItem->flags()&(~Qt::ItemIsEditable));
    pTableWidget->setItem(row,Enum_AnaTable_Type_Col,pTableWidgetItem);

    pTableWidgetItem = new QTableWidgetItem(CDataFun::GetInstance()->ChangeDECToHEX(pNode->dwAttr));
    SetNodeDataPoint(pTableWidgetItem,pNode);
    pTableWidgetItem->setFlags(pTableWidgetItem->flags()&(~Qt::ItemIsEditable));
    pTableWidget->setItem(row,Enum_AnaTable_Attr_Col,pTableWidgetItem);

    pTableWidgetItem = new QTableWidgetItem(CDataFun::GetInstance()->ChangeDecToDec(pNode->wChan));
    if(!CheckNodeIsADChan(pNode))
    {
        pTableWidgetItem->setFlags(pTableWidgetItem->flags()&(~Qt::ItemIsEditable));
    }
    SetNodeDataPoint(pTableWidgetItem,pNode);
    pTableWidget->setItem(row,Enum_AnaTable_Chan_Col,pTableWidgetItem);

    pTableWidgetItem = new QTableWidgetItem(QString::number(pNode->wCoe));
    SetNodeDataPoint(pTableWidgetItem,pNode);
    pTableWidget->setItem(row,Enum_AnaTable_Coe_Col,pTableWidgetItem);

    pTableWidgetItem = new QTableWidgetItem(CDataFun::GetInstance()->GetQStringfromAnaPhase(pNode->wPhaseName));
    SetNodeDataPoint(pTableWidgetItem,pNode);
    pTableWidgetItem->setFlags(pTableWidgetItem->flags()&(~Qt::ItemIsEditable));
    pTableWidget->setItem(row,Enum_AnaTable_Phase_Col,pTableWidgetItem);

    pTableWidgetItem = new QTableWidgetItem(QString::number(pNode->byWidth));
    SetNodeDataPoint(pTableWidgetItem,pNode);
    pTableWidget->setItem(row,Enum_AnaTable_Width_Col,pTableWidgetItem);

    pTableWidgetItem = new QTableWidgetItem(QString::number(pNode->byDotBit));
    SetNodeDataPoint(pTableWidgetItem,pNode);
    pTableWidget->setItem(row,Enum_AnaTable_Dot_Col,pTableWidgetItem);

    pTableWidgetItem = new QTableWidgetItem(CDataFun::GetInstance()->ChangeDecToDec(pNode->dwCapacity));
    SetNodeDataPoint(pTableWidgetItem,pNode);
    pTableWidget->setItem(row,Enum_AnaTable_Capacity_Col,pTableWidgetItem);

    pTableWidgetItem = new QTableWidgetItem(CDataFun::GetInstance()->ChangeDecToDec(pNode->dwXuYcDft));
    SetNodeDataPoint(pTableWidgetItem,pNode);
    pTableWidget->setItem(row,Enum_AnaTable_XuYCDft_Col,pTableWidgetItem);

    pTableWidgetItem = new QTableWidgetItem(pNode->sUnit);
    SetNodeDataPoint(pTableWidgetItem,pNode);
    pTableWidget->setItem(row,Enum_AnaTable_Unit_Col,pTableWidgetItem);

    pTableWidgetItem = new QTableWidgetItem(pNode->sKiloUnit);
    SetNodeDataPoint(pTableWidgetItem,pNode);
    pTableWidget->setItem(row,Enum_AnaTable_KUnit_Col,pTableWidgetItem);
}

void CAnaWnd::RefreshCnnAnaName(QString oldName, QString newName)
{
    for (int i = 0 ; i < m_pGseConfig->gsBiConfig.size(); i ++)
    {
        GooseBiConfig * p = m_pGseConfig->gsBiConfig.at(i);

        if(p->dwAttr < 0x4)
            continue;

        if(p->sHW == oldName)
            p->sHW = newName;

        if(p->sFW == oldName)
            p->sFW = newName;
    }

    for (int i = 0 ; i < m_pGseConfig->gsBoConfig.size(); i ++)
    {
        GooseBoConfig * p = m_pGseConfig->gsBoConfig.at(i);
        if(p->wSrcTab != 3)
            continue;

        if(p->sHW == oldName)
            p->sHW = newName;

        if(p->sFW == oldName)
            p->sFW = newName;
    }
}

void CAnaWnd::ShowAnaInfo(CDataBase *pCDataBase)
{
    m_pGseConfig = pCDataBase->GetConfig();

    QTableWidget * pTableWidget = ui->m_AnaTableWidget;

    pTableWidget->clearContents();

    pTableWidget->setRowCount(m_pGseConfig->anaConfig.size());
    disconnect(pTableWidget, SIGNAL(cellChanged( int, int )), 0, 0);

    for (int i = 0; i < m_pGseConfig->anaConfig.size() ; i++)
    {
        AnaConfig * pNode = m_pGseConfig->anaConfig[i];
        CreateRowOfAnaTableWidget(pNode,i);
    }

    pTableWidget->resizeColumnsToContents();


    //列隐藏
    ShowYCPropertyCheckBoxStateChangedSlot(ui->m_ShowYCPropertyCheckBox->checkState());
    //行隐藏，筛选、类型和字段
    HideSomeRowOfAnaTableWidget();

    connect( pTableWidget, SIGNAL(cellChanged( int, int )), this, SLOT(AnaTableWidgetCellChangedSlot( int, int )) );

    ShowOtherTableWidget();
}

void CAnaWnd::AddOper()
{
    QTableWidget * pTable = ui->m_AnaTableWidget;
    QTableWidgetItem * pItem = NULL;
    int row = 0;
    pItem = pTable->currentItem();
    if(pItem != NULL)
        row = pItem->row() + 1;

    QString newName = InputNewName("NewAna_" + QString::number(m_pGseConfig->anaConfig.size()));

    if(newName == "NULL")
        return;

    AnaConfig * pNewNode = new AnaConfig();

    pNewNode->sName         = newName;

    pNewNode->sDesc         = newName;
    pNewNode->sUnit         = "N";
    pNewNode->sKiloUnit     = "N";
    pNewNode->sAddr         = "";

    pNewNode->wIndex        = row;
    pNewNode->dwAttr        = 0;
    pNewNode->dwXuYcDft     = 0xFFFF;
    pNewNode->wCoe          = 1;
    pNewNode->wChan         = 0xFFFF;
    pNewNode->wBoardChan    = 0xFFFF;
    pNewNode->dwRFVIndex    = 0xFFFF;
    pNewNode->dwSecIn       = 0xFFFF;
    pNewNode->dwMuSoft      = 0xFFFF;
    pNewNode->dwMuHard      = 0xFFFF;
    pNewNode->wChanType     = 0;
    pNewNode->dwCapacity    = 0xFFFF;
    pNewNode->byWidth       = 7;
    pNewNode->byDotBit      = 3;
    pNewNode->wPhaseName    = 0;

    pNewNode->sRFVName      = "NULL";
    pNewNode->sSecInName    = "NULL";
    pNewNode->sMuHardName   = "NULL";
    pNewNode->sMuSoftName   = "NULL";

    disconnect(pTable, SIGNAL(cellChanged( int, int )), 0, 0);
    m_pGseConfig->anaConfig.insert(row,pNewNode);
    pTable->insertRow(row);
    CreateRowOfAnaTableWidget(pNewNode,row);
    connect( pTable, SIGNAL(cellChanged( int, int )), this, SLOT(AnaTableWidgetCellChangedSlot( int, int )) );
}

void CAnaWnd::DeleteOper()
{
    QTableWidget * pTable = ui->m_AnaTableWidget;
    QTableWidgetItem * pItem = NULL;
    pItem = pTable->currentItem();
    if(pItem == NULL)
        return;

    int row = pItem->row();

    QString oldName = m_pGseConfig->anaConfig[row]->sName;

    pTable->removeRow(row);
    m_pGseConfig->anaConfig.remove(row);

    RefreshCnnAnaName(oldName, "NULL");
}

void CAnaWnd::CopyOper()
{
    QTableWidget * pTable = ui->m_AnaTableWidget;
    QTableWidgetItem * pItem = NULL;
    pItem = pTable->currentItem();
    if(pItem == NULL)
        return;

    AnaConfig * pNode = GetNodeDataPoint(pItem);
    QString newName = InputNewName(pNode->sName+"_Copy");

    if(newName == "NULL")
        return;

    int row = pItem->row();
    AnaConfig * pNewNode = new AnaConfig();

    *pNewNode = *pNode;

    pNewNode->sName         = newName;

//    pNewNode->sDesc         = pNode->sDesc;
//    pNewNode->sUnit         = pNode->sUnit;
//    pNewNode->sKiloUnit     = pNode->sKiloUnit;
//    pNewNode->sAddr         = pNode->sAddr;

//    pNewNode->wIndex        = pNode->wIndex;
//    pNewNode->dwAttr        = pNode->dwAttr;
//    pNewNode->dwXuYcDft     = pNode->dwXuYcDft;
//    pNewNode->wCoe          = pNode->wCoe;
//    pNewNode->wChan         = pNode->wChan;
//    pNewNode->wBoardChan    = pNode->wBoardChan;
//    pNewNode->dwRFVIndex    = pNode->dwRFVIndex;
//    pNewNode->dwSecIn       = pNode->dwSecIn;
//    pNewNode->dwMuSoft      = pNode->dwMuSoft;
//    pNewNode->dwMuHard      = pNode->dwMuHard;
//    pNewNode->wChanType     = pNode->wChanType;
//    pNewNode->dwCapacity    = pNode->dwCapacity;
//    pNewNode->byWidth       = pNode->byWidth;
//    pNewNode->byDotBit      = pNode->byDotBit;
//    pNewNode->wPhaseName    = pNode->wPhaseName;

//    pNewNode->sRFVName      = pNode->sRFVName;
//    pNewNode->sSecInName    = pNode->sSecInName;
//    pNewNode->sMuHardName   = pNode->sMuHardName;
//    pNewNode->sMuSoftName   = pNode->sMuSoftName;

    disconnect(pTable, SIGNAL(cellChanged( int, int )), 0, 0);
    m_pGseConfig->anaConfig.insert(row+1,pNewNode);
    pTable->insertRow(row+1);
    CreateRowOfAnaTableWidget(pNewNode,row+1);
    connect( pTable, SIGNAL(cellChanged( int, int )), this, SLOT(AnaTableWidgetCellChangedSlot( int, int )) );
}

void CAnaWnd::UpOper()
{
    QTableWidget * pTable = ui->m_AnaTableWidget;
    QTableWidgetItem * pItem = NULL;
    pItem = pTable->currentItem();
    if(pItem == NULL)
        return;

    int row = pItem->row();
    disconnect(pTable, SIGNAL(cellChanged( int, int )), 0, 0);

    if(row != 0)
    {
        moveRow(pTable,row,row-1);
        m_pGseConfig->anaConfig.swapItemsAt(row,row-1);
    }
    connect( pTable, SIGNAL(cellChanged( int, int )), this, SLOT(AnaTableWidgetCellChangedSlot( int, int )) );
}

void CAnaWnd::DownOper()
{
    QTableWidget * pTable = ui->m_AnaTableWidget;
    QTableWidgetItem * pItem = NULL;
    pItem = pTable->currentItem();
    if(pItem == NULL)
        return;

    int row = pItem->row();
    disconnect(pTable, SIGNAL(cellChanged( int, int )), 0, 0);

    if(row != pTable->rowCount()-1)
    {
        moveRow(pTable,row,row+2);
        m_pGseConfig->anaConfig.swapItemsAt(row,row+1);
    }
    connect( pTable, SIGNAL(cellChanged( int, int )), this, SLOT(AnaTableWidgetCellChangedSlot( int, int )) );
}

void CAnaWnd::AnaSelectComboBoxCurrentIndexChangedSlot(int index)
{
    Q_UNUSED(index);
    HideSomeRowOfAnaTableWidget();
}

void CAnaWnd::AnaPropertyComboBoxCurrentIndexChangedSlot(int index)
{
    Q_UNUSED(index);
    HideSomeRowOfAnaTableWidget();
}

void CAnaWnd::AnaInfoLineEditTextChanged(const QString &text)
{
    Q_UNUSED(text);
    HideSomeRowOfAnaTableWidget();
}

void CAnaWnd::ShowYCPropertyCheckBoxStateChangedSlot(int state)
{
    if(Qt::Unchecked == state)
    {
        ui->m_AnaTableWidget->hideColumn(6);
        ui->m_AnaTableWidget->hideColumn(7);
        ui->m_AnaTableWidget->hideColumn(8);
        ui->m_AnaTableWidget->hideColumn(9);
        ui->m_AnaTableWidget->hideColumn(10);
        ui->m_AnaTableWidget->hideColumn(11);
        ui->m_AnaTableWidget->hideColumn(12);
    }
    else
    {
        ui->m_AnaTableWidget->showColumn(6);
        ui->m_AnaTableWidget->showColumn(7);
        ui->m_AnaTableWidget->showColumn(8);
        ui->m_AnaTableWidget->showColumn(9);
        ui->m_AnaTableWidget->showColumn(10);
        ui->m_AnaTableWidget->showColumn(11);
        ui->m_AnaTableWidget->showColumn(12);
    }
}

void CAnaWnd::AnaTableWidgetCellChangedSlot(int row, int column)
{
    qDebug() << "CAnaWnd::AnaTableWidgetCellChangedSlot called row = " << row << ", column = " << column;
    disconnect(ui->m_AnaTableWidget,SIGNAL(cellChanged(int,int)),0,0);
    QTableWidgetItem * pItem = NULL;
    switch (column)
    {
    case Enum_AnaTable_Name_Col:
        pItem = ui->m_AnaTableWidget->item(row,column);
        ChangeName(pItem);
        break;
    case Enum_AnaTable_Desc_Col:
        pItem = ui->m_AnaTableWidget->item(row,column);
        ChangeDesc(pItem);
        break;
    case Enum_AnaTable_Attr_Col:
        pItem = ui->m_AnaTableWidget->item(row,column);
        ChangeAttr(pItem);
        break;
    case Enum_AnaTable_Chan_Col:
        pItem = ui->m_AnaTableWidget->item(row,column);
        ChangeChan(pItem);
        break;
    case Enum_AnaTable_Coe_Col:
        pItem = ui->m_AnaTableWidget->item(row,column);
        ChangeCoe(pItem);
        break;
    case Enum_AnaTable_Width_Col:
        pItem = ui->m_AnaTableWidget->item(row,column);
        ChangeWidth(pItem);
        break;
    case Enum_AnaTable_Dot_Col:
        pItem = ui->m_AnaTableWidget->item(row,column);
        ChangeDotbit(pItem);
        break;
    case Enum_AnaTable_Capacity_Col:
        pItem = ui->m_AnaTableWidget->item(row,column);
        ChangeCapacity(pItem);
        break;
    case Enum_AnaTable_XuYCDft_Col:
        pItem = ui->m_AnaTableWidget->item(row,column);
        ChangeXuYCDft(pItem);
        break;
    case Enum_AnaTable_Unit_Col:
        pItem = ui->m_AnaTableWidget->item(row,column);
        ChangeUnit(pItem);
        break;
    case Enum_AnaTable_KUnit_Col:
        pItem = ui->m_AnaTableWidget->item(row,column);
        ChangeKUnit(pItem);
        break;
    default:
        break;
    }
    connect( ui->m_AnaTableWidget, SIGNAL(cellChanged( int, int )), this, SLOT(AnaTableWidgetCellChangedSlot( int, int )) );
}

void CAnaWnd::AnaTableWidgetCellDoubleClickedSlot(int row, int column)
{
    if(column == Enum_AnaTable_Attr_Col)
    {
        QTableWidgetItem * pItem = ui->m_AnaTableWidget->item(row,column);
        CAnaAttrDialog * pDialog = new CAnaAttrDialog(ui->m_AnaTableWidget,pItem);
        pDialog->show();
     }
    else if(column == Enum_AnaTable_Type_Col)
    {
        QTableWidgetItem * pItem = ui->m_AnaTableWidget->item(row,column);
        CAnaTypeDialog * pDialog = new CAnaTypeDialog(ui->m_AnaTableWidget,pItem);
        pDialog->show();
     }
    else if(column == Enum_AnaTable_Phase_Col)
    {
        QTableWidgetItem * pItem = ui->m_AnaTableWidget->item(row,column);
        CAnaPhaseDialog * pDialog = new CAnaPhaseDialog(ui->m_AnaTableWidget,pItem);
        pDialog->show();
    }
}

void CAnaWnd::AnaTableWidgetCurrentItemChangedSlot(QTableWidgetItem *current, QTableWidgetItem *previous)
{
    //qDebug() << "AnaTableWidgetCurrentItemChangedSlot called row = " << current->row() << ", column = " << current->column();
    if(current == NULL)
    {
        ui->m_AnaCnnTableWidget->setRowCount(0);
        ui->m_AnaCnnTableWidget->clearContents();
        ui->m_CnnFrame->setVisible(true);
        return;
    }

    ui->m_CnnFrame->setVisible(true);

    if((previous != NULL)
           &&(current->row() == previous->row()) )
        return;

    AnaConfig * pNode = GetNodeDataPoint(current);
    QTableWidget * pTable = ui->m_AnaCnnTableWidget;
    QTableWidgetItem * pItem = NULL;

    pTable->clearContents();
    pTable->setRowCount(4);

    pItem = new QTableWidgetItem("关联一次额定值");
    SetNodeDataPoint(pItem,pNode);
    pItem->setFlags(pItem->flags()&(~Qt::ItemIsEditable));
    pTable->setItem(0,0,pItem);

    SetAnaCnnTable(0);

    pItem = new QTableWidgetItem("关联二次额定值");
    SetNodeDataPoint(pItem,pNode);
    pItem->setFlags(pItem->flags()&(~Qt::ItemIsEditable));
    pTable->setItem(1,0,pItem);

    SetAnaCnnTable(1);

    pItem = new QTableWidgetItem("关联接收软压板");
    SetNodeDataPoint(pItem,pNode);
    pItem->setFlags(pItem->flags()&(~Qt::ItemIsEditable));
    pTable->setItem(2,0,pItem);

    SetAnaCnnTable(2);

    pItem = new QTableWidgetItem("关联接收硬压板");
    SetNodeDataPoint(pItem,pNode);
    pItem->setFlags(pItem->flags()&(~Qt::ItemIsEditable));
    pTable->setItem(3,0,pItem);

    SetAnaCnnTable(3);

    pTable->resizeColumnsToContents();
}

void CAnaWnd::AnaOtherComboBoxCurrentIndexChangedSlot(int index)
{
    //qDebug()<< "AnaOtherComboBoxCurrentIndexChangedSlot called index = " << index;
    Q_UNUSED(index);
    ShowOtherTableWidget();
}

void CAnaWnd::OtherInfoLineEditTextChanged(const QString &text)
{
    //qDebug()<< "AnaOtherComboBoxCurrentIndexChangedSlot called text = " << text;
    Q_UNUSED(text);
    ShowOtherTableWidget();
}

void CAnaWnd::AnaCnnPushButtonClicked(bool checked)
{
    Q_UNUSED(checked);

    QTableWidgetItem * pItem = ui->m_AnaCnnTableWidget->currentItem();
    QTableWidgetItem * pCnnItem = ui->m_AnaOtherTableWidget->currentItem();

    if(pItem == NULL)
    {
        QMessageBox::critical(this,"关联错误","未选中被关联项!");
        return;
    }

    if(pCnnItem == NULL)
    {
        QMessageBox::critical(this,"关联错误","未选中关联项!");
        return;
    }

    AnaConfig * pNode = GetNodeDataPoint(pItem);
    QString qName = ui->m_AnaOtherTableWidget->item(pCnnItem->row(),0)->text();
    QString qDesc = ui->m_AnaOtherTableWidget->item(pCnnItem->row(),1)->text();

    if(pItem->row() == 0)
    {
        if(ui->m_OtherSelectComboBox->currentIndex() != 0)
        {
            QMessageBox::critical(this,"关联错误","需选中定值表中的条目!");
            return;
        }

        pNode->sRFVName = qName;
        ui->m_AnaCnnTableWidget->item(pItem->row(),2)->setText(qName);
        ui->m_AnaCnnTableWidget->item(pItem->row(),3)->setText(qDesc);
    }
    else if(pItem->row() == 1)
    {
        if(ui->m_OtherSelectComboBox->currentIndex() != 0)
        {
            QMessageBox::critical(this,"关联错误","需选中定值表中的条目!");
            return;
        }

        pNode->sSecInName = qName;
        ui->m_AnaCnnTableWidget->item(pItem->row(),2)->setText(qName);
        ui->m_AnaCnnTableWidget->item(pItem->row(),3)->setText(qDesc);
    }
    else if(pItem->row() == 2)
    {
        if(ui->m_OtherSelectComboBox->currentIndex() != 1)
        {
            QMessageBox::critical(this,"关联错误","需选中软压板表中的条目!");
            return;
        }

        pNode->sMuSoftName = qName;
        ui->m_AnaCnnTableWidget->item(pItem->row(),2)->setText(qName);
        ui->m_AnaCnnTableWidget->item(pItem->row(),3)->setText(qDesc);
    }
    else if(pItem->row() == 3)
    {
        if(ui->m_OtherSelectComboBox->currentIndex() != 2)
        {
            QMessageBox::critical(this,"关联错误","需选中开关量表中的条目!");
            return;
        }

        pNode->sMuHardName = qName;
        ui->m_AnaCnnTableWidget->item(pItem->row(),2)->setText(qName);
        ui->m_AnaCnnTableWidget->item(pItem->row(),3)->setText(qDesc);
    }
}

void CAnaWnd::AnaDisPushButtonClicked(bool checked)
{
    Q_UNUSED(checked);

    QTableWidgetItem * pItem = ui->m_AnaCnnTableWidget->currentItem();

    if(pItem == NULL)
    {
        QMessageBox::critical(this,"关联错误","未选中被关联项!");
        return;
    }

    AnaConfig * pNode = GetNodeDataPoint(pItem);

    if(pItem->row() == 0)
    {
        pNode->sRFVName = "NULL";
        ui->m_AnaCnnTableWidget->item(pItem->row(),2)->setText("NULL");
        ui->m_AnaCnnTableWidget->item(pItem->row(),3)->setText("无关联");
    }
    else if(pItem->row() == 1)
    {
        pNode->sSecInName = "NULL";
        ui->m_AnaCnnTableWidget->item(pItem->row(),2)->setText("NULL");
        ui->m_AnaCnnTableWidget->item(pItem->row(),3)->setText("无关联");
    }
    else if(pItem->row() == 2)
    {
        pNode->sMuSoftName = "NULL";
        ui->m_AnaCnnTableWidget->item(pItem->row(),2)->setText("NULL");
        ui->m_AnaCnnTableWidget->item(pItem->row(),3)->setText("无关联");
    }
    else if(pItem->row() == 3)
    {
        pNode->sMuHardName = "NULL";
        ui->m_AnaCnnTableWidget->item(pItem->row(),2)->setText("NULL");
        ui->m_AnaCnnTableWidget->item(pItem->row(),3)->setText("无关联");
    }
}

void CAnaWnd::SetNodeDataPoint(QTableWidgetItem *pItem, AnaConfig *pAna)
{
    if(NULL!=pItem)
    {
        pItem->setData( Qt::UserRole, QVariant((uint)pAna) );
    }
}

AnaConfig *CAnaWnd::GetNodeDataPoint(QTableWidgetItem *pItem)
{
    AnaConfig* pAna = NULL;

    if(NULL!=pItem)
    {
        pAna = (AnaConfig*)(pItem->data( Qt::UserRole ).toUInt());
    }

    return pAna;
}

void CAnaWnd::HideSomeRowOfAnaTableWidget()
{
    int seletedType = ui->m_AnaSelectComboBox->currentIndex();
    int propertyType = ui->m_AnaPropertyComboBox->currentIndex();
    QString keyStr = ui->m_AnaInfoLineEdit->text();

    QTableWidget * pTable = ui->m_AnaTableWidget;
    for (int i = 0; i < pTable->rowCount(); i++) {
        AnaConfig * pNode = GetNodeDataPoint(pTable->item(i,0));

        pTable->setRowHidden(i,true);

        if((keyStr == "")
                ||(pNode->sName.contains(keyStr)))      //关键字筛查
        {
            if((seletedType == 0)                       //显示所有采样通道
                    ||((seletedType == 1)&&((pNode->dwAttr & CN_AD_ANA_BIT) != 0))      //显示AD采样通道
                    ||((seletedType == 2)&&((pNode->dwAttr & CN_SV_ANA_BIT) != 0))      //显示SV采样通道
                    ||((seletedType == 3)&&((pNode->dwAttr & CN_CAL_ANA_BIT) != 0))      //显示衍生通道
                    ||((seletedType == 4)
                       &&((pNode->dwAttr & CN_AD_ANA_BIT) == 0)
                       &&((pNode->dwAttr & CN_SV_ANA_BIT) == 0)
                       &&((pNode->dwAttr & CN_CAL_ANA_BIT) == 0))                       //显示其他采样通道
                    )
            {
                if((propertyType == 0)                  //显示所有类型
                        ||((propertyType == 1)&&(pNode->wChanType == EN_ANA_TYP_U))     //显示电压类型
                        ||((propertyType == 2)&&(pNode->wChanType == EN_ANA_TYP_I))     //显示电流类型
                        ||((propertyType == 3)&&((pNode->wChanType == EN_ANA_TYP_P)     //显示功率类型
                                                 ||(pNode->wChanType == EN_ANA_TYP_P)
                                                 ||(pNode->wChanType == EN_ANA_TYP_P)
                                                 ||(pNode->wChanType == EN_ANA_TYP_P)))
                        ||((propertyType == 4)&&(pNode->wChanType == EN_ANA_TYP_FR))    //显示频率类型
                        ||((propertyType == 5)&&((pNode->wChanType != EN_ANA_TYP_P)     //显示其他类型
                                                 &&(pNode->wChanType != EN_ANA_TYP_P)
                                                 &&(pNode->wChanType != EN_ANA_TYP_P)
                                                 &&(pNode->wChanType != EN_ANA_TYP_P)
                                                 &&(pNode->wChanType != EN_ANA_TYP_I)
                                                 &&(pNode->wChanType != EN_ANA_TYP_U)
                                                 &&(pNode->wChanType != EN_ANA_TYP_FR)))
                    )
                {
                    pTable->setRowHidden(i,false);
                }
            }
        }
    }
}

void CAnaWnd::ChangeName(QTableWidgetItem * pItem)
{
    AnaConfig * p = GetNodeDataPoint(pItem);

    QString name = pItem->text();
    bool bFind = false;
    int indexFind = -1;

    for (int i = 0; i < m_pGseConfig->anaConfig.size(); i++)
    {
        if((p != m_pGseConfig->anaConfig[i])
            &&(name == m_pGseConfig->anaConfig[i]->sName))
        {
            bFind = true;
            indexFind = i;
            break;
        }
    }

    if(bFind)
    {
        QString strInfo = QString("修改失败，%1 存在重名，详见全部模拟量第%2行!").arg(name).arg(QString::number(indexFind+1));
        QMessageBox::critical(this,"修改模拟量名称",strInfo);
        pItem->setText(p->sName);
    }
    else
    {
        QString oldName = p->sName;
        p->sName = name;
        if(name.endsWith("IA")
                ||name.endsWith("IdA")
                ||name.endsWith("IrA")
                ||name.endsWith("IDA")
                ||name.endsWith("IRA")
                ||name.endsWith("Ia")
                ||name.endsWith("Ida")
                ||name.endsWith("Ira")
                ||name.endsWith("IDa")
                ||name.endsWith("IRa")
                ||name.endsWith("UA")
                ||name.endsWith("Ua"))
        {
            ui->m_AnaTableWidget->item(pItem->row(),Enum_AnaTable_Phase_Col)->setText("A");
            p->wPhaseName =1;
        }
        else if(name.endsWith("IB")
                ||name.endsWith("IdB")
                ||name.endsWith("IrB")
                ||name.endsWith("IDB")
                ||name.endsWith("IRB")
                ||name.endsWith("Ib")
                ||name.endsWith("Idb")
                ||name.endsWith("Irb")
                ||name.endsWith("IDb")
                ||name.endsWith("IRb")
                ||name.endsWith("UB")
                ||name.endsWith("Ub"))
        {
            ui->m_AnaTableWidget->item(pItem->row(),Enum_AnaTable_Phase_Col)->setText("B");
            p->wPhaseName =2;
        }
        else if(name.endsWith("IC")
                ||name.endsWith("IdC")
                ||name.endsWith("IrC")
                ||name.endsWith("IDC")
                ||name.endsWith("IRC")
                ||name.endsWith("Ic")
                ||name.endsWith("Idc")
                ||name.endsWith("Irc")
                ||name.endsWith("IDc")
                ||name.endsWith("IRc")
                ||name.endsWith("UC")
                ||name.endsWith("Uc"))
        {
            ui->m_AnaTableWidget->item(pItem->row(),Enum_AnaTable_Phase_Col)->setText("C");
            p->wPhaseName =3;
        }
        else
        {
            ui->m_AnaTableWidget->item(pItem->row(),Enum_AnaTable_Phase_Col)->setText("N");
            p->wPhaseName =0;
        }
        RefreshCnnAnaName(oldName,name);
    }


}

void CAnaWnd::ChangeDesc(QTableWidgetItem * pItem)
{
    AnaConfig * p = GetNodeDataPoint(pItem);
    p->sDesc = pItem->text();
}

void CAnaWnd::ChangeAttr(QTableWidgetItem *pItem)
{
    AnaConfig * pAna = GetNodeDataPoint(pItem);
    if(CheckNodeIsADChan(pAna))
    {
        QTableWidgetItem * pChanItem = ui->m_AnaTableWidget->item(pItem->row(),Enum_AnaTable_Chan_Col);
        pChanItem->setFlags(pChanItem->flags()|(Qt::ItemIsEditable));
    }
    else
    {
        QTableWidgetItem * pChanItem = ui->m_AnaTableWidget->item(pItem->row(),Enum_AnaTable_Chan_Col);
        pChanItem->setText("0xFFFF");
        pChanItem->setFlags(pChanItem->flags()&(~Qt::ItemIsEditable));
        pAna->wChan = 0xFFFF;
    }
}

void CAnaWnd::ChangeChan(QTableWidgetItem *pItem)
{
    AnaConfig * pAna = GetNodeDataPoint(pItem);

    if(pAna != NULL)
    {
        pAna->wChan = CDataFun::GetInstance()->ChangeQStringToUInt(pItem->text());
    }
}

void CAnaWnd::ChangeCoe(QTableWidgetItem *pItem)
{
    AnaConfig * pAna = GetNodeDataPoint(pItem);

    if(pAna != NULL)
    {
        pAna->wCoe = CDataFun::GetInstance()->ChangeQStringToUInt(pItem->text());
    }
}

void CAnaWnd::ChangeWidth(QTableWidgetItem *pItem)
{
    AnaConfig * pAna = GetNodeDataPoint(pItem);

    if(pAna != NULL)
    {
        pAna->byWidth = CDataFun::GetInstance()->ChangeQStringToUInt(pItem->text());
    }
}

void CAnaWnd::ChangeDotbit(QTableWidgetItem *pItem)
{
    AnaConfig * pAna = GetNodeDataPoint(pItem);

    if(pAna != NULL)
    {
        pAna->byDotBit = CDataFun::GetInstance()->ChangeQStringToUInt(pItem->text());
    }
}

void CAnaWnd::ChangeCapacity(QTableWidgetItem *pItem)
{
    AnaConfig * pAna = GetNodeDataPoint(pItem);

    if(pAna != NULL)
    {
        pAna->dwCapacity = CDataFun::GetInstance()->ChangeQStringToUInt(pItem->text());
    }
}

void CAnaWnd::ChangeXuYCDft(QTableWidgetItem *pItem)
{
    AnaConfig * pAna = GetNodeDataPoint(pItem);

    if(pAna != NULL)
    {
        pAna->dwXuYcDft = CDataFun::GetInstance()->ChangeQStringToUInt(pItem->text());
    }
}

void CAnaWnd::ChangeType(QTableWidgetItem *pItem)
{
    QStringList unitList;
    unitList << "N" << "A" << "V" << "V" << "W" << "var" << "VA" << "N" << "V" << "V" << "N" << "Hz"
                << "℃" << "N" << "N" << "N" << "N" << "Ω" << "V" << "A" << "A" << "A" << "Km" << "V";
    QStringList kUnitList;
    kUnitList << "N" << "KA" << "KV" << "KV" << "MW" << "Mvar" << "MVA" << "N" << "KV" << "KV" << "N" << "Hz"
                << "℃" << "N" << "N" << "N" << "N" << "Ω" << "KV" << "KA" << "KA" << "KA" << "Km" << "KV";

    AnaConfig * pAna = GetNodeDataPoint(pItem);

    pAna->sUnit = unitList[pAna->wChanType];
    pAna->sKiloUnit = kUnitList[pAna->wChanType];
}

void CAnaWnd::ChangeUnit(QTableWidgetItem *pItem)
{
    AnaConfig * p = GetNodeDataPoint(pItem);
    p->sUnit = pItem->text();
}

void CAnaWnd::ChangeKUnit(QTableWidgetItem *pItem)
{
    AnaConfig * p = GetNodeDataPoint(pItem);
    p->sKiloUnit = pItem->text();
}

bool CAnaWnd::CheckNodeIsADChan(AnaConfig *pAna)
{
    if(pAna == NULL)
        return false;

    if(pAna->dwAttr & CN_AD_ANA_BIT)
        return  true;
    else
        return  false;
}

bool CAnaWnd::CheckNodeIsSVChan(AnaConfig *pAna)
{
    if(pAna == NULL)
        return false;

    if(pAna->dwAttr & CN_SV_ANA_BIT)
        return  true;
    else
        return  false;
}

bool CAnaWnd::CheckNodeIsCalChan(AnaConfig *pAna)
{
    if(pAna == NULL)
        return false;

    if(pAna->dwAttr & CN_CAL_ANA_BIT)
        return  true;
    else
        return  false;
}

QString CAnaWnd::GetSettingDesc(QString name)
{
    if(name == "NULL")
        return "无关联";

    if(name == "ERROR")
        return "未找到,请重新关联";

    for (int i = 0; i < m_pGseConfig->settingConfig.size(); i++) {
        if(name == m_pGseConfig->settingConfig[i]->sName)
            return m_pGseConfig->settingConfig[i]->sDesc;
    }
    return "未找到,请重新关联";
}

QString CAnaWnd::GetSoftYBDesc(QString name)
{
    if(name == "NULL")
        return "无关联";

    if(name == "ERROR")
        return "未找到,请重新关联";

    for (int i = 0; i < m_pGseConfig->softYbConfig.size(); i++) {
        if(name == m_pGseConfig->softYbConfig[i]->sName)
            return m_pGseConfig->softYbConfig[i]->sDesc;
    }
    return "未找到,请重新关联";
}

QString CAnaWnd::GetBIDesc(QString name)
{
    if(name == "NULL")
        return "无关联";

    if(name == "ERROR")
        return "未找到,请重新关联";

    for (int i = 0; i < m_pGseConfig->biConfig.size(); i++) {
        if(name == m_pGseConfig->biConfig[i]->sName)
            return m_pGseConfig->biConfig[i]->sDesc;
    }
    return "未找到,请重新关联";
}

void CAnaWnd::SetAnaCnnTable(int row)
{
    QTableWidget * pTable = ui->m_AnaCnnTableWidget;
    QTableWidgetItem * pItem = pTable->item(row,0);
    AnaConfig * pNode = GetNodeDataPoint(pItem);

    if(row == 0)
    {
        pItem = new QTableWidgetItem("定值表");
        SetNodeDataPoint(pItem,pNode);
        pItem->setFlags(pItem->flags()&(~Qt::ItemIsEditable));
        pTable->setItem(row,1,pItem);

        QString cnnDesc = GetSettingDesc(pNode->sRFVName);

        if(cnnDesc == "未找到,请重新关联")
            pNode->sRFVName = "ERROR";

        pItem = new QTableWidgetItem(pNode->sRFVName);
        SetNodeDataPoint(pItem,pNode);
        pItem->setFlags(pItem->flags()&(~Qt::ItemIsEditable));
        pTable->setItem(row,2,pItem);

        pItem = new QTableWidgetItem(cnnDesc);
        SetNodeDataPoint(pItem,pNode);
        pItem->setFlags(pItem->flags()&(~Qt::ItemIsEditable));
        pTable->setItem(row,3,pItem);
    }
    else if(row == 1)
    {
        pItem = new QTableWidgetItem("定值表");
        SetNodeDataPoint(pItem,pNode);
        pItem->setFlags(pItem->flags()&(~Qt::ItemIsEditable));
        pTable->setItem(row,1,pItem);

        QString cnnDesc = GetSettingDesc(pNode->sSecInName);

        if(cnnDesc == "未找到,请重新关联")
            pNode->sSecInName = "ERROR";

        pItem = new QTableWidgetItem(pNode->sSecInName);
        SetNodeDataPoint(pItem,pNode);
        pItem->setFlags(pItem->flags()&(~Qt::ItemIsEditable));
        pTable->setItem(row,2,pItem);

        pItem = new QTableWidgetItem(cnnDesc);
        SetNodeDataPoint(pItem,pNode);
        pItem->setFlags(pItem->flags()&(~Qt::ItemIsEditable));
        pTable->setItem(row,3,pItem);
    }
    else if(row == 2)
    {
        pItem = new QTableWidgetItem("软压板表");
        SetNodeDataPoint(pItem,pNode);
        pItem->setFlags(pItem->flags()&(~Qt::ItemIsEditable));
        pTable->setItem(row,1,pItem);

        QString cnnDesc = GetSoftYBDesc(pNode->sMuSoftName);

        if(cnnDesc == "未找到,请重新关联")
            pNode->sRFVName = "ERROR";

        pItem = new QTableWidgetItem(pNode->sMuSoftName);
        SetNodeDataPoint(pItem,pNode);
        pItem->setFlags(pItem->flags()&(~Qt::ItemIsEditable));
        pTable->setItem(row,2,pItem);

        pItem = new QTableWidgetItem(cnnDesc);
        SetNodeDataPoint(pItem,pNode);
        pItem->setFlags(pItem->flags()&(~Qt::ItemIsEditable));
        pTable->setItem(row,3,pItem);
    }
    else if(row == 3)
    {
        pItem = new QTableWidgetItem("开关量表");
        SetNodeDataPoint(pItem,pNode);
        pItem->setFlags(pItem->flags()&(~Qt::ItemIsEditable));
        pTable->setItem(row,1,pItem);

        QString cnnDesc = GetBIDesc(pNode->sMuHardName);

        if(cnnDesc == "未找到,请重新关联")
            pNode->sRFVName = "ERROR";

        pItem = new QTableWidgetItem(pNode->sMuHardName);
        SetNodeDataPoint(pItem,pNode);
        pItem->setFlags(pItem->flags()&(~Qt::ItemIsEditable));
        pTable->setItem(row,2,pItem);

        pItem = new QTableWidgetItem(cnnDesc);
        SetNodeDataPoint(pItem,pNode);
        pItem->setFlags(pItem->flags()&(~Qt::ItemIsEditable));
        pTable->setItem(row,3,pItem);
    }
}

void CAnaWnd::ShowOtherTableWidget()
{
    qDebug()<< "ShowOtherTableWidget called index = " << ui->m_OtherSelectComboBox->currentIndex();
    if(0 == ui->m_OtherSelectComboBox->currentIndex())
        ShowOtherSettingTableWidget();
    else if(1 == ui->m_OtherSelectComboBox->currentIndex())
        ShowOtherSoftYBTableWidget();
    else if(2 == ui->m_OtherSelectComboBox->currentIndex())
        ShowOtherBITableWidget();

    HideOtherTableWidget();
}

void CAnaWnd::ShowOtherSettingTableWidget()
{
    QTableWidget * pTable = ui->m_AnaOtherTableWidget;
    QTableWidgetItem * pItem = NULL;
    pTable->clearContents();
    pTable->setRowCount(m_pGseConfig->settingConfig.size());

    for (int i = 0; i < m_pGseConfig->settingConfig.size(); i++)
    {
        SettingConfig * pNode = m_pGseConfig->settingConfig.at(i);
        pItem = new QTableWidgetItem(pNode->sName);
        pItem->setFlags(pItem->flags()&(~Qt::ItemIsEditable));
        pTable->setItem(i,0,pItem);

        pItem = new QTableWidgetItem(pNode->sDesc);
        pItem->setFlags(pItem->flags()&(~Qt::ItemIsEditable));
        pTable->setItem(i,1,pItem);
    }
    pTable->resizeColumnsToContents();
}

void CAnaWnd::ShowOtherSoftYBTableWidget()
{
    QTableWidget * pTable = ui->m_AnaOtherTableWidget;
    QTableWidgetItem * pItem = NULL;
    pTable->clearContents();
    pTable->setRowCount(m_pGseConfig->softYbConfig.size());

    for (int i = 0; i < m_pGseConfig->softYbConfig.size(); i++)
    {
        SoftYbConfig * pNode = m_pGseConfig->softYbConfig.at(i);
        pItem = new QTableWidgetItem(pNode->sName);
        pItem->setFlags(pItem->flags()&(~Qt::ItemIsEditable));
        pTable->setItem(i,0,pItem);

        pItem = new QTableWidgetItem(pNode->sDesc);
        pItem->setFlags(pItem->flags()&(~Qt::ItemIsEditable));
        pTable->setItem(i,1,pItem);
    }
    pTable->resizeColumnsToContents();
}

void CAnaWnd::ShowOtherBITableWidget()
{
    QTableWidget * pTable = ui->m_AnaOtherTableWidget;
    QTableWidgetItem * pItem = NULL;
    pTable->clearContents();
    pTable->setRowCount(m_pGseConfig->biConfig.size());

    for (int i = 0; i < m_pGseConfig->biConfig.size(); i++)
    {
        BiConfig * pNode = m_pGseConfig->biConfig.at(i);
        pItem = new QTableWidgetItem(pNode->sName);
        pItem->setFlags(pItem->flags()&(~Qt::ItemIsEditable));
        pTable->setItem(i,0,pItem);

        pItem = new QTableWidgetItem(pNode->sDesc);
        pItem->setFlags(pItem->flags()&(~Qt::ItemIsEditable));
        pTable->setItem(i,1,pItem);
    }
    pTable->resizeColumnsToContents();
}

void CAnaWnd::HideOtherTableWidget()
{
    QTableWidget * pTable = ui->m_AnaOtherTableWidget;
    QString key = ui->m_OtherInfoLineEdit->text();

    for (int i = 0 ; i < pTable->rowCount(); i++)
    {
        if((key == "")
                ||(pTable->item(i,0)->text().contains(key)))
            pTable->setRowHidden(i,false);
        else
            pTable->setRowHidden(i,true);
    }
}

void CAnaWnd::moveRow(QTableWidget *pTable, int fromRow, int toRow)
{
    if( pTable == NULL )
        return;

    pTable->setFocus();

    if( fromRow == toRow )
        return;

    if( fromRow < 0 || toRow < 0 )
        return;

    int nRowCount = pTable->rowCount();

    if( fromRow >= nRowCount  || toRow > nRowCount )
        return;

    if( toRow < fromRow )
        fromRow++;

    pTable->insertRow( toRow );
    int nCol = pTable->columnCount();

    for( int i=0; i<nCol; i++ )
    {
        pTable->setItem( toRow, i, pTable->takeItem( fromRow , i ) );
    }

    if( fromRow < toRow  )
        toRow--;

    pTable->removeRow( fromRow );
    pTable->selectRow( toRow );
}

QString CAnaWnd::InputNewName(QString old)
{
    bool ok;
    QString text = QInputDialog::getText(this, tr("名称修改"),tr("请输入不重复的通道名称"), QLineEdit::Normal,old, &ok);
    if (ok && !text.isEmpty())
    {
        bool bFindSame = false;
        for (int i = 0; i < m_pGseConfig->anaConfig.size(); i++)
        {
            if(text == m_pGseConfig->anaConfig[i]->sName)
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


