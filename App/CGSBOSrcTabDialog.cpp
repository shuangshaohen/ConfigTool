#include "CGSBOSrcTabDialog.h"
#include "ui_GSBOSrcTabDialog.h"

CGSBOSrcTabDialog::CGSBOSrcTabDialog(QWidget *parent, QTableWidgetItem *pItem) :
    QDialog(parent),
    ui(new Ui::CGSBOSrcTabDialog),
    m_pItem(pItem)
{
    ui->setupUi(this);

    m_pButtonGroup = new QButtonGroup(this);
    m_pButtonGroup->addButton(ui->m_NullRadioButton,0);
    m_pButtonGroup->addButton(ui->m_OneRadioButton,1);
    m_pButtonGroup->addButton(ui->m_TwoRadioButton,2);
    m_pButtonGroup->addButton(ui->m_ThreeRadioButton,3);

    m_pGSBO = (GooseBoConfig*)(pItem->data( Qt::UserRole ).toUInt());

    if(m_pGSBO != NULL)
    {
        setWindowTitle(m_pGSBO->sName + "来源表设置");
        SetCheckBox(m_pGSBO->wSrcTab);
    }

    connect(ui->buttonBox,SIGNAL(accepted()),this,SLOT(DialogAcceptSlot()));
}

CGSBOSrcTabDialog::~CGSBOSrcTabDialog()
{
    delete ui;
}

void CGSBOSrcTabDialog::SetCheckBox(unsigned int srcTab)
{
    if(srcTab == 1)
    {
       ui->m_OneRadioButton->setChecked(true);
    }
    else if(srcTab == 2)
    {
       ui->m_TwoRadioButton->setChecked(true);
    }
    else if(srcTab == 3)
    {
       ui->m_ThreeRadioButton->setChecked(true);
    }
    else
    {
        ui->m_NullRadioButton->setChecked(true);
    }
}

void CGSBOSrcTabDialog::DialogAcceptSlot()
{
    if(m_pButtonGroup->checkedId() != 0)
    {
        m_pItem->setText(QString::number(m_pButtonGroup->checkedId()));
    }
}
