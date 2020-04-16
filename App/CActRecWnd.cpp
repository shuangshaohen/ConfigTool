#include "CActRecWnd.h"
#include "CDataFun.h"
#include "CAttrDialog.h"
#include <QMessageBox>
#include <QInputDialog>
#include "ui_ActRecWnd.h"

CActRecWnd::CActRecWnd(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CActRecWnd)
{
    ui->setupUi(this);
    m_pTable = ui->m_tableWidget;

    connect(m_pTable, SIGNAL(cellChanged( int, int )), this, SLOT(TableWidgetCellChangedSlot( int, int )) );
    connect(m_pTable, SIGNAL(cellDoubleClicked(int,int)),this,SLOT(TableWidgetCellDoubleClickedSlot(int,int)));
}

CActRecWnd::~CActRecWnd()
{
    delete ui;
    m_pTable = NULL;
    m_pGseConfig = NULL;
}

void CActRecWnd::ClearTableModel()
{
    m_pTable->clearContents();
    //m_pTable = NULL;
    m_pGseConfig = NULL;
}

void CActRecWnd::ShowActInfo(CDataBase *pCDataBase)
{
    m_pGseConfig = pCDataBase->GetConfig();

    m_pTable->clearContents();
    m_pTable->setRowCount(m_pGseConfig->actConfig.size());

    disconnect(m_pTable, SIGNAL(cellChanged( int, int )), 0, 0);

    for (int i = 0; i < m_pGseConfig->actConfig.size() ; i++)
    {
        RecConfig * pNode = m_pGseConfig->actConfig[i];
        CreateRowOfTable(pNode,i);
    }

    m_pTable->resizeColumnsToContents();

    connect( m_pTable, SIGNAL(cellChanged( int, int )), this, SLOT(TableWidgetCellChangedSlot( int, int )) );
}

void CActRecWnd::AddOper()
{
    QTableWidgetItem * pItem = NULL;
    int row = 0;
    pItem = m_pTable->currentItem();
    if(pItem != NULL)
        row = pItem->row() + 1;

    QString newName = InputNewName("NewAct_" + QString::number(m_pGseConfig->actConfig.size()));

    if(newName == "NULL")
        return;

    RecConfig * pNewNode = new RecConfig();

    pNewNode->sName         = newName;
    pNewNode->sDesc         = newName;
    pNewNode->sAddr         = "ACT:BO1:" + QString::number(row);

    pNewNode->wIndex        = row;
    pNewNode->dwAttr        = 0xB;
    pNewNode->dwBOConfig    = 0;
    pNewNode->dwBOConfig2   = 0;
    pNewNode->dwBOSignal    = 0;
    pNewNode->dwLight       = 0;

    disconnect(m_pTable, SIGNAL(cellChanged( int, int )), 0, 0);
    m_pGseConfig->actConfig.insert(row,pNewNode);
    m_pTable->insertRow(row);
    CreateRowOfTable(pNewNode,row);
    connect( m_pTable, SIGNAL(cellChanged( int, int )), this, SLOT(TableWidgetCellChangedSlot( int, int )) );
}

void CActRecWnd::DeleteOper()
{
    QTableWidgetItem * pItem = NULL;
    pItem = m_pTable->currentItem();
    if(pItem == NULL)
        return;

    int row = pItem->row();

    m_pTable->removeRow(row);
    m_pGseConfig->actConfig.remove(row);
}

void CActRecWnd::CopyOper()
{
    QTableWidgetItem * pItem = NULL;
    pItem = m_pTable->currentItem();
    if(pItem == NULL)
        return;

    RecConfig * pNode = GetNodeDataPoint(pItem);
    QString newName = InputNewName(pNode->sName+"_Copy");

    if(newName == "NULL")
        return;

    int row = pItem->row();
    RecConfig * pNewNode = new RecConfig();

    *pNewNode = *pNode;

    pNewNode->sName  = newName;

    disconnect(m_pTable, SIGNAL(cellChanged( int, int )), 0, 0);
    m_pGseConfig->actConfig.insert(row+1,pNewNode);
    m_pTable->insertRow(row+1);
    CreateRowOfTable(pNewNode,row+1);
    connect( m_pTable, SIGNAL(cellChanged( int, int )), this, SLOT(TableWidgetCellChangedSlot( int, int )) );
}

void CActRecWnd::UpOper()
{
    QTableWidgetItem * pItem = NULL;
    pItem = m_pTable->currentItem();
    if(pItem == NULL)
        return;

    int row = pItem->row();
    disconnect(m_pTable, SIGNAL(cellChanged( int, int )), 0, 0);

    if(row != 0)
    {
        CDataFun::GetInstance()->moveRow(m_pTable,row,row-1);
        m_pGseConfig->actConfig.swapItemsAt(row,row-1);
    }
    connect( m_pTable, SIGNAL(cellChanged( int, int )), this, SLOT(TableWidgetCellChangedSlot( int, int )) );
}

void CActRecWnd::DownOper()
{
    QTableWidgetItem * pItem = NULL;
    pItem = m_pTable->currentItem();
    if(pItem == NULL)
        return;

    int row = pItem->row();
    disconnect(m_pTable, SIGNAL(cellChanged( int, int )), 0, 0);

    if(row != m_pTable->rowCount()-1)
    {
        CDataFun::GetInstance()->moveRow(m_pTable,row,row+2);
        m_pGseConfig->actConfig.swapItemsAt(row,row+1);
    }
    connect( m_pTable, SIGNAL(cellChanged( int, int )), this, SLOT(TableWidgetCellChangedSlot( int, int )) );
}

void CActRecWnd::TableWidgetCellChangedSlot(int row, int column)
{
    disconnect(m_pTable,SIGNAL(cellChanged(int,int)),0,0);
    QTableWidgetItem * pItem = NULL;
    switch (column)
    {
    case Enum_RecTable_Name_Col:
        pItem = m_pTable->item(row,column);
        ChangeName(pItem);
        break;
    case Enum_RecTable_Attr_Col:
        pItem = m_pTable->item(row,column);
        ChangeAttr(pItem);
        break;
    case Enum_RecTable_BO1_Col:
        pItem = m_pTable->item(row,column);
        ChangeBO1(pItem);
        break;
    case Enum_RecTable_BO2_Col:
        pItem = m_pTable->item(row,column);
        ChangeBO2(pItem);
        break;
    case Enum_RecTable_Sign_Col:
        pItem = m_pTable->item(row,column);
        ChangeSignal(pItem);
        break;
    case Enum_RecTable_Light_Col:
        pItem = m_pTable->item(row,column);
        ChangeLight(pItem);
        break;
    case Enum_RecTable_Desc_Col:
        pItem = m_pTable->item(row,column);
        ChangeDesc(pItem);
        break;
    default:
        break;
    }
    connect( m_pTable, SIGNAL(cellChanged( int, int )), this, SLOT(TableWidgetCellChangedSlot( int, int )) );
}

void CActRecWnd::TableWidgetCellDoubleClickedSlot(int row, int column)
{
    if(column == Enum_RecTable_Attr_Col)
    {
        QTableWidgetItem * pItem = ui->m_tableWidget->item(row,column);
        CAttrDialog * pDialog = new CAttrDialog(ui->m_tableWidget,pItem);
        pDialog->SetCheckBoxText(0,"DB0:可见标志");
        pDialog->SetCheckBoxText(1,"DB1:信号上送标志");
        pDialog->SetCheckBoxText(3,"DB3:不上送软遥信");
        pDialog->SetCheckBox(CDataFun::GetInstance()->ChangeQStringToUInt(pItem->text()));
        pDialog->setWindowTitle("事件属性配置");
        pDialog->show();
    }
    else if(column == Enum_RecTable_BO1_Col)
    {
        QTableWidgetItem * pItem = ui->m_tableWidget->item(row,column);
        CAttrDialog * pDialog = new CAttrDialog(ui->m_tableWidget,pItem);
        for (int i = 0; i < m_pGseConfig->tripMatrix.size(); i++)
        {
            if(i > 31)
                break;

            TripMatrixConfig * pTrip = m_pGseConfig->tripMatrix[i];
            QString str = "DB" + QString::number(i) + ":" + pTrip->sDesc;
            pDialog->SetCheckBoxText(i,str);
        }
        pDialog->SetCheckBox(CDataFun::GetInstance()->ChangeQStringToUInt(pItem->text()));
        pDialog->setWindowTitle("开出控制字1配置");
        pDialog->show();
    }
    else if(column == Enum_RecTable_BO2_Col)
    {
        QTableWidgetItem * pItem = ui->m_tableWidget->item(row,column);
        CAttrDialog * pDialog = new CAttrDialog(ui->m_tableWidget,pItem);
        for (int i = 32; i < m_pGseConfig->tripMatrix.size(); i++)
        {
            TripMatrixConfig * pTrip = m_pGseConfig->tripMatrix[i];
            QString str = "DB" + QString::number(i-32) + ":" + pTrip->sDesc;
            pDialog->SetCheckBoxText(i-32,str);
        }
        pDialog->SetCheckBox(CDataFun::GetInstance()->ChangeQStringToUInt(pItem->text()));
        pDialog->setWindowTitle("开出控制字2配置");
        pDialog->show();
    }
    else if(column == Enum_RecTable_Sign_Col)
    {
        QTableWidgetItem * pItem = ui->m_tableWidget->item(row,column);
        CAttrDialog * pDialog = new CAttrDialog(ui->m_tableWidget,pItem);
        for (int i = 0; i < 32; i++)
        {
            QString str = "DB" + QString::number(i) + ":中央信号出口";
            pDialog->SetCheckBoxText(i,str);
        }
        pDialog->SetCheckBox(CDataFun::GetInstance()->ChangeQStringToUInt(pItem->text()));
        pDialog->setWindowTitle("中央信号出口配置");
        pDialog->show();
    }
    else if(column == Enum_RecTable_Light_Col)
    {
        QTableWidgetItem * pItem = ui->m_tableWidget->item(row,column);
        CAttrDialog * pDialog = new CAttrDialog(ui->m_tableWidget,pItem);
        for (int i = 0; i < 32; i++)
        {
            QString str = "DB" + QString::number(i) + ":点灯信号配置";
            pDialog->SetCheckBoxText(i,str);
        }
        pDialog->SetCheckBox(CDataFun::GetInstance()->ChangeQStringToUInt(pItem->text()));
        pDialog->setWindowTitle("点灯信号配置");
        pDialog->show();
    }
}

void CActRecWnd::SetNodeDataPoint(QTableWidgetItem *pItem, RecConfig *pNode)
{
    if(NULL!=pItem)
    {
        pItem->setData( Qt::UserRole, QVariant((uint)pNode) );
    }
}

RecConfig *CActRecWnd::GetNodeDataPoint(QTableWidgetItem *pItem)
{
    RecConfig* pNode = NULL;

    if(NULL!=pItem)
    {
        pNode = (RecConfig*)(pItem->data( Qt::UserRole ).toUInt());
    }

    return pNode;
}

void CActRecWnd::CreateRowOfTable(RecConfig *pNode, int row)
{
    QTableWidgetItem * pTableWidgetItem = NULL;

    pTableWidgetItem = new QTableWidgetItem(pNode->sName);
    SetNodeDataPoint(pTableWidgetItem,pNode);
    m_pTable->setItem(row,Enum_RecTable_Name_Col,pTableWidgetItem);

    pTableWidgetItem = new QTableWidgetItem(pNode->sDesc);
    SetNodeDataPoint(pTableWidgetItem,pNode);
    m_pTable->setItem(row,Enum_RecTable_Desc_Col,pTableWidgetItem);

    pTableWidgetItem = new QTableWidgetItem(CDataFun::GetInstance()->ChangeDECToHEX(pNode->dwAttr));
    SetNodeDataPoint(pTableWidgetItem,pNode);
    pTableWidgetItem->setFlags(pTableWidgetItem->flags()&(~Qt::ItemIsEditable));
    m_pTable->setItem(row,Enum_RecTable_Attr_Col,pTableWidgetItem);

    pTableWidgetItem = new QTableWidgetItem(CDataFun::GetInstance()->ChangeDECToHEX(pNode->dwBOConfig));
    SetNodeDataPoint(pTableWidgetItem,pNode);
    pTableWidgetItem->setFlags(pTableWidgetItem->flags()&(~Qt::ItemIsEditable));
    m_pTable->setItem(row,Enum_RecTable_BO1_Col,pTableWidgetItem);

    pTableWidgetItem = new QTableWidgetItem(CDataFun::GetInstance()->ChangeDECToHEX(pNode->dwBOConfig2));
    SetNodeDataPoint(pTableWidgetItem,pNode);
    pTableWidgetItem->setFlags(pTableWidgetItem->flags()&(~Qt::ItemIsEditable));
    m_pTable->setItem(row,Enum_RecTable_BO2_Col,pTableWidgetItem);

    pTableWidgetItem = new QTableWidgetItem(CDataFun::GetInstance()->ChangeDECToHEX(pNode->dwBOSignal));
    SetNodeDataPoint(pTableWidgetItem,pNode);
    pTableWidgetItem->setFlags(pTableWidgetItem->flags()&(~Qt::ItemIsEditable));
    m_pTable->setItem(row,Enum_RecTable_Sign_Col,pTableWidgetItem);

    pTableWidgetItem = new QTableWidgetItem(CDataFun::GetInstance()->ChangeDECToHEX(pNode->dwLight));
    SetNodeDataPoint(pTableWidgetItem,pNode);
    pTableWidgetItem->setFlags(pTableWidgetItem->flags()&(~Qt::ItemIsEditable));
    m_pTable->setItem(row,Enum_RecTable_Light_Col,pTableWidgetItem);
}

void CActRecWnd::ChangeName(QTableWidgetItem *pItem)
{
    RecConfig * p = GetNodeDataPoint(pItem);

    QString oldName = p->sName;
    QString name = pItem->text();
    bool bFind = false;
    int indexFind = -1;

    for (int i = 0; i < m_pGseConfig->actConfig.size(); i++)
    {
        if((p != m_pGseConfig->actConfig[i])
            &&(name == m_pGseConfig->actConfig[i]->sName))
        {
            bFind = true;
            indexFind = i;
            break;
        }
    }

    if(bFind)
    {
        QString strInfo = QString("修改失败，%1 存在重名，详见动作表第%2行!").arg(name).arg(QString::number(indexFind+1));
        QMessageBox::critical(this,"修改动作名称",strInfo);
        pItem->setText(p->sName);
    }
    else
    {
        p->sName = name;
    }
}

void CActRecWnd::ChangeAttr(QTableWidgetItem *pItem)
{
    RecConfig * p = GetNodeDataPoint(pItem);
    p->dwAttr = CDataFun::GetInstance()->ChangeQStringToUInt(pItem->text());
}

void CActRecWnd::ChangeBO1(QTableWidgetItem *pItem)
{
    RecConfig * p = GetNodeDataPoint(pItem);
    p->dwBOConfig = CDataFun::GetInstance()->ChangeQStringToUInt(pItem->text());
}

void CActRecWnd::ChangeBO2(QTableWidgetItem *pItem)
{
    RecConfig * p = GetNodeDataPoint(pItem);
    p->dwBOConfig2 = CDataFun::GetInstance()->ChangeQStringToUInt(pItem->text());
}

void CActRecWnd::ChangeSignal(QTableWidgetItem *pItem)
{
    RecConfig * p = GetNodeDataPoint(pItem);
    p->dwBOSignal = CDataFun::GetInstance()->ChangeQStringToUInt(pItem->text());
}

void CActRecWnd::ChangeLight(QTableWidgetItem *pItem)
{
    RecConfig * p = GetNodeDataPoint(pItem);
    p->dwLight = CDataFun::GetInstance()->ChangeQStringToUInt(pItem->text());
}

void CActRecWnd::ChangeDesc(QTableWidgetItem *pItem)
{
    RecConfig * p = GetNodeDataPoint(pItem);
    p->sDesc = pItem->text();
}

QString CActRecWnd::InputNewName(QString old)
{
    bool ok;
    QString text = QInputDialog::getText(this, tr("名称修改"),tr("请输入不重复的通道名称"), QLineEdit::Normal,old, &ok);
    if (ok && !text.isEmpty())
    {
        bool bFindSame = false;
        for (int i = 0; i < m_pGseConfig->actConfig.size(); i++)
        {
            if(text == m_pGseConfig->actConfig[i]->sName)
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
