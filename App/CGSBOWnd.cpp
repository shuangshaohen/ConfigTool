#include "CGSBOWnd.h"
#include "CDataFun.h"
#include "CGSBOAttrDialog.h"
#include "CGSBOSrcTabDialog.h"
#include <QMessageBox>
#include <QInputDialog>
#include <QDebug>
#include "ui_GSBOWnd.h"

CGSBOWnd::CGSBOWnd(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CGSBOWnd)
{
    ui->setupUi(this);

    connect(ui->m_pGSBOTableWidget, SIGNAL(cellChanged( int, int )), this, SLOT(GSBOTableWidgetCellChangedSlot( int, int )) );
    connect(ui->m_pGSBOTableWidget, SIGNAL(cellDoubleClicked(int,int)),this,SLOT(GSBOTableWidgetCellDoubleClickedSlot(int,int)));
    connect(ui->m_pGSBOTableWidget, SIGNAL(cellClicked(int,int)),this,SLOT(GSBOTableWidgetCellClickedSlot(int,int)));

    connect(ui->m_pBOInfoComboBox,SIGNAL(currentIndexChanged(int)), this, SLOT(GSBOInfoComboBoxCurrentIndexChangedSlot(int)));
    connect(ui->m_pBOInfoLineEdit,SIGNAL(textChanged(const QString &)), this, SLOT(GSBOInfoLineEditTextChanged(const QString &)));

    connect(ui->m_pCnnInfoComboBox,SIGNAL(currentIndexChanged(int)), this, SLOT(CnnComboBoxCurrentIndexChangedSlot(int)));
    connect(ui->m_pCnnInfoLineEdit,SIGNAL(textChanged(const QString &)), this, SLOT(CnnInfoLineEditTextChanged(const QString &)));

    connect(ui->m_pCnnPushButton,SIGNAL(clicked(bool)), this, SLOT(CnnPushButtonClicked(bool)));
    connect(ui->m_pDisPushButton,SIGNAL(clicked(bool)), this, SLOT(DisPushButtonClicked(bool)));
}

CGSBOWnd::~CGSBOWnd()
{
    delete ui;
}

void CGSBOWnd::ClearTableModel()
{
    m_pGseConfig = NULL;
    ui->m_pGSBOTableWidget->clearContents();
    ui->m_pCnnInfoTableWidget->clearContents();
    ui->m_pBOInfoLineEdit->clear();
    ui->m_pCnnInfoLineEdit->clear();
}

void CGSBOWnd::ShowGSBOInfo(CDataBase *pCDataBase)
{
    m_pGseConfig = pCDataBase->GetConfig();

    QTableWidget * pTableWidget = ui->m_pGSBOTableWidget;

    pTableWidget->clearContents();
    pTableWidget->setRowCount(m_pGseConfig->gsBoConfig.size());

    disconnect(pTableWidget, SIGNAL(cellChanged( int, int )), 0, 0);

    for (int i = 0; i < m_pGseConfig->gsBoConfig.size() ; i++)
    {
        GooseBoConfig * pNode = m_pGseConfig->gsBoConfig[i];
        CreateRowOfGSBOTableWidget(pNode,i);
    }

    pTableWidget->resizeColumnsToContents();

    //行隐藏，筛选、类型和字段
    HideSomeRowOfGSBOTableWidget();

    connect( pTableWidget, SIGNAL(cellChanged( int, int )), this, SLOT(GSBOTableWidgetCellChangedSlot( int, int )) );

    CnnComboBoxCurrentIndexChangedSlot(ui->m_pCnnInfoComboBox->currentIndex());

    ui->m_pCnnPushButton->setEnabled(false);
    ui->m_pDisPushButton->setEnabled(false);
    ui->m_pCnnPushButton->setText("<<");
    ui->m_pDisPushButton->setText("<<");
}

void CGSBOWnd::AddOper()
{
    QTableWidget * pTable = ui->m_pGSBOTableWidget;
    QTableWidgetItem * pItem = NULL;
    int row = 0;
    pItem = pTable->currentItem();
    if(pItem != NULL)
        row = pItem->row() + 1;

    QString newName = InputNewName("NewGSBO_" + QString::number(m_pGseConfig->gsBoConfig.size()));

    if(newName == "NULL")
        return;

    GooseBoConfig * pNewNode = new GooseBoConfig();

    pNewNode->sName         = newName;
    pNewNode->sEnName       = newName;
    pNewNode->sAddr         = "";
    pNewNode->wSrcTab       = 2;

    pNewNode->wIndex        = row;
    pNewNode->dwAttr        = 0x1;
    pNewNode->HWChan        = 0xFFFF;
    pNewNode->FWChan        = 0xFFFF;

    pNewNode->sHW           = "NULL";
    pNewNode->sFW           = "NULL";

    disconnect(pTable, SIGNAL(cellChanged( int, int )), 0, 0);
    m_pGseConfig->gsBoConfig.insert(row,pNewNode);
    pTable->insertRow(row);
    CreateRowOfGSBOTableWidget(pNewNode,row);
    connect( pTable, SIGNAL(cellChanged( int, int )), this, SLOT(GSBOTableWidgetCellChangedSlot( int, int )) );
}

void CGSBOWnd::DeleteOper()
{
    QTableWidget * pTable = ui->m_pGSBOTableWidget;
    QTableWidgetItem * pItem = NULL;
    pItem = pTable->currentItem();
    if(pItem == NULL)
        return;

    int row = pItem->row();

    pTable->removeRow(row);
    m_pGseConfig->gsBoConfig.remove(row);
}

void CGSBOWnd::CopyOper()
{
    QTableWidget * pTable = ui->m_pGSBOTableWidget;
    QTableWidgetItem * pItem = NULL;
    pItem = pTable->currentItem();
    if(pItem == NULL)
        return;

    GooseBoConfig * pNode = GetNodeDataPoint(pItem);
    QString newName = InputNewName(pNode->sName+"_Copy");

    if(newName == "NULL")
        return;

    int row = pItem->row();
    GooseBoConfig * pNewNode = new GooseBoConfig();

    *pNewNode = *pNode;

    pNewNode->sName         = newName;
    pNewNode->sEnName       = newName;

    disconnect(pTable, SIGNAL(cellChanged( int, int )), 0, 0);
    m_pGseConfig->gsBoConfig.insert(row+1,pNewNode);
    pTable->insertRow(row+1);
    CreateRowOfGSBOTableWidget(pNewNode,row+1);
    connect( pTable, SIGNAL(cellChanged( int, int )), this, SLOT(GSBOTableWidgetCellChangedSlot( int, int )) );
}

void CGSBOWnd::UpOper()
{
    QTableWidget * pTable = ui->m_pGSBOTableWidget;
    QTableWidgetItem * pItem = NULL;
    pItem = pTable->currentItem();
    if(pItem == NULL)
        return;

    int row = pItem->row();
    disconnect(pTable, SIGNAL(cellChanged( int, int )), 0, 0);

    if(row != 0)
    {
        CDataFun::GetInstance()->moveRow(pTable,row,row-1);
        m_pGseConfig->gsBoConfig.swapItemsAt(row,row-1);
    }
    connect( pTable, SIGNAL(cellChanged( int, int )), this, SLOT(GSBOTableWidgetCellChangedSlot( int, int )) );
}

void CGSBOWnd::DownOper()
{
    QTableWidget * pTable = ui->m_pGSBOTableWidget;
    QTableWidgetItem * pItem = NULL;
    pItem = pTable->currentItem();
    if(pItem == NULL)
        return;

    int row = pItem->row();
    disconnect(pTable, SIGNAL(cellChanged( int, int )), 0, 0);

    if(row != pTable->rowCount()-1)
    {
        CDataFun::GetInstance()->moveRow(pTable,row,row+2);
        m_pGseConfig->gsBoConfig.swapItemsAt(row,row+1);
    }
    connect( pTable, SIGNAL(cellChanged( int, int )), this, SLOT(GSBOTableWidgetCellChangedSlot( int, int )) );
}

void CGSBOWnd::GSBOTableWidgetCellChangedSlot(int row, int column)
{
    disconnect(ui->m_pGSBOTableWidget,SIGNAL(cellChanged(int,int)),0,0);
    QTableWidgetItem * pItem = NULL;
    switch (column)
    {
    case Enum_GSBOTable_Name_Col:
        pItem = ui->m_pGSBOTableWidget->item(row,column);
        ChangeName(pItem);
        break;
    case Enum_GSBOTable_Attr_Col:
        pItem = ui->m_pGSBOTableWidget->item(row,column);
        ChangeAttr(pItem);
        break;
    case Enum_GSBOTable_SrcTab_Col:
        pItem = ui->m_pGSBOTableWidget->item(row,column);
        ChangeSrcTab(pItem);
        break;
    default:
        break;
    }
    connect( ui->m_pGSBOTableWidget, SIGNAL(cellChanged( int, int )), this, SLOT(GSBOTableWidgetCellChangedSlot( int, int )) );
}

void CGSBOWnd::GSBOTableWidgetCellDoubleClickedSlot(int row, int column)
{
    if(column == Enum_GSBOTable_Attr_Col)
    {
        QTableWidgetItem * pItem = ui->m_pGSBOTableWidget->item(row,column);
        CGSBOAttrDialog * pDialog = new CGSBOAttrDialog(ui->m_pGSBOTableWidget,pItem);
        pDialog->show();
    }
    else if(column == Enum_GSBOTable_SrcTab_Col)
    {
        QTableWidgetItem * pItem = ui->m_pGSBOTableWidget->item(row,column);
        CGSBOSrcTabDialog * pDialog = new CGSBOSrcTabDialog(ui->m_pGSBOTableWidget,pItem);
        pDialog->show();
    }
}

void CGSBOWnd::GSBOTableWidgetCellClickedSlot(int row, int column)
{
    Q_UNUSED(row);
    if(column == Enum_GSBOTable_HWChan_Col)
    {
        ui->m_pCnnPushButton->setEnabled(true);
        ui->m_pDisPushButton->setEnabled(true);
        ui->m_pCnnPushButton->setText("合位绑定");
        ui->m_pDisPushButton->setText("解除绑定");
    }
    else if(column == Enum_GSBOTable_FWChan_Col)
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

void CGSBOWnd::GSBOInfoComboBoxCurrentIndexChangedSlot(int index)
{
    Q_UNUSED(index);
    HideSomeRowOfGSBOTableWidget();
}

void CGSBOWnd::GSBOInfoLineEditTextChanged(const QString &text)
{
    Q_UNUSED(text);
    HideSomeRowOfGSBOTableWidget();
}

void CGSBOWnd::CnnComboBoxCurrentIndexChangedSlot(int index)
{
    Q_UNUSED(index);
    if(ui->m_pCnnInfoComboBox->currentIndex() == 0)
    {
        showCnnTZJZTable();
    }
    else if(ui->m_pCnnInfoComboBox->currentIndex() == 1)
    {
        ShowCnnBITable();
    }
    else if(ui->m_pCnnInfoComboBox->currentIndex() == 2)
    {
        ShowCnnAnaTable();
    }
    HideSomeRowOfCnnTable();
}

void CGSBOWnd::CnnInfoLineEditTextChanged(const QString &text)
{
    Q_UNUSED(text);
    HideSomeRowOfCnnTable();
}

void CGSBOWnd::CnnPushButtonClicked(bool checked)
{
    Q_UNUSED(checked);

    QTableWidgetItem * pItem = ui->m_pGSBOTableWidget->currentItem();
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

    GooseBoConfig * pNode = GetNodeDataPoint(pItem);
    int tabIndex = ui->m_pCnnInfoComboBox->currentIndex();
    QString qName = ui->m_pCnnInfoTableWidget->item(pCnnItem->row(),0)->text();

    if((pItem->column() != Enum_GSBOTable_HWChan_Col)
            &&(pItem->column() != Enum_GSBOTable_FWChan_Col))
    {
        QMessageBox::critical(this,"关联错误",QString("需选中GOOSE开入表中的关联通道(%1)!").arg(pItem->row()));
        return;
    }

    if((pItem->column() == Enum_GSBOTable_FWChan_Col)
            &&(pNode->dwAttr != 0x2))
    {
        QMessageBox::critical(this,"关联错误","需选中GOOSE开入通道非双点信号，无需关联分位通道!");
        return;
    }

    if(pNode->dwAttr < 0x4)     //单点/双点应关联BI表
    {
        if(tabIndex == 2)
        {
            QMessageBox::critical(this,"关联错误","需选中GOOSE开入通道为单点/双点，需关联跳闸矩阵量表或开关量表!");
            return;
        }

        if(pNode->wSrcTab == 3)
        {
            QMessageBox::critical(this,"来源表与属性冲突","需选中GOOSE开入通道为单点/双点，来源表应为跳闸矩阵量表或开关量表!");
            return;
        }

        if((tabIndex != 0)&&(pNode->wSrcTab == 1))
        {
            QMessageBox::critical(this,"关联错误","需选中GOOSE开入通道来源表应为跳闸矩阵表!");
            return;
        }

        if((tabIndex != 1)&&(pNode->wSrcTab == 2))
        {
            QMessageBox::critical(this,"关联错误","需选中GOOSE开入通道来源表应为开关量表!");
            return;
        }

        pItem->setText(qName);
        if(pItem->column() == Enum_GSBOTable_HWChan_Col)
        {
            pNode->sHW = qName;
        }
        else if(pItem->column() == Enum_GSBOTable_FWChan_Col)
        {
            pNode->sFW = qName;
        }
    }
    else
    {
        if(pNode->wSrcTab != 3)
        {
            QMessageBox::critical(this,"来源表与属性冲突","需选中GOOSE开入通道为浮点/整型，来源表应为模拟量表!");
            return;
        }

        if(tabIndex != 2)
        {
            QMessageBox::critical(this,"关联错误","需选中GOOSE开入通道为浮点/整型，需关联模拟量表!");
            return;
        }

        pItem->setText(qName);
        pNode->sHW = qName;
    }
}

void CGSBOWnd::DisPushButtonClicked(bool checked)
{
    Q_UNUSED(checked);

    QTableWidgetItem * pItem = ui->m_pGSBOTableWidget->currentItem();
    //int column = ui->m_pGSBITableWidget->currentColumn();
    //qDebug() << "DisPushButtonClicked called column = " <<column << " , item.column = "<< pItem->column();

    if(pItem == NULL)
    {
        QMessageBox::critical(this,"关联错误","未选中被关联项!");
        return;
    }

    GooseBoConfig * pNode = GetNodeDataPoint(pItem);

    if(pItem->column() == Enum_GSBOTable_HWChan_Col)
    {
        pItem->setText("NULL");
        pNode->sHW = "NULL";
    }
    else if(pItem->column() == Enum_GSBOTable_FWChan_Col)
    {
        pItem->setText("NULL");
        pNode->sFW = "NULL";
    }
    else
    {
        QMessageBox::critical(this,"关联错误","未选中GOOSE开出表中的关联通道!");
    }
}

void CGSBOWnd::HideSomeRowOfGSBOTableWidget()
{
    int attrIndex = ui->m_pBOInfoComboBox->currentIndex();
    QString key = ui->m_pBOInfoLineEdit->text();
    QTableWidget * pTable = ui->m_pGSBOTableWidget;

    for (int i = 0; i < pTable->rowCount(); i++)
    {
        QTableWidgetItem * pItem = pTable->item(i,0);
        GooseBoConfig * pNode = GetNodeDataPoint(pItem);
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

void CGSBOWnd::SetNodeDataPoint(QTableWidgetItem *pItem, GooseBoConfig *pGSBO)
{
    if(NULL!=pItem)
    {
        pItem->setData( Qt::UserRole, QVariant((uint)pGSBO) );
    }
}

GooseBoConfig *CGSBOWnd::GetNodeDataPoint(QTableWidgetItem *pItem)
{
    GooseBoConfig* pGSBO = NULL;

    if(NULL!=pItem)
    {
        pGSBO = (GooseBoConfig*)(pItem->data( Qt::UserRole ).toUInt());
    }

    return pGSBO;
}

void CGSBOWnd::CreateRowOfGSBOTableWidget(GooseBoConfig *pNode, int row)
{
    QTableWidget * pTableWidget = ui->m_pGSBOTableWidget;
    QTableWidgetItem * pTableWidgetItem = NULL;

    pTableWidgetItem = new QTableWidgetItem(pNode->sName);
    SetNodeDataPoint(pTableWidgetItem,pNode);
    pTableWidget->setItem(row,Enum_GSBOTable_Name_Col,pTableWidgetItem);

    pTableWidgetItem = new QTableWidgetItem(CDataFun::GetInstance()->ChangeDECToHEX(pNode->dwAttr));
    SetNodeDataPoint(pTableWidgetItem,pNode);
    pTableWidgetItem->setFlags(pTableWidgetItem->flags()&(~Qt::ItemIsEditable));
    pTableWidget->setItem(row,Enum_GSBOTable_Attr_Col,pTableWidgetItem);

    pTableWidgetItem = new QTableWidgetItem(QString::number(pNode->wSrcTab));
    SetNodeDataPoint(pTableWidgetItem,pNode);
    pTableWidgetItem->setFlags(pTableWidgetItem->flags()&(~Qt::ItemIsEditable));
    pTableWidget->setItem(row,Enum_GSBOTable_SrcTab_Col,pTableWidgetItem);

    pTableWidgetItem = new QTableWidgetItem(pNode->sHW);
    SetNodeDataPoint(pTableWidgetItem,pNode);
    pTableWidgetItem->setFlags(pTableWidgetItem->flags()&(~Qt::ItemIsEditable));
    pTableWidget->setItem(row,Enum_GSBOTable_HWChan_Col,pTableWidgetItem);

    pTableWidgetItem = new QTableWidgetItem(pNode->sFW);
    SetNodeDataPoint(pTableWidgetItem,pNode);
    pTableWidgetItem->setFlags(pTableWidgetItem->flags()&(~Qt::ItemIsEditable));
    pTableWidget->setItem(row,Enum_GSBOTable_FWChan_Col,pTableWidgetItem);
}

void CGSBOWnd::ShowCnnBITable()
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

void CGSBOWnd::ShowCnnAnaTable()
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

void CGSBOWnd::showCnnTZJZTable()
{
    QTableWidget * pTable = ui->m_pCnnInfoTableWidget;
    pTable->clearContents();
    pTable->setRowCount(m_pGseConfig->tripMatrix.size());

    QTableWidgetItem * pItem = NULL;
    for (int i = 0; i < m_pGseConfig->tripMatrix.size(); i++)
    {
        TripMatrixConfig * pNode = m_pGseConfig->tripMatrix.at(i);

        pItem = new QTableWidgetItem(pNode->sName);
        pItem->setFlags(pItem->flags()&(~Qt::ItemIsEditable));
        pTable->setItem(i,0,pItem);

        pItem = new QTableWidgetItem(pNode->sDesc);
        pItem->setFlags(pItem->flags()&(~Qt::ItemIsEditable));
        pTable->setItem(i,1,pItem);
    }
    pTable->resizeColumnsToContents();
}

void CGSBOWnd::HideSomeRowOfCnnTable()
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

void CGSBOWnd::ChangeName(QTableWidgetItem *pItem)
{
    GooseBoConfig * p = GetNodeDataPoint(pItem);

    QString oldName = p->sName;
    QString name = pItem->text();
    bool bFind = false;
    int indexFind = -1;

    for (int i = 0; i < m_pGseConfig->gsBoConfig.size(); i++)
    {
        if((p != m_pGseConfig->gsBoConfig[i])
            &&(name == m_pGseConfig->gsBoConfig[i]->sName))
        {
            bFind = true;
            indexFind = i;
            break;
        }
    }

    if(bFind)
    {
        QString strInfo = QString("修改失败，%1 存在重名，详见全部GS开出表第%2行!").arg(name).arg(QString::number(indexFind+1));
        QMessageBox::critical(this,"修改GS开出量名称",strInfo);
        pItem->setText(p->sName);
    }
    else
    {
        p->sName = name;
        p->sEnName = name;
    }
}

void CGSBOWnd::ChangeAttr(QTableWidgetItem *pItem)
{
    GooseBoConfig * pNode = GetNodeDataPoint(pItem);
    unsigned int oldAttr = pNode->dwAttr;
    pNode->dwAttr = CDataFun::GetInstance()->ChangeQStringToUInt(pItem->text());
    unsigned int newAttr = pNode->dwAttr;

    if((((oldAttr == 0x1)||(oldAttr == 0x2))&&((newAttr == 0x4)||(newAttr == 0x8)))
            ||(((oldAttr == 0x4)||(oldAttr == 0x8))&&((newAttr == 0x1)||(newAttr == 0x2))))
    {
        ui->m_pGSBOTableWidget->item(pItem->row(),Enum_GSBOTable_HWChan_Col)->setText("NULL");
        ui->m_pGSBOTableWidget->item(pItem->row(),Enum_GSBOTable_FWChan_Col)->setText("NULL");
        pNode->sHW = "NULL";
        pNode->sFW = "NULL";
    }
    else if(newAttr != 0x2)
    {
        ui->m_pGSBOTableWidget->item(pItem->row(),Enum_GSBOTable_FWChan_Col)->setText("NULL");
        pNode->sFW = "NULL";
    }
}

void CGSBOWnd::ChangeSrcTab(QTableWidgetItem *pItem)
{
    GooseBoConfig * pNode = GetNodeDataPoint(pItem);
    unsigned int oldSrcTab = pNode->wSrcTab;
    pNode->wSrcTab = pItem->text().toUInt();
    if(oldSrcTab != pNode->wSrcTab)
    {
        ui->m_pGSBOTableWidget->item(pItem->row(),Enum_GSBOTable_HWChan_Col)->setText("NULL");
        ui->m_pGSBOTableWidget->item(pItem->row(),Enum_GSBOTable_FWChan_Col)->setText("NULL");
        pNode->sHW = "NULL";
        pNode->sFW = "NULL";
    }
}

QString CGSBOWnd::InputNewName(QString old)
{
    bool ok;
    QString text = QInputDialog::getText(this, tr("名称修改"),tr("请输入不重复的通道名称"), QLineEdit::Normal,old, &ok);
    if (ok && !text.isEmpty())
    {
        bool bFindSame = false;
        for (int i = 0; i < m_pGseConfig->gsBoConfig.size(); i++)
        {
            if(text == m_pGseConfig->gsBoConfig[i]->sName)
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
