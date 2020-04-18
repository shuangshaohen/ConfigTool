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

    void            Submit();
    void            Revert();

    const           QList<CMsgInfo>& GetMsgInfoList(){ return m_MsgInfoList; };

    void            ClearMsgInfoList();

    static bool     splitStr(QString & str1, QString & str2, QString src);
    static bool     splitUnInt(unsigned int & num1, unsigned int & num2, QString src);

    bool            checkName(QString name);
    bool            checkNameDuplicate(QString name);
    bool            checkSPSetCnnInfo(QString info);
    bool            checkSoftYBCnnInfo(QString info);
    bool            checkHardBICnnInfo(QString info);

    static QString  changeDecToHex(unsigned int val);
    static bool     changeQStringToUInt(unsigned int &val, QString str);
private:
    GseConfig       m_Config;
    GseConfig       m_SaveConfig;
    bool            m_bModified;
    QList<CMsgInfo>	m_MsgInfoList;

    void            copyConfig(GseConfig * pSrc, GseConfig * pDst);

    //文件解析
    void            ParseDeviceInfo(QDomElement element);
    void            ParseDeviceParas(QDomElement element);

    void            ParseAnaTable(QDomElement element , AnaConfig * ana);
    void            ParseAnaItem(QDomElement element , AnaConfig * parent);

    //文件保存
    void            SaveInstruction(QDomDocument &doc);
    void            SaveRoot(QDomDocument &doc, QString str);
    void            SaveDeviceInfo(QDomDocument &doc, QDomElement & parentNode);
    void            SaveDeviceDeviceParas(QDomDocument &doc, QDomElement & parentNode);
    void            SaveAnaItem(QDomDocument &doc, QDomElement &parentNode, AnaItem *p);
    void            SaveAnaTable(QDomDocument &doc, QDomElement &parentNode, AnaConfig *ana, QString key);

    void            CheckConfig();

    QString         bindString(QString str1, QString str2);
};

#endif // CDATABASE_H
