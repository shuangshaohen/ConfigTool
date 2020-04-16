#include "CBIAttrDialog.h"
#include "CDataFun.h"
#include "ui_BIAttrDialog.h"

CBIAttrDialog::CBIAttrDialog(QWidget *parent, QTableWidgetItem *pItem) :
    QDialog(parent),
    ui(new Ui::CBIAttrDialog),
    m_pItem(pItem)
{
    ui->setupUi(this);

    m_pButtonGroup = new QButtonGroup(this);
    m_pButtonGroup->addButton(ui->m_pFreeRadioButton,ENum_Free_BIAttrGoup);
    m_pButtonGroup->addButton(ui->m_pRelyHardRadioButton,ENum_RelayHard_BIAttrGoup);
    m_pButtonGroup->addButton(ui->m_pRlySoftRadioButton,ENum_RelaySoft_BIAttrGoup);
    m_pButtonGroup->addButton(ui->m_pRelyGSRadioButton,ENum_RelayGS_BIAttrGoup);
    m_pButtonGroup->addButton(ui->m_pCheckBIradioButton,ENum_CheckBI_BIAttrGoup);
    m_pButtonGroup->addButton(ui->m_pResetBIradioButton,ENum_ResetBI_BIAttrGoup);
    m_pButtonGroup->addButton(ui->m_pRemoteOpradioButton,ENum_RemoteOpBI_BIAttrGoup);

    CreateCheckBoxes();

    m_pBI = (BiConfig*)(pItem->data( Qt::UserRole ).toUInt());

    if(m_pBI != NULL)
    {
        setWindowTitle(m_pBI->sName + "属性设置");
        SetCheckBox(m_pBI->dwAttr);
        SetAllCheckBoxCheckable(false);
        if(CheckModeCheckBI())
            ui->m_pCheckBIradioButton->setChecked(true);
        else if(CheckModeResetBI())
            ui->m_pResetBIradioButton->setChecked(true);
        else if(CheckModeRemoteOpBI())
            ui->m_pRemoteOpradioButton->setChecked(true);
        else if(CheckModeRelayHard())
            ui->m_pRelyHardRadioButton->setChecked(true);
        else if(CheckModeRelaySoft())
            ui->m_pRlySoftRadioButton->setChecked(true);
        else if(CheckModeRelayGS())
            ui->m_pRelyGSRadioButton->setChecked(true);
        else
        {
            ui->m_pFreeRadioButton->setChecked(true);
            SetAllCheckBoxCheckable(true);
        }

    }

    connect(m_pButtonGroup,SIGNAL(buttonClicked(int)),this, SLOT(ButtonGroupClickedSlot(int)));
    connect(ui->m_buttonBox,SIGNAL(accepted()),this,SLOT(DialogAcceptSlot()));
}

CBIAttrDialog::~CBIAttrDialog()
{
    delete ui;
}

void CBIAttrDialog::CreateCheckBoxes()
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

bool CBIAttrDialog::CheckModeRelayHard()
{
    if(m_pBI->dwAttr == CN_RELAYHARD_BI_ATTR)
        return true;
    else
        return false;
}

bool CBIAttrDialog::CheckModeRelaySoft()
{
    if(m_pBI->dwAttr == CN_RELAYSOFT_BI_ATTR)
        return true;
    else
        return false;
}

bool CBIAttrDialog::CheckModeRelayGS()
{
    if(m_pBI->dwAttr == CN_RELAYGOOSE_BI_ATTR)
        return true;
    else
        return false;
}

bool CBIAttrDialog::CheckModeCheckBI()
{
    if(m_pBI->dwAttr == CN_CHECK_BI_ATTR)
        return true;
    else
        return false;
}

bool CBIAttrDialog::CheckModeResetBI()
{
    if(m_pBI->dwAttr == CN_RESET_BI_ATTR)
        return true;
    else
        return false;
}

bool CBIAttrDialog::CheckModeRemoteOpBI()
{
    if(m_pBI->dwAttr == CN_REMOTEOP_BI_ATTR)
        return true;
    else
        return false;
}

void CBIAttrDialog::SetAllCheckBoxCheckable(bool b)
{
    for (int i = 0; i < m_CheckBoxes.size(); i++)
    {
        QCheckBox * pCheckBox = m_CheckBoxes.at(i);
        pCheckBox->setEnabled(b);
    }
}

void CBIAttrDialog::SetCheckBox(unsigned int dwAttr)
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

void CBIAttrDialog::ButtonGroupClickedSlot(int index)
{
    switch (index) {
    case ENum_Free_BIAttrGoup:
        SetAllCheckBoxCheckable(true);
        break;
    case ENum_RelayHard_BIAttrGoup:
        SetCheckBox(CN_RELAYHARD_BI_ATTR);
        SetAllCheckBoxCheckable(false);
        break;
    case ENum_RelaySoft_BIAttrGoup:
        SetCheckBox(CN_RELAYSOFT_BI_ATTR);
        SetAllCheckBoxCheckable(false);
        break;
    case ENum_RelayGS_BIAttrGoup:
        SetCheckBox(CN_RELAYGOOSE_BI_ATTR);
        SetAllCheckBoxCheckable(false);
        break;
    case ENum_CheckBI_BIAttrGoup:
        SetCheckBox(CN_CHECK_BI_ATTR);
        SetAllCheckBoxCheckable(false);
        break;
    case ENum_ResetBI_BIAttrGoup:
        SetCheckBox(CN_RESET_BI_ATTR);
        SetAllCheckBoxCheckable(false);
        break;
    case ENum_RemoteOpBI_BIAttrGoup:
        SetCheckBox(CN_REMOTEOP_BI_ATTR);
        SetAllCheckBoxCheckable(false);
        break;
    default:
        SetAllCheckBoxCheckable(true);
        break;
    }
}

void CBIAttrDialog::DialogAcceptSlot()
{
    m_pBI->dwAttr = 0;
    unsigned int cmp = 0x1;
    for (int i = 0; i < m_CheckBoxes.size(); i++)
    {
        QCheckBox * pCheckBox = m_CheckBoxes.at(i);

        if(pCheckBox->isChecked())
            m_pBI->dwAttr += cmp;

        cmp = cmp << 1;
    }
    m_pItem->setText(CDataFun::GetInstance()->ChangeDECToHEX(m_pBI->dwAttr));
}
