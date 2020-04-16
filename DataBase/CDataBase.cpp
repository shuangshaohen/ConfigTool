#include "CDataBase.h"
#include <QFile>
#include <QDebug>
#include <QDateTime>

CDataBase::CDataBase()
{
    this->ClearConfig();
    m_MsgInfoList.clear();
    m_bModified = false;
}

CDataBase::~CDataBase()
{
    this->ClearConfig();
    m_MsgInfoList.clear();
    m_bModified = false;
}

bool CDataBase::ParseConfig(const QString &fileName)
{
    ClearConfig();

    QFile file(fileName);
    if ( !file.open(QIODevice::ReadOnly | QIODevice::Text) )
    {
        qDebug()<<"Open file fail";

        QString strInfo = QString("打开%1失败！").arg(fileName);
        CMsgInfo msgInfo( CMsgInfo::Enum_Application_Parse_Mode, CMsgInfo::CN_ERROR_MSG, strInfo );
        m_MsgInfoList.append(msgInfo);

        return false;
    }

    QString strInfo = QString("打开%1成功！").arg(fileName);
    CMsgInfo msgInfo( CMsgInfo::Enum_Application_Parse_Mode, CMsgInfo::CN_HINT_MSG, strInfo );
    m_MsgInfoList.append(msgInfo);

    QByteArray utfData = file.readAll();
    file.close();

    QDomDocument document;
    QString errorMsg;
    int row = 0, col = 0;
    if ( !document.setContent(utfData, false, &errorMsg, &row, &col) )
    {
        qDebug()<<"parse file failed at row and column:"
            <<QString::number(row, 10)<<QString(", ")<<QString::number(col, 10);

        QString strInfo = QString("文件解析失败,%1行,%2列:%3")
                                .arg(QString::number(row, 10))
                                .arg(QString::number(col, 10))
                                .arg(errorMsg);
        CMsgInfo msgInfo( CMsgInfo::Enum_Application_Parse_Mode, CMsgInfo::CN_ERROR_MSG, strInfo );
        m_MsgInfoList.append(msgInfo);

        return false;
    }

    if ( document.isNull() )
    {
        qDebug()<<"document is Null!";

        QString strInfo ="内存开辟失败!";
        CMsgInfo msgInfo( CMsgInfo::Enum_Application_Parse_Mode, CMsgInfo::CN_ERROR_MSG, strInfo );
        m_MsgInfoList.append(msgInfo);

        return false;
    }

    QDomElement elemRoot = document.documentElement();
    if ( elemRoot.isNull() )
    {
        return false;
    }

    ParseDeviceInfo(elemRoot.firstChildElement("DeviceInfo"));
    ParseDeviceParas(elemRoot.firstChildElement("DeviceParas"));

    //解析模拟量;
    QDomElement elemAna = elemRoot.firstChildElement("SmpConfig");
    if(!elemAna.isNull())
    {
        //AD模拟量通道配置
        ParseAnaTable(elemAna.firstChildElement("AnaConfig"),&m_Config.adAnaConfig);
        //衍生通道配置
        ParseAnaTable(elemAna.firstChildElement("DerivedConfig"),&m_Config.derivedConfig);
        //Sv通道配置
        ParseAnaTable(elemAna.firstChildElement("SvConfig"),&m_Config.svConfig);
        //GS浮点通道配置
        ParseAnaTable(elemAna.firstChildElement("GSConfig"),&m_Config.gsAnaConfig);
        //其它通道配置
        ParseAnaTable(elemAna.firstChildElement("OtherConfig"),&m_Config.otherAnaConfig);
    }

    QString strInfoEnd = QString("解析结束！");
    CMsgInfo msgInfoEnd( CMsgInfo::Enum_Application_Parse_Mode, CMsgInfo::CN_HINT_MSG, strInfoEnd);
    m_MsgInfoList.append(msgInfoEnd);

    return true;
}

QString CDataBase::SaveConfig(QString fileName)
{
    QFile file(fileName);
    if(!file.open(QFile::WriteOnly|QFile::Text))
    {
        QString strInfo = QString(" 保存文件 %1 失败:\n%2.")
                            .arg(fileName)
                            .arg(file.errorString());
        CMsgInfo msgInfo( CMsgInfo::Enum_Application_Save_Mode, CMsgInfo::CN_ERROR_MSG, strInfo );
        m_MsgInfoList.append(msgInfo);

        return strInfo;
    }

    QTextStream out(&file);
    out.setCodec("UTF-8");

    CheckConfig();

    QDomDocument document;  //QDomDocument类
    this->SaveInstruction(document);
    this->SaveRoot(document,"Root");

    document.save(out, 4, QDomNode::EncodingFromTextStream);
    file.close();

    QString strInfo = QString(" 保存文件 %1 成功！")
                        .arg(fileName);
    CMsgInfo msgInfo( CMsgInfo::Enum_Application_Save_Mode, CMsgInfo::CN_HINT_MSG, strInfo );
    m_MsgInfoList.append(msgInfo);

    m_bModified = false;

    return "";
}

void CDataBase::SaveInstruction(QDomDocument &doc)
{
    QDomProcessingInstruction instruction = doc.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\"");
    doc.appendChild(instruction);
}

void CDataBase::SaveRoot(QDomDocument &doc, QString str)
{
    QDomElement root = doc.createElement(str);//创建根节点  QDomElemet元素

    //DeviceInfo
    this->SaveDeviceInfo(doc,root);
    this->SaveDeviceDeviceParas(doc,root);

    //SmpConfig
    QDomElement smpConfig = doc.createElement("SmpConfig");
    smpConfig.setAttribute("Desc",QString("采样通道配置"));
    //子表保存
    SaveAnaTable(doc,smpConfig,&m_Config.adAnaConfig,"AnaConfig");
    SaveAnaTable(doc,smpConfig,&m_Config.derivedConfig,"DerivedConfig");
    SaveAnaTable(doc,smpConfig,&m_Config.svConfig,"SvConfig");
    SaveAnaTable(doc,smpConfig,&m_Config.gsAnaConfig,"GSConfig");
    SaveAnaTable(doc,smpConfig,&m_Config.otherAnaConfig,"OtherConfig");

    root.appendChild(smpConfig);

    doc.appendChild(root);//添加根节点
}

void CDataBase::SaveDeviceInfo(QDomDocument &doc, QDomElement &parentNode)
{
    QDomElement deviceInfo = doc.createElement("DeviceInfo");
    deviceInfo.setAttribute("Type", m_Config.deviceInfo.sType);
    deviceInfo.setAttribute("Desc",  m_Config.deviceInfo.sDesc);
    deviceInfo.setAttribute("Version", m_Config.deviceInfo.sVersion);
    QDateTime time = QDateTime::currentDateTime();
    QString str = time.toString("yyyy-MM-dd hh:mm:ss");
    deviceInfo.setAttribute("Date", str);
    deviceInfo.setAttribute("CRC", m_Config.deviceInfo.sCRC);
    parentNode.appendChild(deviceInfo);
}

void CDataBase::SaveDeviceDeviceParas(QDomDocument &doc, QDomElement &parentNode)
{
    QDomElement deviceParas = doc.createElement("DeviceParas");
    deviceParas.setAttribute("MaxAnaNum", QString::number(m_Config.deviceParas.wMaxAnaNum));
    deviceParas.setAttribute("MaxBiNum", QString::number(m_Config.deviceParas.wMaxBiNum));
    deviceParas.setAttribute("MaxSvNum", QString::number(m_Config.deviceParas.wMaxSvNum));
    deviceParas.setAttribute("MaxGooseNum", QString::number(m_Config.deviceParas.wMaxGooseNum));
    deviceParas.setAttribute("MaxBoNum", QString::number(m_Config.deviceParas.wMaxBoNum));
    deviceParas.setAttribute("SmpRate", QString::number(m_Config.deviceParas.wSmpRate));
    parentNode.appendChild(deviceParas);
}

void CDataBase::SaveAnaItem(QDomDocument &doc, QDomElement &parentNode, AnaItem *p)
{
    QDomElement item = doc.createElement("Item");
    item.setAttribute("Index",QString::number(p->wIndex));
    item.setAttribute("Desc",p->sDesc);
    item.setAttribute("ChanAttr",changeDecToHex(p->dwAttr));
    item.setAttribute("ChanType",p->sChanType);
    item.setAttribute("ChanFlag",p->sChanFlag);
    item.setAttribute("VirtualDft",QString::number(p->iXuYCDft));
    item.setAttribute("Coe",p->wCoe);
    item.setAttribute("RatedSetIndex",p->sRateSetP+"/"+p->sRateSetS);
    item.setAttribute("RecYbIndex",p->sRecSYb+"/"+p->sRecHYb);
    item.setAttribute("DataAttr",QString::number(p->byWidth)+"/"+QString::number(p->byDotBit));
    item.setAttribute("Unit",p->sKiloUnit+"/"+p->sUnit);
    item.setAttribute("Desc",p->sName);
    parentNode.appendChild(item);
}

void CDataBase::SaveAnaTable(QDomDocument &doc, QDomElement &parentNode, AnaConfig *ana , QString key)
{
    QDomElement anaConfig = doc.createElement(key);
    anaConfig.setAttribute("Desc",ana->sDesc);
    anaConfig.setAttribute("SAddrIndexName",ana->sKey);
    for(int i = 0 ; i < ana->items.size(); i++)
    {
        SaveAnaItem(doc,anaConfig,ana->items[i]);
    }
    parentNode.appendChild(anaConfig);
}

void CDataBase::ClearConfig()
{
    m_Config.clear();
}

GseConfig *CDataBase::GetConfig()
{
    return &m_Config;
}

void CDataBase::SetModified(bool bModified)
{
    m_bModified = bModified;
}

bool CDataBase::GetModified()
{
    return m_bModified;
}

void CDataBase::ClearMsgInfoList()
{
    m_MsgInfoList.clear();
}

void CDataBase::ParseDeviceInfo(QDomElement element)
{
    if(element.isNull())
        return;
    DeviceInfo * info = &m_Config.deviceInfo;
    info->sType = element.attributeNode("Type").value();
    info->sDesc = element.attributeNode("Desc").value();
    info->sVersion = element.attributeNode("Version").value();
    info->sDate = element.attributeNode("Date").value();
    info->sCRC = element.attributeNode("CRC").value();
}

void CDataBase::ParseDeviceParas(QDomElement element)
{
    if(element.isNull())
        return;
    DeviceParas * paras = &m_Config.deviceParas;
    paras->wMaxAnaNum = element.attributeNode("MaxAnaNum").value().toUInt();
    paras->wMaxBiNum = element.attributeNode("MaxBiNum").value().toUInt();
    paras->wMaxSvNum = element.attributeNode("MaxSvNum").value().toUInt();
    paras->wMaxBoNum = element.attributeNode("MaxBoNum").value().toUInt();
    paras->wSmpRate = element.attributeNode("SmpRate").value().toUInt();
}

void CDataBase::ParseAnaTable(QDomElement element, AnaConfig *ana)
{
    if(!element.isNull())
    {
        ana->sDesc  = element.attributeNode("Desc").value();
        ana->sKey   = element.attributeNode("SAddrIndexName").value();

        QDomElement elemItem = element.firstChildElement("Item");
        while ( !elemItem.isNull() )
        {
            ParseAnaItem(elemItem,ana);
            elemItem = elemItem.nextSiblingElement("Item");
        }
    }

    QString strInfoAna = QString("解析%1表完成:%2").arg(ana->sDesc).arg(QString::number(ana->items.size()));
    CMsgInfo msgInfoAna( CMsgInfo::Enum_Application_Parse_Mode, CMsgInfo::CN_HINT_MSG, strInfoAna );
    m_MsgInfoList.append(msgInfoAna);
}

void CDataBase::ParseAnaItem(QDomElement element, AnaConfig *parent)
{
    if(element.isNull())
        return;

    AnaItem * item = new AnaItem(parent->items.size());

    unsigned int index = element.attributeNode("Index").value().toUInt();
    if(index != item->wIndex)
    {
        CMsgInfo msgInfo( CMsgInfo::Enum_Application_Parse_Mode, CMsgInfo::CN_WARNNING_MSG,
                          QString("%1表第%2条index与其顺序不匹配,已自动修正!").arg(parent->sDesc).arg(item->wIndex) );
        m_MsgInfoList.append(msgInfo);
    }

    item->sDesc     = element.attributeNode("Desc").value();
    item->sName     = element.attributeNode("Name").value();

    if(changeQStringToUInt(item->dwAttr, element.attributeNode("ChanAttr").value()) == false)
    {
        CMsgInfo msgInfo( CMsgInfo::Enum_Application_Parse_Mode, CMsgInfo::CN_WARNNING_MSG,
                          QString("%1表第%2条ChanAttr参数%3解析格式失败!")
                          .arg(parent->sDesc).arg(item->wIndex).arg(element.attributeNode("ChanAttr").value()) );
        m_MsgInfoList.append(msgInfo);
    }

    item->sChanType = element.attributeNode("ChanType").value();
    item->sChanFlag = element.attributeNode("ChanFlag").value();

    bool ok;
    item->iXuYCDft  = element.attributeNode("VirtualDft").value().toInt(&ok);
    if(ok == false)
    {
        CMsgInfo msgInfo( CMsgInfo::Enum_Application_Parse_Mode, CMsgInfo::CN_WARNNING_MSG,
                          QString("%1表第%2条VirtualDft参数%3解析格式失败!")
                          .arg(parent->sDesc).arg(item->wIndex).arg(element.attributeNode("VirtualDft").value()) );
        m_MsgInfoList.append(msgInfo);
        item->iXuYCDft = -1;
    }

    item->wCoe      = element.attributeNode("Coe").value().toUInt();

    if(false == splitStr(item->sRateSetP,item->sRateSetS,element.attributeNode("RatedSetIndex").value()))
    {
        CMsgInfo msgInfo( CMsgInfo::Enum_Application_Parse_Mode, CMsgInfo::CN_WARNNING_MSG,
                          QString("%1表第%2条RatedSetIndex参数%3解析格式失败!")
                          .arg(parent->sDesc).arg(item->wIndex).arg(element.attributeNode("RatedSetIndex").value()) );
        m_MsgInfoList.append(msgInfo);
    }

    if(false == splitStr(item->sRecSYb,item->sRecHYb,element.attributeNode("RecYbIndex").value()))
    {
        CMsgInfo msgInfo( CMsgInfo::Enum_Application_Parse_Mode, CMsgInfo::CN_WARNNING_MSG,
                          QString("%1表第%2条RecYbIndex参数%3解析格式失败!")
                          .arg(parent->sDesc).arg(item->wIndex).arg(element.attributeNode("RecYbIndex").value()) );
        m_MsgInfoList.append(msgInfo);
    }

    if(false == splitUnInt(item->byWidth,item->byDotBit,element.attributeNode("DataAttr").value()))
    {
        CMsgInfo msgInfo( CMsgInfo::Enum_Application_Parse_Mode, CMsgInfo::CN_WARNNING_MSG,
                          QString("%1表第%2条DataAttr参数%3解析格式失败!")
                          .arg(parent->sDesc).arg(item->wIndex).arg(element.attributeNode("DataAttr").value()) );
        m_MsgInfoList.append(msgInfo);
    }

    if(false == splitStr(item->sKiloUnit,item->sUnit,element.attributeNode("Unit").value()))
    {
        CMsgInfo msgInfo( CMsgInfo::Enum_Application_Parse_Mode, CMsgInfo::CN_WARNNING_MSG,
                          QString("%1表第%2条Unit参数%3解析格式失败!")
                          .arg(parent->sDesc).arg(item->wIndex).arg(element.attributeNode("Unit").value()) );
        m_MsgInfoList.append(msgInfo);
    }

    parent->items.push_back(item);
}

bool CDataBase::splitStr(QString &str1, QString &str2, QString src)
{
    QStringList strList = src.split("/");
    if(strList.size() == 2)
    {
        str1 = strList[0];
        str2 = strList[1];
        return true;
    }
    else
    {
        str1 = src;
        return false;
    }
}

bool CDataBase::splitUnInt(unsigned int &num1, unsigned int &num2, QString src)
{
    QStringList strList = src.split("/");
    if(strList.size() == 2)
    {
        bool ok1;
        bool ok2;
        int tem1 = strList[0].toUInt(&ok1);
        int tem2 = strList[1].toUInt(&ok2);
        if(ok1)
            num1 = tem1;
        if(ok2)
            num2 = tem2;

        if(ok1&&ok2)
            return true;
        else
            return false;
    }
    else
    {
        return false;
    }
}

QString CDataBase::changeDecToHex(unsigned int val)
{
    if(val == 0)
        return "0";

    QString s = "0x" + QString::number(val,16).toUpper();
    return s;
}

bool CDataBase::changeQStringToUInt(unsigned int &val, QString str)
{
    unsigned int num;
    bool ok = false;
    if(str.startsWith("0x"))
        num = str.toUInt(&ok,16);
    else
        num = str.toUInt(&ok,10);

    if(!ok)
        return false;

    val = num;
    return true;
}

void CDataBase::CheckConfig()
{
    CMsgInfo msgInfoStart( CMsgInfo::Enum_Application_Parse_Mode, CMsgInfo::CN_HINT_MSG, QString("表重名检测开始！"));
    m_MsgInfoList.append(msgInfoStart);

    //TODO

    CMsgInfo msgInfoEnd( CMsgInfo::Enum_Application_Parse_Mode, CMsgInfo::CN_HINT_MSG, QString("表重名检测完成！"));
    m_MsgInfoList.append(msgInfoEnd);
}
