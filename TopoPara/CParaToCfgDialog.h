#ifndef CPARATOCFGDIALOG_H
#define CPARATOCFGDIALOG_H

#include <QDialog>
#include "CDataBase.h"
#include "ParaStruct.h"

namespace Ui {
class CParaToCfgDialog;
}

class CParaToCfgDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CParaToCfgDialog(tdsTopoPara * para , QWidget *parent = nullptr);
    ~CParaToCfgDialog();

    void Save(const QString &fileName);
private:
    Ui::CParaToCfgDialog *ui;
    tdsTopoPara * m_para;
    CDataBase   * m_data;

    QVector<tdsPara *> m_sources;
    QVector<tdsPara *> m_busbars;
    QVector<tdsPara *> m_lines;
    QVector<tdsPara *> m_transformers;
    QVector<tdsPara *> m_loads;
    QVector<tdsPara *> m_remotes;
    QVector<tdsPara *> m_swicthes;

    int m_ADCurCTParaStartIndex;

    void CreateDeviceInfo();
    void CreateDevicePara();

    void CreateAnaConfig();
    void CreateDrvConfig();
    void CreateSVConfig();
    void CreateGSFLTConfig();
    void CreateOtherConfig();

    void CreateGeneralBiConfig();
    void CreateSignal();
    void CreateGooseBi();

    void CreateTripMatrix();

    void CreateSPSetConfig();
    void CreateSGSetConfig();
    void CreateSoftYBConfig();

    void CreateEvt();
    void CreateRmt();

    AnaItem * createADAnaU(int &index, QString name, QString desc, QString flag);
    AnaItem * createADAnaI(int &index, QString name, QString desc, QString flag, QString setP, QString setS);

    AnaItem * createSVAnaU(int &index, QString name, QString desc, QString flag);
    AnaItem * createSVAnaI(int &index, QString name, QString desc, QString flag);

    AnaItem * createDrvU(int &index, QString name, QString desc, QString flag);
    AnaItem * createDrvI(int &index, QString name, QString desc, QString flag);
    AnaItem * createDrvFR(int &index, QString name, QString desc);
    AnaItem * createDrvP(int &index, QString name, QString desc);

    AnaItem * createGSAnaP(int &index, QString name, QString desc);

    AnaItem * creatSFPPower(int &index, QString name, QString desc);

    BiItem * createGeneralBi(int &index, QString name, QString desc , unsigned int dwAttr);
    BiItem * createSignalBi(int &index, QString name, QString desc);
    BiItem * createGooseBi(int &index, QString name, QString desc);

    SettingItem * createSTRSet(int &index, QString name, QString desc);
    SettingItem * createZoneNumSet(int &index);
    SettingItem * createPTPrimary(int &index);
    SettingItem * createPTSecondary(int &index);
    SettingItem * createCTPrimary(int &index, QString name, QString desc);
    SettingItem * createCTSecondary(int &index, QString name, QString desc);

    SettingItem * createCTLSet(int &index, QString name, QString desc);

    SoftYBItem * createSoftYB(int &index, QString name, QString desc, unsigned int dwAttr, unsigned int dftVal);

    void AppendDrvConfig(tdsPara * para, int &index);
    void AppendGSAnaConfig(tdsPara * para, int &index);
    void AppendGeneralBiConfig(tdsPara * para, int &index);
    void AppendSignalConfig(tdsPara * para, int &index);
    void AppendGooseBiConfig(tdsPara * para, int &index);

    void AppendEvtConfig(tdsPara * para);
    void AppendSGSetConfig(tdsPara * para);
};

#endif // CPARATOCFGDIALOG_H
