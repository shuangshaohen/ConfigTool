#include "CAnaTypeDialog.h"
#include "CDataFun.h"
#include "ui_AnaTypeDialog.h"

CAnaTypeDialog::CAnaTypeDialog(QWidget *parent, QTableWidgetItem *pItem) :
    QDialog(parent),
    ui(new Ui::CAnaTypeDialog),
    m_pItem(pItem)
{
    ui->setupUi(this);

    m_RadioButtons.clear();

    m_RadioButtons.push_back(ui->m_radioButton0);
    m_RadioButtons.push_back(ui->m_radioButton1);
    m_RadioButtons.push_back(ui->m_radioButton2);
    m_RadioButtons.push_back(ui->m_radioButton3);
    m_RadioButtons.push_back(ui->m_radioButton4);
    m_RadioButtons.push_back(ui->m_radioButton5);
    m_RadioButtons.push_back(ui->m_radioButton6);
    m_RadioButtons.push_back(ui->m_radioButton7);
    m_RadioButtons.push_back(ui->m_radioButton8);
    m_RadioButtons.push_back(ui->m_radioButton9);
    m_RadioButtons.push_back(ui->m_radioButton10);
    m_RadioButtons.push_back(ui->m_radioButton11);
    m_RadioButtons.push_back(ui->m_radioButton12);
    m_RadioButtons.push_back(ui->m_radioButton13);
    m_RadioButtons.push_back(ui->m_radioButton14);
    m_RadioButtons.push_back(ui->m_radioButton15);
    m_RadioButtons.push_back(ui->m_radioButton16);
    m_RadioButtons.push_back(ui->m_radioButton17);
    m_RadioButtons.push_back(ui->m_radioButton18);
    m_RadioButtons.push_back(ui->m_radioButton19);
    m_RadioButtons.push_back(ui->m_radioButton20);
    m_RadioButtons.push_back(ui->m_radioButton21);
    m_RadioButtons.push_back(ui->m_radioButton22);
    m_RadioButtons.push_back(ui->m_radioButton23);

    m_pButtonGroup = new QButtonGroup(this);

    for (int i = 0; i < m_RadioButtons.size(); i++)
    {
        QRadioButton * p = m_RadioButtons.at(i);
        p->setText(CDataFun::GetInstance()->GetQStringfromAnaType(i));
        m_pButtonGroup->addButton(p,i);
    }

    m_pAna = (AnaConfig*)(pItem->data( Qt::UserRole ).toUInt());

    if(m_pAna != NULL)
        setWindowTitle(m_pAna->sName + "类型设置");

    m_RadioButtons.at(m_pAna->wChanType)->setChecked(true);

    connect(ui->buttonBox,SIGNAL(accepted()),this,SLOT(DialogAcceptSlot()));
}

CAnaTypeDialog::~CAnaTypeDialog()
{
    delete ui;
}

void CAnaTypeDialog::DialogAcceptSlot()
{
    m_pAna->wChanType = m_pButtonGroup->checkedId();
    m_pItem->setText(CDataFun::GetInstance()->GetQStringfromAnaType(m_pAna->wChanType));
}
