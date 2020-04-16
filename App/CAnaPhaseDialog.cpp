#include "CAnaPhaseDialog.h"
#include "CDataFun.h"
#include "ui_AnaPhaseDialog.h"

CAnaPhaseDialog::CAnaPhaseDialog(QWidget *parent, QTableWidgetItem *pItem) :
    QDialog(parent),
    ui(new Ui::CAnaPhaseDialog),
    m_pItem(pItem)
{
    ui->setupUi(this);
    m_RadioButtons.clear();

    m_RadioButtons.push_back(ui->m_radioButtonN);
    m_RadioButtons.push_back(ui->m_radioButtonA);
    m_RadioButtons.push_back(ui->m_radioButtonB);
    m_RadioButtons.push_back(ui->m_radioButtonC);

    m_pButtonGroup = new QButtonGroup(this);

    for (int i = 0; i < m_RadioButtons.size(); i++)
    {
        QRadioButton * p = m_RadioButtons.at(i);
        p->setText(CDataFun::GetInstance()->GetQStringfromAnaPhase(i));
        m_pButtonGroup->addButton(p,i);
    }

    m_pAna = (AnaConfig*)(pItem->data( Qt::UserRole ).toUInt());

    if(m_pAna != NULL)
        setWindowTitle(m_pAna->sName + "相别设置");

    m_RadioButtons.at(m_pAna->wPhaseName)->setChecked(true);

    connect(ui->buttonBox,SIGNAL(accepted()),this,SLOT(DialogAcceptSlot()));
}

CAnaPhaseDialog::~CAnaPhaseDialog()
{
    delete ui;
}

void CAnaPhaseDialog::DialogAcceptSlot()
{
    m_pAna->wPhaseName = m_pButtonGroup->checkedId();
    m_pItem->setText(CDataFun::GetInstance()->GetQStringfromAnaPhase(m_pAna->wPhaseName));
}
