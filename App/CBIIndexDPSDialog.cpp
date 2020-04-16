#include "CBIIndexDPSDialog.h"
#include <QTabWidget>
#include "ui_BIIndexDPSDialog.h"

CBIIndexDPSDialog::CBIIndexDPSDialog(QWidget *parent, QTableWidgetItem * pItem , GseConfig * pGseConfig) :
    QDialog(parent),
    ui(new Ui::CBIIndexDPSDialog),
    m_pItem(pItem),
    m_pGseConfig(pGseConfig)
{
    ui->setupUi(this);

    m_pBI = (BiConfig*)(pItem->data( Qt::UserRole ).toUInt());

    QTableWidget * pTable = ui->tableWidget;
    QTableWidgetItem * pTableItem = NULL;
    pTable->clearContents();
    pTable->setRowCount(m_pGseConfig->biConfig.size());

    int row = -1;

    for (int i = 0 ; i < m_pGseConfig->biConfig.size(); i++)
    {
        BiConfig * pNode = m_pGseConfig->biConfig.at(i);

        pTableItem = new QTableWidgetItem(pNode->sName);
        if(pNode->sName == m_pItem->text())
        {
            row = i;
            pTableItem->setForeground(QBrush(QColor(255,0,0)));
        }
        pTable->setItem(i,0,pTableItem);

        pTableItem = new QTableWidgetItem(pNode->sDesc);
        pTable->setItem(i,1,pTableItem);


    }
    pTable->resizeColumnsToContents();

    if(row != -1)
        pTable->setCurrentItem(pTable->item(row,0));

    connect(ui->buttonBox,SIGNAL(accepted()),this,SLOT(DialogAcceptSlot()));
}

CBIIndexDPSDialog::~CBIIndexDPSDialog()
{
    delete ui;
}

void CBIIndexDPSDialog::DialogAcceptSlot()
{
    QTableWidgetItem * current = ui->tableWidget->currentItem();
    if(current == NULL)
        return;

    int row = current->row();
    QString name = ui->tableWidget->item(row,0)->text();

    m_pItem->setText(name);
    m_pBI->sIndexDPS = name;
}
