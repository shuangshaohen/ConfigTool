#include "CParaToCfgDialog.h"
#include "ui_ParaToCfgDialog.h"

#define DEV_8220_ADVOL_NUM 4
#define DEV_8220_ADCURGROUP_NUM 6

CParaToCfgDialog::CParaToCfgDialog(tdsTopoPara *para, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CParaToCfgDialog)
{
    ui->setupUi(this);
    setWindowTitle("导出Config配置文件");
    m_para = para;

    m_data = new CDataBase();
    m_data->GetConfig()->inital();

    m_ADCurCTParaStartIndex = 0;
    m_swicthes.clear();

    if(m_para != nullptr)
    {
        for (unsigned int i = 0; i < m_para->ptPara->sons.size(); i++) {
            tdsPara * p = m_para->ptPara->sons[i];
            if(p->key == "Switch")
            {
                m_swicthes.push_back(p);
            }
            else if(p->key == "Source")
            {
                m_sources.push_back(p);
            }
            else if(p->key == "Busbar")
            {
                m_busbars.push_back(p);
            }
            else if(p->key == "Line")
            {
                m_lines.push_back(p);
            }
            else if(p->key == "Transformer")
            {
                m_transformers.push_back(p);
            }
            else if(p->key == "Load")
            {
                m_loads.push_back(p);
            }
            else if(p->key == "Remote")
            {
                m_remotes.push_back(p);
            }
        }
    }
}

CParaToCfgDialog::~CParaToCfgDialog()
{
    delete ui;
    delete m_data;
}

void CParaToCfgDialog::Save(const QString &fileName)
{
    CreateDeviceInfo();
    CreateDevicePara();

    //SPSet得在前面，获取一二次额定值开始得索引
    CreateSPSetConfig();
    CreateSoftYBConfig();
    CreateAnaConfig();
    CreateDrvConfig();
    CreateSVConfig();

    CreateOtherConfig();

    CreateGeneralBiConfig();
    CreateSignal();
    CreateGooseBi();
    //GSFLT得在GOOSEBI后
    CreateGSFLTConfig();

    CreateSGSetConfig();
    CreateTripMatrix();
    CreateEvt();
    CreateRmt();

    m_data->Submit();
    m_data->SaveConfig(fileName);
}

void CParaToCfgDialog::CreateDeviceInfo()
{
    DeviceInfo * info = &m_data->GetConfig()->deviceInfo;
    info->sType = ui->comboBox->currentText();
    info->sVersion = ui->lineEditVersion->text();
    if(ui->comboBox->currentIndex() == 0)
        info->sDesc = "配网保护控制子站";
    else
        info->sDesc = "配网保护控制终端";
    info->sCRC = "1234ABCD";
}

void CParaToCfgDialog::CreateDevicePara()
{
    DeviceParas * para = &m_data->GetConfig()->deviceParas;
    para->wMaxAnaNum = 60;
    para->wMaxSvNum = 120;
    para->wMaxBiNum = 80;
    para->wMaxBoNum = 60;
    para->wMaxGooseNum = 120;
    para->wSmpRate = 2000;
    para->wZoneNum = 20;
}

void CParaToCfgDialog::CreateAnaConfig()
{
    if(ui->comboBox->currentIndex() == 0)
        return;

    BaseTab * adAna = &m_data->GetConfig()->adAnaConfig;
    int index = 0;
    adAna->items.push_back(createADAnaU(index,"EN_ANA_UA","AD采集电压UA","UA"));
    adAna->items.push_back(createADAnaU(index,"EN_ANA_UB","AD采集电压UB","UB"));
    adAna->items.push_back(createADAnaU(index,"EN_ANA_UC","AD采集电压UC","UC"));
    adAna->items.push_back(createADAnaU(index,"EN_ANA_3U0","AD采集电压3U0","3U0"));

    for (int i = 0; i < DEV_8220_ADCURGROUP_NUM; i++)
    {
        int startSetIndex = m_ADCurCTParaStartIndex;
        adAna->items.push_back(createADAnaI(index,
                                            QString("EN_ANA_GROUP%1_IA").arg(i+1),
                                            QString("AD采集第%1组电流IA").arg(i+1),
                                            "IA",
                                            QString("SPSET:%1").arg(startSetIndex + i*4),
                                            QString("SPSET:%1").arg(startSetIndex + i*4 +1)));

        adAna->items.push_back(createADAnaI(index,
                                            QString("EN_ANA_GROUP%1_IB").arg(i+1),
                                            QString("AD采集第%1组电流IB").arg(i+1),
                                            "IB",
                                            QString("SPSET:%1").arg(startSetIndex + i*4),
                                            QString("SPSET:%1").arg(startSetIndex + i*4 +1)));

        adAna->items.push_back(createADAnaI(index,
                                            QString("EN_ANA_GROUP%1_IC").arg(i+1),
                                            QString("AD采集第%1组电流IC").arg(i+1),
                                            "IC",
                                            QString("SPSET:%1").arg(startSetIndex + i*4),
                                            QString("SPSET:%1").arg(startSetIndex + i*4 +1)));

        adAna->items.push_back(createADAnaI(index,
                                            QString("EN_ANA_GROUP%1_3I0").arg(i+1),
                                            QString("AD采集第%1组电流3I0").arg(i+1),
                                            "3I0",
                                            QString("SPSET:%1").arg(startSetIndex + i*4 +2),
                                            QString("SPSET:%1").arg(startSetIndex + i*4 +3)));
    }
}

void CParaToCfgDialog::CreateDrvConfig()
{
    int index = 0;
    AppendDrvConfig(m_para->ptPara,index);
}

void CParaToCfgDialog::CreateSVConfig()
{
    BaseTab * SVAna = &m_data->GetConfig()->svConfig;
    int index = 0;

    if(ui->comboBox->currentIndex() == 0)
    {
        for (int i = 0 ; i < m_sources.size() ;i++) {
            QString desc = QString::fromStdString(m_sources[i]->desc);
            SVAna->items.push_back(createSVAnaU(index,
                                                QString("EN_SV_SOURCE%1_UA").arg(i+1),
                                                QString("%1电压SV接收通道UA").arg(desc),
                                                "UA"));

            SVAna->items.push_back(createSVAnaU(index,
                                                QString("EN_SV_SOURCE%1_UB").arg(i+1),
                                                QString("%1电压SV接收通道UB").arg(desc),
                                                "UB"));

            SVAna->items.push_back(createSVAnaU(index,
                                                QString("EN_SV_SOURCE%1_UC").arg(i+1),
                                                QString("%1电压SV接收通道UC").arg(desc),
                                                "UC"));

            SVAna->items.push_back(createSVAnaU(index,
                                                QString("EN_SV_SOURCE%1_3U0").arg(i+1),
                                                QString("%1电压SV接收通道3U0").arg(desc),
                                                "3U0"));

            SVAna->items.push_back(createSVAnaU(index,
                                                QString("EN_SV_SOURCE%1_UAB").arg(i+1),
                                                QString("%1电压SV接收通道UAB").arg(desc),
                                                "UAB"));

            SVAna->items.push_back(createSVAnaU(index,
                                                QString("EN_SV_SOURCE%1_UBC").arg(i+1),
                                                QString("%1电压SV接收通道UBC").arg(desc),
                                                "UBC"));

            SVAna->items.push_back(createSVAnaU(index,
                                                QString("EN_SV_SOURCE%1_UCA").arg(i+1),
                                                QString("%1电压SV接收通道UCA").arg(desc),
                                                "UCA"));
        }

        for (int i = 0 ; i < m_busbars.size() ;i++) {
            QString desc = QString::fromStdString(m_busbars[i]->desc);
            SVAna->items.push_back(createSVAnaU(index,
                                                QString("EN_SV_BUSBAR%1_UA").arg(i+1),
                                                QString("%1电压SV接收通道UA").arg(desc),
                                                "UA"));

            SVAna->items.push_back(createSVAnaU(index,
                                                QString("EN_SV_BUSBAR%1_UB").arg(i+1),
                                                QString("%1电压SV接收通道UB").arg(desc),
                                                "UB"));

            SVAna->items.push_back(createSVAnaU(index,
                                                QString("EN_SV_BUSBAR%1_UC").arg(i+1),
                                                QString("%1电压SV接收通道UC").arg(desc),
                                                "UC"));

            SVAna->items.push_back(createSVAnaU(index,
                                                QString("EN_SV_BUSBAR%1_3U0").arg(i+1),
                                                QString("%1电压SV接收通道3U0").arg(desc),
                                                "3U0"));

            SVAna->items.push_back(createSVAnaU(index,
                                                QString("EN_SV_BUSBAR%1_UAB").arg(i+1),
                                                QString("%1电压SV接收通道UAB").arg(desc),
                                                "UAB"));

            SVAna->items.push_back(createSVAnaU(index,
                                                QString("EN_SV_BUSBAR%1_UBC").arg(i+1),
                                                QString("%1电压SV接收通道UBC").arg(desc),
                                                "UBC"));

            SVAna->items.push_back(createSVAnaU(index,
                                                QString("EN_SV_BUSBAR%1_UCA").arg(i+1),
                                                QString("%1电压SV接收通道UCA").arg(desc),
                                                "UCA"));
        }

        for (int i = 0 ; i < m_remotes.size() ;i++) {
            QString desc = QString::fromStdString(m_remotes[i]->desc);
            SVAna->items.push_back(createSVAnaU(index,
                                                QString("EN_SV_REMOTE%1_UA").arg(i+1),
                                                QString("%1电压SV接收通道UA").arg(desc),
                                                "UA"));

            SVAna->items.push_back(createSVAnaU(index,
                                                QString("EN_SV_REMOTE%1_UB").arg(i+1),
                                                QString("%1电压SV接收通道UB").arg(desc),
                                                "UB"));

            SVAna->items.push_back(createSVAnaU(index,
                                                QString("EN_SV_REMOTE%1_UC").arg(i+1),
                                                QString("%1电压SV接收通道UC").arg(desc),
                                                "UC"));

            SVAna->items.push_back(createSVAnaU(index,
                                                QString("EN_SV_REMOTE%1_3U0").arg(i+1),
                                                QString("%1电压SV接收通道3U0").arg(desc),
                                                "3U0"));

            SVAna->items.push_back(createSVAnaU(index,
                                                QString("EN_SV_REMOTE%1_UAB").arg(i+1),
                                                QString("%1电压SV接收通道UAB").arg(desc),
                                                "UAB"));

            SVAna->items.push_back(createSVAnaU(index,
                                                QString("EN_SV_REMOTE%1_UBC").arg(i+1),
                                                QString("%1电压SV接收通道UBC").arg(desc),
                                                "UBC"));

            SVAna->items.push_back(createSVAnaU(index,
                                                QString("EN_SV_REMOTE%1_UCA").arg(i+1),
                                                QString("%1电压SV接收通道UCA").arg(desc),
                                                "UCA"));
        }

        for (int i = 0 ; i < m_swicthes.size() ;i++) {
            QString desc = QString::fromStdString(m_swicthes[i]->desc);
            SVAna->items.push_back(createSVAnaI(index,
                                                QString("EN_SV_SWITCH%1_IA").arg(i+1),
                                                QString("%1电流SV接收通道IA").arg(desc),
                                                "IA"));

            SVAna->items.push_back(createSVAnaI(index,
                                                QString("EN_SV_REMOTE%1_IB").arg(i+1),
                                                QString("%1电流SV接收通道IB").arg(desc),
                                                "IB"));

            SVAna->items.push_back(createSVAnaI(index,
                                                QString("EN_SV_REMOTE%1_IC").arg(i+1),
                                                QString("%1电流SV接收通道IC").arg(desc),
                                                "IC"));

            SVAna->items.push_back(createSVAnaI(index,
                                                QString("EN_SV_REMOTE%1_3I0").arg(i+1),
                                                QString("%1电流SV接收通道3I0").arg(desc),
                                                "3I0"));
        }
    }

    for (int i = 0 ; i < m_remotes.size() ;i++) {
        QString desc = QString::fromStdString(m_remotes[i]->desc);
        SVAna->items.push_back(createSVAnaI(index,
                                            QString("EN_SV_REMOTE%1_IMA").arg(i+1),
                                            QString("%1m侧电流SV接收通道ImA").arg(desc),
                                            "IA"));

        SVAna->items.push_back(createSVAnaI(index,
                                            QString("EN_SV_REMOTE%1_IMB").arg(i+1),
                                            QString("%1m侧电流SV接收通道ImB").arg(desc),
                                            "IB"));

        SVAna->items.push_back(createSVAnaI(index,
                                            QString("EN_SV_REMOTE%1_IMC").arg(i+1),
                                            QString("%1m侧电流SV接收通道ImC").arg(desc),
                                            "IC"));

        SVAna->items.push_back(createSVAnaI(index,
                                            QString("EN_SV_REMOTE%1_3IM0").arg(i+1),
                                            QString("%1m侧电流SV接收通道3Im0").arg(desc),
                                            "3I0"));

        SVAna->items.push_back(createSVAnaI(index,
                                            QString("EN_SV_REMOTE%1_INA").arg(i+1),
                                            QString("%1n侧电流SV接收通道InA").arg(desc),
                                            "IA"));

        SVAna->items.push_back(createSVAnaI(index,
                                            QString("EN_SV_REMOTE%1_INB").arg(i+1),
                                            QString("%1n侧电流SV接收通道InB").arg(desc),
                                            "IB"));

        SVAna->items.push_back(createSVAnaI(index,
                                            QString("EN_SV_REMOTE%1_INC").arg(i+1),
                                            QString("%1n侧电流SV接收通道InC").arg(desc),
                                            "IC"));

        SVAna->items.push_back(createSVAnaI(index,
                                            QString("EN_SV_REMOTE%1_3IN0").arg(i+1),
                                            QString("%1n侧电流SV接收通道3In0").arg(desc),
                                            "3I0"));
    }
}

void CParaToCfgDialog::CreateGSFLTConfig()
{
    int index = 0;
    AppendGSAnaConfig(m_para->ptPara,index);
}

void CParaToCfgDialog::CreateOtherConfig()
{
    BaseTab * other = &m_data->GetConfig()->otherAnaConfig;
    int index = 0;

    for (int i = 0; i < 8; i++)
    {
        other->items.push_back(creatSFPPower(index,QString("EN_OTHER_SFP%1POWER_RX").arg(i+1),
                                             QString("光口%1接收光功率").arg(i+1)));
        other->items.push_back(creatSFPPower(index,QString("EN_OTHER_SFP%1POWER_TX").arg(i+1),
                                             QString("光口%1发送光功率").arg(i+1)));
    }
}

void CParaToCfgDialog::CreateGeneralBiConfig()
{
    BaseTab * generalBi = &m_data->GetConfig()->generalBiConfig;
    int index = 0;

    generalBi->items.push_back(createGeneralBi(index,"EN_BI_CHECK","投装置检修",0x21F));
    generalBi->items.push_back(createGeneralBi(index,"EN_BI_FUGUI","外部复归",0x41F));
    generalBi->items.push_back(createGeneralBi(index,"EN_BI_BAK2","电池活化",0x1F));
    generalBi->items.push_back(createGeneralBi(index,"EN_BI_BAK3","电池欠压",0x1F));
    generalBi->items.push_back(createGeneralBi(index,"EN_BI_BAK4","电池故障",0x1F));
    generalBi->items.push_back(createGeneralBi(index,"EN_BI_REMOTE","投远方操作",0x401F));
    generalBi->items.push_back(createGeneralBi(index,"EN_BI_BAK6","备用",0x1F));
    generalBi->items.push_back(createGeneralBi(index,"EN_BI_BAK7","备用",0x1F));

    AppendGeneralBiConfig(m_para->ptPara,index);
}

void CParaToCfgDialog::CreateSignal()
{
    int index = 0;
    AppendSignalConfig(m_para->ptPara,index);
}

void CParaToCfgDialog::CreateGooseBi()
{
    int index = 0;
    AppendGooseBiConfig(m_para->ptPara,index);
}

void CParaToCfgDialog::CreateTripMatrix()
{
    BaseTab * tripMaTrix = &m_data->GetConfig()->tripMaxtrixConfig;
    if(ui->comboBox->currentIndex() == 0)
    {
        for (int i = 0; i < 4 ; i++)
        {
            TripMatrixItem * item = new TripMatrixItem(i);
            item->sName = QString("BO%1").arg(i+1);
            item->sDesc = QString("出口%1").arg(i+1);
            item->dwAttr = 0x7;
            item->wBoardIndex = 1;
            item->wBOFlag = 0x1 << i;
            tripMaTrix->items.push_back(item);
        }
    }
    else
    {
        for (int i = 6; i < 10 ; i++)
        {
            for (int j = 0; j < 11; j++)
            {
                TripMatrixItem * item = new TripMatrixItem(11*(i-6) + j);
                item->sName = QString("BO%1_%2").arg(i).arg(j+1);
                item->sDesc = QString("板%1出口%2").arg(i).arg(j+1);
                item->dwAttr = 0x7;
                item->wBoardIndex = 1;
                item->wBOFlag = 0x1 << j;
                tripMaTrix->items.push_back(item);
            }
        }
    }
}

void CParaToCfgDialog::CreateSPSetConfig()
{
    BaseTab * SPSet = &m_data->GetConfig()->settingSPConifg;
    int index = 0;
    //被保护电房,固定放在第一个
    SPSet->items.push_back(createSTRSet(index, "EN_SET_NAME_DEV","被保护区域"));
    SPSet->items.push_back(createZoneNumSet(index));
    //PT一二次变比
    SPSet->items.push_back(createPTPrimary(index));
    SPSet->items.push_back(createPTSecondary(index));
    //基准CT一二次变比
    SPSet->items.push_back(createCTPrimary(index,"EN_SET_I_BASE_PRIMARY_I","基准CT一次值"));
    SPSet->items.push_back(createCTSecondary(index,"EN_SET_I_BASE_SECOND_I","基准CT二次值"));
    //AD采集的一二次变比、极性
    m_ADCurCTParaStartIndex = index;
    int numADCurGroup = 0;
    if(ui->comboBox->currentIndex() == 1)
        numADCurGroup = DEV_8220_ADCURGROUP_NUM;
    for (int i = 0; i < numADCurGroup; i++)
    {
        SPSet->items.push_back(createCTPrimary(index,QString("EN_SET_I_ANA_GROUP%1_PRIMARY_I").arg(i+1),QString("第%1组AD采集电流CT一次值").arg(i+1)));
        SPSet->items.push_back(createCTSecondary(index,QString("EN_SET_I_ANA_GROUP%1_SECOND_I").arg(i+1),QString("第%1组AD采集电流CT二次值").arg(i+1)));

        SPSet->items.push_back(createCTPrimary(index,QString("EN_SET_I_ANA_GROUP%1_PRIMARY_3I0").arg(i+1),QString("第%1组AD采集零序电流CT一次值").arg(i+1)));
        SPSet->items.push_back(createCTSecondary(index,QString("EN_SET_I_ANA_GROUP%1_SECOND_3I0").arg(i+1),QString("第%1组AD采集零序电流CT二次值").arg(i+1)));

        SPSet->items.push_back(createCTLSet(index,QString("EN_SET_CTL_ANA_GROUP%1_IA_POSITIVE").arg(i+1),QString("第%1组AD采集A相电流极性").arg(i+1)));
        SPSet->items.push_back(createCTLSet(index,QString("EN_SET_CTL_ANA_GROUP%1_IB_POSITIVE").arg(i+1),QString("第%1组AD采集B相电流极性").arg(i+1)));
        SPSet->items.push_back(createCTLSet(index,QString("EN_SET_CTL_ANA_GROUP%1_IC_POSITIVE").arg(i+1),QString("第%1组AD采集C相电流极性").arg(i+1)));
        SPSet->items.push_back(createCTLSet(index,QString("EN_SET_CTL_ANA_GROUP%1_3I0_POSITIVE").arg(i+1),QString("第%1组AD采集零序电流极性").arg(i+1)));
    }
    //SV采集的极性
    if(ui->comboBox->currentIndex() == 0)
    {
        for (int i = 0; i < m_swicthes.size();i++)
        {
            QString key = QString::fromStdString(m_swicthes[i]->key).toUpper() + QString::number(i+1);
            QString desc = QString::fromStdString(m_swicthes[i]->desc);

            SPSet->items.push_back(createCTLSet(index,QString("EN_SET_CTL_SV_%1_IA_POSITIVE").arg(key),QString("%1SV采集A相电流极性").arg(desc)));
            SPSet->items.push_back(createCTLSet(index,QString("EN_SET_CTL_SV_%1_IB_POSITIVE").arg(key),QString("%1SV采集B相电流极性").arg(desc)));
            SPSet->items.push_back(createCTLSet(index,QString("EN_SET_CTL_SV_%1_IC_POSITIVE").arg(key),QString("%1SV采集C相电流极性").arg(desc)));
            SPSet->items.push_back(createCTLSet(index,QString("EN_SET_CTL_SV_%1_3I0_POSITIVE").arg(key),QString("%1SV采集零序电流极性").arg(desc)));

            for (unsigned int j = 0; j < m_swicthes[i]->items.size(); j++) {
                if(QString::fromStdString(m_swicthes[i]->items[j]->name).startsWith("EN_SPSET_CTL_POWERPOSITIVE"))
                {
                    SPSet->items.push_back(createCTLSet(index,QString::fromStdString(m_swicthes[i]->items[j]->name),
                                                        QString::fromStdString(m_swicthes[i]->items[j]->desc)));
                }
            }
        }

        for (int i = 0; i < m_transformers.size();i++)
        {
            for (unsigned int j = 0; j < m_transformers[i]->items.size(); j++) {
                if(QString::fromStdString(m_transformers[i]->items[j]->name).startsWith("EN_SPSET_U_HPTPRIMARY"))
                {
                    SettingItem * hPrimary = new SettingItem(index++);
                    hPrimary->sDesc = QString::fromStdString(m_transformers[i]->items[j]->desc);
                    hPrimary->sName = QString::fromStdString(m_transformers[i]->items[j]->name);
                    hPrimary->dwAttr = 0xF;

                    hPrimary->wValMin = 400;
                    hPrimary->wValMax = 10000;
                    hPrimary->wValDft = 10000;
                    hPrimary->wValStep = 100;

                    hPrimary->sType = "VOL";
                    hPrimary->byWidth = 5;
                    hPrimary->byDotBit = 0;

                    hPrimary->sUnit = "V";
                    hPrimary->sKiloUnit = "kV";
                    SPSet->items.push_back(hPrimary);
                }
                else if(QString::fromStdString(m_transformers[i]->items[j]->name).startsWith("EN_SPSET_U_LPTPRIMARY"))
                {
                    SettingItem * lPrimary = new SettingItem(index++);
                    lPrimary->sDesc = QString::fromStdString(m_transformers[i]->items[j]->desc);
                    lPrimary->sName = QString::fromStdString(m_transformers[i]->items[j]->name);
                    lPrimary->dwAttr = 0xF;

                    lPrimary->wValMin = 400;
                    lPrimary->wValMax = 10000;
                    lPrimary->wValDft = 400;
                    lPrimary->wValStep = 100;

                    lPrimary->sType = "VOL";
                    lPrimary->byWidth = 5;
                    lPrimary->byDotBit = 0;

                    lPrimary->sUnit = "V";
                    lPrimary->sKiloUnit = "kV";
                    SPSet->items.push_back(lPrimary);
                }
                else if(QString::fromStdString(m_transformers[i]->items[j]->name).startsWith("EN_SPSET_CNNMODE"))
                {
                    SettingItem * cnnMode = new SettingItem(index++);
                    cnnMode->sDesc = QString::fromStdString(m_transformers[i]->items[j]->desc);
                    cnnMode->sName = QString::fromStdString(m_transformers[i]->items[j]->name);
                    cnnMode->dwAttr = 0xF;

                    cnnMode->wValMin = 0;
                    cnnMode->wValMax = 2;
                    cnnMode->wValDft = 0;
                    cnnMode->wValStep = 100;

                    cnnMode->sType = "NULL";
                    cnnMode->byWidth = 1;
                    cnnMode->byDotBit = 0;

                    cnnMode->sUnit = "N";
                    cnnMode->sKiloUnit = "N";
                    SPSet->items.push_back(cnnMode);
                }
            }
        }
    }
}

void CParaToCfgDialog::CreateSGSetConfig()
{
    AppendSGSetConfig(m_para->ptPara);
}

void CParaToCfgDialog::CreateSoftYBConfig()
{
    BaseTab * softYB = &m_data->GetConfig()->softYBConfig;
    int index = 0;

    for (unsigned int i = 0; i < m_para->ptPara->items.size(); i++) {
        tdsItem * item = m_para->ptPara->items[i];
        if(QString::fromStdString(item->name).startsWith("EN_SOFTYB_"))
        {
            softYB->items.push_back(createSoftYB(index,QString::fromStdString(item->name)
                                                 ,QString::fromStdString(item->desc),0x7,0));
        }
    }
    softYB->items.push_back(createSoftYB(index,"EN_SOFTYB_RMT_CTL_SYB","远方投退压板",0x4F,0));
    softYB->items.push_back(createSoftYB(index,"EN_SOFTYB_RMT_SWITCH_SET","远方定值切换",0x2F,0));
    softYB->items.push_back(createSoftYB(index,"EN_SOFTYB_RMT_MODIFY_SET","远方定值修改",0x1F,0));
}

void CParaToCfgDialog::CreateEvt()
{
    AppendEvtConfig(m_para->ptPara);
}

void CParaToCfgDialog::CreateRmt()
{
    if(ui->comboBox->currentIndex() == 0)
        return;
    BaseTab * gsIn = &m_data->GetConfig()->gooseBiConfig;
    BaseTab * rmt = &m_data->GetConfig()->remoteTripConfig;

    for (int i = 0; i < m_swicthes.size(); i++)
    {
        int biIndex = gsIn->items.size();
        BiItem * biTrip = new BiItem(biIndex);
        biTrip->sName = QString("EN_GSIN_RMT_SWITCH%1_TRIP").arg(i+1);
        biTrip->sDesc = QString("开关%1远方跳闸开入").arg(i+1);
        biTrip->dwAttr = 0x2F;
        biTrip->sChanType = "SPS";
        gsIn->items.push_back(biTrip);

        EvtItem * evtTrip = new EvtItem(rmt->items.size());
        evtTrip->sName= QString("EN_RMT_SWITCH%1_TRIP").arg(i+1);
        evtTrip->sDesc = QString("开关%1远方跳闸开入").arg(i+1);
        evtTrip->sCnnGSIn = QString("GSBI:%1").arg(biIndex);
        rmt->items.push_back(evtTrip);


        BiItem * biClose = new BiItem(biIndex+1);
        biClose->sName = QString("EN_GSIN_RMT_SWITCH%1_CLOSE").arg(i+1);
        biClose->sDesc = QString("开关%1远方合闸开入").arg(i+1);
        biClose->dwAttr = 0x2F;
        biClose->sChanType = "SPS";
        gsIn->items.push_back(biClose);

        EvtItem * evtClose = new EvtItem(rmt->items.size());
        evtClose->sName= QString("EN_RMT_SWITCH%1_CLOSE").arg(i+1);
        evtClose->sDesc = QString("开关%1远方合闸开入").arg(i+1);
        evtClose->sCnnGSIn = QString("GSBI:%1").arg(biIndex+1);
        rmt->items.push_back(evtClose);
    }
}

AnaItem *CParaToCfgDialog::createADAnaU(int &index, QString name, QString desc, QString flag)
{
    AnaItem * ana = new AnaItem(index++);
    ana->sName = name;
    ana->sDesc = desc;
    ana->dwAttr = 0xF;
    ana->sChanType = "AC";
    ana->sChanFlag = flag;
    ana->iXuYCDft = 57735;
    ana->wCoe = 154;
    ana->sRateSetP = "SPSET:2";
    ana->sRateSetS = "SPSET:3";
    ana->sUnit = "V";
    ana->sKiloUnit = "kV";
    return ana;
}

AnaItem *CParaToCfgDialog::createADAnaI(int &index, QString name, QString desc, QString flag, QString setP, QString setS)
{
    AnaItem * ana = new AnaItem(index++);
    ana->sName = name;
    ana->sDesc = desc;
    ana->dwAttr = 0xF;
    ana->sChanType = "AC";
    ana->sChanFlag = flag;
    ana->iXuYCDft = 1000;
    ana->wCoe = 153;
    ana->sRateSetP = setP;
    ana->sRateSetS = setS;
    ana->sUnit = "A";
    ana->sKiloUnit = "kA";
    return ana;
}

AnaItem *CParaToCfgDialog::createSVAnaU(int &index, QString name, QString desc, QString flag)
{
    AnaItem * ana = new AnaItem(index++);
    ana->sName = name;
    ana->sDesc = desc;
    ana->dwAttr = 0xF;
    ana->sChanType = "AC";
    ana->sChanFlag = flag;
    ana->iXuYCDft = 57735;
    ana->wCoe = 100;
    ana->sRateSetP = "SPSET:2";
    ana->sRateSetS = "SPSET:3";
    ana->sUnit = "V";
    ana->sKiloUnit = "kV";
    return ana;
}

AnaItem *CParaToCfgDialog::createSVAnaI(int &index, QString name, QString desc, QString flag)
{
    AnaItem * ana = new AnaItem(index++);
    ana->sName = name;
    ana->sDesc = desc;
    ana->dwAttr = 0xF;
    ana->sChanType = "AC";
    ana->sChanFlag = flag;
    ana->iXuYCDft = 1000;
    ana->wCoe = 1000;
    ana->sRateSetP = "SPSET:4";
    ana->sRateSetS = "SPSET:5";
    ana->sUnit = "A";
    ana->sKiloUnit = "kA";
    return ana;
}

AnaItem *CParaToCfgDialog::createDrvU(int &index, QString name, QString desc, QString flag)
{
    AnaItem * ana = new AnaItem(index++);
    ana->sName = name;
    ana->sDesc = desc;
    ana->dwAttr = 0x7;
    ana->sChanType = "AC";
    ana->sChanFlag = flag;
    ana->iXuYCDft = 57735;
    ana->wCoe = 100;
    ana->sRateSetP = "SPSET:2";
    ana->sRateSetS = "SPSET:3";
    ana->sUnit = "V";
    ana->sKiloUnit = "kV";
    return ana;
}

AnaItem *CParaToCfgDialog::createDrvI(int &index, QString name, QString desc, QString flag)
{
    AnaItem * ana = new AnaItem(index++);
    ana->sName = name;
    ana->sDesc = desc;
    ana->dwAttr = 0x7;
    ana->sChanType = "AC";
    ana->sChanFlag = flag;
    ana->iXuYCDft = 1000;
    ana->wCoe = 1000;
    ana->sRateSetP = "SPSET:4";
    ana->sRateSetS = "SPSET:5";
    ana->sUnit = "A";
    ana->sKiloUnit = "kA";
    return ana;
}

AnaItem *CParaToCfgDialog::createDrvP(int &index, QString name, QString desc)
{
    AnaItem * ana = new AnaItem(index++);
    ana->sName = name;
    ana->sDesc = desc;
    ana->dwAttr = 0x7;
    ana->sChanType = "DC";
    ana->sChanFlag = "P";
    ana->iXuYCDft = 1000;
    ana->wCoe = 1000;
    ana->byWidth = 7;
    ana->byDotBit = 3;
    ana->sUnit = "W";
    ana->sKiloUnit = "MW";
    return ana;
}

AnaItem *CParaToCfgDialog::createGSAnaP(int &index, QString name, QString desc)
{
    AnaItem * ana = new AnaItem(index++);
    ana->sName = name;
    ana->sDesc = desc;
    ana->dwAttr = 0xF;
    ana->sChanType = "DC";
    ana->sChanFlag = "P";
    ana->iXuYCDft = 1000;
    ana->wCoe = 1000;
    ana->byWidth = 7;
    ana->byDotBit = 3;
    ana->sUnit = "W";
    ana->sKiloUnit = "MW";
    return ana;
}

AnaItem *CParaToCfgDialog::creatSFPPower(int &index, QString name, QString desc)
{
    AnaItem * ana = new AnaItem(index++);
    ana->sName = name;
    ana->sDesc = desc;
    ana->dwAttr = 0xF;
    ana->sChanType = "DC";
    ana->sChanFlag = "P";
    ana->iXuYCDft = 1000;
    ana->wCoe = 1000;
    ana->byWidth = 7;
    ana->byDotBit = 3;
    ana->sUnit = "mW";
    ana->sKiloUnit = "mW";
    return ana;
}

BiItem *CParaToCfgDialog::createGeneralBi(int &index, QString name, QString desc, unsigned int dwAttr)
{
    BiItem * bi = new BiItem(index++);
    bi->sName = name;
    bi->sDesc = desc;
    bi->dwAttr = dwAttr;
    return bi;
}

BiItem *CParaToCfgDialog::createSignalBi(int &index, QString name, QString desc)
{
    BiItem * bi = new BiItem(index++);
    bi->sName = name;
    bi->sDesc = desc;
    bi->dwAttr = 0x27;
    return bi;
}

BiItem *CParaToCfgDialog::createGooseBi(int &index, QString name, QString desc)
{
    BiItem * bi = new BiItem(index++);
    bi->sName = name;
    bi->sDesc = desc;
    bi->dwAttr = 0x2F;
    return bi;
}

AnaItem *CParaToCfgDialog::createDrvFR(int &index, QString name, QString desc)
{
    AnaItem * ana = new AnaItem(index++);
    ana->sName = name;
    ana->sDesc = desc;
    ana->dwAttr = 0x7;
    ana->sChanType = "DC";
    ana->sChanFlag = "FR";
    ana->iXuYCDft = 5000;
    ana->wCoe = 100;
    ana->byWidth = 5;
    ana->byDotBit = 2;
    ana->sUnit = "Hz";
    ana->sKiloUnit = "Hz";
    return ana;
}

SettingItem *CParaToCfgDialog::createSTRSet(int &index, QString name, QString desc)
{
    SettingItem * set = new SettingItem(index++);
    set->sDesc = desc;
    set->sName = name;
    set->dwAttr = 0xF;

    set->sType = "STR";
    set->byWidth = 16;
    set->byDotBit = 0;

    return set;
}

SettingItem *CParaToCfgDialog::createZoneNumSet(int &index)
{
    SettingItem * set = new SettingItem(index++);
    set->sDesc = "定值区号";
    set->sName = "EN_SET_GROUP_NO";
    set->dwAttr = 0xF;

    set->wValMin = 1;
    set->wValMax = m_data->GetConfig()->deviceParas.wZoneNum;
    set->wValDft = 1;

    set->sType = "NULL";
    set->byWidth = 2;
    set->byDotBit = 0;

    return set;
}

SettingItem *CParaToCfgDialog::createPTPrimary(int &index)
{
    SettingItem * set = new SettingItem(index++);
    set->sDesc = "PT一次值";
    set->sName = "EN_SET_U_BASE_PRIMARY_U";
    set->dwAttr = 0xF;

    set->wValMin = 1;
    set->wValMax = 220;
    set->wValDft = 10;

    set->sType = "PTCT";
    set->byWidth = 3;
    set->byDotBit = 0;

    set->sUnit = "kV";
    set->sKiloUnit = "kV";

    return set;
}

SettingItem *CParaToCfgDialog::createPTSecondary(int &index)
{
    SettingItem * set = new SettingItem(index++);
    set->sDesc = "PT二次值";
    set->sName = "EN_SET_U_BASE_SECOND_U";
    set->dwAttr = 0xE;

    set->wValMin = 100;
    set->wValMax = 100;
    set->wValDft = 100;

    set->sType = "PTCT";
    set->byWidth = 3;
    set->byDotBit = 0;

    set->sUnit = "V";
    set->sKiloUnit = "kV";

    return set;
}

SettingItem *CParaToCfgDialog::createCTPrimary(int &index, QString name, QString desc)
{
    SettingItem * set = new SettingItem(index++);
    set->sDesc = desc;
    set->sName = name;
    set->dwAttr = 0xF;

    set->wValMin = 1;
    set->wValMax = 9999;
    set->wValDft = 1000;

    set->sType = "PTCT";
    set->byWidth = 4;
    set->byDotBit = 0;

    set->sUnit = "A";
    set->sKiloUnit = "A";

    return set;
}

SettingItem *CParaToCfgDialog::createCTSecondary(int &index, QString name, QString desc)
{
    SettingItem * set = new SettingItem(index++);
    set->sDesc = desc;
    set->sName = name;
    set->dwAttr = 0xF;

    set->wValMin = 1;
    set->wValMax = 5;
    set->wValDft = 5;
    set->wValStep = 4;

    set->sType = "PTCT";
    set->byWidth = 1;
    set->byDotBit = 0;

    set->sUnit = "A";
    set->sKiloUnit = "A";

    return set;
}

SettingItem *CParaToCfgDialog::createCTLSet(int &index, QString name, QString desc)
{
    SettingItem * set = new SettingItem(index++);
    set->sDesc = desc;
    set->sName = name;
    set->dwAttr = 0xF;

    set->wValMin = 0;
    set->wValMax = 1;
    set->wValDft = 1;

    set->sType = "CLT";
    set->byWidth = 1;
    set->byDotBit = 0;

    set->sUnit = "N";
    set->sKiloUnit = "N";

    return set;
}

SoftYBItem *CParaToCfgDialog::createSoftYB(int &index, QString name, QString desc, unsigned int dwAttr, unsigned int dftVal)
{
    SoftYBItem * soft = new SoftYBItem(index++);
    soft->sName = name;
    soft->sDesc = desc;
    soft->dwAttr = dwAttr;
    soft->wDftVal = dftVal;
    return soft;
}

void CParaToCfgDialog::AppendDrvConfig(tdsPara *para , int &index)
{
    BaseTab * drvAna = &m_data->GetConfig()->derivedConfig;

    for (unsigned int i = 0; i < para->items.size(); i++)
    {
        tdsItem * item = para->items[i];
        if(QString::fromStdString(item->name).startsWith("EN_DRV_U")
                ||QString::fromStdString(item->name).startsWith("EN_DRV_3U0"))
        {
            QStringList list = QString::fromStdString(item->name).split("_");
            QString flag = "U";
            if(list.size() > 3)
                flag = list[2];

            drvAna->items.push_back(createDrvU(index,QString::fromStdString(item->name),QString::fromStdString(item->desc),flag));
        }
        else if(QString::fromStdString(item->name).startsWith("EN_DRV_I")
                ||QString::fromStdString(item->name).startsWith("EN_DRV_3I"))
        {
            QStringList list = QString::fromStdString(item->name).split("_");
            QString flag = "I";
            if(list.size() > 3)
                flag = list[2];

            drvAna->items.push_back(createDrvI(index,QString::fromStdString(item->name),QString::fromStdString(item->desc),flag));
        }
        else if(QString::fromStdString(item->name).startsWith("EN_DRV_P"))
        {
            if(ui->comboBox->currentIndex() == 1)
            {
                BaseTab * gsbo = &m_data->GetConfig()->gooseBoConfig;
                GooseBOItem * bo = new GooseBOItem(gsbo->items.size());
                bo->sName = "EN_GSOUT" + QString::fromStdString(item->name).remove(0,6);
                bo->sDesc = QString::fromStdString(item->desc);
                bo->dwAttr = 0x3;
                bo->sHWChan = "DRV:" + QString::number(index);
                gsbo->items.push_back(bo);
            }

            drvAna->items.push_back(createDrvP(index,QString::fromStdString(item->name),QString::fromStdString(item->desc)));
        }
        else if(QString::fromStdString(item->name).startsWith("EN_DRV_FR"))
        {
            drvAna->items.push_back(createDrvFR(index,QString::fromStdString(item->name),QString::fromStdString(item->desc)));
        }
    }

    for (unsigned int i = 0; i < para->sons.size(); i++)
    {
        tdsPara * son = para->sons[i];
        AppendDrvConfig(son,index);
    }
}

void CParaToCfgDialog::AppendGSAnaConfig(tdsPara *para, int &index)
{
    BaseTab * gsAna = &m_data->GetConfig()->gsAnaConfig;
    BaseTab * gsBi = &m_data->GetConfig()->gooseBiConfig;

    for (unsigned int i = 0; i < para->items.size(); i++)
    {
        tdsItem * item = para->items[i];
        if(QString::fromStdString(item->name).startsWith("EN_GSANA_POWER"))
        {

            BiItem * bi = new BiItem(gsBi->items.size());
            bi->sName = "EN_GSIN" + QString::fromStdString(item->name).remove(0,8);
            bi->sDesc = QString::fromStdString(item->desc);
            bi->dwAttr = 0xF;
            bi->sChanType = "FLS";
            bi->sIndexAna = "GSFLT:" + QString::number(index);
            gsBi->items.push_back(bi);
            gsAna->items.push_back(createGSAnaP(index,QString::fromStdString(item->name),QString::fromStdString(item->desc)));

        }
    }

    for (unsigned int i = 0; i < para->sons.size(); i++)
    {
        tdsPara * son = para->sons[i];
        AppendGSAnaConfig(son,index);
    }
}

void CParaToCfgDialog::AppendGeneralBiConfig(tdsPara *para, int &index)
{
    BaseTab * generalBi = &m_data->GetConfig()->generalBiConfig;
    BaseTab * gsbo = &m_data->GetConfig()->gooseBoConfig;

    for (unsigned int i = 0; i < para->items.size(); i++)
    {
        tdsItem * item = para->items[i];
        if(QString::fromStdString(item->name).startsWith("EN_BI"))
        {
            if(ui->comboBox->currentIndex() == 1)
            {
                GooseBOItem * bo = new GooseBOItem(gsbo->items.size());
                bo->sName = "EN_GSOUT" + QString::fromStdString(item->name).remove(0,5);
                bo->sDesc = QString::fromStdString(item->desc);
                bo->dwAttr = 0x3;
                bo->sHWChan = "BI:" + QString::number(index);
                gsbo->items.push_back(bo);
            }

            generalBi->items.push_back(createGeneralBi(index,QString::fromStdString(item->name),QString::fromStdString(item->desc),0x1F));
        }
    }

    for (unsigned int i = 0; i < para->sons.size(); i++)
    {
        tdsPara * son = para->sons[i];
        AppendGeneralBiConfig(son,index);
    }
}

void CParaToCfgDialog::AppendSignalConfig(tdsPara *para, int &index)
{
    BaseTab * signal = &m_data->GetConfig()->signalConfig;
    BaseTab * gsbo = &m_data->GetConfig()->gooseBoConfig;

    for (unsigned int i = 0; i < para->items.size(); i++)
    {
        tdsItem * item = para->items[i];
        if(QString::fromStdString(item->name).startsWith("EN_SIGNAL"))
        {
            if(ui->comboBox->currentIndex() == 1)
            {
                if(QString::fromStdString(item->name).startsWith("EN_SIGNAL_HAVEU")
                        ||QString::fromStdString(item->name).startsWith("EN_SIGNAL_COMPU")
                        ||QString::fromStdString(item->name).startsWith("EN_SIGNAL_CURRENT")
                        ||QString::fromStdString(item->name).startsWith("EN_SIGNAL_OVERZ")
                        ||QString::fromStdString(item->name).startsWith("EN_SIGNAL_DIFF")
                        ||QString::fromStdString(item->name).startsWith("EN_SIGNAL_PERMIT")
                        ||QString::fromStdString(item->name).startsWith("EN_SIGNAL_HAVEI"))
                {
                    GooseBOItem * bo = new GooseBOItem(gsbo->items.size());
                    bo->sName = "EN_GSOUT" + QString::fromStdString(item->name).remove(0,9);
                    bo->sDesc = QString::fromStdString(item->desc);
                    bo->dwAttr = 0x3;
                    bo->sHWChan = "SIGNAL:" + QString::number(index);
                    gsbo->items.push_back(bo);
                }
            }
            else
            {
                if(QString::fromStdString(item->name).startsWith("EN_SIGNAL_OPER"))
                {
                    GooseBOItem * bo = new GooseBOItem(gsbo->items.size());
                    bo->sName = "EN_GSOUT" + QString::fromStdString(item->name).remove(0,9);
                    bo->sDesc = QString::fromStdString(item->desc);
                    bo->dwAttr = 0x3;
                    bo->sHWChan = "SIGNAL:" + QString::number(index);
                    gsbo->items.push_back(bo);
                }
            }

            signal->items.push_back(createSignalBi(index,QString::fromStdString(item->name),QString::fromStdString(item->desc)));
        }
    }

    for (unsigned int i = 0; i < para->sons.size(); i++)
    {
        tdsPara * son = para->sons[i];
        AppendSignalConfig(son,index);
    }
}

void CParaToCfgDialog::AppendGooseBiConfig(tdsPara *para, int &index)
{
    BaseTab * goosebi = &m_data->GetConfig()->gooseBiConfig;

    for (unsigned int i = 0; i < para->items.size(); i++)
    {
        tdsItem * item = para->items[i];
        if(QString::fromStdString(item->name).startsWith("EN_GSIN"))
        {
            goosebi->items.push_back(createGooseBi(index,QString::fromStdString(item->name),QString::fromStdString(item->desc)));
        }
    }

    for (unsigned int i = 0; i < para->sons.size(); i++)
    {
        tdsPara * son = para->sons[i];
        AppendGooseBiConfig(son,index);
    }
}

void CParaToCfgDialog::AppendEvtConfig(tdsPara *para)
{
    BaseTab * alm = &m_data->GetConfig()->evtAlmConfig;
    BaseTab * act = &m_data->GetConfig()->evtActConfig;

    for (unsigned int i = 0; i < para->items.size(); i++)
    {
        tdsItem * item = para->items[i];
        if(QString::fromStdString(item->name).startsWith("EN_ALM"))
        {
            EvtItem * evt = new EvtItem(alm->items.size());
            evt->sName = QString::fromStdString(item->name);
            evt->sDesc = QString::fromStdString(item->desc);
            evt->dwAttr = 0x7;
            evt->boLight = 0x2;
            if(ui->comboBox->currentIndex() == 0)
                evt->boConfig1 = 0x2;
            alm->items.push_back(evt);
        }
        else if(QString::fromStdString(item->name).startsWith("EN_ACT"))
        {
            EvtItem * evt = new EvtItem(act->items.size());
            evt->sName = QString::fromStdString(item->name);
            evt->sDesc = QString::fromStdString(item->desc);
            evt->dwAttr = 0x7;
            if(evt->sName != "EN_ACT_RLYSTART")
            {
                evt->boLight = 0x1;
                if(ui->comboBox->currentIndex() == 0)
                    evt->boConfig1 = 0x1;
            }
            act->items.push_back(evt);
        }
    }

    for (unsigned int i = 0; i < para->sons.size(); i++)
    {
        tdsPara * son = para->sons[i];
        AppendEvtConfig(son);
    }
}

void CParaToCfgDialog::AppendSGSetConfig(tdsPara *para)
{
    BaseTab * sgSet = &m_data->GetConfig()->settingSGConifg;

    for (unsigned int i = 0; i < para->items.size(); i++)
    {
        tdsItem * item = para->items[i];
        QString name = QString::fromStdString(item->name);

        if(name.startsWith("EN_SET"))
        {
            SettingItem * set = new SettingItem(sgSet->items.size());
            set->dwAttr = 0x7;
            set->sName = name;
            set->sDesc = QString::fromStdString(item->desc);

            if(name.startsWith("EN_SET_CTL"))
            {
                set->wValMin = 0;
                set->wValMax = 1;
                set->wValDft = 0;
                set->sType = "CTL";
                set->byWidth = 1;
                set->byDotBit = 0;
            }
            else if(name.startsWith("EN_SET_T"))
            {
                set->wValMin = 0;
                set->wValMax = 99999;
                set->wValDft = 300;
                set->sType = "TIME";
                set->byWidth = 6;
                set->byDotBit = 3;
                set->sUnit = "s";
                set->sKiloUnit = "s";
            }
            else if(name.startsWith("EN_SET_I"))
            {
                set->wValMin = 0;
                set->wValMax = 30000;
                set->wValDft = 1000;
                set->sType = "CUR";
                set->wCoeRet = 95;
                set->sSecIn = "SPSET:3";
                set->byWidth = 6;
                set->byDotBit = 3;
                set->sUnit = "A";
                set->sKiloUnit = "kA";
            }
            else if(name.startsWith("EN_SET_MIN_U"))
            {
                set->wValMin = 0;
                set->wValMax = 15000;
                set->wValDft = 30;
                set->sType = "VOL";
                set->wCoeRet = 105;
                set->byWidth = 6;
                set->byDotBit = 3;
                set->sUnit = "V";
                set->sKiloUnit = "kV";
            }
            else if(name.startsWith("EN_SET_MAX_U"))
            {
                set->wValMin = 0;
                set->wValMax = 15000;
                set->wValDft = 120;
                set->sType = "VOL";
                set->wCoeRet = 95;
                set->byWidth = 6;
                set->byDotBit = 3;
                set->sUnit = "V";
                set->sKiloUnit = "kV";
            }
            else if(name.startsWith("EN_SET_MIN_Fr"))
            {
                set->wValMin = 4500;
                set->wValMax = 5500;
                set->wValDft = 4900;
                set->wCoeRet = 105;
                set->byWidth = 5;
                set->byDotBit = 2;
                set->sUnit = "Hz";
                set->sKiloUnit = "Hz";
            }
            else if(name.startsWith("EN_SET_MAX_Fr"))
            {
                set->wValMin = 4500;
                set->wValMax = 5500;
                set->wValDft = 5100;
                set->wCoeRet = 95;
                set->byWidth = 5;
                set->byDotBit = 2;
                set->sUnit = "Hz";
                set->sKiloUnit = "Hz";
            }
            else if(name.startsWith("EN_SET_MAX_3U0"))
            {
                set->wValMin = 0;
                set->wValMax = 100000;
                set->wValDft = 20000;
                set->wCoeRet = 95;
                set->byWidth = 6;
                set->byDotBit = 3;
                set->sUnit = "V";
                set->sKiloUnit = "kV";
            }
            else if(name.startsWith("EN_SET_UL"))
            {
                set->wValMin = 0;
                set->wValMax = 150000;
                set->wValDft = 90000;
                set->wCoeRet = 105;
                set->byWidth = 6;
                set->byDotBit = 3;
                set->sUnit = "V";
                set->sKiloUnit = "kV";
            }
            else if(name.startsWith("EN_SET_U2"))
            {
                set->wValMin = 0;
                set->wValMax = 150000;
                set->wValDft = 8000;
                set->wCoeRet = 95;
                set->byWidth = 6;
                set->byDotBit = 3;
                set->sUnit = "V";
                set->sKiloUnit = "kV";
            }
            else if(name.startsWith("EN_SET_3U0"))
            {
                set->wValMin = 0;
                set->wValMax = 150000;
                set->wValDft = 20000;
                set->wCoeRet = 95;
                set->byWidth = 6;
                set->byDotBit = 3;
                set->sUnit = "V";
                set->sKiloUnit = "kV";
            }
            else if(name.startsWith("EN_SET_P"))
            {
                set->wValMin = 0;
                set->wValMax = 3000000;
                set->wValDft = 100000;
                set->wCoeRet = 105;
                set->byWidth = 7;
                set->byDotBit = 3;
                set->sUnit = "W";
                set->sKiloUnit = "MW";
            }

            sgSet->items.push_back(set);
        }

    }

    for (unsigned int i = 0; i < para->sons.size(); i++)
    {
        tdsPara * son = para->sons[i];
        AppendSGSetConfig(son);
    }
}
