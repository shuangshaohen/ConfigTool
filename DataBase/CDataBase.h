#ifndef CDATABASE_H
#define CDATABASE_H

#include "DataBase_global.h"
#include "GSEConfig.h"
#include "CMsgInfo.h"
#include <QList>
#include <QDomDocument>

class DATABASE_EXPORT CDataBase
{
public:
    CDataBase();
    virtual ~CDataBase();

    //解析配置，父方法为xml文件的解析
    virtual bool    ParseConfig(const QString & fileName);

    //保存配置
    virtual QString SaveConfig(QString fileName);

    QString         SaveSQL(QString fileName);

    void            ClearConfig();

    GseConfig *     GetConfig();

    void            SetModified(bool bModified);

    bool            GetModified();

    void            Submit();
    void            Revert();

    const           QList<CMsgInfo>& GetMsgInfoList(){ return m_MsgInfoList; };

    void            ClearMsgInfoList();

    void            CheckConfig();

    bool            checkNameNoExit(QString name);
    //名称重复检查
    bool            checkNameNotDuplicate(QString name);
    //一次值二次值关联检查
    bool            checkSPSetCnnInfo(QString info);
    //软压板关联检查
    bool            checkSoftYBCnnInfo(QString info);
    //硬开入关联检查
    bool            checkHardBICnnInfo(QString info);
    //GS开入关联检查
    bool            checkGSINCnnInfo(QString info);
    //双点信号常闭节点关联索引检查
    bool            checkIndexDPS(QString info);
    //浮点GS开入开练检查
    bool            checkIndexGSAna(QString info);
    //开入类型检查
    bool            checkBiType(QString type);
    //告警级别检查
    bool            checkAlmLevel(QString level);
    //软压板默认值检查
    bool            checkCtlVal(unsigned int val);
    //HW关联类型检查
    bool            checkHWChan(QString info);
    //FW关联类型检查
    bool            checkFWChan(QString info);

    //定值类型检查
    bool            checkSetType(QString info);

    static bool     splitStr(QString & str1, QString & str2, QString src);
    static bool     splitUnInt(unsigned int & num1, unsigned int & num2, QString src);
    static bool     split4UnInt(unsigned int & num1, unsigned int & num2, unsigned int & num3, unsigned int &num4, QString src);
    static bool     changeQStringToUInt(unsigned int &val, QString str);
    static bool     splitChan0(unsigned int & board, unsigned int &flag, QString chan0);

    static QString  changeDecToHex(unsigned int val);
    static QString  changeToChan0(unsigned int board, unsigned int flag);
private:
    GseConfig       m_Config;
    QVector<BaseTab *> m_tables;
    GseConfig       m_SaveConfig;
    bool            m_bModified;
    QList<CMsgInfo>	m_MsgInfoList;

    void            copyConfig(GseConfig * pSrc, GseConfig * pDst);

    //文件解析
    void            ParseDeviceInfo(QDomElement element);
    void            ParseDeviceParas(QDomElement element);

    void            ParseTable(QDomElement element , BaseTab * config, int type);
    void            ParseAnaItem(QDomElement element , BaseTab * parent);
    void            ParseBiItem(QDomElement element , BaseTab *parent);
    void            ParseSoftYBItem(QDomElement element , BaseTab *parent);
    void            ParseGooseBoItem(QDomElement element , BaseTab *parent);
    void            ParseTripMatrixItem(QDomElement element , BaseTab *parent);
    void            ParseSettingItem(QDomElement element , BaseTab *parent);
    void            ParseEvtItem(QDomElement element , BaseTab *parent);

    void            ParseYKItem(QDomElement element , BaseTab *parent);

    //文件保存
    void            SaveInstruction(QDomDocument &doc);
    void            SaveRoot(QDomDocument &doc, QString str);
    void            SaveDeviceInfo(QDomDocument &doc, QDomElement & parentNode);
    void            SaveDeviceDeviceParas(QDomDocument &doc, QDomElement & parentNode);
    void            SaveAnaItem(QDomDocument &doc, QDomElement &parentNode, AnaItem *p);
    void            SaveAnaTable(QDomDocument &doc, QDomElement &parentNode, BaseTab *ana, QString key);
    void            SaveGeneralBiConfigTable(QDomDocument &doc, QDomElement &parentNode, BaseTab *config);
    void            SaveSignalConfigTable(QDomDocument &doc, QDomElement &parentNode, BaseTab *config);
    void            SaveGooseBiConfigTable(QDomDocument &doc, QDomElement &parentNode, BaseTab *config);
    void            SaveSoftYBTable(QDomDocument &doc, QDomElement &parentNode, BaseTab *config);

    void            SaveGooseBOTable(QDomDocument &doc, QDomElement &parentNode, BaseTab *config);
    void            SaveTripMatrixTable(QDomDocument &doc, QDomElement &parentNode, BaseTab *config);
    void            SaveYKTable(QDomDocument &doc, QDomElement &parentNode, BaseTab *config);

    void            SaveSettingTable(QDomDocument &doc, QDomElement &parentNode, BaseTab *config, QString key);
    void            SaveEvtTable(QDomDocument &doc, QDomElement &parentNode, BaseTab *config, QString key);
    void            SaveRemoteTripTable(QDomDocument &doc, QDomElement &parentNode, BaseTab *config);

    //配置校验
    void            CheckAnaTab(BaseTab * config);
    void            CheckBiTab(BaseTab * config);
    void            CheckSoftYBTab(BaseTab * config);
    void            CheckGsBoTab(BaseTab * config);
    void            CheckTripMatrixTab(BaseTab * config);
    void            CheckYKTab(BaseTab * config);
    void            CheckSettingTab(BaseTab * config);
    void            CheckEvtTab(BaseTab * config);

    QString         bindString(QString str1, QString str2);
};

#endif // CDATABASE_H
