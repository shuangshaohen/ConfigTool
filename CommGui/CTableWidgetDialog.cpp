#include "CTableWidgetDialog.h"
#include "ui_TableWidgetDialog.h"

CTableWidgetDialog::CTableWidgetDialog(QWidget *parent, QTableWidgetItem *pItem) :
    QDialog(parent),
    ui(new Ui::CTableWidgetDialog),
    m_pItem(pItem)
{
    ui->setupUi(this);

    m_pGseConfig = NULL;

    connect(ui->buttonBox,SIGNAL(accepted()),this,SLOT(DialogAcceptSlot()));
}

CTableWidgetDialog::~CTableWidgetDialog()
{
    delete ui;
}

QTableWidget *CTableWidgetDialog::GetTableWidget()
{
    return ui->tableWidget;
}

void CTableWidgetDialog::SetConfig(GseConfig *pGseConfig)
{
    m_pGseConfig = pGseConfig;
}

void CTableWidgetDialog::ShowSetWithSecInType()
{
    if(m_pGseConfig == NULL)
        return;

    QTableWidget * pTable = ui->tableWidget;
    pTable->clearContents();
    QTableWidgetItem * pTemItem = NULL;
    pTable->setRowCount(m_pGseConfig->settingConfig.size());

    for (int i = 0; i < m_pGseConfig->settingConfig.size(); i++)
    {
        SettingConfig * pTem = m_pGseConfig->settingConfig.at(i);

        pTemItem = new QTableWidgetItem(pTem->sName);
        pTemItem->setFlags(pTemItem->flags()&(~Qt::ItemIsEditable));
        pTable->setItem(i,0,pTemItem);

        pTemItem = new QTableWidgetItem(pTem->sDesc);
        pTemItem->setFlags(pTemItem->flags()&(~Qt::ItemIsEditable));
        pTable->setItem(i,1,pTemItem);

        if(pTem->wType != EN_CT2N_TYPE)
            pTable->setRowHidden(i,true);
    }
    pTable->resizeColumnsToContents();
}

void CTableWidgetDialog::DialogAcceptSlot()
{
    QTableWidgetItem * pItem = NULL;
    pItem = ui->tableWidget->item(ui->tableWidget->currentRow(),0);

    if(pItem == NULL)
        m_pItem->setText("NULL");
    else
        m_pItem->setText(pItem->text());
}
