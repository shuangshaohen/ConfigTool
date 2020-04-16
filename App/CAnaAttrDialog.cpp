#include "CAnaAttrDialog.h"
#include <QDebug>
#include "CDataFun.h"
#include "ui_AnaAttrDialog.h"

CAnaAttrDialog::CAnaAttrDialog(QWidget *parent, QTableWidgetItem *pItem) :
    QDialog(parent),
    ui(new Ui::CAnaAttrDialog),
    m_pItem(pItem)
{
    ui->setupUi(this);

    m_pButtonGroup = new QButtonGroup(this);
    m_pButtonGroup->addButton(ui->m_pFreeRadioButton,ENum_Free_AnaAttrGoup);
    m_pButtonGroup->addButton(ui->m_pADRelyRadioButton,ENum_ADRelay_AnaAttrGoup);
    m_pButtonGroup->addButton(ui->m_pSVRlyRadioButton,ENum_SVRelay_AnaAttrGoup);
    m_pButtonGroup->addButton(ui->m_pCalRelyRadioButton,ENum_CalRelay_AnaAttrGoup);
    m_pButtonGroup->addButton(ui->m_pDevInfoRadioButton,ENum_DevInfo_AnaAttrGoup);

    CreateCheckBoxes();

    m_pAna = (AnaConfig*)(pItem->data( Qt::UserRole ).toUInt());

    if(m_pAna != NULL)
    {
        setWindowTitle(m_pAna->sName + "属性设置");
        SetCheckBox(m_pAna->dwAttr);
        SetAllCheckBoxCheckable(false);
        if(CheckModeADRelay())
            ui->m_pADRelyRadioButton->setChecked(true);
        else if(CheckModeSVRelay())
            ui->m_pSVRlyRadioButton->setChecked(true);
        else if(CheckModeCalRelay())
            ui->m_pCalRelyRadioButton->setChecked(true);
        else if(CheckModeDevInfo())
            ui->m_pDevInfoRadioButton->setChecked(true);
        else
        {
            ui->m_pFreeRadioButton->setChecked(true);
            SetAllCheckBoxCheckable(true);
        }

    }

    connect(m_pButtonGroup,SIGNAL(buttonClicked(int)),this, SLOT(ButtonGroupClickedSlot(int)));
    connect(ui->m_buttonBox,SIGNAL(accepted()),this,SLOT(DialogAcceptSlot()));
}

CAnaAttrDialog::~CAnaAttrDialog()
{
    delete ui;
}

void CAnaAttrDialog::CreateCheckBoxes()
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

bool CAnaAttrDialog::CheckModeADRelay()
{
    if(m_pAna->dwAttr == CN_AD_RELAY_ANA_ATTR)
        return true;
    else
        return false;
}

bool CAnaAttrDialog::CheckModeSVRelay()
{
    if(m_pAna->dwAttr == CN_SV_RELAY_ANA_ATTR)
        return true;
    else
        return false;
}

bool CAnaAttrDialog::CheckModeCalRelay()
{
    if(m_pAna->dwAttr == CN_CAL_RELAY_ANA_ATTR)
        return true;
    else
        return false;
}

bool CAnaAttrDialog::CheckModeDevInfo()
{
    if(m_pAna->dwAttr == CN_DEVINFO_ANA_ATTR)
        return true;
    else
        return false;
}

void CAnaAttrDialog::SetAllCheckBoxCheckable(bool b)
{
    for (int i = 0; i < m_CheckBoxes.size(); i++)
    {
        QCheckBox * pCheckBox = m_CheckBoxes.at(i);
        pCheckBox->setEnabled(b);
    }
}

void CAnaAttrDialog::SetCheckBox(unsigned int dwAttr)
{
    unsigned int cmp = 0x1;
    for (int i = 0; i < m_CheckBoxes.size(); i++)
    {
        QCheckBox * pCheckBox = m_CheckBoxes.at(i);

        if(dwAttr & cmp)
            pCheckBox->setChecked(true);
        else
            pCheckBox->setChecked(false);

        cmp = cmp << 1;
    }
}

void CAnaAttrDialog::ButtonGroupClickedSlot(int index)
{
    //qDebug() << "ButtonGroupClickedSlot called index = " << index;
    switch (index) {
    case ENum_Free_AnaAttrGoup:
        SetAllCheckBoxCheckable(true);
        break;
    case ENum_ADRelay_AnaAttrGoup:
        SetCheckBox(CN_AD_RELAY_ANA_ATTR);
        SetAllCheckBoxCheckable(false);
        break;
    case ENum_SVRelay_AnaAttrGoup:
        SetCheckBox(CN_SV_RELAY_ANA_ATTR);
        SetAllCheckBoxCheckable(false);
        break;
    case ENum_CalRelay_AnaAttrGoup:
        SetCheckBox(CN_CAL_RELAY_ANA_ATTR);
        SetAllCheckBoxCheckable(false);
        break;
    case ENum_DevInfo_AnaAttrGoup:
        SetCheckBox(CN_DEVINFO_ANA_ATTR);
        SetAllCheckBoxCheckable(false);
        break;
    default:
        SetAllCheckBoxCheckable(true);
        break;
    }
}

void CAnaAttrDialog::DialogAcceptSlot()
{
    m_pAna->dwAttr = 0;
    unsigned int cmp = 0x1;
    for (int i = 0; i < m_CheckBoxes.size(); i++)
    {
        QCheckBox * pCheckBox = m_CheckBoxes.at(i);

        if(pCheckBox->isChecked())
            m_pAna->dwAttr += cmp;

        cmp = cmp << 1;
    }
    m_pItem->setText(CDataFun::GetInstance()->ChangeDECToHEX(m_pAna->dwAttr));
}
