#include "CGSBOAttrDialog.h"
#include "CDataFun.h"
#include "ui_GSBOAttrDialog.h"

CGSBOAttrDialog::CGSBOAttrDialog(QWidget *parent, QTableWidgetItem *pItem) :
    QDialog(parent),
    ui(new Ui::CGSBOAttrDialog),
    m_pItem(pItem)
{
    ui->setupUi(this);

    m_pButtonGroup = new QButtonGroup(this);
    m_pButtonGroup->addButton(ui->m_DB0CheckBox,0x1);
    m_pButtonGroup->addButton(ui->m_DB1CheckBox,0x2);
    m_pButtonGroup->addButton(ui->m_DB2CheckBox,0x4);
    m_pButtonGroup->addButton(ui->m_DB3CheckBox,0x8);

    m_pButtonGroup->addButton(ui->m_DB4CheckBox,0x10);
    m_pButtonGroup->addButton(ui->m_DB5CheckBox,0x20);
    m_pButtonGroup->addButton(ui->m_DB6CheckBox,0x40);
    m_pButtonGroup->addButton(ui->m_DB7CheckBox,0x80);

    m_pButtonGroup->addButton(ui->m_DB8CheckBox,0x100);
    m_pButtonGroup->addButton(ui->m_DB9CheckBox,0x200);
    m_pButtonGroup->addButton(ui->m_DB10CheckBox,0x400);
    m_pButtonGroup->addButton(ui->m_DB11CheckBox,0x800);

    m_pButtonGroup->addButton(ui->m_DB12CheckBox,0x1000);
    m_pButtonGroup->addButton(ui->m_DB13CheckBox,0x2000);
    m_pButtonGroup->addButton(ui->m_DB14CheckBox,0x4000);
    m_pButtonGroup->addButton(ui->m_DB15CheckBox,0x8000);

    m_pButtonGroup->addButton(ui->m_DB16CheckBox,0x10000);
    m_pButtonGroup->addButton(ui->m_DB17CheckBox,0x20000);
    m_pButtonGroup->addButton(ui->m_DB18CheckBox,0x40000);
    m_pButtonGroup->addButton(ui->m_DB19CheckBox,0x80000);

    m_pButtonGroup->addButton(ui->m_DB20CheckBox,0x100000);
    m_pButtonGroup->addButton(ui->m_DB21CheckBox,0x200000);
    m_pButtonGroup->addButton(ui->m_DB22CheckBox,0x400000);
    m_pButtonGroup->addButton(ui->m_DB23CheckBox,0x800000);

    m_pButtonGroup->addButton(ui->m_DB24CheckBox,0x1000000);
    m_pButtonGroup->addButton(ui->m_DB25CheckBox,0x2000000);
    m_pButtonGroup->addButton(ui->m_DB26CheckBox,0x4000000);
    m_pButtonGroup->addButton(ui->m_DB27CheckBox,0x8000000);

    m_pButtonGroup->addButton(ui->m_DB28CheckBox,0x10000000);
    m_pButtonGroup->addButton(ui->m_DB29CheckBox,0x20000000);
    m_pButtonGroup->addButton(ui->m_DB30CheckBox,0x40000000);
    m_pButtonGroup->addButton(ui->m_DB31CheckBox,0x80000000);

    m_pGSBO = (GooseBoConfig*)(pItem->data( Qt::UserRole ).toUInt());

    CreateCheckBoxes();

    if(m_pGSBO != NULL)
    {
        setWindowTitle(m_pGSBO->sName + "属性设置");
        SetCheckBox(m_pGSBO->dwAttr);
    }

    connect(ui->buttonBox,SIGNAL(accepted()),this,SLOT(DialogAcceptSlot()));
}

CGSBOAttrDialog::~CGSBOAttrDialog()
{
    delete ui;
    m_CheckBoxes.clear();
    m_pGSBO = NULL;
    m_pItem = NULL;
    m_pButtonGroup = NULL;
}

void CGSBOAttrDialog::CreateCheckBoxes()
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

void CGSBOAttrDialog::SetCheckBox(unsigned int dwAttr)
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

void CGSBOAttrDialog::DialogAcceptSlot()
{
    //m_pGSBI->dwAttr = 0;
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
