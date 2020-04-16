#include "CBITypeDialog.h"
#include "CDataFun.h"
#include "ui_BITypeDialog.h"

CBITypeDialog::CBITypeDialog(QWidget *parent, QTableWidgetItem *pItem) :
    QDialog(parent),
    ui(new Ui::CBITypeDialog),
    m_pItem(pItem)
{
    ui->setupUi(this);

    m_RadioButtons.clear();

    m_RadioButtons.push_back(ui->m_radioButtonA);
    m_RadioButtons.push_back(ui->m_radioButtonB);
    m_RadioButtons.push_back(ui->m_radioButtonC);
    m_RadioButtons.push_back(ui->m_radioButtonNULL);

    m_pButtonGroup = new QButtonGroup(this);

    for (int i = 0; i < m_RadioButtons.size(); i++)
    {
        QRadioButton * p = m_RadioButtons.at(i);
        p->setText(CDataFun::GetInstance()->GetQStringfromBIType(i));
        m_pButtonGroup->addButton(p,i);
    }

    m_pBI = (BiConfig*)(pItem->data( Qt::UserRole ).toUInt());

    if(m_pBI != NULL)
        setWindowTitle(m_pBI->sName + "遥信类型设置");

    if(m_pBI->wYXType == 0xFF)
    {
        m_RadioButtons.at(3)->setChecked(true);
    }
    else
    {
        m_RadioButtons.at(m_pBI->wYXType)->setChecked(true);
    }

    connect(ui->buttonBox,SIGNAL(accepted()),this,SLOT(DialogAcceptSlot()));
}

CBITypeDialog::~CBITypeDialog()
{
    delete ui;
}

void CBITypeDialog::DialogAcceptSlot()
{
    if(m_pButtonGroup->checkedId() == 3)
        m_pBI->wYXType = 0xFF;
    else
        m_pBI->wYXType = m_pButtonGroup->checkedId();
    m_pItem->setText(CDataFun::GetInstance()->GetQStringfromBIType(m_pBI->wYXType));
}
