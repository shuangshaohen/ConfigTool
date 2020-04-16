#include "CTypeDialog.h"
#include "ui_TypeDialog.h"

CTypeDialog::CTypeDialog(QWidget *parent, QTableWidgetItem *pItem) :
    QDialog(parent),
    ui(new Ui::CTypeDialog),
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
        p->setEnabled(false);
        m_pButtonGroup->addButton(p,i);
    }

    connect(ui->buttonBox,SIGNAL(accepted()),this,SLOT(DialogAcceptSlot()));
}

CTypeDialog::~CTypeDialog()
{
    delete ui;
}

void CTypeDialog::SetRadioButtonText(int num, QString text)
{
    if(num > m_RadioButtons.size())
        return;

    QRadioButton * p = m_RadioButtons.at(num);
    p->setText(text);

    if(p->text().contains("备用"))
        p->setEnabled(false);
    else
        p->setEnabled(true);
}

void CTypeDialog::SetType(unsigned int type)
{
    m_RadioButtons.at(type)->setChecked(true);
}

void CTypeDialog::DialogAcceptSlot()
{
    m_pItem->setText(QString::number(m_pButtonGroup->checkedId()));
}
