#include "CGSBIWnd.h"
#include "CDataFun.h"
#include "CGSBIAttrDialog.h"
#include <QMessageBox>
#include <QInputDialog>
#include <QDebug>
#include "ui_GSBIWnd.h"

CGSBIWnd::CGSBIWnd(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CGSBIWnd)
{
    ui->setupUi(this);

    connect(ui->m_pGSBITableWidget, SIGNAL(cellChanged( int, int )), this, SLOT(GSBITableWidgetCellChangedSlot( int, int )) );
    connect(ui->m_pGSBITableWidget, SIGNAL(cellDoubleClicked(int,int)),this,SLOT(GSBITableWidgetCellDoubleClickedSlot(int,int)));
    connect(ui->m_pGSBITableWidget, SIGNAL(cellClicked(int,int)),this,SLOT(GSBITableWidgetCellClickedSlot(int,int)));

    connect(ui->m_pBIInfoComboBox,SIGNAL(currentIndexChanged(int)), this, SLOT(GSBIInfoComboBoxCurrentIndexChangedSlot(int)));
    connect(ui->m_pBIInfoLineEdit,SIGNAL(textChanged(const QString &)), this, SLOT(GSBIInfoLineEditTextChanged(const QString &)));

    connect(ui->m_pCnnInfoComboBox,SIGNAL(currentIndexChanged(int)), this, SLOT(CnnComboBoxCurrentIndexChangedSlot(int)));
    connect(ui->m_pCnnInfoLineEdit,SIGNAL(textChanged(const QString &)), this, SLOT(CnnInfoLineEditTextChanged(const QString &)));

    connect(ui->m_pCnnPushButton,SIGNAL(clicked(bool)), this, SLOT(CnnPushButtonClicked(bool)));
    connect(ui->m_pDisPushButton,SIGNAL(clicked(bool)), this, SLOT(DisPushButtonClicked(bool)));
}

CGSBIWnd::~CGSBIWnd()
{
    delete ui;
}

void CGSBIWnd::ClearTableModel()
{
    m_pGseConfig = NULL;
    ui->m_pGSBITableWidget->clearContents();
    ui->m_pCnnInfoTableWidget->clearContents();
    ui->m_pBIInfoLineEdit->clear();
    ui->m_pCnnInfoLineEdit->clear();
}

void CGSBIWnd::ShowGSBIInfo(CDataBase *pCDataBase)
{
    m_pGseConfig = pCDataBase->GetConfig();

    QTableWidget * pTableWidget = ui->m_pGSBITableWidget;

    pTableWidget->clearContents();
    pTableWidget->setRowCount(m_pGseConfig->gsBiConfig.size());

    disconnect(pTableWidget, SIGNAL(cellChanged( int, int )), 0, 0);

    for (int i = 0; i < m_pGseConfig->gsBiConfig.size() ; i++)
    {
        GooseBiConfig * pNode = m_pGseConfig->gsBiConfig[i];
        CreateRowOfGSBITableWidget(pNode,i);
    }

    pTableWidget->resizeColumnsToContents();

    //行隐藏，筛选、类型和字段
    HideSomeRowOfGSBITableWidget();

    connect( pTableWidget, SIGNAL(cellChanged( int, int )), this, SLOT(GSBITableWidgetCellChangedSlot( int, int )) );

    if(ui->m_pCnnInfoComboBox->currentIndex() == 0)
    {
        ShowCnnBITable();
    }
    else
    {
        ShowCnnAnaTable();
    }
    HideSomeRowOfCnnTable();

    ui->m_pCnnPushButton->setEnabled(false);
    ui->m_pDisPushButton->setEnabled(false);
    ui->m_pCnnPushButton->setText("<<");
    ui->m_pDisPushButton->setText("<<");
}

void CGSBIWnd::AddOper()
{
    QTableWidget * pTable = ui->m_pGSBITableWidget;
    QTableWidgetItem * pItem = NULL;
    int row = 0;
    pItem = pTable->currentItem();
    if(pItem != NULL)
        row = pItem->row() + 1;

    QString newName = InputNewName("NewGSBI_" + QString::number(m_pGseConfig->gsBiConfig.size()));

    if(newName == "NULL")
        return;

    GooseBiConfig * pNewNode = new GooseBiConfig();

    pNewNode->sName         = newName;
    pNewNode->sEnName       = newName;
    pNewNode->sAddr         = "";

    pNewNode->wIndex        = row;
    pNewNode->dwAttr        = 0x1;
    pNewNode->HWChan        = 0xFFFF;
    pNewNode->FWChan        = 0xFFFF;

    pNewNode->sHW           = "NULL";
    pNewNode->sFW           = "NULL";

    disconnect(pTable, SIGNAL(cellChanged( int, int )), 0, 0);
    m_pGseConfig->gsBiConfig.insert(row,pNewNode);
    pTable->insertRow(row);
    CreateRowOfGSBITableWidget(pNewNode,row);
    connect( pTable, SIGNAL(cellChanged( int, int )), this, SLOT(GSBITableWidgetCellChangedSlot( int, int )) );
}

void CGSBIWnd::DeleteOper()
{
    QTableWidget * pTable = ui->m_pGSBITableWidget;
    QTableWidgetItem * pItem = NULL;
    pItem = pTable->currentItem();
    if(pItem == NULL)
        return;

    int row = pItem->row();

    pTable->removeRow(row);
    m_pGseConfig->gsBiConfig.remove(row);
}

void CGSBIWnd::CopyOper()
{
    QTableWidget * pTable = ui->m_pGSBITableWidget;
    QTableWidgetItem * pItem = NULL;
    pItem = pTable->currentItem();
    if(pItem == NULL)
        return;

    GooseBiConfig * pNode = GetNodeDataPoint(pItem);
    QString newName = InputNewName(pNode->sName+"_Copy");

    if(newName == "NULL")
        return;

    int row = pItem->row();
    GooseBiConfig * pNewNode = new GooseBiConfig();

    *pNewNode = *pNode;

    pNewNode->sName         = newName;
    pNewNode->sEnName       = newName;

    disconnect(pTable, SIGNAL(cellChanged( int, int )), 0, 0);
    m_pGseConfig->gsBiConfig.insert(row+1,pNewNode);
    pTable->insertRow(row+1);
    CreateRowOfGSBITableWidget(pNewNode,row+1);
    connect( pTable, SIGNAL(cellChanged( int, int )), this, SLOT(GSBITableWidgetCellChangedSlot( int, int )) );
}

void CGSBIWnd::UpOper()
{
    QTableWidget * pTable = ui->m_pGSBITableWidget;
    QTableWidgetItem * pItem = NULL;
    pItem = pTable->currentItem();
    if(pItem == NULL)
        return;

    int row = pItem->row();
    disconnect(pTable, SIGNAL(cellChanged( int, int )), 0, 0);

    if(row != 0)
    {
        CDataFun::GetInstance()->moveRow(pTable,row,row-1);
        m_pGseConfig->gsBiConfig.swapItemsAt(row,row-1);
    }
    connect( pTable, SIGNAL(cellChanged( int, int )), this, SLOT(GSBITableWidgetCellChangedSlot( int, int )) );
}

void CGSBIWnd::DownOper()
{
    QTableWidget * pTable = ui->m_pGSBITableWidget;
    QTableWidgetItem * pItem = NULL;
    pItem = pTable->currentItem();
    if(pItem == NULL)
        return;

    int row = pItem->row();
    disconnect(pTable, SIGNAL(cellChanged( int, int )), 0, 0);

    if(row != pTable->rowCount()-1)
    {
        CDataFun::GetInstance()->moveRow(pTable,row,row+2);
        m_pGseConfig->gsBiConfig.swapItemsAt(row,row+1);
    }
    connect( pTable, SIGNAL(cellChanged( int, int )), this, SLOT(GSBITableWidgetCellChangedSlot( int, int )) );
}

void CGSBIWnd::GSBITableWidgetCellChangedSlot(int row, int column)
{
    disconnect(ui->m_pGSBITableWidget,SIGNAL(cellChanged(int,int)),0,0);
    QTableWidgetItem * pItem = NULL;
    switch (column)
    {
    case Enum_GSBITable_Name_Col:
        pItem = ui->m_pGSBITableWidget->item(row,column);
        ChangeName(pItem);
        break;
    case Enum_GSBITable_Attr_Col:
        pItem = ui->m_pGSBITableWidget->item(row,column);
        ChangeAttr(pItem);
        break;
    default:
        break;
    }
    connect( ui->m_pGSBITableWidget, SIGNAL(cellChanged( int, int )), this, SLOT(GSBITableWidgetCellChangedSlot( int, int )) );
}

void CGSBIWnd::GSBITableWidgetCellDoubleClickedSlot(int row, int column)
{
    if(column == Enum_GSBITable_Attr_Col)
    {
        QTableWidgetItem * pItem = ui->m_pGSBITableWidget->item(row,column);
        //GooseBiConfig *pNode = GetNodeDataPoint(pItem);
        //unsigned int oldAttr = pNode->dwAttr;
        CGSBIAttrDialog * pDialog = new CGSBIAttrDialog(ui->m_pGSBITableWidget,pItem);
        pDialog->show();
        //unsigned int newAttr = pNode->dwAttr;


//        if(((oldAttr < 0x4)&&(newAttr > 0x2))
//                ||((oldAttr > 0x2)&&(newAttr < 0x4)))
//        {
//            ui->m_pGSBITableWidget->item(row,Enum_GSBITable_HWChan_Col)->setText("NULL");
//            ui->m_pGSBITableWidget->item(row,Enum_GSBITable_FWChan_Col)->setText("NULL");
//            pNode->sHW = "NULL";
//            pNode->sFW = "NULL";
//        }
//        else if(newAttr != 0x2)
//        {
//            ui->m_pGSBITableWidget->item(row,Enum_GSBITable_FWChan_Col)->setText("NULL");
//            pNode->sFW = "NULL";
//        }

    }
}

void CGSBIWnd::GSBITableWidgetCellClickedSlot(int row, int column)
{
    Q_UNUSED(row);
    if(column == Enum_GSBITable_HWChan_Col)
    {
        ui->m_pCnnPushButton->setEnabled(true);
        ui->m_pDisPushButton->setEnabled(true);
        ui->m_pCnnPushButton->setText("合位绑定");
        ui->m_pDisPushButton->setText("解除绑定");
    }
    else if(column == Enum_GSBITable_FWChan_Col)
    {
        ui->m_pCnnPushButton->setEnabled(true);
        ui->m_pDisPushButton->setEnabled(true);
        ui->m_pCnnPushButton->setText("分位绑定");
        ui->m_pDisPushButton->setText("解除绑定");
    }
    else
    {
        ui->m_pCnnPushButton->setEnabled(false);
        ui->m_pDisPushButton->setEnabled(false);
        ui->m_pCnnPushButton->setText("<<");
        ui->m_pDisPushButton->setText("<<");
    }
}

void CGSBIWnd::GSBIInfoComboBoxCurrentIndexChangedSlot(int index)
{
    Q_UNUSED(index);
    HideSomeRowOfGSBITableWidget();
}

void CGSBIWnd::GSBIInfoLineEditTextChanged(const QString &text)
{
    Q_UNUSED(text);
    HideSomeRowOfGSBITableWidget();
}

void CGSBIWnd::CnnComboBoxCurrentIndexChangedSlot(int index)
{
    Q_UNUSED(index);
    if(ui->m_pCnnInfoComboBox->currentIndex() == 0)
    {
        ShowCnnBITable();
    }
    else
    {
        ShowCnnAnaTable();
    }
    HideSomeRowOfCnnTable();
}

void CGSBIWnd::CnnInfoLineEditTextChanged(const QString &text)
{
    Q_UNUSED(text);
    HideSomeRowOfCnnTable();
}

void CGSBIWnd::CnnPushButtonClicked(bool checked)
{
    Q_UNUSED(checked);

    QTableWidgetItem * pItem = ui->m_pGSBITableWidget->currentItem();
    QTableWidgetItem * pCnnItem = ui->m_pCnnInfoTableWidget->currentItem();

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

    GooseBiConfig * pNode = GetNodeDataPoint(pItem);
    int tabIndex = ui->m_pCnnInfoComboBox->currentIndex();
    QString qName = ui->m_pCnnInfoTableWidget->item(pCnnItem->row(),0)->text();

    if((pItem->column() != Enum_GSBITable_HWChan_Col)
            &&(pItem->column() != Enum_GSBITable_FWChan_Col))
    {
        QMessageBox::critical(this,"关联错误",QString("需选中GOOSE开入表中的关联通道(%1)!").arg(pItem->row()));
        return;
    }

    if((pItem->column() == Enum_GSBITable_FWChan_Col)
            &&(pNode->dwAttr != 0x2))
    {
        QMessageBox::critical(this,"关联错误","需选中GOOSE开入通道非双点信号，无需关联分位通道!");
        return;
    }

    if(pNode->dwAttr < 0x4)     //单点/双点应关联BI表
    {
        if(tabIndex != 0)
        {
            QMessageBox::critical(this,"关联错误","需选中GOOSE开入通道为单点/双点，需关联开关量表!");
            return;
        }

        pItem->setText(qName);
        if(pItem->column() == Enum_GSBITable_HWChan_Col)
        {
            pNode->sHW = qName;
        }
        else if(pItem->column() == Enum_GSBITable_FWChan_Col)
        {
            pNode->sFW = qName;
        }
    }
    else
    {
        if(tabIndex != 1)
        {
            QMessageBox::critical(this,"关联错误","需选中GOOSE开入通道为浮点/整型，需关联模拟量表!");
            return;
        }

        pItem->setText(qName);
        pNode->sHW = qName;
    }
}

void CGSBIWnd::DisPushButtonClicked(bool checked)
{
    Q_UNUSED(checked);

    QTableWidgetItem * pItem = ui->m_pGSBITableWidget->currentItem();
    //int column = ui->m_pGSBITableWidget->currentColumn();
    //qDebug() << "DisPushButtonClicked called column = " <<column << " , item.column = "<< pItem->column();

    if(pItem == NULL)
    {
        QMessageBox::critical(this,"关联错误","未选中被关联项!");
        return;
    }

    GooseBiConfig * pNode = GetNodeDataPoint(pItem);

    if(pItem->column() == Enum_GSBITable_HWChan_Col)
    {
        pItem->setText("NULL");
        pNode->sHW = "NULL";
    }
    else if(pItem->column() == Enum_GSBITable_FWChan_Col)
    {
        pItem->setText("NULL");
        pNode->sFW = "NULL";
    }
    else
    {
        QMessageBox::critical(this,"关联错误","未选中GOOSE开入表中的关联通道!");
    }
}

void CGSBIWnd::HideSomeRowOfGSBITableWidget()
{
    int attrIndex = ui->m_pBIInfoComboBox->currentIndex();
    QString key = ui->m_pBIInfoLineEdit->text();
    QTableWidget * pTable = ui->m_pGSBITableWidget;

    for (int i = 0; i < pTable->rowCount(); i++)
    {
        QTableWidgetItem * pItem = pTable->item(i,0);
        GooseBiConfig * pNode = GetNodeDataPoint(pItem);
        if((key == "")
                ||(pItem->text().contains(key)))
        {
            if((attrIndex == 0)
                    ||((attrIndex == 1)&&(pNode->dwAttr == 0x1))
                    ||((attrIndex == 2)&&(pNode->dwAttr == 0x2))
                    ||((attrIndex == 3)&&(pNode->dwAttr == 0x4))
                    ||((attrIndex == 4)&&(pNode->dwAttr == 0x8))
                    )
            {
                pTable->setRowHidden(i,false);
            }
            else
            {
                pTable->setRowHidden(i,true);
            }
        }
        else
        {
            pTable->setRowHidden(i,true);
        }
    }
}

void CGSBIWnd::SetNodeDataPoint(QTableWidgetItem *pItem, GooseBiConfig *pGSBI)
{
    if(NULL!=pItem)
    {
        pItem->setData( Qt::UserRole, QVariant((uint)pGSBI) );
    }
}

GooseBiConfig *CGSBIWnd::GetNodeDataPoint(QTableWidgetItem *pItem)
{
    GooseBiConfig* pGSBI = NULL;

    if(NULL!=pItem)
    {
        pGSBI = (GooseBiConfig*)(pItem->data( Qt::UserRole ).toUInt());
    }

    return pGSBI;
}

void CGSBIWnd::CreateRowOfGSBITableWidget(GooseBiConfig *pNode, int row)
{
    QTableWidget * pTableWidget = ui->m_pGSBITableWidget;
    QTableWidgetItem * pTableWidgetItem = NULL;

    pTableWidgetItem = new QTableWidgetItem(pNode->sName);
    SetNodeDataPoint(pTableWidgetItem,pNode);
    pTableWidget->setItem(row,Enum_GSBITable_Name_Col,pTableWidgetItem);

    pTableWidgetItem = new QTableWidgetItem(CDataFun::GetInstance()->ChangeDECToHEX(pNode->dwAttr));
    SetNodeDataPoint(pTableWidgetItem,pNode);
    pTableWidgetItem->setFlags(pTableWidgetItem->flags()&(~Qt::ItemIsEditable));
    pTableWidget->setItem(row,Enum_GSBITable_Attr_Col,pTableWidgetItem);

    pTableWidgetItem = new QTableWidgetItem(pNode->sHW);
    SetNodeDataPoint(pTableWidgetItem,pNode);
    pTableWidgetItem->setFlags(pTableWidgetItem->flags()&(~Qt::ItemIsEditable));
    pTableWidget->setItem(row,Enum_GSBITable_HWChan_Col,pTableWidgetItem);

    pTableWidgetItem = new QTableWidgetItem(pNode->sFW);
    SetNodeDataPoint(pTableWidgetItem,pNode);
    pTableWidgetItem->setFlags(pTableWidgetItem->flags()&(~Qt::ItemIsEditable));
    pTableWidget->setItem(row,Enum_GSBITable_FWChan_Col,pTableWidgetItem);
}

void CGSBIWnd::ShowCnnBITable()
{
    QTableWidget * pTable = ui->m_pCnnInfoTableWidget;
    pTable->clearContents();
    pTable->setRowCount(m_pGseConfig->biConfig.size());

    QTableWidgetItem * pItem = NULL;
    for (int i = 0; i < m_pGseConfig->biConfig.size(); i++)
    {
        BiConfig * pNode = m_pGseConfig->biConfig.at(i);

        if((pNode->dwAttr & CN_GOOSE_BI_BIT) == 0)
        {
            pItem = new QTableWidgetItem("NoMatch");
            pItem->setFlags(pItem->flags()&(~Qt::ItemIsEditable));
            pTable->setItem(i,0,pItem);

            pItem = new QTableWidgetItem("NoMatch");
            pItem->setFlags(pItem->flags()&(~Qt::ItemIsEditable));
            pTable->setItem(i,1,pItem);
        }
        else {
            pItem = new QTableWidgetItem(pNode->sName);
            pItem->setFlags(pItem->flags()&(~Qt::ItemIsEditable));
            pTable->setItem(i,0,pItem);

            pItem = new QTableWidgetItem(pNode->sDesc);
            pItem->setFlags(pItem->flags()&(~Qt::ItemIsEditable));
            pTable->setItem(i,1,pItem);
        }

    }
    pTable->resizeColumnsToContents();
}

void CGSBIWnd::ShowCnnAnaTable()
{
    QTableWidget * pTable = ui->m_pCnnInfoTableWidget;
    pTable->clearContents();
    pTable->setRowCount(m_pGseConfig->anaConfig.size());

    QTableWidgetItem * pItem = NULL;
    for (int i = 0; i < m_pGseConfig->anaConfig.size(); i++)
    {
        AnaConfig * pNode = m_pGseConfig->anaConfig.at(i);

        if((pNode->dwAttr & CN_GS_ANA_BIT) == 0)
        {
            pItem = new QTableWidgetItem("NoMatch");
            pItem->setFlags(pItem->flags()&(~Qt::ItemIsEditable));
            pTable->setItem(i,0,pItem);

            pItem = new QTableWidgetItem("NoMatch");
            pItem->setFlags(pItem->flags()&(~Qt::ItemIsEditable));
            pTable->setItem(i,1,pItem);
        }
        else
        {
            pItem = new QTableWidgetItem(pNode->sName);
            pItem->setFlags(pItem->flags()&(~Qt::ItemIsEditable));
            pTable->setItem(i,0,pItem);

            pItem = new QTableWidgetItem(pNode->sDesc);
            pItem->setFlags(pItem->flags()&(~Qt::ItemIsEditable));
            pTable->setItem(i,1,pItem);
        }
    }
    pTable->resizeColumnsToContents();
}

void CGSBIWnd::HideSomeRowOfCnnTable()
{
    QString key = ui->m_pCnnInfoLineEdit->text();
    QTableWidget * pTable = ui->m_pCnnInfoTableWidget;

    for (int i = 0; i < pTable->rowCount(); i++)
    {
        if(pTable->item(i,0)->text() == "NoMatch")
        {
            pTable->setRowHidden(i,true);
            continue;
        }

        if((key == "")
                ||(pTable->item(i,0)->text().contains(key)))
        {
            pTable->setRowHidden(i,false);
        }
        else
        {
            pTable->setRowHidden(i,true);
        }
    }
}

void CGSBIWnd::ChangeName(QTableWidgetItem *pItem)
{
    GooseBiConfig * p = GetNodeDataPoint(pItem);

    QString oldName = p->sName;
    QString name = pItem->text();
    bool bFind = false;
    int indexFind = -1;

    for (int i = 0; i < m_pGseConfig->gsBiConfig.size(); i++)
    {
        if((p != m_pGseConfig->gsBiConfig[i])
            &&(name == m_pGseConfig->gsBiConfig[i]->sName))
        {
            bFind = true;
            indexFind = i;
            break;
        }
    }

    if(bFind)
    {
        QString strInfo = QString("修改失败，%1 存在重名，详见全部GS开入量第%2行!").arg(name).arg(QString::number(indexFind+1));
        QMessageBox::critical(this,"修改GS开入量名称",strInfo);
        pItem->setText(p->sName);
    }
    else
    {
        p->sName = name;
        p->sEnName = name;
    }
}

void CGSBIWnd::ChangeAttr(QTableWidgetItem *pItem)
{
    GooseBiConfig * pNode = GetNodeDataPoint(pItem);
    unsigned int oldAttr = pNode->dwAttr;
    pNode->dwAttr = CDataFun::GetInstance()->ChangeQStringToUInt(pItem->text());
    unsigned int newAttr = pNode->dwAttr;

    if((((oldAttr == 0x1)||(oldAttr == 0x2))&&((newAttr == 0x4)||(newAttr == 0x8)))
            ||(((oldAttr == 0x4)||(oldAttr == 0x8))&&((newAttr == 0x1)||(newAttr == 0x2))))
    {
        ui->m_pGSBITableWidget->item(pItem->row(),Enum_GSBITable_HWChan_Col)->setText("NULL");
        ui->m_pGSBITableWidget->item(pItem->row(),Enum_GSBITable_FWChan_Col)->setText("NULL");
        pNode->sHW = "NULL";
        pNode->sFW = "NULL";
    }
    else if(newAttr != 0x2)
    {
        ui->m_pGSBITableWidget->item(pItem->row(),Enum_GSBITable_FWChan_Col)->setText("NULL");
        pNode->sFW = "NULL";
    }
}

QString CGSBIWnd::InputNewName(QString old)
{
    bool ok;
    QString text = QInputDialog::getText(this, tr("名称修改"),tr("请输入不重复的通道名称"), QLineEdit::Normal,old, &ok);
    if (ok && !text.isEmpty())
    {
        bool bFindSame = false;
        for (int i = 0; i < m_pGseConfig->gsBiConfig.size(); i++)
        {
            if(text == m_pGseConfig->gsBiConfig[i]->sName)
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
