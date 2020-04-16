#include "CTripMatrixWnd.h"
#include "CDataFun.h"
#include "CAttrDialog.h"
#include <QMessageBox>
#include <QInputDialog>
#include "ui_TripMatrixWnd.h"
#include <QDebug>

CTripMatrixWnd::CTripMatrixWnd(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CTripMatrixWnd)
{
    ui->setupUi(this);

    connect( ui->m_tableWidget, SIGNAL(cellChanged( int, int )), this, SLOT(TableWidgetCellChangedSlot( int, int )) );
    connect( ui->m_tableWidget, SIGNAL(cellDoubleClicked( int, int )), this, SLOT(TableWidgetCellDoubleClickedSlot( int, int )) );
}

CTripMatrixWnd::~CTripMatrixWnd()
{
    delete ui;
}

void CTripMatrixWnd::ClearTableModel()
{
    m_pGseConfig = NULL;
    ui->m_tableWidget->clearContents();
}

void CTripMatrixWnd::ShowTripMatrixInfo(CDataBase *pCDataBase)
{
    m_pGseConfig = pCDataBase->GetConfig();

    QTableWidget * pTableWidget = ui->m_tableWidget;

    pTableWidget->clearContents();
    pTableWidget->setRowCount(m_pGseConfig->tripMatrix.size());

    disconnect(pTableWidget, SIGNAL(cellChanged( int, int )), 0, 0);

    for (int i = 0; i < m_pGseConfig->tripMatrix.size() ; i++)
    {
        TripMatrixConfig * pNode = m_pGseConfig->tripMatrix[i];
        CreateRowOfTableWidget(pNode,i);
    }

    pTableWidget->resizeColumnsToContents();

    connect( pTableWidget, SIGNAL(cellChanged( int, int )), this, SLOT(TableWidgetCellChangedSlot( int, int )) );
}

void CTripMatrixWnd::AddOper()
{
    QTableWidget * pTable = ui->m_tableWidget;
    QTableWidgetItem * pItem = NULL;
    int row = 0;
    pItem = pTable->currentItem();
    if(pItem != NULL)
        row = pItem->row() + 1;

    QString newName = InputNewName("NewTripMatrix_" + QString::number(m_pGseConfig->tripMatrix.size()));

    if(newName == "NULL")
        return;

    TripMatrixConfig * pNewNode = new TripMatrixConfig();

    pNewNode->sName         = newName;
    pNewNode->sEnName       = newName;
    pNewNode->sDesc         = newName;

    pNewNode->wIndex        = row;
    pNewNode->dwAttr        = 0x3;
    pNewNode->Chan0         = 0;
    pNewNode->IndexDPS      = 0xFFFF;

    disconnect(pTable, SIGNAL(cellChanged( int, int )), 0, 0);
    m_pGseConfig->tripMatrix.insert(row,pNewNode);
    pTable->insertRow(row);
    CreateRowOfTableWidget(pNewNode,row);
    connect( pTable, SIGNAL(cellChanged( int, int )), this, SLOT(TableWidgetCellChangedSlot( int, int )) );
}

void CTripMatrixWnd::DeleteOper()
{
    QTableWidget * pTable = ui->m_tableWidget;
    QTableWidgetItem * pItem = NULL;
    pItem = pTable->currentItem();
    if(pItem == NULL)
        return;

    int row = pItem->row();

    pTable->removeRow(row);
    m_pGseConfig->tripMatrix.remove(row);
}

void CTripMatrixWnd::CopyOper()
{
    QTableWidget * pTable = ui->m_tableWidget;
    QTableWidgetItem * pItem = NULL;
    pItem = pTable->currentItem();
    if(pItem == NULL)
        return;

    TripMatrixConfig * pNode = GetNodeDataPoint(pItem);
    QString newName = InputNewName(pNode->sName+"_Copy");

    if(newName == "NULL")
        return;

    int row = pItem->row();
    TripMatrixConfig * pNewNode = new TripMatrixConfig();

    *pNewNode = *pNode;

    pNewNode->sName         = newName;
    pNewNode->sEnName       = newName;

    disconnect(pTable, SIGNAL(cellChanged( int, int )), 0, 0);
    m_pGseConfig->tripMatrix.insert(row+1,pNewNode);
    pTable->insertRow(row+1);
    CreateRowOfTableWidget(pNewNode,row+1);
    connect( pTable, SIGNAL(cellChanged( int, int )), this, SLOT(TableWidgetCellChangedSlot( int, int )) );
}

void CTripMatrixWnd::UpOper()
{
    QTableWidget * pTable = ui->m_tableWidget;
    QTableWidgetItem * pItem = NULL;
    pItem = pTable->currentItem();
    if(pItem == NULL)
        return;

    int row = pItem->row();
    disconnect(pTable, SIGNAL(cellChanged( int, int )), 0, 0);

    if(row != 0)
    {
        CDataFun::GetInstance()->moveRow(pTable,row,row-1);
        m_pGseConfig->tripMatrix.swapItemsAt(row,row-1);
    }
    connect( pTable, SIGNAL(cellChanged( int, int )), this, SLOT(TableWidgetCellChangedSlot( int, int )) );
}

void CTripMatrixWnd::DownOper()
{
    QTableWidget * pTable = ui->m_tableWidget;
    QTableWidgetItem * pItem = NULL;
    pItem = pTable->currentItem();
    if(pItem == NULL)
        return;

    int row = pItem->row();
    disconnect(pTable, SIGNAL(cellChanged( int, int )), 0, 0);

    if(row != pTable->rowCount()-1)
    {
        CDataFun::GetInstance()->moveRow(pTable,row,row+2);
        m_pGseConfig->tripMatrix.swapItemsAt(row,row+1);
    }
    connect( pTable, SIGNAL(cellChanged( int, int )), this, SLOT(TableWidgetCellChangedSlot( int, int )) );
}

void CTripMatrixWnd::TableWidgetCellChangedSlot(int row, int column)
{
    disconnect(ui->m_tableWidget,SIGNAL(cellChanged(int,int)),0,0);
    QTableWidgetItem * pItem = NULL;
    switch (column)
    {
    case Enum_TripMatrixTable_Name_Col:
        pItem = ui->m_tableWidget->item(row,column);
        ChangeName(pItem);
        break;
    case Enum_TripMatrixTable_Attr_Col:
        pItem = ui->m_tableWidget->item(row,column);
        ChangeAttr(pItem);
        break;
    case Enum_TripMatrixTable_BoardNum_Col:
        pItem = ui->m_tableWidget->item(row,column);
        ChangeBoardNum(pItem);
        break;
    case Enum_TripMatrixTable_ChanNum_Col:
        pItem = ui->m_tableWidget->item(row,column);
        ChangeChanNum(pItem);
        break;
    case Enum_TripMatrixTable_Desc_Col:
        pItem = ui->m_tableWidget->item(row,column);
        ChangeDesc(pItem);
        break;
    default:
        break;
    }
    connect( ui->m_tableWidget, SIGNAL(cellChanged( int, int )), this, SLOT(TableWidgetCellChangedSlot( int, int )) );
}

void CTripMatrixWnd::TableWidgetCellDoubleClickedSlot(int row, int column)
{
    if(column == Enum_TripMatrixTable_Attr_Col)
    {
        QTableWidgetItem * pItem = ui->m_tableWidget->item(row,column);
        CAttrDialog * pDialog = new CAttrDialog(ui->m_tableWidget,pItem);
        pDialog->SetCheckBoxText(0,"DB0:录波");
        pDialog->SetCheckBoxText(1,"DB1:跳闸控制字");
        pDialog->SetCheckBoxText(2,"DB2:信号");
        pDialog->SetCheckBoxText(3,"DB3:遥控");
        pDialog->SetCheckBoxText(4,"DB4:遥控闭锁接点");
        pDialog->SetCheckBoxText(5,"DB5:保护中间标志");
        pDialog->SetCheckBox(CDataFun::GetInstance()->ChangeQStringToUInt(pItem->text()));
        pDialog->show();
    }
    if(column == Enum_TripMatrixTable_ChanNum_Col)
    {
        QTableWidgetItem * pItem = ui->m_tableWidget->item(row,column);
        CAttrDialog * pDialog = new CAttrDialog(ui->m_tableWidget,pItem);
        for (int i = 0; i < CN_MAX_BOARD_BO_NUM ; i++)
        {
            pDialog->SetCheckBoxText(i,"DB" + QString::number(i) + ":BO" + QString::number(i+1));
        }
        pDialog->SetCheckBox(CDataFun::GetInstance()->ChangeQStringToUInt(pItem->text()));
        pDialog->show();
    }
}

void CTripMatrixWnd::SetNodeDataPoint(QTableWidgetItem *pItem, TripMatrixConfig *pNode)
{
    if(NULL!=pItem)
    {
        pItem->setData( Qt::UserRole, QVariant((uint)pNode) );
    }
}

TripMatrixConfig *CTripMatrixWnd::GetNodeDataPoint(QTableWidgetItem *pItem)
{
    TripMatrixConfig* pNode = NULL;

    if(NULL!=pItem)
    {
        pNode = (TripMatrixConfig*)(pItem->data( Qt::UserRole ).toUInt());
    }

    return pNode;
}

void CTripMatrixWnd::CreateRowOfTableWidget(TripMatrixConfig *pNode, int row)
{
    QTableWidget * pTableWidget = ui->m_tableWidget;
    QTableWidgetItem * pTableWidgetItem = NULL;

    pTableWidgetItem = new QTableWidgetItem(pNode->sName);
    SetNodeDataPoint(pTableWidgetItem,pNode);
    pTableWidget->setItem(row,Enum_TripMatrixTable_Name_Col,pTableWidgetItem);

    pTableWidgetItem = new QTableWidgetItem(pNode->sDesc);
    SetNodeDataPoint(pTableWidgetItem,pNode);
    pTableWidget->setItem(row,Enum_TripMatrixTable_Desc_Col,pTableWidgetItem);

    pTableWidgetItem = new QTableWidgetItem(CDataFun::GetInstance()->ChangeDECToHEX(pNode->dwAttr));
    SetNodeDataPoint(pTableWidgetItem,pNode);
    pTableWidgetItem->setFlags(pTableWidgetItem->flags()&(~Qt::ItemIsEditable));
    pTableWidget->setItem(row,Enum_TripMatrixTable_Attr_Col,pTableWidgetItem);

    unsigned int boardNum = pNode->Chan0 >> 28;
    unsigned int chanNum = pNode->Chan0 & 0x0FFFFFFF;
    pTableWidgetItem = new QTableWidgetItem(QString::number(boardNum));
    SetNodeDataPoint(pTableWidgetItem,pNode);
    pTableWidget->setItem(row,Enum_TripMatrixTable_BoardNum_Col,pTableWidgetItem);

    pTableWidgetItem = new QTableWidgetItem(CDataFun::GetInstance()->ChangeDECToHEX(chanNum));
    SetNodeDataPoint(pTableWidgetItem,pNode);
    pTableWidgetItem->setFlags(pTableWidgetItem->flags()&(~Qt::ItemIsEditable));
    pTableWidget->setItem(row,Enum_TripMatrixTable_ChanNum_Col,pTableWidgetItem);
}

void CTripMatrixWnd::ChangeName(QTableWidgetItem *pItem)
{
    TripMatrixConfig * p = GetNodeDataPoint(pItem);

    QString oldName = p->sName;
    QString name = pItem->text();
    bool bFind = false;
    int indexFind = -1;

    for (int i = 0; i < m_pGseConfig->tripMatrix.size(); i++)
    {
        if((p != m_pGseConfig->tripMatrix[i])
            &&(name == m_pGseConfig->tripMatrix[i]->sName))
        {
            bFind = true;
            indexFind = i;
            break;
        }
    }

    if(bFind)
    {
        QString strInfo = QString("修改失败，%1 存在重名，详见全部跳闸矩阵表第%2行!").arg(name).arg(QString::number(indexFind+1));
        QMessageBox::critical(this,"修改跳闸矩阵名称",strInfo);
        pItem->setText(p->sName);
    }
    else
    {
        p->sName = name;
        p->sEnName = name;
    }
}

void CTripMatrixWnd::ChangeAttr(QTableWidgetItem *pItem)
{
    TripMatrixConfig * p = GetNodeDataPoint(pItem);
    p->dwAttr = CDataFun::GetInstance()->ChangeQStringToUInt(pItem->text());
}

void CTripMatrixWnd::ChangeBoardNum(QTableWidgetItem *pItem)
{
    TripMatrixConfig * p = GetNodeDataPoint(pItem);
    unsigned int boadNum = CDataFun::GetInstance()->ChangeQStringToUInt(pItem->text());
    if(boadNum > 0xFF)
    {
        pItem->setText(QString::number(p->Chan0 >> 28));
    }
    else
    {
        QTableWidgetItem *pChanItem = ui->m_tableWidget->item(pItem->row(),Enum_TripMatrixTable_ChanNum_Col);
        unsigned int chanNum = CDataFun::GetInstance()->ChangeQStringToUInt(pChanItem->text());
        //qDebug()<<"before p->Chan0 = " << CDataFun::GetInstance()->ChangeDECToHEX(p->Chan0);
        p->Chan0 = (boadNum << 28) + (chanNum & 0x0FFFFFFF);
        //qDebug()<<"after p->Chan0 = " << CDataFun::GetInstance()->ChangeDECToHEX(p->Chan0);
    }
}

void CTripMatrixWnd::ChangeChanNum(QTableWidgetItem *pItem)
{
    TripMatrixConfig * p = GetNodeDataPoint(pItem);
    unsigned int chanNum = CDataFun::GetInstance()->ChangeQStringToUInt(pItem->text());
    if((chanNum == 0xFFFF)
            ||(chanNum > 0x0FFFFFFF))
    {
        pItem->setText(CDataFun::GetInstance()->ChangeDECToHEX(p->Chan0 & 0x0FFFFFFF));
    }
    else
    {
        QTableWidgetItem *pBoadItem = ui->m_tableWidget->item(pItem->row(),Enum_TripMatrixTable_BoardNum_Col);
        unsigned int boadNum = CDataFun::GetInstance()->ChangeQStringToUInt(pBoadItem->text());
        p->Chan0 = (boadNum << 28) + (chanNum & 0x0FFFFFFF);
        pItem->setText(CDataFun::GetInstance()->ChangeDECToHEX(chanNum));
    }

}

void CTripMatrixWnd::ChangeDesc(QTableWidgetItem *pItem)
{
    TripMatrixConfig * p = GetNodeDataPoint(pItem);
    p->sDesc = pItem->text();
}

QString CTripMatrixWnd::InputNewName(QString old)
{
    bool ok;
    QString text = QInputDialog::getText(this, tr("名称修改"),tr("请输入不重复的通道名称"), QLineEdit::Normal,old, &ok);
    if (ok && !text.isEmpty())
    {
        bool bFindSame = false;
        for (int i = 0; i < m_pGseConfig->tripMatrix.size(); i++)
        {
            if(text == m_pGseConfig->tripMatrix[i]->sName)
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

void CTripMatrixWnd::RefreshCnnTripMatrixName(QString oldName, QString newName)
{
    for (int i = 0 ; i < m_pGseConfig->gsBoConfig.size(); i ++)
    {
        GooseBoConfig * p = m_pGseConfig->gsBoConfig.at(i);
        if(p->wSrcTab != 1)
            continue;

        if(p->sHW == oldName)
            p->sHW = newName;

        if(p->sFW == oldName)
            p->sFW = newName;
    }

}
