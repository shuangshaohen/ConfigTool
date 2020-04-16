#include "CBIWnd.h"
#include "CDataFun.h"
#include "CBIAttrDialog.h"
#include "CBITypeDialog.h"
#include "CBIIndexDPSDialog.h"
#include <QMessageBox>
#include <QInputDialog>
#include "ui_BIWnd.h"

CBIWnd::CBIWnd(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CBIWnd)
{
    ui->setupUi(this);

    connect(ui->m_BITableWidget, SIGNAL(cellChanged( int, int )), this, SLOT(BITableWidgetCellChangedSlot( int, int )) );
    connect(ui->m_BITableWidget, SIGNAL(cellDoubleClicked(int,int)),this,SLOT(BITableWidgetCellDoubleClickedSlot(int,int)));


    connect(ui->m_BISelectComboBox,SIGNAL(currentIndexChanged(int)), this, SLOT(BISelectComboBoxCurrentIndexChangedSlot(int)));
    connect(ui->m_BIPropertyComboBox,SIGNAL(currentIndexChanged(int)), this, SLOT(BIPropertyComboBoxCurrentIndexChangedSlot(int)));
    connect(ui->m_BIInfoLineEdit,SIGNAL(textChanged(const QString &)), this, SLOT(BIInfoLineEditTextChanged(const QString &)));

}

CBIWnd::~CBIWnd()
{
    delete ui;
}

void CBIWnd::ClearTableModel()
{
    m_pGseConfig = NULL;
    ui->m_BITableWidget->clearContents();
    ui->m_BIInfoLineEdit->clear();
}

void CBIWnd::ShowBIInfo(CDataBase *pCDataBase)
{
    m_pGseConfig = pCDataBase->GetConfig();

    QTableWidget * pTableWidget = ui->m_BITableWidget;

    pTableWidget->clearContents();
    pTableWidget->setRowCount(m_pGseConfig->biConfig.size());

    disconnect(pTableWidget, SIGNAL(cellChanged( int, int )), 0, 0);

    for (int i = 0; i < m_pGseConfig->biConfig.size() ; i++)
    {
        BiConfig * pNode = m_pGseConfig->biConfig[i];
        CreateRowOfBITableWidget(pNode,i);
    }

    pTableWidget->resizeColumnsToContents();

    //行隐藏，筛选、类型和字段
    HideSomeRowOfBITableWidget();

    connect( pTableWidget, SIGNAL(cellChanged( int, int )), this, SLOT(BITableWidgetCellChangedSlot( int, int )) );
}

void CBIWnd::AddOper()
{
    QTableWidget * pTable = ui->m_BITableWidget;
    QTableWidgetItem * pItem = NULL;
    int row = 0;
    pItem = pTable->currentItem();
    if(pItem != NULL)
        row = pItem->row() + 1;

    QString newName = InputNewName("NewBI_" + QString::number(m_pGseConfig->biConfig.size()));

    if(newName == "NULL")
        return;

    BiConfig * pNewNode = new BiConfig();

    pNewNode->sName         = newName;
    pNewNode->sEnName       = newName;
    pNewNode->sDesc         = newName;
    pNewNode->sAddr         = "";

    pNewNode->wIndex        = row;
    pNewNode->dwAttr        = 0;
    pNewNode->wChan         = 0xFFFF;
    pNewNode->wBoardChan    = 0xFFFF;
    pNewNode->wIndexDPS    = 0xFFFF;
    pNewNode->wHoldTime       = 1;
    pNewNode->wYXType      = 0xFF;

    pNewNode->sIndexDPS      = "NULL";

    disconnect(pTable, SIGNAL(cellChanged( int, int )), 0, 0);
    m_pGseConfig->biConfig.insert(row,pNewNode);
    pTable->insertRow(row);
    CreateRowOfBITableWidget(pNewNode,row);
    connect( pTable, SIGNAL(cellChanged( int, int )), this, SLOT(BITableWidgetCellChangedSlot( int, int )) );
}

void CBIWnd::DeleteOper()
{
    QTableWidget * pTable = ui->m_BITableWidget;
    QTableWidgetItem * pItem = NULL;
    pItem = pTable->currentItem();
    if(pItem == NULL)
        return;

    int row = pItem->row();

    QString oldName = m_pGseConfig->biConfig[row]->sName;

    pTable->removeRow(row);
    m_pGseConfig->biConfig.remove(row);

    RefreshCnnBIName(oldName,"NULL");
}

void CBIWnd::CopyOper()
{
    QTableWidget * pTable = ui->m_BITableWidget;
    QTableWidgetItem * pItem = NULL;
    pItem = pTable->currentItem();
    if(pItem == NULL)
        return;

    BiConfig * pNode = GetNodeDataPoint(pItem);
    QString newName = InputNewName(pNode->sName+"_Copy");

    if(newName == "NULL")
        return;

    int row = pItem->row();
    BiConfig * pNewNode = new BiConfig();

    *pNewNode = *pNode;

    pNewNode->sName         = newName;
    pNewNode->sEnName       = newName;

    disconnect(pTable, SIGNAL(cellChanged( int, int )), 0, 0);
    m_pGseConfig->biConfig.insert(row+1,pNewNode);
    pTable->insertRow(row+1);
    CreateRowOfBITableWidget(pNewNode,row+1);
    connect( pTable, SIGNAL(cellChanged( int, int )), this, SLOT(BITableWidgetCellChangedSlot( int, int )) );
}

void CBIWnd::UpOper()
{
    QTableWidget * pTable = ui->m_BITableWidget;
    QTableWidgetItem * pItem = NULL;
    pItem = pTable->currentItem();
    if(pItem == NULL)
        return;

    int row = pItem->row();
    disconnect(pTable, SIGNAL(cellChanged( int, int )), 0, 0);

    if(row != 0)
    {
        moveRow(pTable,row,row-1);
        m_pGseConfig->biConfig.swapItemsAt(row,row-1);
    }
    connect( pTable, SIGNAL(cellChanged( int, int )), this, SLOT(BITableWidgetCellChangedSlot( int, int )) );
}

void CBIWnd::DownOper()
{
    QTableWidget * pTable = ui->m_BITableWidget;
    QTableWidgetItem * pItem = NULL;
    pItem = pTable->currentItem();
    if(pItem == NULL)
        return;

    int row = pItem->row();
    disconnect(pTable, SIGNAL(cellChanged( int, int )), 0, 0);

    if(row != pTable->rowCount()-1)
    {
        moveRow(pTable,row,row+2);
        m_pGseConfig->biConfig.swapItemsAt(row,row+1);
    }
    connect( pTable, SIGNAL(cellChanged( int, int )), this, SLOT(BITableWidgetCellChangedSlot( int, int )) );
}

void CBIWnd::BITableWidgetCellChangedSlot(int row, int column)
{
    disconnect(ui->m_BITableWidget,SIGNAL(cellChanged(int,int)),0,0);
    QTableWidgetItem * pItem = NULL;
    switch (column)
    {
    case Enum_BITable_Name_Col:
        pItem = ui->m_BITableWidget->item(row,column);
        ChangeName(pItem);
        break;
    case Enum_BITable_Desc_Col:
        pItem = ui->m_BITableWidget->item(row,column);
        ChangeDesc(pItem);
        break;
    case Enum_BITable_Attr_Col:
        pItem = ui->m_BITableWidget->item(row,column);
        ChangeAttr(pItem);
        break;
    case Enum_BITable_Chan_Col:
        pItem = ui->m_BITableWidget->item(row,column);
        ChangeChan(pItem);
        break;
    case Enum_BITable_HoldTime_Col:
        pItem = ui->m_BITableWidget->item(row,column);
        ChangeHoldTime(pItem);
        break;
    case Enum_BITable_IndexDPS_Col:
        pItem = ui->m_BITableWidget->item(row,column);
        ChangeIndexDPS(pItem);
        break;
    case Enum_BITable_Type_Col:
        pItem = ui->m_BITableWidget->item(row,column);
        ChangeType(pItem);
        break;
    default:
        break;
    }
    connect( ui->m_BITableWidget, SIGNAL(cellChanged( int, int )), this, SLOT(BITableWidgetCellChangedSlot( int, int )) );
}

void CBIWnd::BITableWidgetCellDoubleClickedSlot(int row, int column)
{
    if(column == Enum_BITable_Attr_Col)
    {
        QTableWidgetItem * pItem = ui->m_BITableWidget->item(row,column);
        CBIAttrDialog * pDialog = new CBIAttrDialog(ui->m_BITableWidget,pItem);
        pDialog->show();
     }
    else if(column == Enum_BITable_IndexDPS_Col)
    {
        QTableWidgetItem * pItem = ui->m_BITableWidget->item(row,column);
        BiConfig * pNode = GetNodeDataPoint(pItem);
        if((pNode->dwAttr & CN_DOUBLE_BI_BIT) != 0)
        {
            CBIIndexDPSDialog * pDialog = new CBIIndexDPSDialog(ui->m_BITableWidget,pItem,m_pGseConfig);
            pDialog->show();
        }
     }
    else if(column == Enum_BITable_Type_Col)
    {
        QTableWidgetItem * pItem = ui->m_BITableWidget->item(row,column);
        CBITypeDialog * pDialog = new CBITypeDialog(ui->m_BITableWidget,pItem);
        pDialog->show();
    }
}

void CBIWnd::BISelectComboBoxCurrentIndexChangedSlot(int index)
{
    Q_UNUSED(index);
    HideSomeRowOfBITableWidget();
}

void CBIWnd::BIPropertyComboBoxCurrentIndexChangedSlot(int index)
{
    Q_UNUSED(index);
    HideSomeRowOfBITableWidget();
}

void CBIWnd::BIInfoLineEditTextChanged(const QString &text)
{
    Q_UNUSED(text);
    HideSomeRowOfBITableWidget();
}

void CBIWnd::HideSomeRowOfBITableWidget()
{
    int seletedType = ui->m_BISelectComboBox->currentIndex();
    int propertyType = ui->m_BIPropertyComboBox->currentIndex();
    QString keyStr = ui->m_BIInfoLineEdit->text();

    QTableWidget * pTable = ui->m_BITableWidget;
    for (int i = 0; i < pTable->rowCount(); i++) {
        BiConfig * pNode = GetNodeDataPoint(pTable->item(i,0));

        pTable->setRowHidden(i,true);

        if((keyStr == "")
                ||(pNode->sName.contains(keyStr)))      //关键字筛查
        {
            if((seletedType == 0)                       //显示所有采样通道
                    ||((seletedType == 1)&&(pNode->wYXType == 0xFF))                    //显示无类型
                    ||((seletedType == 2)&&(pNode->wYXType == 0))                       //显示A类遥信
                    ||((seletedType == 3)&&(pNode->wYXType == 1))                       //显示B类遥信
                    ||((seletedType == 4)&&(pNode->wYXType == 2))                       //显示B类遥信
                    )
            {
                if((propertyType == 0)                  //显示所有类型
                        ||((propertyType == 1)&&((pNode->dwAttr & CN_HARD_BI_BIT) != 0))     //单点硬开入
                        ||((propertyType == 2)&&((pNode->dwAttr & CN_SOFT_BI_BIT) != 0))     //单点软遥信
                        ||((propertyType == 3)&&((pNode->dwAttr & CN_DOUBLE_BI_BIT) != 0))     //双点遥信
                    )
                {
                    pTable->setRowHidden(i,false);
                }
            }
        }
    }
}

void CBIWnd::SetNodeDataPoint(QTableWidgetItem *pItem, BiConfig *pBI)
{
    if(NULL!=pItem)
    {
        pItem->setData( Qt::UserRole, QVariant((uint)pBI) );
    }
}

BiConfig *CBIWnd::GetNodeDataPoint(QTableWidgetItem *pItem)
{
    BiConfig* pBI = NULL;

    if(NULL!=pItem)
    {
        pBI = (BiConfig*)(pItem->data( Qt::UserRole ).toUInt());
    }

    return pBI;
}

bool CBIWnd::CheckNodeIsHardYX(BiConfig *pBI)
{
    if(pBI == NULL)
        return false;

    if(pBI->dwAttr & CN_HARD_BI_BIT)
        return  true;
    else
        return  false;
}

bool CBIWnd::CheckNodeIsSoftYX(BiConfig *pBI)
{
    if(pBI == NULL)
        return false;

    if(pBI->dwAttr & CN_SOFT_BI_BIT)
        return  true;
    else
        return  false;
}

bool CBIWnd::CheckNodeIsDoubleYX(BiConfig *pBI)
{
    if(pBI == NULL)
        return false;

    if(pBI->dwAttr & CN_DOUBLE_BI_BIT)
        return  true;
    else
        return  false;
}

bool CBIWnd::CheckNodeIsGOOSEYX(BiConfig *pBI)
{
    if(pBI == NULL)
        return false;

    if(pBI->dwAttr & CN_GOOSE_BI_BIT)
        return  true;
    else
        return  false;
}

void CBIWnd::ChangeName(QTableWidgetItem *pItem)
{
    BiConfig * p = GetNodeDataPoint(pItem);

    QString oldName = p->sName;
    QString name = pItem->text();
    bool bFind = false;
    int indexFind = -1;

    for (int i = 0; i < m_pGseConfig->biConfig.size(); i++)
    {
        if((p != m_pGseConfig->biConfig[i])
            &&(name == m_pGseConfig->biConfig[i]->sName))
        {
            bFind = true;
            indexFind = i;
            break;
        }
    }

    if(bFind)
    {
        QString strInfo = QString("修改失败，%1 存在重名，详见全部开关量第%2行!").arg(name).arg(QString::number(indexFind+1));
        QMessageBox::critical(this,"修改开关量名称",strInfo);
        pItem->setText(p->sName);
    }
    else
    {
        p->sName = name;
        p->sEnName = name;
        RefreshCnnBIName(oldName,name);
    }
}

void CBIWnd::ChangeDesc(QTableWidgetItem *pItem)
{
    BiConfig * p = GetNodeDataPoint(pItem);
    p->sDesc = pItem->text();
}

void CBIWnd::ChangeAttr(QTableWidgetItem *pItem)
{
    BiConfig * pBI = GetNodeDataPoint(pItem);
    if(CheckNodeIsHardYX(pBI))
    {
        QTableWidgetItem * pChanItem = ui->m_BITableWidget->item(pItem->row(),Enum_BITable_Chan_Col);
        pChanItem->setFlags(pChanItem->flags()|(Qt::ItemIsEditable));

        QTableWidgetItem * pHoldTimeItem = ui->m_BITableWidget->item(pItem->row(),Enum_BITable_HoldTime_Col);
        pHoldTimeItem->setFlags(pHoldTimeItem->flags()|(Qt::ItemIsEditable));
        pHoldTimeItem->setText("40");
        pBI->wHoldTime = 40;
    }
    else
    {
        QTableWidgetItem * pChanItem = ui->m_BITableWidget->item(pItem->row(),Enum_BITable_Chan_Col);
        pChanItem->setText("0xFFFF");
        pChanItem->setFlags(pChanItem->flags()&(~Qt::ItemIsEditable));
        pBI->wChan = 0xFFFF;

        QTableWidgetItem * pHoldTimeItem = ui->m_BITableWidget->item(pItem->row(),Enum_BITable_HoldTime_Col);
        pHoldTimeItem->setFlags(pHoldTimeItem->flags()&(~Qt::ItemIsEditable));
        if(CheckNodeIsGOOSEYX(pBI))
        {
            pHoldTimeItem->setText("0");
            pBI->wHoldTime = 0;
        }
        else
        {
            pHoldTimeItem->setText("1");
            pBI->wHoldTime = 1;
        }
    }

    if((pBI->dwAttr & CN_DOUBLE_BI_BIT)==0)
    {
        QTableWidgetItem * pIndexDPSItem = ui->m_BITableWidget->item(pItem->row(),Enum_BITable_IndexDPS_Col);
        pIndexDPSItem->setText("NULL");
        pBI->sIndexDPS = "NULL";
    }
}

void CBIWnd::ChangeChan(QTableWidgetItem *pItem)
{
    BiConfig * pBI = GetNodeDataPoint(pItem);

    if(pBI != NULL)
    {
        pBI->wChan = CDataFun::GetInstance()->ChangeQStringToUInt(pItem->text());
    }
}

void CBIWnd::ChangeHoldTime(QTableWidgetItem *pItem)
{
    BiConfig * pBI = GetNodeDataPoint(pItem);

    if(pBI != NULL)
    {
        pBI->wHoldTime = CDataFun::GetInstance()->ChangeQStringToUInt(pItem->text());
    }
}

void CBIWnd::ChangeIndexDPS(QTableWidgetItem *pItem)
{
    Q_UNUSED(pItem);
}

void CBIWnd::ChangeType(QTableWidgetItem *pItem)
{
    Q_UNUSED(pItem);
}

void CBIWnd::moveRow(QTableWidget *pTable, int fromRow, int toRow)
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

QString CBIWnd::InputNewName(QString old)
{
    bool ok;
    QString text = QInputDialog::getText(this, tr("名称修改"),tr("请输入不重复的通道名称"), QLineEdit::Normal,old, &ok);
    if (ok && !text.isEmpty())
    {
        bool bFindSame = false;
        for (int i = 0; i < m_pGseConfig->biConfig.size(); i++)
        {
            if(text == m_pGseConfig->biConfig[i]->sName)
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

void CBIWnd::CreateRowOfBITableWidget(BiConfig *pNode, int row)
{
    QTableWidget * pTableWidget = ui->m_BITableWidget;
    QTableWidgetItem * pTableWidgetItem = NULL;

    pTableWidgetItem = new QTableWidgetItem(pNode->sName);
    SetNodeDataPoint(pTableWidgetItem,pNode);
    pTableWidget->setItem(row,Enum_BITable_Name_Col,pTableWidgetItem);

    pTableWidgetItem = new QTableWidgetItem(pNode->sDesc);
    SetNodeDataPoint(pTableWidgetItem,pNode);
    pTableWidget->setItem(row,Enum_BITable_Desc_Col,pTableWidgetItem);

    pTableWidgetItem = new QTableWidgetItem(CDataFun::GetInstance()->ChangeDECToHEX(pNode->dwAttr));
    SetNodeDataPoint(pTableWidgetItem,pNode);
    pTableWidgetItem->setFlags(pTableWidgetItem->flags()&(~Qt::ItemIsEditable));
    pTableWidget->setItem(row,Enum_BITable_Attr_Col,pTableWidgetItem);

    pTableWidgetItem = new QTableWidgetItem(CDataFun::GetInstance()->ChangeDecToDec(pNode->wChan));
    if(!CheckNodeIsHardYX(pNode))
    {
        pTableWidgetItem->setFlags(pTableWidgetItem->flags()&(~Qt::ItemIsEditable));
    }
    SetNodeDataPoint(pTableWidgetItem,pNode);
    pTableWidget->setItem(row,Enum_BITable_Chan_Col,pTableWidgetItem);

    pTableWidgetItem = new QTableWidgetItem(QString::number(pNode->wHoldTime));
    if(CheckNodeIsHardYX(pNode))
    {
        pTableWidgetItem->setFlags(pTableWidgetItem->flags()|(Qt::ItemIsEditable));
    }
    else
    {
        pTableWidgetItem->setFlags(pTableWidgetItem->flags()&(~Qt::ItemIsEditable));
    }
    SetNodeDataPoint(pTableWidgetItem,pNode);
    pTableWidget->setItem(row,Enum_BITable_HoldTime_Col,pTableWidgetItem);

    pTableWidgetItem = new QTableWidgetItem(pNode->sIndexDPS);
    SetNodeDataPoint(pTableWidgetItem,pNode);
    pTableWidgetItem->setFlags(pTableWidgetItem->flags()&(~Qt::ItemIsEditable));
    pTableWidget->setItem(row,Enum_BITable_IndexDPS_Col,pTableWidgetItem);

    pTableWidgetItem = new QTableWidgetItem(CDataFun::GetInstance()->GetQStringfromBIType(pNode->wYXType));
    SetNodeDataPoint(pTableWidgetItem,pNode);
    pTableWidgetItem->setFlags(pTableWidgetItem->flags()&(~Qt::ItemIsEditable));
    pTableWidget->setItem(row,Enum_BITable_Type_Col,pTableWidgetItem);
}

void CBIWnd::RefreshCnnBIName(QString oldName, QString newName)
{
    for (int i = 0 ; i < m_pGseConfig->anaConfig.size(); i ++)
    {
        AnaConfig * p = m_pGseConfig->anaConfig.at(i);
        if(p->sMuHardName == oldName)
            p->sMuHardName = newName;
    }

    QTableWidget * pTable = ui->m_BITableWidget;
    for (int i = 0 ; i < pTable->rowCount(); i ++)
    {
        QTableWidgetItem * pItem = pTable->item(i,Enum_BITable_IndexDPS_Col);
        if(pItem->text() == oldName)
        {
            pItem->setText(newName);
            BiConfig * p = GetNodeDataPoint(pItem);
            p->sIndexDPS = newName;
        }
    }

    for (int i = 0 ; i < m_pGseConfig->gsBiConfig.size(); i ++)
    {
        GooseBiConfig * p = m_pGseConfig->gsBiConfig.at(i);

        if(p->dwAttr > 0x2)
            continue;

        if(p->sHW == oldName)
            p->sHW = newName;

        if(p->sFW == oldName)
            p->sFW = newName;
    }

    for (int i = 0 ; i < m_pGseConfig->gsBoConfig.size(); i ++)
    {
        GooseBoConfig * p = m_pGseConfig->gsBoConfig.at(i);
        if(p->wSrcTab != 2)
            continue;

        if(p->sHW == oldName)
            p->sHW = newName;

        if(p->sFW == oldName)
            p->sFW = newName;
    }

    for (int i = 0 ; i < m_pGseConfig->remoteTrip.size(); i ++)
    {
        RemoteTripConfig * p = m_pGseConfig->remoteTrip.at(i);
        if(p->sBI == oldName)
            p->sBI = newName;
    }
}
