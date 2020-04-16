#include "CSoftYBWnd.h"
#include "CDataFun.h"
#include "CAttrDialog.h"
#include <QMessageBox>
#include <QInputDialog>
#include "ui_SoftYBWnd.h"

CSoftYBWnd::CSoftYBWnd(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CSoftYBWnd)
{
    ui->setupUi(this);

    m_pTable = ui->m_tableWidget;

    connect(m_pTable, SIGNAL(cellChanged( int, int )), this, SLOT(TableWidgetCellChangedSlot( int, int )) );
    connect(m_pTable, SIGNAL(cellDoubleClicked(int,int)),this,SLOT(TableWidgetCellDoubleClickedSlot(int,int)));
}

CSoftYBWnd::~CSoftYBWnd()
{
    delete ui;
}

void CSoftYBWnd::ClearTableModel()
{
    m_pTable->clearContents();
    //m_pTable = NULL;
    m_pGseConfig = NULL;
}

void CSoftYBWnd::ShowSoftYBInfo(CDataBase *pCDataBase)
{
    m_pGseConfig = pCDataBase->GetConfig();

    m_pTable->clearContents();
    m_pTable->setRowCount(m_pGseConfig->softYbConfig.size());

    disconnect(m_pTable, SIGNAL(cellChanged( int, int )), 0, 0);

    for (int i = 0; i < m_pGseConfig->softYbConfig.size() ; i++)
    {
        SoftYbConfig * pNode = m_pGseConfig->softYbConfig[i];
        CreateRowOfTable(pNode,i);
    }

    m_pTable->resizeColumnsToContents();

    connect( m_pTable, SIGNAL(cellChanged( int, int )), this, SLOT(TableWidgetCellChangedSlot( int, int )) );
}

void CSoftYBWnd::AddOper()
{
    QTableWidgetItem * pItem = NULL;
    int row = 0;
    pItem = m_pTable->currentItem();
    if(pItem != NULL)
        row = pItem->row() + 1;

    QString newName = InputNewName("NewSoftYB_" + QString::number(m_pGseConfig->softYbConfig.size()));

    if(newName == "NULL")
        return;

    SoftYbConfig * pNewNode = new SoftYbConfig();

    pNewNode->sName         = newName;
    pNewNode->sDesc         = newName;

    pNewNode->wIndex        = row;
    pNewNode->dwAttr        = 0x3;
    pNewNode->ValDft        = 0;

    disconnect(m_pTable, SIGNAL(cellChanged( int, int )), 0, 0);
    m_pGseConfig->softYbConfig.insert(row,pNewNode);
    m_pTable->insertRow(row);
    CreateRowOfTable(pNewNode,row);
    connect( m_pTable, SIGNAL(cellChanged( int, int )), this, SLOT(TableWidgetCellChangedSlot( int, int )) );
}

void CSoftYBWnd::DeleteOper()
{
    QTableWidgetItem * pItem = NULL;
    pItem = m_pTable->currentItem();
    if(pItem == NULL)
        return;

    int row = pItem->row();

    m_pTable->removeRow(row);
    m_pGseConfig->softYbConfig.remove(row);
}

void CSoftYBWnd::CopyOper()
{
    QTableWidgetItem * pItem = NULL;
    pItem = m_pTable->currentItem();
    if(pItem == NULL)
        return;

    SoftYbConfig * pNode = GetNodeDataPoint(pItem);
    QString newName = InputNewName(pNode->sName+"_Copy");

    if(newName == "NULL")
        return;

    int row = pItem->row();
    SoftYbConfig * pNewNode = new SoftYbConfig();

    *pNewNode = *pNode;

    pNewNode->sName  = newName;

    disconnect(m_pTable, SIGNAL(cellChanged( int, int )), 0, 0);
    m_pGseConfig->softYbConfig.insert(row+1,pNewNode);
    m_pTable->insertRow(row+1);
    CreateRowOfTable(pNewNode,row+1);
    connect( m_pTable, SIGNAL(cellChanged( int, int )), this, SLOT(TableWidgetCellChangedSlot( int, int )) );
}

void CSoftYBWnd::UpOper()
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
        m_pGseConfig->softYbConfig.swapItemsAt(row,row-1);
    }
    connect( m_pTable, SIGNAL(cellChanged( int, int )), this, SLOT(TableWidgetCellChangedSlot( int, int )) );
}

void CSoftYBWnd::DownOper()
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
        m_pGseConfig->softYbConfig.swapItemsAt(row,row+1);
    }
    connect( m_pTable, SIGNAL(cellChanged( int, int )), this, SLOT(TableWidgetCellChangedSlot( int, int )) );
}

void CSoftYBWnd::TableWidgetCellChangedSlot(int row, int column)
{
    disconnect(m_pTable,SIGNAL(cellChanged(int,int)),0,0);
    QTableWidgetItem * pItem = NULL;
    switch (column)
    {
    case Enum_SoftYBTable_Name_Col:
        pItem = m_pTable->item(row,column);
        ChangeName(pItem);
        break;
    case Enum_SoftYBTable_Attr_Col:
        pItem = m_pTable->item(row,column);
        ChangeAttr(pItem);
        break;
    case Enum_SoftYBTable_DftVal_Col:
        pItem = m_pTable->item(row,column);
        ChangeDftVal(pItem);
        break;
    case Enum_SoftYBTable_Desc_Col:
        pItem = m_pTable->item(row,column);
        ChangeDesc(pItem);
        break;
    default:
        break;
    }
    connect( m_pTable, SIGNAL(cellChanged( int, int )), this, SLOT(TableWidgetCellChangedSlot( int, int )) );
}

void CSoftYBWnd::TableWidgetCellDoubleClickedSlot(int row, int column)
{
    if(column == Enum_SoftYBTable_Attr_Col)
    {
        QTableWidgetItem * pItem = ui->m_tableWidget->item(row,column);
        CAttrDialog * pDialog = new CAttrDialog(ui->m_tableWidget,pItem);
        pDialog->SetCheckBoxText(0,"DB0:可见标志");
        pDialog->SetCheckBoxText(1,"DB1:信号上送标志");
        pDialog->SetCheckBoxText(2,"DB2:录波");
        pDialog->SetCheckBoxText(3,"DB3:软压板远方不可控");
        pDialog->SetCheckBoxText(4,"DB4:远方修改定值软压板");
        pDialog->SetCheckBoxText(5,"DB5:远方切换定值区软压板");
        pDialog->SetCheckBoxText(6,"DB6:远方控制压板");
        pDialog->SetCheckBox(CDataFun::GetInstance()->ChangeQStringToUInt(pItem->text()));
        pDialog->setWindowTitle("事件属性配置");
        pDialog->show();
    }
}

void CSoftYBWnd::SetNodeDataPoint(QTableWidgetItem *pItem, SoftYbConfig *pNode)
{
    if(NULL!=pItem)
    {
        pItem->setData( Qt::UserRole, QVariant((uint)pNode) );
    }
}

SoftYbConfig *CSoftYBWnd::GetNodeDataPoint(QTableWidgetItem *pItem)
{
    SoftYbConfig* pNode = NULL;

    if(NULL!=pItem)
    {
        pNode = (SoftYbConfig*)(pItem->data( Qt::UserRole ).toUInt());
    }

    return pNode;
}

void CSoftYBWnd::CreateRowOfTable(SoftYbConfig *pNode, int row)
{
    QTableWidgetItem * pTableWidgetItem = NULL;

    pTableWidgetItem = new QTableWidgetItem(pNode->sName);
    SetNodeDataPoint(pTableWidgetItem,pNode);
    m_pTable->setItem(row,Enum_SoftYBTable_Name_Col,pTableWidgetItem);

    pTableWidgetItem = new QTableWidgetItem(pNode->sDesc);
    SetNodeDataPoint(pTableWidgetItem,pNode);
    m_pTable->setItem(row,Enum_SoftYBTable_Desc_Col,pTableWidgetItem);

    pTableWidgetItem = new QTableWidgetItem(CDataFun::GetInstance()->ChangeDECToHEX(pNode->dwAttr));
    SetNodeDataPoint(pTableWidgetItem,pNode);
    pTableWidgetItem->setFlags(pTableWidgetItem->flags()&(~Qt::ItemIsEditable));
    m_pTable->setItem(row,Enum_SoftYBTable_Attr_Col,pTableWidgetItem);

    pTableWidgetItem = new QTableWidgetItem(QString::number(pNode->ValDft));
    SetNodeDataPoint(pTableWidgetItem,pNode);
    m_pTable->setItem(row,Enum_SoftYBTable_DftVal_Col,pTableWidgetItem);
}

void CSoftYBWnd::ChangeName(QTableWidgetItem *pItem)
{
    SoftYbConfig * p = GetNodeDataPoint(pItem);

    QString oldName = p->sName;
    QString name = pItem->text();
    bool bFind = false;
    int indexFind = -1;

    for (int i = 0; i < m_pGseConfig->softYbConfig.size(); i++)
    {
        if((p != m_pGseConfig->softYbConfig[i])
            &&(name == m_pGseConfig->softYbConfig[i]->sName))
        {
            bFind = true;
            indexFind = i;
            break;
        }
    }

    if(bFind)
    {
        QString strInfo = QString("修改失败，%1 存在重名，详见软压板表第%2行!").arg(name).arg(QString::number(indexFind+1));
        QMessageBox::critical(this,"修改软压板名称",strInfo);
        pItem->setText(p->sName);
    }
    else
    {
        QString oldName = p->sName;
        p->sName = name;
        RefreshCnnSoftYBName(oldName,name);
    }
}

void CSoftYBWnd::ChangeAttr(QTableWidgetItem *pItem)
{
    SoftYbConfig * p = GetNodeDataPoint(pItem);
    p->dwAttr = CDataFun::GetInstance()->ChangeQStringToUInt(pItem->text());
}

void CSoftYBWnd::ChangeDftVal(QTableWidgetItem *pItem)
{
    SoftYbConfig * p = GetNodeDataPoint(pItem);
    bool ok = false;
    int dftVal = pItem->text().toInt(&ok,10);
    if(ok&((dftVal == 0)||(dftVal == 1)))
    {
        p->ValDft = dftVal;
    }
    else
    {
        pItem->setText(QString::number(p->ValDft));
        QMessageBox::critical(this,"修改软压板缺省值","修改失败，请输入0，或者1!");
    }
}

void CSoftYBWnd::ChangeDesc(QTableWidgetItem *pItem)
{
    SoftYbConfig * p = GetNodeDataPoint(pItem);
    p->sDesc = pItem->text();
}

QString CSoftYBWnd::InputNewName(QString old)
{
    bool ok;
    QString text = QInputDialog::getText(this, tr("名称修改"),tr("请输入不重复的通道名称"), QLineEdit::Normal,old, &ok);
    if (ok && !text.isEmpty())
    {
        bool bFindSame = false;
        for (int i = 0; i < m_pGseConfig->softYbConfig.size(); i++)
        {
            if(text == m_pGseConfig->softYbConfig[i]->sName)
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

void CSoftYBWnd::RefreshCnnSoftYBName(QString oldName, QString newName)
{
    for (int i = 0 ; i < m_pGseConfig->anaConfig.size(); i ++)
    {
        AnaConfig * p = m_pGseConfig->anaConfig.at(i);
        if(p->sMuSoftName == oldName)
            p->sMuSoftName = newName;
    }
}
