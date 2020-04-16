#include "CAttrDialog.h"
#include "CDataFun.h"
#include "ui_AttrDialog.h"

CAttrDialog::CAttrDialog(QWidget *parent, QTableWidgetItem *pItem) :
    QDialog(parent),
    ui(new Ui::CAttrDialog),
    m_pItem(pItem)
{
    ui->setupUi(this);

    CreateCheckBoxes();

    setWindowTitle("属性设置");

    connect(ui->buttonBox,SIGNAL(accepted()),this,SLOT(DialogAcceptSlot()));
}

CAttrDialog::~CAttrDialog()
{
    delete ui;
    m_pItem = NULL;
    m_CheckBoxes.clear();
}

void CAttrDialog::SetCheckBoxText(int num, QString text)
{
    m_CheckBoxes[num]->setText(text);
}

void CAttrDialog::CreateCheckBoxes()
{
    m_CheckBoxes.clear();

    m_CheckBoxes.push_back(ui->m_DB0CheckBox );
    m_CheckBoxes.push_back(ui->m_DB1CheckBox );
    m_CheckBoxes.push_back(ui->m_DB2CheckBox );
    m_CheckBoxes.push_back(ui->m_DB3CheckBox );
    m_CheckBoxes.push_back(ui->m_DB4CheckBox );
    m_CheckBoxes.push_back(ui->m_DB5CheckBox );
    m_CheckBoxes.push_back(ui->m_DB6CheckBox );
    m_CheckBoxes.push_back(ui->m_DB7CheckBox );
    m_CheckBoxes.push_back(ui->m_DB8CheckBox );
    m_CheckBoxes.push_back(ui->m_DB9CheckBox );
    m_CheckBoxes.push_back(ui->m_DB10CheckBox);
    m_CheckBoxes.push_back(ui->m_DB11CheckBox);
    m_CheckBoxes.push_back(ui->m_DB12CheckBox);
    m_CheckBoxes.push_back(ui->m_DB13CheckBox);
    m_CheckBoxes.push_back(ui->m_DB14CheckBox);
    m_CheckBoxes.push_back(ui->m_DB15CheckBox);
    m_CheckBoxes.push_back(ui->m_DB16CheckBox);
    m_CheckBoxes.push_back(ui->m_DB17CheckBox);
    m_CheckBoxes.push_back(ui->m_DB18CheckBox);
    m_CheckBoxes.push_back(ui->m_DB19CheckBox);
    m_CheckBoxes.push_back(ui->m_DB20CheckBox);
    m_CheckBoxes.push_back(ui->m_DB21CheckBox);
    m_CheckBoxes.push_back(ui->m_DB22CheckBox);
    m_CheckBoxes.push_back(ui->m_DB23CheckBox);
    m_CheckBoxes.push_back(ui->m_DB24CheckBox);
    m_CheckBoxes.push_back(ui->m_DB25CheckBox);
    m_CheckBoxes.push_back(ui->m_DB26CheckBox);
    m_CheckBoxes.push_back(ui->m_DB27CheckBox);
    m_CheckBoxes.push_back(ui->m_DB28CheckBox);
    m_CheckBoxes.push_back(ui->m_DB29CheckBox);
    m_CheckBoxes.push_back(ui->m_DB30CheckBox);
    m_CheckBoxes.push_back(ui->m_DB31CheckBox);
}


void CAttrDialog::SetCheckBox(unsigned int dwAttr)
{
    unsigned int cmp = 0x1;
    for (int i = 0; i < m_CheckBoxes.size(); i++)
    {
        QCheckBox * pCheckBox = m_CheckBoxes.at(i);

        if(pCheckBox->text().contains("备用"))
            pCheckBox->setEnabled(false);

        if(dwAttr & cmp)
            pCheckBox->setChecked(true);
        else
            pCheckBox->setChecked(false);

        cmp = cmp << 1;
    }
}

void CAttrDialog::DialogAcceptSlot()
{
    unsigned int dwAttr = 0;
    unsigned int cmp = 0x1;
    for (int i = 0; i < m_CheckBoxes.size(); i++)
    {
        QCheckBox * pCheckBox = m_CheckBoxes.at(i);

        if(pCheckBox->isChecked())
            dwAttr += cmp;

        cmp = cmp << 1;
    }
    m_pItem->setText(CDataFun::GetInstance()->ChangeDECToHEX(dwAttr));
}
