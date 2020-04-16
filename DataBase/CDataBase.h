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

    void            ClearConfig();

    GseConfig *     GetConfig();

    void            SetModified(bool bModified);

    bool            GetModified();

    const           QList<CMsgInfo>& GetMsgInfoList(){ return m_MsgInfoList; };

    void            ClearMsgInfoList();

    //配置校验
    void            CheckConfig();
    void            CheckCnn();
private:
    GseConfig       m_Config;
    bool            m_bModified;
    QList<CMsgInfo>	m_MsgInfoList;

    //关联关系更新为name，仅在parse时，需整体调用，其他时候动态关联name
    void            RefreshCnn();
    QString         RefreshSet(int wIndex, QString msg);
    QString         RefreshSoftYB(int wIndex, QString msg);
    QString         RefreshBI(int wIndex, QString msg);
    QString         RefreshTripMatrix(int wIndex, QString msg);
    QString         RefreshAna(int wIndex, QString msg);

    //文件保存
    void            SaveInstruction(QDomDocument *doc);
    void            SaveRoot(QDomDocument *doc, QString str);
    void            SaveDeviceInfo(QDomDocument *doc, QDomElement * parentNode);
    void            SaveAnaItem(QDomDocument *doc, QDomElement *parentNode, AnaConfig *p);
    void            SaveBiItem(QDomDocument *doc, QDomElement *parentNode, BiConfig *p);
    void            SaveGsBiItem(QDomDocument *doc, QDomElement *parentNode, GooseBiConfig *p);
    void            SaveGsBoItem(QDomDocument *doc, QDomElement *parentNode, GooseBoConfig *p);
    void            SaveTripMatrixItem(QDomDocument *doc, QDomElement *parentNode, TripMatrixConfig *p);
    void            SaveSettingItem(QDomDocument *doc, QDomElement *parentNode, SettingConfig *p);
    void            SaveRecItem(QDomDocument *doc, QDomElement *parentNode, RecConfig *p);
    void            SaveRemoteTripItem(QDomDocument *doc, QDomElement *parentNode, RemoteTripConfig *p);
    void            SaveSoftYBItem(QDomDocument *doc, QDomElement *parentNode, SoftYbConfig *p);

    //配置校验
    void            CheckAnaConfig();
    void            CheckBIConfig();
    void            CheckGSBIConfig();
    void            CheckGSBOConfig();
    void            CheckTripMatrixConfig();
    void            CheckSettingConfig();
    void            CheckAlarmConfig();
    void            CheckActConfig();
    void            CheckRemoteTripConfig();

    unsigned int    GetSetIndex(QString name, QString msg);
    unsigned int    GetSoftIndex(QString name, QString msg);
    unsigned int    GetBIIndex(QString name, QString msg);
    unsigned int    GetTripMatrixIndex(QString name, QString msg);
    unsigned int    GetAnaIndex(QString name, QString msg);

    unsigned int    ChangeStringToInt(QString str);
    QString         ChangeNumToDEC(unsigned int num);
    QString         ChangeShortToDEC(unsigned int num);
    QString         ChangeNumToHEX(unsigned int num);
};

#endif // CDATABASE_H
