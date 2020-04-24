#include "CDataBase.h"
#include <QFile>
#include <QDebug>
#include <QDateTime>

CDataBase::CDataBase()
{
    this->ClearConfig();
    m_MsgInfoList.clear();
    m_bModified = false;
    m_tables.clear();

    m_Config.inital();
    m_tables.push_back(&m_Config.adAnaConfig);
    m_tables.push_back(&m_Config.derivedConfig);
    m_tables.push_back(&m_Config.svConfig);
    m_tables.push_back(&m_Config.gsAnaConfig);
    m_tables.push_back(&m_Config.otherAnaConfig);
    m_tables.push_back(&m_Config.generalBiConfig);
    m_tables.push_back(&m_Config.signalConfig);
    m_tables.push_back(&m_Config.gooseBiConfig);
    m_tables.push_back(&m_Config.softYBConfig);
    m_tables.push_back(&m_Config.gooseBoConfig);
    m_tables.push_back(&m_Config.tripMaxtrixConfig );
    m_tables.push_back(&m_Config.ykConfig);
    m_tables.push_back(&m_Config.settingSPConifg);
    m_tables.push_back(&m_Config.settingSGConifg);
    m_tables.push_back(&m_Config.evtAlmConfig);
    m_tables.push_back(&m_Config.evtActConfig);
    m_tables.push_back(&m_Config.evtCheckConfig);
    m_tables.push_back(&m_Config.remoteTripConfig);
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
        ParseTable(elemAna.firstChildElement("AnaConfig"),&m_Config.adAnaConfig,Enum_Table_Type_Ana);
        //衍生通道配置
        ParseTable(elemAna.firstChildElement("DerivedConfig"),&m_Config.derivedConfig,Enum_Table_Type_Ana);
        //Sv通道配置
        ParseTable(elemAna.firstChildElement("SvConfig"),&m_Config.svConfig,Enum_Table_Type_Ana);
        //GS浮点通道配置
        ParseTable(elemAna.firstChildElement("GSConfig"),&m_Config.gsAnaConfig,Enum_Table_Type_Ana);
        //其它通道配置
        ParseTable(elemAna.firstChildElement("OtherConfig"),&m_Config.otherAnaConfig,Enum_Table_Type_Ana);
    }

    //解析开关量
    QDomElement elemBi = elemRoot.firstChildElement("BiConfig");
    if(!elemBi.isNull())
    {
        //硬开入通道配置
        ParseTable(elemBi.firstChildElement("GeneralBiConfig"),&m_Config.generalBiConfig,Enum_Table_Type_Bi);
        //中间信号通道配置
        ParseTable(elemBi.firstChildElement("SignalBiConfig"),&m_Config.signalConfig,Enum_Table_Type_Bi);
        //GS开入通道配置
        ParseTable(elemBi.firstChildElement("GooseBiConfig"),&m_Config.gooseBiConfig,Enum_Table_Type_Bi);

        //软压板
        ParseTable(elemBi.firstChildElement("SoftYb"),&m_Config.softYBConfig,Enum_Table_Type_SoftYB);
    }

    //解析开出
    QDomElement elemBo = elemRoot.firstChildElement("BoConfig");
    if(!elemBo.isNull())
    {
        //GS开出通道配置
        ParseTable(elemBo.firstChildElement("GooseBoConfig"),&m_Config.gooseBoConfig,Enum_Table_Type_GsBo);
        //继电器开出通道配置
        ParseTable(elemBo.firstChildElement("TripMatrix"),&m_Config.tripMaxtrixConfig,Enum_Table_Type_TripMatrix);
        //遥控通道配置
        ParseTable(elemBo.firstChildElement("YKConfig"),&m_Config.ykConfig,Enum_Table_Type_Yk);
    }

    //解析定值
    QDomElement elemSet = elemRoot.firstChildElement("Setting");
    if(!elemSet.isNull())
    {
        //不分组定值通道配置
        ParseTable(elemSet.firstChildElement("SPSet"),&m_Config.settingSPConifg,Enum_Table_Type_Set);
        //分组定值通道配置
        ParseTable(elemSet.firstChildElement("SGSet"),&m_Config.settingSGConifg,Enum_Table_Type_Set);
    }

    //解析事件
    QDomElement elemEvt = elemRoot.firstChildElement("EvtConfig");
    if(!elemEvt.isNull())
    {
        //告警通道配置
        ParseTable(elemEvt.firstChildElement("AlarmRec"),&m_Config.evtAlmConfig,Enum_Table_Type_Evt);
        //动作通道配置
        ParseTable(elemEvt.firstChildElement("ActRec"),&m_Config.evtActConfig,Enum_Table_Type_Evt);
        //自检通道配置
        ParseTable(elemEvt.firstChildElement("CheckRec"),&m_Config.evtCheckConfig,Enum_Table_Type_Evt);
        //远跳通道配置
        ParseTable(elemEvt.firstChildElement("RemoteTrip"),&m_Config.remoteTripConfig,Enum_Table_Type_Evt);
    }


    QString strInfoEnd = QString("解析结束！");
    CMsgInfo msgInfoEnd( CMsgInfo::Enum_Application_Parse_Mode, CMsgInfo::CN_HINT_MSG, strInfoEnd);
    m_MsgInfoList.append(msgInfoEnd);

    copyConfig(&m_Config,&m_SaveConfig);
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
    SaveAnaTable(doc,smpConfig,&m_SaveConfig.adAnaConfig,"AnaConfig");
    SaveAnaTable(doc,smpConfig,&m_SaveConfig.derivedConfig,"DerivedConfig");
    SaveAnaTable(doc,smpConfig,&m_SaveConfig.svConfig,"SvConfig");
    SaveAnaTable(doc,smpConfig,&m_SaveConfig.gsAnaConfig,"GSConfig");
    SaveAnaTable(doc,smpConfig,&m_SaveConfig.otherAnaConfig,"OtherConfig");

    root.appendChild(smpConfig);

    //BiConfig
    QDomElement biConfig = doc.createElement("BiConfig");
    biConfig.setAttribute("Desc",QString("开入量配置"));
    //子表保存
    SaveGeneralBiConfigTable(doc,biConfig,&m_SaveConfig.generalBiConfig);
    SaveSignalConfigTable(doc,biConfig,&m_SaveConfig.signalConfig);
    SaveGooseBiConfigTable(doc,biConfig,&m_SaveConfig.gooseBiConfig);
    SaveSoftYBTable(doc,biConfig,&m_SaveConfig.softYBConfig);

    root.appendChild(biConfig);

    //BoConfig
    QDomElement boConfig = doc.createElement("BoConfig");
    boConfig.setAttribute("Desc",QString("开出配置"));
    //子表保存
    SaveGooseBOTable(doc,boConfig,&m_SaveConfig.gooseBoConfig);
    SaveTripMatrixTable(doc,boConfig,&m_SaveConfig.tripMaxtrixConfig);
    SaveYKTable(doc,boConfig,&m_SaveConfig.ykConfig);

    root.appendChild(boConfig);

    //Setting
    QDomElement setConfig = doc.createElement("Setting");
    setConfig.setAttribute("Desc",QString("定值配置信息"));
    //子表保存
    SaveSettingTable(doc,setConfig,&m_SaveConfig.settingSPConifg,"SPSet");
    SaveSettingTable(doc,setConfig,&m_SaveConfig.settingSGConifg,"SGSet");

    root.appendChild(setConfig);

    //EvtConfig
    QDomElement evtConfig = doc.createElement("EvtConfig");
    evtConfig.setAttribute("Desc",QString("事件配置"));
    //子表保存
    SaveEvtTable(doc,evtConfig,&m_SaveConfig.evtAlmConfig,"AlarmRec");
    SaveEvtTable(doc,evtConfig,&m_SaveConfig.evtActConfig,"ActRec");
    SaveEvtTable(doc,evtConfig,&m_SaveConfig.evtCheckConfig,"CheckRec");
    SaveRemoteTripTable(doc,evtConfig,&m_SaveConfig.remoteTripConfig);

    root.appendChild(evtConfig);


    doc.appendChild(root);//添加根节点
}

void CDataBase::SaveDeviceInfo(QDomDocument &doc, QDomElement &parentNode)
{
    QDomElement deviceInfo = doc.createElement("DeviceInfo");
    deviceInfo.setAttribute("Type", m_SaveConfig.deviceInfo.sType);
    deviceInfo.setAttribute("Desc",  m_SaveConfig.deviceInfo.sDesc);
    deviceInfo.setAttribute("Version", m_SaveConfig.deviceInfo.sVersion);
    QDateTime time = QDateTime::currentDateTime();
    QString str = time.toString("yyyy-MM-dd hh:mm:ss");
    deviceInfo.setAttribute("Date", str);
    m_SaveConfig.deviceInfo.sDate = str;
    m_Config.deviceInfo.sDate = str;
    deviceInfo.setAttribute("CRC", m_SaveConfig.deviceInfo.sCRC);
    parentNode.appendChild(deviceInfo);
}

void CDataBase::SaveDeviceDeviceParas(QDomDocument &doc, QDomElement &parentNode)
{
    QDomElement deviceParas = doc.createElement("DeviceParas");
    deviceParas.setAttribute("MaxAnaNum", QString::number(m_SaveConfig.deviceParas.wMaxAnaNum));
    deviceParas.setAttribute("MaxBiNum", QString::number(m_SaveConfig.deviceParas.wMaxBiNum));
    deviceParas.setAttribute("MaxSvNum", QString::number(m_SaveConfig.deviceParas.wMaxSvNum));
    deviceParas.setAttribute("MaxGooseNum", QString::number(m_SaveConfig.deviceParas.wMaxGooseNum));
    deviceParas.setAttribute("MaxBoNum", QString::number(m_SaveConfig.deviceParas.wMaxBoNum));
    deviceParas.setAttribute("SmpRate", QString::number(m_SaveConfig.deviceParas.wSmpRate));
    deviceParas.setAttribute("ZoneNum", QString::number(m_SaveConfig.deviceParas.wZoneNum));
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
    item.setAttribute("RatedSetIndex",bindString(p->sRateSetP,p->sRateSetS));
    item.setAttribute("RecYbIndex",bindString(p->sRecSYb,p->sRecHYb));
    item.setAttribute("DataAttr",QString::number(p->byWidth)+"/"+QString::number(p->byDotBit));
    item.setAttribute("Unit",p->sKiloUnit+"/"+p->sUnit);
    item.setAttribute("Name",p->sName);
    parentNode.appendChild(item);
}

void CDataBase::SaveAnaTable(QDomDocument &doc, QDomElement &parentNode, BaseTab *ana , QString key)
{
    QDomElement anaConfig = doc.createElement(key);
    anaConfig.setAttribute("Desc",ana->sDesc);
    anaConfig.setAttribute("SAddrIndexName",ana->sKey);
    for(int i = 0 ; i < ana->items.size(); i++)
    {
        SaveAnaItem(doc,anaConfig,(AnaItem *)ana->items[i]);
    }
    parentNode.appendChild(anaConfig);
}

void CDataBase::SaveGeneralBiConfigTable(QDomDocument &doc, QDomElement &parentNode, BaseTab *config)
{
    QDomElement generalBi = doc.createElement("GeneralBiConfig");
    generalBi.setAttribute("Desc",config->sDesc);
    generalBi.setAttribute("SAddrIndexName",config->sKey);
    for(int i = 0 ; i < config->items.size(); i++)
    {
        BiItem * p = (BiItem *)config->items[i];
        QDomElement item = doc.createElement("Item");
        item.setAttribute("Index",QString::number(p->wIndex));
        item.setAttribute("Desc",p->sDesc);
        item.setAttribute("ChanAttr",changeDecToHex(p->dwAttr));
        item.setAttribute("ChanType",p->sChanType);
        item.setAttribute("IndexDPS",p->sIndexDPS);
        //item.setAttribute("IndexAna",p->sIndexAna);
        item.setAttribute("HoldTime",p->wHoldTime);
        item.setAttribute("Name",p->sName);
        item.setAttribute("AlmLevel",p->sAlmLevel);
        generalBi.appendChild(item);

    }
    parentNode.appendChild(generalBi);
}

void CDataBase::SaveSignalConfigTable(QDomDocument &doc, QDomElement &parentNode, BaseTab *config)
{
    QDomElement signal = doc.createElement("SignalBiConfig");
    signal.setAttribute("Desc",config->sDesc);
    signal.setAttribute("SAddrIndexName",config->sKey);
    for(int i = 0 ; i < config->items.size(); i++)
    {
        BiItem * p = (BiItem *)config->items[i];
        QDomElement item = doc.createElement("Item");
        item.setAttribute("Index",QString::number(p->wIndex));
        item.setAttribute("Desc",p->sDesc);
        item.setAttribute("ChanAttr",changeDecToHex(p->dwAttr));
        item.setAttribute("ChanType",p->sChanType);
        item.setAttribute("IndexDPS",p->sIndexDPS);
        //item.setAttribute("IndexAna",p->sIndexAna);
        //item.setAttribute("HoldTime",p->wHoldTime);
        item.setAttribute("Name",p->sName);
        item.setAttribute("AlmLevel",p->sAlmLevel);
        signal.appendChild(item);

    }
    parentNode.appendChild(signal);
}

void CDataBase::SaveGooseBiConfigTable(QDomDocument &doc, QDomElement &parentNode, BaseTab *config)
{
    QDomElement gooseBi = doc.createElement("GooseBiConfig");
    gooseBi.setAttribute("Desc",config->sDesc);
    gooseBi.setAttribute("SAddrIndexName",config->sKey);
    for(int i = 0 ; i < config->items.size(); i++)
    {
        BiItem * p = (BiItem *)config->items[i];
        QDomElement item = doc.createElement("Item");
        item.setAttribute("Index",QString::number(p->wIndex));
        item.setAttribute("Desc",p->sDesc);
        item.setAttribute("ChanAttr",changeDecToHex(p->dwAttr));
        item.setAttribute("ChanType",p->sChanType);
        item.setAttribute("IndexDPS",p->sIndexDPS);
        item.setAttribute("IndexAna",p->sIndexAna);
        //item.setAttribute("HoldTime",p->wHoldTime);
        item.setAttribute("Name",p->sName);
        item.setAttribute("AlmLevel",p->sAlmLevel);
        gooseBi.appendChild(item);

    }
    parentNode.appendChild(gooseBi);
}

void CDataBase::SaveSoftYBTable(QDomDocument &doc, QDomElement &parentNode, BaseTab *config)
{
    QDomElement softYB = doc.createElement("SoftYb");
    softYB.setAttribute("Desc",config->sDesc);
    softYB.setAttribute("SAddrIndexName",config->sKey);
    for(int i = 0 ; i < config->items.size(); i++)
    {
        SoftYBItem * p = (SoftYBItem *)config->items[i];
        QDomElement item = doc.createElement("Item");
        item.setAttribute("Index",QString::number(p->wIndex));
        item.setAttribute("Desc",p->sDesc);
        item.setAttribute("ChanAttr",changeDecToHex(p->dwAttr));
        item.setAttribute("ValDft",p->wDftVal);
        item.setAttribute("Name",p->sName);
        softYB.appendChild(item);

    }
    parentNode.appendChild(softYB);
}

void CDataBase::SaveGooseBOTable(QDomDocument &doc, QDomElement &parentNode, BaseTab *config)
{
    QDomElement gooseBo = doc.createElement("GooseBoConfig");
    gooseBo.setAttribute("Desc",config->sDesc);
    gooseBo.setAttribute("SAddrIndexName",config->sKey);
    for(int i = 0 ; i < config->items.size(); i++)
    {
        GooseBOItem * p = (GooseBOItem *)config->items[i];
        QDomElement item = doc.createElement("Item");
        item.setAttribute("Index",QString::number(p->wIndex));
        item.setAttribute("Desc",p->sDesc);
        item.setAttribute("ChanAttr",changeDecToHex(p->dwAttr));
        item.setAttribute("HWChan",p->sHWChan);
        item.setAttribute("FWChan",p->sFWChan);
        item.setAttribute("Name",p->sName);
        gooseBo.appendChild(item);

    }
    parentNode.appendChild(gooseBo);
}

void CDataBase::SaveTripMatrixTable(QDomDocument &doc, QDomElement &parentNode, BaseTab *config)
{
    QDomElement tripMatrix = doc.createElement("TripMatrix");
    tripMatrix.setAttribute("Desc",config->sDesc);
    tripMatrix.setAttribute("SAddrIndexName",config->sKey);
    for(int i = 0 ; i < config->items.size(); i++)
    {
        TripMatrixItem * p = (TripMatrixItem *)config->items[i];
        QDomElement item = doc.createElement("Item");
        item.setAttribute("Index",QString::number(p->wIndex));
        item.setAttribute("Desc",p->sDesc);
        item.setAttribute("ChanAttr",changeDecToHex(p->dwAttr));
        item.setAttribute("Chan0",changeToChan0(p->wBoardIndex,p->wBOFlag));
        item.setAttribute("Name",p->sName);
        tripMatrix.appendChild(item);

    }
    parentNode.appendChild(tripMatrix);
}

void CDataBase::SaveYKTable(QDomDocument &doc, QDomElement &parentNode, BaseTab *config)
{
    QDomElement ykConfig = doc.createElement("YKConfig");
    ykConfig.setAttribute("Desc",config->sDesc);
    ykConfig.setAttribute("SAddrIndexName",config->sKey);
    for(int i = 0 ; i < config->items.size(); i++)
    {
        //TODO::
    }
    parentNode.appendChild(ykConfig);
}

void CDataBase::SaveSettingTable(QDomDocument &doc, QDomElement &parentNode, BaseTab *config, QString key)
{
    QDomElement setting = doc.createElement(key);
    setting.setAttribute("Desc",config->sDesc);
    setting.setAttribute("SAddrIndexName",config->sKey);
    for(int i = 0 ; i < config->items.size(); i++)
    {
        SettingItem * p = (SettingItem *)config->items[i];
        QDomElement item = doc.createElement("Item");
        item.setAttribute("Index",QString::number(p->wIndex));
        item.setAttribute("Desc",p->sDesc);
        item.setAttribute("ChanAttr",changeDecToHex(p->dwAttr));
        item.setAttribute("ValAttr",QString::number(p->wValMin)+"/"+QString::number(p->wValMax)+"/"+QString::number(p->wValDft)+"/"+QString::number(p->wValStep));
        item.setAttribute("SecIn",p->sSecIn);
        item.setAttribute("CoeRet",p->wCoeRet);
        item.setAttribute("ChanType",p->sType);
        item.setAttribute("DataAttr",QString::number(p->byWidth)+"/"+QString::number(p->byDotBit));
        item.setAttribute("Unit",p->sKiloUnit+"/"+p->sUnit);
        item.setAttribute("Name",p->sName);
        setting.appendChild(item);
    }
    parentNode.appendChild(setting);
}

void CDataBase::SaveEvtTable(QDomDocument &doc, QDomElement &parentNode, BaseTab *config, QString key)
{
    QDomElement evt = doc.createElement(key);
    evt.setAttribute("Desc",config->sDesc);
    evt.setAttribute("SAddrIndexName",config->sKey);
    for(int i = 0 ; i < config->items.size(); i++)
    {
        EvtItem * p = (EvtItem *)config->items[i];
        QDomElement item = doc.createElement("Item");
        item.setAttribute("Index",QString::number(p->wIndex));
        item.setAttribute("Desc",p->sDesc);
        item.setAttribute("ChanAttr",changeDecToHex(p->dwAttr));
        item.setAttribute("BoConfig",changeDecToHex(p->boConfig1));
        item.setAttribute("BoConfig2",changeDecToHex(p->boConfig2));
        item.setAttribute("BoSignal",changeDecToHex(p->boSignal));
        item.setAttribute("Light",changeDecToHex(p->boLight));
        item.setAttribute("Name",p->sName);
        evt.appendChild(item);
    }
    parentNode.appendChild(evt);
}

void CDataBase::SaveRemoteTripTable(QDomDocument &doc, QDomElement &parentNode, BaseTab *config)
{
    QDomElement remote = doc.createElement("RemoteTrip");
    remote.setAttribute("Desc",config->sDesc);
    remote.setAttribute("SAddrIndexName",config->sKey);
    for(int i = 0 ; i < config->items.size(); i++)
    {
        EvtItem * p = (EvtItem *)config->items[i];
        QDomElement item = doc.createElement("Item");
        item.setAttribute("Index",QString::number(p->wIndex));
        item.setAttribute("Desc",p->sDesc);
        item.setAttribute("ChanAttr",changeDecToHex(p->dwAttr));
        item.setAttribute("BoConfig",changeDecToHex(p->boConfig1));
        item.setAttribute("BoConfig2",changeDecToHex(p->boConfig2));
        item.setAttribute("BoSignal",changeDecToHex(p->boSignal));
        item.setAttribute("Light",changeDecToHex(p->boLight));
        item.setAttribute("BIIndex",p->sCnnGSIn);
        item.setAttribute("Name",p->sName);
        remote.appendChild(item);
    }
    parentNode.appendChild(remote);
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

void CDataBase::Submit()
{
    copyConfig(&m_Config,&m_SaveConfig);
    m_bModified = true;
}

void CDataBase::Revert()
{
    copyConfig(&m_SaveConfig,&m_Config);
    m_bModified = true;
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
    paras->wZoneNum = element.attributeNode("ZoneNum").value().toUInt();
}

void CDataBase::ParseTable(QDomElement element, BaseTab *config , int type)
{
    if(!element.isNull())
    {
        config->sDesc  = element.attributeNode("Desc").value();
        config->sKey   = element.attributeNode("SAddrIndexName").value();

        QDomElement elemItem = element.firstChildElement("Item");
        while ( !elemItem.isNull() )
        {
            switch (type)
            {
            case Enum_Table_Type_Ana:
                ParseAnaItem(elemItem,config);
                break;
            case Enum_Table_Type_Bi:
                ParseBiItem(elemItem,config);
                break;
            case Enum_Table_Type_SoftYB:
                ParseSoftYBItem(elemItem,config);
                break;
            case Enum_Table_Type_GsBo:
                ParseGooseBoItem(elemItem,config);
                break;
            case Enum_Table_Type_TripMatrix:
                ParseTripMatrixItem(elemItem,config);
                break;
            case Enum_Table_Type_Yk:
                ParseYKItem(elemItem,config);
                break;
            case Enum_Table_Type_Set:
                ParseSettingItem(elemItem,config);
                break;
            case Enum_Table_Type_Evt:
                ParseEvtItem(elemItem,config);
                break;
            default:
                break;
            }
            elemItem = elemItem.nextSiblingElement("Item");
        }
    }

    QString strInfo = QString("解析%1表完成:%2").arg(config->sDesc).arg(QString::number(config->items.size()));
    CMsgInfo msgInfo( CMsgInfo::Enum_Application_Parse_Mode, CMsgInfo::CN_HINT_MSG, strInfo );
    m_MsgInfoList.append(msgInfo);
}

void CDataBase::ParseAnaItem(QDomElement element, BaseTab *parent)
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
        CMsgInfo msgInfo( CMsgInfo::Enum_Application_Parse_Mode, CMsgInfo::CN_ERROR_MSG,
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
        CMsgInfo msgInfo( CMsgInfo::Enum_Application_Parse_Mode, CMsgInfo::CN_ERROR_MSG,
                          QString("%1表第%2条VirtualDft参数%3解析格式失败!")
                          .arg(parent->sDesc).arg(item->wIndex).arg(element.attributeNode("VirtualDft").value()) );
        m_MsgInfoList.append(msgInfo);
        item->iXuYCDft = -1;
    }

    item->wCoe      = element.attributeNode("Coe").value().toUInt();

    if(false == splitStr(item->sRateSetP,item->sRateSetS,element.attributeNode("RatedSetIndex").value()))
    {
        CMsgInfo msgInfo( CMsgInfo::Enum_Application_Parse_Mode, CMsgInfo::CN_ERROR_MSG,
                          QString("%1表第%2条RatedSetIndex参数%3解析格式失败!")
                          .arg(parent->sDesc).arg(item->wIndex).arg(element.attributeNode("RatedSetIndex").value()) );
        m_MsgInfoList.append(msgInfo);
    }

    if(false == splitStr(item->sRecSYb,item->sRecHYb,element.attributeNode("RecYbIndex").value()))
    {
        CMsgInfo msgInfo( CMsgInfo::Enum_Application_Parse_Mode, CMsgInfo::CN_ERROR_MSG,
                          QString("%1表第%2条RecYbIndex参数%3解析格式失败!")
                          .arg(parent->sDesc).arg(item->wIndex).arg(element.attributeNode("RecYbIndex").value()) );
        m_MsgInfoList.append(msgInfo);
    }

    if(false == splitUnInt(item->byWidth,item->byDotBit,element.attributeNode("DataAttr").value()))
    {
        CMsgInfo msgInfo( CMsgInfo::Enum_Application_Parse_Mode, CMsgInfo::CN_ERROR_MSG,
                          QString("%1表第%2条DataAttr参数%3解析格式失败!")
                          .arg(parent->sDesc).arg(item->wIndex).arg(element.attributeNode("DataAttr").value()) );
        m_MsgInfoList.append(msgInfo);
    }

    if(item->byDotBit >= item->byWidth)
    {
        CMsgInfo msgInfo( CMsgInfo::Enum_Application_Parse_Mode, CMsgInfo::CN_ERROR_MSG,
                          QString("%1表第%2条精度(%3)大于或等于位宽(%4)!")
                          .arg(parent->sDesc).arg(item->wIndex).arg(item->byDotBit).arg(item->byWidth) );
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

void CDataBase::ParseBiItem(QDomElement element, BaseTab *parent)
{
    if(element.isNull())
        return;

    BiItem * item = new BiItem(parent->items.size());

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
        CMsgInfo msgInfo( CMsgInfo::Enum_Application_Parse_Mode, CMsgInfo::CN_ERROR_MSG,
                          QString("%1表第%2条ChanAttr参数%3解析格式失败!")
                          .arg(parent->sDesc).arg(item->wIndex).arg(element.attributeNode("ChanAttr").value()) );
        m_MsgInfoList.append(msgInfo);
    }

    item->sChanType = element.attributeNode("ChanType").value();
    if(checkBiType(item->sChanType) == false)
    {
        CMsgInfo msgInfo( CMsgInfo::Enum_Application_Parse_Mode, CMsgInfo::CN_WARNNING_MSG,
                          QString("%1表第%2ChanType(%3)不属于标准类型（SPS：单点，DPS：双点，INS：整型，FLS：浮点数）!")
                          .arg(parent->sDesc).arg(item->wIndex).arg(element.attributeNode("ChanType").value()) );
        m_MsgInfoList.append(msgInfo);
    }

    item->sIndexDPS = element.attributeNode("IndexDPS").value();
    item->sIndexAna = element.attributeNode("IndexAna").value();

    bool ok;
    item->wHoldTime  = element.attributeNode("HoldTime").value().toUInt(&ok);
    if(( "" != element.attributeNode("HoldTime").value())&&(ok == false))
    {
        CMsgInfo msgInfo( CMsgInfo::Enum_Application_Parse_Mode, CMsgInfo::CN_ERROR_MSG,
                          QString("%1表第%2条HoldTime参数(%3)解析格式失败!")
                          .arg(parent->sDesc).arg(item->wIndex).arg(element.attributeNode("HoldTime").value()) );
        m_MsgInfoList.append(msgInfo);
    }

    item->sAlmLevel = element.attributeNode("AlmLevel").value();
    if(checkAlmLevel(item->sAlmLevel) == false)
    {
        CMsgInfo msgInfo( CMsgInfo::Enum_Application_Parse_Mode, CMsgInfo::CN_WARNNING_MSG,
                          QString("%1表第%2AlmLevel%3不属于标准类型（A类、B类、C类）!")
                          .arg(parent->sDesc).arg(item->wIndex).arg(element.attributeNode("AlmLevel").value()) );
        m_MsgInfoList.append(msgInfo);
    }

    parent->items.push_back(item);
}

void CDataBase::ParseSoftYBItem(QDomElement element, BaseTab *parent)
{
    if(element.isNull())
        return;

    SoftYBItem * item = new SoftYBItem(parent->items.size());

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
        CMsgInfo msgInfo( CMsgInfo::Enum_Application_Parse_Mode, CMsgInfo::CN_ERROR_MSG,
                          QString("%1表第%2条ChanAttr参数(%3)解析格式失败!")
                          .arg(parent->sDesc).arg(item->wIndex).arg(element.attributeNode("ChanAttr").value()) );
        m_MsgInfoList.append(msgInfo);
    }

    bool ok;
    item->wDftVal  = element.attributeNode("ValDft").value().toUInt(&ok);
    if(ok == false)
    {
        CMsgInfo msgInfo( CMsgInfo::Enum_Application_Parse_Mode, CMsgInfo::CN_ERROR_MSG,
                          QString("%1表第%2条ValDft参数(%3)解析格式失败!")
                          .arg(parent->sDesc).arg(item->wIndex).arg(element.attributeNode("ValDft").value()) );
        m_MsgInfoList.append(msgInfo);
    }

    if(checkCtlVal(item->wDftVal) == false)
    {
        CMsgInfo msgInfo( CMsgInfo::Enum_Application_Parse_Mode, CMsgInfo::CN_ERROR_MSG,
                          QString("%1表第%2条ValDft参数(%3)只能为0或者1!")
                          .arg(parent->sDesc).arg(item->wIndex).arg(element.attributeNode("ValDft").value()) );
        m_MsgInfoList.append(msgInfo);
    }

    parent->items.push_back(item);
}

void CDataBase::ParseGooseBoItem(QDomElement element, BaseTab *parent)
{
    if(element.isNull())
        return;

    GooseBOItem * item = new GooseBOItem(parent->items.size());

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
        CMsgInfo msgInfo( CMsgInfo::Enum_Application_Parse_Mode, CMsgInfo::CN_ERROR_MSG,
                          QString("%1表第%2条ChanAttr参数(%3)解析格式失败!")
                          .arg(parent->sDesc).arg(item->wIndex).arg(element.attributeNode("ChanAttr").value()) );
        m_MsgInfoList.append(msgInfo);
    }

    item->sHWChan   = element.attributeNode("HWChan").value();
    item->sFWChan   = element.attributeNode("FWChan").value();

    parent->items.push_back(item);
}

void CDataBase::ParseTripMatrixItem(QDomElement element, BaseTab *parent)
{
    if(element.isNull())
        return;

    TripMatrixItem * item = new TripMatrixItem(parent->items.size());

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
        CMsgInfo msgInfo( CMsgInfo::Enum_Application_Parse_Mode, CMsgInfo::CN_ERROR_MSG,
                          QString("%1表第%2条ChanAttr参数(%3)解析格式失败!")
                          .arg(parent->sDesc).arg(item->wIndex).arg(element.attributeNode("ChanAttr").value()) );
        m_MsgInfoList.append(msgInfo);
    }

    if(splitChan0(item->wBoardIndex,item->wBOFlag,element.attributeNode("Chan0").value()) == false)
    {
        CMsgInfo msgInfo( CMsgInfo::Enum_Application_Parse_Mode, CMsgInfo::CN_ERROR_MSG,
                          QString("%1表第%2条Chan0参数(%3)解析格式失败!")
                          .arg(parent->sDesc).arg(item->wIndex).arg(element.attributeNode("Chan0").value()) );
        m_MsgInfoList.append(msgInfo);
    }

    parent->items.push_back(item);
}

void CDataBase::ParseSettingItem(QDomElement element, BaseTab *parent)
{
    if(element.isNull())
        return;

    SettingItem * item = new SettingItem(parent->items.size());

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
        CMsgInfo msgInfo( CMsgInfo::Enum_Application_Parse_Mode, CMsgInfo::CN_ERROR_MSG,
                          QString("%1表第%2条ChanAttr参数(%3)解析格式失败!")
                          .arg(parent->sDesc).arg(item->wIndex).arg(element.attributeNode("ChanAttr").value()) );
        m_MsgInfoList.append(msgInfo);
    }

    if(false == split4UnInt(item->wValMin,item->wValMax,item->wValDft , item->wValStep ,element.attributeNode("ValAttr").value()))
    {
        CMsgInfo msgInfo( CMsgInfo::Enum_Application_Parse_Mode, CMsgInfo::CN_ERROR_MSG,
                          QString("%1表第%2条ValAttr参数(%3)解析格式失败!")
                          .arg(parent->sDesc).arg(item->wIndex).arg(element.attributeNode("ValAttr").value()) );
        m_MsgInfoList.append(msgInfo);
    }

    item->sSecIn     = element.attributeNode("SecIn").value();

    bool ok;
    item->wCoeRet  = element.attributeNode("CoeRet").value().toUInt(&ok);
    if(ok == false)
    {
        CMsgInfo msgInfo( CMsgInfo::Enum_Application_Parse_Mode, CMsgInfo::CN_ERROR_MSG,
                          QString("%1表第%2条CoeRet参数(%3)解析格式失败!")
                          .arg(parent->sDesc).arg(item->wIndex).arg(element.attributeNode("CoeRet").value()) );
        m_MsgInfoList.append(msgInfo);
    }

    item->sType     = element.attributeNode("ChanType").value();

    if(false == splitUnInt(item->byWidth,item->byDotBit,element.attributeNode("DataAttr").value()))
    {
        CMsgInfo msgInfo( CMsgInfo::Enum_Application_Parse_Mode, CMsgInfo::CN_ERROR_MSG,
                          QString("%1表第%2条DataAttr参数(%3)解析格式失败!")
                          .arg(parent->sDesc).arg(item->wIndex).arg(element.attributeNode("DataAttr").value()) );
        m_MsgInfoList.append(msgInfo);
    }

    if(false == splitStr(item->sKiloUnit,item->sUnit,element.attributeNode("Unit").value()))
    {
        CMsgInfo msgInfo( CMsgInfo::Enum_Application_Parse_Mode, CMsgInfo::CN_WARNNING_MSG,
                          QString("%1表第%2条Unit参数(%3)解析格式失败!")
                          .arg(parent->sDesc).arg(item->wIndex).arg(element.attributeNode("Unit").value()) );
        m_MsgInfoList.append(msgInfo);
    }

    parent->items.push_back(item);
}

void CDataBase::ParseEvtItem(QDomElement element, BaseTab *parent)
{
    if(element.isNull())
        return;

    EvtItem * item = new EvtItem(parent->items.size());

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
        CMsgInfo msgInfo( CMsgInfo::Enum_Application_Parse_Mode, CMsgInfo::CN_ERROR_MSG,
                          QString("%1表第%2条ChanAttr参数(%3)解析格式失败!")
                          .arg(parent->sDesc).arg(item->wIndex).arg(element.attributeNode("ChanAttr").value()) );
        m_MsgInfoList.append(msgInfo);
    }


    if(changeQStringToUInt(item->boConfig1, element.attributeNode("BoConfig").value()) == false)
    {
        CMsgInfo msgInfo( CMsgInfo::Enum_Application_Parse_Mode, CMsgInfo::CN_ERROR_MSG,
                          QString("%1表第%2条BoConfig参数(%3)解析格式失败!")
                          .arg(parent->sDesc).arg(item->wIndex).arg(element.attributeNode("BoConfig").value()) );
        m_MsgInfoList.append(msgInfo);
    }

    if(changeQStringToUInt(item->boConfig2, element.attributeNode("BoConfig2").value()) == false)
    {
        CMsgInfo msgInfo( CMsgInfo::Enum_Application_Parse_Mode, CMsgInfo::CN_ERROR_MSG,
                          QString("%1表第%2条BoConfig2参数(%3)解析格式失败!")
                          .arg(parent->sDesc).arg(item->wIndex).arg(element.attributeNode("BoConfig2").value()) );
        m_MsgInfoList.append(msgInfo);
    }

    if(changeQStringToUInt(item->boSignal, element.attributeNode("BoSignal").value()) == false)
    {
        CMsgInfo msgInfo( CMsgInfo::Enum_Application_Parse_Mode, CMsgInfo::CN_ERROR_MSG,
                          QString("%1表第%2条BoSignal参数(%3)解析格式失败!")
                          .arg(parent->sDesc).arg(item->wIndex).arg(element.attributeNode("BoSignal").value()) );
        m_MsgInfoList.append(msgInfo);
    }

    if(changeQStringToUInt(item->boLight, element.attributeNode("Light").value()) == false)
    {
        CMsgInfo msgInfo( CMsgInfo::Enum_Application_Parse_Mode, CMsgInfo::CN_ERROR_MSG,
                          QString("%1表第%2条Light参数(%3)解析格式失败!")
                          .arg(parent->sDesc).arg(item->wIndex).arg(element.attributeNode("Light").value()) );
        m_MsgInfoList.append(msgInfo);
    }

    item->sCnnGSIn    = element.attributeNode("BIIndex").value();

    parent->items.push_back(item);
}

void CDataBase::ParseYKItem(QDomElement element, BaseTab *parent)
{
    //TODO::
    Q_UNUSED(element);
    Q_UNUSED(parent);
    return;
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
        str2 = "";
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

bool CDataBase::split4UnInt(unsigned int &num1, unsigned int &num2, unsigned int &num3, unsigned int &num4, QString src)
{
    QStringList strList = src.split("/");
    if(strList.size() == 4)
    {
        bool ok1;
        bool ok2;
        bool ok3;
        bool ok4;
        int tem1 = strList[0].toUInt(&ok1);
        int tem2 = strList[1].toUInt(&ok2);
        int tem3 = strList[2].toUInt(&ok3);
        int tem4 = strList[3].toUInt(&ok4);
        if(ok1)
            num1 = tem1;
        if(ok2)
            num2 = tem2;
        if(ok3)
            num3 = tem3;
        if(ok4)
            num4 = tem4;

        if(ok1&&ok2&&ok3&&ok4)
            return true;
        else
            return false;
    }
    else
    {
        return false;
    }
}

bool CDataBase::checkNameNoExit(QString name)
{
    //改名的时候用，名称全局唯一,已有此名字时提示输出，返回false
    if(name == "")
        return true;

    for(int i = 0; i < m_tables.size() ; i++)
    {
        if(m_tables[i]->getNameCounts(name) > 0)
            return false;
    }

    return true;
}

bool CDataBase::checkNameNotDuplicate(QString name)
{
    //校验时用，名称全局唯一,有重复（找到2个及以上时）时提示输出，返回false
    if(name == "")
        return true;

    int num = 0;
    for(int i = 0; i < m_tables.size() ; i++)
    {
        num += m_tables[i]->getNameCounts(name);
        if(num > 1)
            return false;
    }

    return true;
}

bool CDataBase::checkSPSetCnnInfo(QString info)
{
    //检查不分组定值外部连接信息的格式是否正确，是否可以找到外部链接信息,需要输出检查记录
    //格式错误、关键字错误、索引越限
    return m_Config.settingSPConifg.matchKeyAndRowExit(info);
}

bool CDataBase::checkSoftYBCnnInfo(QString info)
{
    //检查软压板外部连接信息的格式是否正确，是否可以找到外部链接信息,需要输出检查记录
    //格式错误、关键字错误、索引越限
    return m_Config.softYBConfig.matchKeyAndRowExit(info);
}

bool CDataBase::checkHardBICnnInfo(QString info)
{
    //检查硬开入外部连接信息的格式是否正确，是否可以找到外部链接信息,需要输出检查记录
    //格式错误、关键字错误、索引越限
    return m_Config.generalBiConfig.matchKeyAndRowExit(info);
}

bool CDataBase::checkGSINCnnInfo(QString info)
{
    return m_Config.gooseBiConfig.matchKeyAndRowExit(info);
}

bool CDataBase::checkIndexDPS(QString info)
{
    //可能是硬开入，可能是软遥信，可能是goose开入
    if(m_Config.generalBiConfig.matchKeyAndRowExit(info)
            ||m_Config.signalConfig.matchKeyAndRowExit(info)
            ||m_Config.gooseBiConfig.matchKeyAndRowExit(info))
        return true;
    else
        return false;
}

bool CDataBase::checkIndexGSAna(QString info)
{
    return m_Config.gsAnaConfig.matchKeyAndRowExit(info);
}

bool CDataBase::checkBiType(QString type)
{
    //BI类型为SPS：单点，DPS：双点，INS：整型，FLS：浮点数
    if(("SPS" == type)
            || ("DPS" == type)
            || ("INS" == type)
            || ("FLS" == type))
        return true;
    else
        return false;
}

bool CDataBase::checkAlmLevel(QString level)
{
    //A类、B类、C类，缺省A类
    if(("A" == level)
            || ("B" == level)
            || ("C" == level))
        return true;
    else
        return false;
}

bool CDataBase::checkCtlVal(unsigned int val)
{
    if((0 != val)
            &&(1 != val))
        return false;
    else
        return true;
}

bool CDataBase::checkHWChan(QString info)
{
    //可能是跳闸矩阵.硬开入，软遥信，模拟量通道
    if(m_Config.tripMaxtrixConfig.matchKeyAndRowExit(info)
            ||m_Config.generalBiConfig.matchKeyAndRowExit(info)
            ||m_Config.signalConfig.matchKeyAndRowExit(info)
            ||m_Config.adAnaConfig.matchKeyAndRowExit(info)
            ||m_Config.derivedConfig.matchKeyAndRowExit(info)
            ||m_Config.svConfig.matchKeyAndRowExit(info)
            ||m_Config.otherAnaConfig.matchKeyAndRowExit(info))
        return true;
    else
        return false;
}

bool CDataBase::checkFWChan(QString info)
{
    //可能是跳闸矩阵.硬开入，软遥信
    if(m_Config.tripMaxtrixConfig.matchKeyAndRowExit(info)
            ||m_Config.generalBiConfig.matchKeyAndRowExit(info)
            ||m_Config.signalConfig.matchKeyAndRowExit(info))
        return true;
    else
        return false;
}

bool CDataBase::checkSetType(QString info)
{
    QStringList list;
    list << "NULL" << "CTL" << "CUR" << "VOL" << "IMP" << "ANG" << "TIME" << "PTCT" << "STR";
    if(list.contains(info))
        return true;
    else
        return false;
}

QString CDataBase::changeDecToHex(unsigned int val)
{
    if(val == 0)
        return "0";

    QString s = "0x" + QString::number(val,16).toUpper();
    return s;
}

QString CDataBase::changeToChan0(unsigned int board, unsigned int flag)
{
    if(board + flag == 0)
        return "0";

    unsigned int num = board << 28;
    num += flag;
    QString s = "0x" + QString::number(num,16).toUpper();
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

bool CDataBase::splitChan0(unsigned int &board, unsigned int &flag, QString chan0)
{
    unsigned int num;
    bool ok = false;
    if(chan0.startsWith("0x"))
        num = chan0.toUInt(&ok,16);
    else
        num = chan0.toUInt(&ok,10);

    if(!ok)
        return false;

    board = num >> 28;
    flag = num & 0xFFFFFFF;
    return true;
}

void CDataBase::copyConfig(GseConfig *pSrc, GseConfig *pDst)
{
    pDst->clear();
    pDst->deviceInfo = pSrc->deviceInfo;
    pDst->deviceParas = pSrc->deviceParas;

    pDst->adAnaConfig.sDesc = pSrc->adAnaConfig.sDesc;
    pDst->adAnaConfig.sKey = pSrc->adAnaConfig.sKey;
    for(int i = 0 ; i < pSrc->adAnaConfig.items.size(); i++)
    {
        AnaItem * item = new AnaItem(i);
        *item = *(AnaItem *)(pSrc->adAnaConfig.items[i]);
        pDst->adAnaConfig.items.push_back(item);
    }

    pDst->derivedConfig.sDesc = pSrc->derivedConfig.sDesc;
    pDst->derivedConfig.sKey = pSrc->derivedConfig.sKey;
    for(int i = 0 ; i < pSrc->derivedConfig.items.size(); i++)
    {
        AnaItem * item = new AnaItem(i);
        *item = *(AnaItem *)(pSrc->derivedConfig.items[i]);
        pDst->derivedConfig.items.push_back(item);
    }

    pDst->svConfig.sDesc = pSrc->svConfig.sDesc;
    pDst->svConfig.sKey = pSrc->svConfig.sKey;
    for(int i = 0 ; i < pSrc->svConfig.items.size(); i++)
    {
        AnaItem * item = new AnaItem(i);
        *item = *(AnaItem *)(pSrc->svConfig.items[i]);
        pDst->svConfig.items.push_back(item);
    }

    pDst->gsAnaConfig.sDesc = pSrc->gsAnaConfig.sDesc;
    pDst->gsAnaConfig.sKey = pSrc->gsAnaConfig.sKey;
    for(int i = 0 ; i < pSrc->gsAnaConfig.items.size(); i++)
    {
        AnaItem * item = new AnaItem(i);
        *item = *(AnaItem *)(pSrc->gsAnaConfig.items[i]);
        pDst->gsAnaConfig.items.push_back(item);
    }

    pDst->otherAnaConfig.sDesc = pSrc->otherAnaConfig.sDesc;
    pDst->otherAnaConfig.sKey = pSrc->otherAnaConfig.sKey;
    for(int i = 0 ; i < pSrc->otherAnaConfig.items.size(); i++)
    {
        AnaItem * item = new AnaItem(i);
        *item = *(AnaItem *)(pSrc->otherAnaConfig.items[i]);
        pDst->otherAnaConfig.items.push_back(item);
    }

    pDst->generalBiConfig.sDesc = pSrc->generalBiConfig.sDesc;
    pDst->generalBiConfig.sKey = pSrc->generalBiConfig.sKey;
    for(int i = 0 ; i < pSrc->generalBiConfig.items.size(); i++)
    {
        BiItem * item = new BiItem(i);
        *item = *(BiItem *)(pSrc->generalBiConfig.items[i]);
        pDst->generalBiConfig.items.push_back(item);
    }

    pDst->signalConfig.sDesc = pSrc->signalConfig.sDesc;
    pDst->signalConfig.sKey = pSrc->signalConfig.sKey;
    for(int i = 0 ; i < pSrc->signalConfig.items.size(); i++)
    {
        BiItem * item = new BiItem(i);
        *item = *(BiItem *)(pSrc->signalConfig.items[i]);
        pDst->signalConfig.items.push_back(item);
    }

    pDst->gooseBiConfig.sDesc = pSrc->gooseBiConfig.sDesc;
    pDst->gooseBiConfig.sKey = pSrc->gooseBiConfig.sKey;
    for(int i = 0 ; i < pSrc->gooseBiConfig.items.size(); i++)
    {
        BiItem * item = new BiItem(i);
        *item = *(BiItem *)(pSrc->gooseBiConfig.items[i]);
        pDst->gooseBiConfig.items.push_back(item);
    }

    pDst->softYBConfig.sDesc = pSrc->softYBConfig.sDesc;
    pDst->softYBConfig.sKey = pSrc->softYBConfig.sKey;
    for(int i = 0 ; i < pSrc->softYBConfig.items.size(); i++)
    {
        SoftYBItem * item = new SoftYBItem(i);
        *item = *(SoftYBItem *)(pSrc->softYBConfig.items[i]);
        pDst->softYBConfig.items.push_back(item);
    }

    pDst->gooseBoConfig.sDesc = pSrc->gooseBoConfig.sDesc;
    pDst->gooseBoConfig.sKey = pSrc->gooseBoConfig.sKey;
    for(int i = 0 ; i < pSrc->gooseBoConfig.items.size(); i++)
    {
        GooseBOItem * item = new GooseBOItem(i);
        *item = *(GooseBOItem *)(pSrc->gooseBoConfig.items[i]);
        pDst->gooseBoConfig.items.push_back(item);
    }

    pDst->tripMaxtrixConfig.sDesc = pSrc->tripMaxtrixConfig.sDesc;
    pDst->tripMaxtrixConfig.sKey = pSrc->tripMaxtrixConfig.sKey;
    for(int i = 0 ; i < pSrc->tripMaxtrixConfig.items.size(); i++)
    {
        TripMatrixItem * item = new TripMatrixItem(i);
        *item = *(TripMatrixItem *)(pSrc->tripMaxtrixConfig.items[i]);
        pDst->tripMaxtrixConfig.items.push_back(item);
    }

    pDst->ykConfig.sDesc = pSrc->ykConfig.sDesc;
    pDst->ykConfig.sKey = pSrc->ykConfig.sKey;
    for(int i = 0 ; i < pSrc->ykConfig.items.size(); i++)
    {
        BaseItem * item = new BaseItem(i);
        *item = *(BaseItem *)(pSrc->ykConfig.items[i]);
        pDst->ykConfig.items.push_back(item);
    }

    pDst->settingSPConifg.sDesc = pSrc->settingSPConifg.sDesc;
    pDst->settingSPConifg.sKey = pSrc->settingSPConifg.sKey;
    for(int i = 0 ; i < pSrc->settingSPConifg.items.size(); i++)
    {
        SettingItem * item = new SettingItem(i);
        *item = *(SettingItem *)(pSrc->settingSPConifg.items[i]);
        pDst->settingSPConifg.items.push_back(item);
    }

    pDst->settingSGConifg.sDesc = pSrc->settingSGConifg.sDesc;
    pDst->settingSGConifg.sKey = pSrc->settingSGConifg.sKey;
    for(int i = 0 ; i < pSrc->settingSGConifg.items.size(); i++)
    {
        SettingItem * item = new SettingItem(i);
        *item = *(SettingItem *)(pSrc->settingSGConifg.items[i]);
        pDst->settingSGConifg.items.push_back(item);
    }

    pDst->evtAlmConfig.sDesc = pSrc->evtAlmConfig.sDesc;
    pDst->evtAlmConfig.sKey = pSrc->evtAlmConfig.sKey;
    for(int i = 0 ; i < pSrc->evtAlmConfig.items.size(); i++)
    {
        EvtItem * item = new EvtItem(i);
        *item = *(EvtItem *)(pSrc->evtAlmConfig.items[i]);
        pDst->evtAlmConfig.items.push_back(item);
    }

    pDst->evtActConfig.sDesc = pSrc->evtActConfig.sDesc;
    pDst->evtActConfig.sKey = pSrc->evtActConfig.sKey;
    for(int i = 0 ; i < pSrc->evtActConfig.items.size(); i++)
    {
        EvtItem * item = new EvtItem(i);
        *item = *(EvtItem *)(pSrc->evtActConfig.items[i]);
        pDst->evtActConfig.items.push_back(item);
    }

    pDst->evtCheckConfig.sDesc = pSrc->evtCheckConfig.sDesc;
    pDst->evtCheckConfig.sKey = pSrc->evtCheckConfig.sKey;
    for(int i = 0 ; i < pSrc->evtCheckConfig.items.size(); i++)
    {
        EvtItem * item = new EvtItem(i);
        *item = *(EvtItem *)(pSrc->evtCheckConfig.items[i]);
        pDst->evtCheckConfig.items.push_back(item);
    }

    pDst->remoteTripConfig.sDesc = pSrc->remoteTripConfig.sDesc;
    pDst->remoteTripConfig.sKey = pSrc->remoteTripConfig.sKey;
    for(int i = 0 ; i < pSrc->remoteTripConfig.items.size(); i++)
    {
        EvtItem * item = new EvtItem(i);
        *item = *(EvtItem *)(pSrc->remoteTripConfig.items[i]);
        pDst->remoteTripConfig.items.push_back(item);
    }
}

void CDataBase::CheckConfig()
{
    CMsgInfo msgInfoStart( CMsgInfo::Enum_Application_Verify_Mode, CMsgInfo::CN_HINT_MSG, QString("配置校验开始！"));
    m_MsgInfoList.append(msgInfoStart);

    //模拟量表校验
    CheckAnaTab(&m_Config.adAnaConfig);
    CheckAnaTab(&m_Config.derivedConfig);
    CheckAnaTab(&m_Config.svConfig);
    CheckAnaTab(&m_Config.gsAnaConfig);
    CheckAnaTab(&m_Config.otherAnaConfig);

    //开关量校验
    CheckBiTab(&m_Config.generalBiConfig);
    CheckBiTab(&m_Config.signalConfig);
    CheckBiTab(&m_Config.gooseBiConfig);
    CheckSoftYBTab(&m_Config.softYBConfig);

    //开出量
    CheckGsBoTab(&m_Config.gooseBoConfig);
    CheckTripMatrixTab(&m_Config.tripMaxtrixConfig);
    CheckYKTab(&m_Config.ykConfig);

    //定值表
    CheckSettingTab(&m_Config.settingSPConifg);
    CheckSettingTab(&m_Config.settingSGConifg);

    //事件表
    CheckEvtTab(&m_Config.evtAlmConfig);
    CheckEvtTab(&m_Config.evtActConfig);
    CheckEvtTab(&m_Config.evtCheckConfig);
    CheckEvtTab(&m_Config.remoteTripConfig);

    CMsgInfo msgInfoEnd( CMsgInfo::Enum_Application_Verify_Mode, CMsgInfo::CN_HINT_MSG, QString("配置校验完成！"));
    m_MsgInfoList.append(msgInfoEnd);
}

void CDataBase::CheckAnaTab(BaseTab *config)
{
    for(int i = 0; i < config->items.size(); i++)
    {
        AnaItem * p = (AnaItem *)config->items[i];

        if(checkNameNotDuplicate(p->sName) == false)
        {
            CMsgInfo msgInfo( CMsgInfo::Enum_Application_Verify_Mode, CMsgInfo::CN_ERROR_MSG,
                              QString("%1表第%2条名称（%3）重复!")
                              .arg(config->sDesc).arg(p->wIndex+1).arg(p->sName) );
            m_MsgInfoList.append(msgInfo);
        }

        if(checkSPSetCnnInfo(p->sRateSetP) == false)
        {
            CMsgInfo msgInfo( CMsgInfo::Enum_Application_Verify_Mode, CMsgInfo::CN_WARNNING_MSG,
                              QString("%1表第%2条一次额定值关联（%3）异常!")
                              .arg(config->sDesc).arg(p->wIndex+1).arg(p->sRateSetP) );
            m_MsgInfoList.append(msgInfo);
        }

        if(checkSPSetCnnInfo(p->sRateSetS) == false)
        {
            CMsgInfo msgInfo( CMsgInfo::Enum_Application_Verify_Mode, CMsgInfo::CN_WARNNING_MSG,
                              QString("%1表第%2条二次额定值关联（%3）异常!")
                              .arg(config->sDesc).arg(p->wIndex+1).arg(p->sRateSetS) );
            m_MsgInfoList.append(msgInfo);
        }

        if(checkSoftYBCnnInfo(p->sRecSYb) == false)
        {
            CMsgInfo msgInfo( CMsgInfo::Enum_Application_Verify_Mode, CMsgInfo::CN_WARNNING_MSG,
                              QString("%1表第%2条接收软压板关联（%3）异常!")
                              .arg(config->sDesc).arg(p->wIndex+1).arg(p->sRecSYb) );
            m_MsgInfoList.append(msgInfo);
        }

        if(checkBiType(p->sRecHYb) == false)
        {
            CMsgInfo msgInfo( CMsgInfo::Enum_Application_Verify_Mode, CMsgInfo::CN_WARNNING_MSG,
                              QString("%1表第%2条接收硬压板关联（%3）异常!")
                              .arg(config->sDesc).arg(p->wIndex+1).arg(p->sRecHYb) );
            m_MsgInfoList.append(msgInfo);
        }

        if(p->byWidth <= p->byDotBit)
        {
            CMsgInfo msgInfo( CMsgInfo::Enum_Application_Verify_Mode, CMsgInfo::CN_ERROR_MSG,
                              QString("%1表第%2条位宽（%3）小于或等于精度（%4）!")
                              .arg(config->sDesc).arg(p->wIndex+1).arg(p->byWidth).arg(p->byDotBit) );
            m_MsgInfoList.append(msgInfo);
        }
    }
}

void CDataBase::CheckBiTab(BaseTab *config)
{
    for(int i = 0; i < config->items.size(); i++)
    {
        BiItem * p = (BiItem *)config->items[i];

        if(checkNameNotDuplicate(p->sName) == false)
        {
            CMsgInfo msgInfo( CMsgInfo::Enum_Application_Verify_Mode, CMsgInfo::CN_ERROR_MSG,
                              QString("%1表第%2条名称（%3）重复!")
                              .arg(config->sDesc).arg(p->wIndex+1).arg(p->sName) );
            m_MsgInfoList.append(msgInfo);
        }

        if(checkBiType(p->sChanType) == false)
        {
            CMsgInfo msgInfo( CMsgInfo::Enum_Application_Verify_Mode, CMsgInfo::CN_WARNNING_MSG,
                              QString("%1表第%2条遥信类型（%3）不属于标准类型（SPS：单点，DPS：双点，INS：整型，FLS：浮点数）!")
                              .arg(config->sDesc).arg(p->wIndex+1).arg(p->sChanType) );
            m_MsgInfoList.append(msgInfo);
        }

        if(checkIndexDPS(p->sIndexDPS) == false)
        {
            CMsgInfo msgInfo( CMsgInfo::Enum_Application_Verify_Mode, CMsgInfo::CN_WARNNING_MSG,
                              QString("%1表第%2条常闭节点关联（%3）异常!")
                              .arg(config->sDesc).arg(p->wIndex+1).arg(p->sIndexDPS) );
            m_MsgInfoList.append(msgInfo);
        }

        if(checkIndexGSAna(p->sIndexAna) == false)
        {
            CMsgInfo msgInfo( CMsgInfo::Enum_Application_Verify_Mode, CMsgInfo::CN_WARNNING_MSG,
                              QString("%1表第%2条浮点值通道关联（%3）异常!")
                              .arg(config->sDesc).arg(p->wIndex+1).arg(p->sIndexAna) );
            m_MsgInfoList.append(msgInfo);
        }

        if(checkAlmLevel(p->sAlmLevel) == false)
        {
            CMsgInfo msgInfo( CMsgInfo::Enum_Application_Verify_Mode, CMsgInfo::CN_WARNNING_MSG,
                              QString("%1表第%2条告警类型（%3）异常!")
                              .arg(config->sDesc).arg(p->wIndex+1).arg(p->sAlmLevel) );
            m_MsgInfoList.append(msgInfo);
        }
    }
}

void CDataBase::CheckSoftYBTab(BaseTab *config)
{
    for(int i = 0; i < config->items.size(); i++)
    {
        SoftYBItem * p = (SoftYBItem *)config->items[i];

        if(checkNameNotDuplicate(p->sName) == false)
        {
            CMsgInfo msgInfo( CMsgInfo::Enum_Application_Verify_Mode, CMsgInfo::CN_ERROR_MSG,
                              QString("%1表第%2条名称（%3）重复!")
                              .arg(config->sDesc).arg(p->wIndex+1).arg(p->sName) );
            m_MsgInfoList.append(msgInfo);
        }

        if(checkCtlVal(p->wDftVal) == false)
        {
            CMsgInfo msgInfo( CMsgInfo::Enum_Application_Verify_Mode, CMsgInfo::CN_WARNNING_MSG,
                              QString("%1表第%2条缺省值（%3）异常!")
                              .arg(config->sDesc).arg(p->wIndex+1).arg(p->wDftVal) );
            m_MsgInfoList.append(msgInfo);
        }
    }
}

void CDataBase::CheckGsBoTab(BaseTab *config)
{
    for(int i = 0; i < config->items.size(); i++)
    {
        GooseBOItem * p = (GooseBOItem *)config->items[i];

        if(checkNameNotDuplicate(p->sName) == false)
        {
            CMsgInfo msgInfo( CMsgInfo::Enum_Application_Verify_Mode, CMsgInfo::CN_ERROR_MSG,
                              QString("%1表第%2条名称（%3）重复!")
                              .arg(config->sDesc).arg(p->wIndex+1).arg(p->sName) );
            m_MsgInfoList.append(msgInfo);
        }

        if(checkHWChan(p->sHWChan) == false)
        {
            CMsgInfo msgInfo( CMsgInfo::Enum_Application_Verify_Mode, CMsgInfo::CN_WARNNING_MSG,
                              QString("%1表第%2条合位关联（%3）异常!")
                              .arg(config->sDesc).arg(p->wIndex+1).arg(p->sHWChan) );
            m_MsgInfoList.append(msgInfo);
        }

        if(checkFWChan(p->sFWChan) == false)
        {
            CMsgInfo msgInfo( CMsgInfo::Enum_Application_Verify_Mode, CMsgInfo::CN_WARNNING_MSG,
                              QString("%1表第%2条分位关联（%3）异常!")
                              .arg(config->sDesc).arg(p->wIndex+1).arg(p->sFWChan) );
            m_MsgInfoList.append(msgInfo);
        }
    }
}

void CDataBase::CheckTripMatrixTab(BaseTab *config)
{
    for(int i = 0; i < config->items.size(); i++)
    {
        TripMatrixItem * p = (TripMatrixItem *)config->items[i];

        if(checkNameNotDuplicate(p->sName) == false)
        {
            CMsgInfo msgInfo( CMsgInfo::Enum_Application_Verify_Mode, CMsgInfo::CN_ERROR_MSG,
                              QString("%1表第%2条名称（%3）重复!")
                              .arg(config->sDesc).arg(p->wIndex+1).arg(p->sName) );
            m_MsgInfoList.append(msgInfo);
        }
    }
}

void CDataBase::CheckYKTab(BaseTab *config)
{
    for(int i = 0; i < config->items.size(); i++)
    {
        BaseItem * p = (BaseItem *)config->items[i];

        if(checkNameNotDuplicate(p->sName) == false)
        {
            CMsgInfo msgInfo( CMsgInfo::Enum_Application_Verify_Mode, CMsgInfo::CN_ERROR_MSG,
                              QString("%1表第%2条名称（%3）重复!")
                              .arg(config->sDesc).arg(p->wIndex+1).arg(p->sName) );
            m_MsgInfoList.append(msgInfo);
        }
    }
}

void CDataBase::CheckSettingTab(BaseTab *config)
{
    for(int i = 0; i < config->items.size(); i++)
    {
        SettingItem * p = (SettingItem *)config->items[i];

        if(checkNameNotDuplicate(p->sName) == false)
        {
            CMsgInfo msgInfo( CMsgInfo::Enum_Application_Verify_Mode, CMsgInfo::CN_ERROR_MSG,
                              QString("%1表第%2条名称（%3）重复!")
                              .arg(config->sDesc).arg(p->wIndex+1).arg(p->sName) );
            m_MsgInfoList.append(msgInfo);
        }

        if(p->wValMax < p->wValMin)
        {
            CMsgInfo msgInfo( CMsgInfo::Enum_Application_Verify_Mode, CMsgInfo::CN_ERROR_MSG,
                              QString("%1表第%2条最大值（%3）小于最小值（%4）!")
                              .arg(config->sDesc).arg(p->wIndex+1).arg(p->wValMax).arg(p->wValMin) );
            m_MsgInfoList.append(msgInfo);
        }

        if(p->wValDft < p->wValMin)
        {
            CMsgInfo msgInfo( CMsgInfo::Enum_Application_Verify_Mode, CMsgInfo::CN_ERROR_MSG,
                              QString("%1表第%2条缺省值（%3）小于最小值（%4）!")
                              .arg(config->sDesc).arg(p->wIndex+1).arg(p->wValDft).arg(p->wValMin) );
            m_MsgInfoList.append(msgInfo);
        }

        if(p->wValDft > p->wValMax)
        {
            CMsgInfo msgInfo( CMsgInfo::Enum_Application_Verify_Mode, CMsgInfo::CN_ERROR_MSG,
                              QString("%1表第%2条缺省值（%3）大于最大值（%4）!")
                              .arg(config->sDesc).arg(p->wIndex+1).arg(p->wValDft).arg(p->wValMax) );
            m_MsgInfoList.append(msgInfo);
        }

        if(checkSPSetCnnInfo(p->sSecIn) == false)
        {
            CMsgInfo msgInfo( CMsgInfo::Enum_Application_Verify_Mode, CMsgInfo::CN_WARNNING_MSG,
                              QString("%1表第%2条二次额定值关联（%3）异常!")
                              .arg(config->sDesc).arg(p->wIndex+1).arg(p->sSecIn) );
            m_MsgInfoList.append(msgInfo);
        }

        if(checkSetType(p->sType) == false)
        {
            CMsgInfo msgInfo( CMsgInfo::Enum_Application_Verify_Mode, CMsgInfo::CN_WARNNING_MSG,
                              QString("%1表第%2条定值类型（%3）异常，不属于0.缺省（NULL），1：控制字(CTL)，2：电流型(CUR)，3：电压型(VOL)，4：阻抗型(IMP)，5：角度型(ANG)，6：时间型(TIME)，7：PTCT一二次额定值(PTCT)，8：字符串(STR)中的一种!")
                              .arg(config->sDesc).arg(p->wIndex+1).arg(p->sType) );
            m_MsgInfoList.append(msgInfo);
        }

        if(p->byWidth <= p->byDotBit)
        {
            CMsgInfo msgInfo( CMsgInfo::Enum_Application_Verify_Mode, CMsgInfo::CN_ERROR_MSG,
                              QString("%1表第%2条位宽（%3）小于或等于精度（%4）!")
                              .arg(config->sDesc).arg(p->wIndex+1).arg(p->byWidth).arg(p->byDotBit) );
            m_MsgInfoList.append(msgInfo);
        }
    }
}

void CDataBase::CheckEvtTab(BaseTab *config)
{
    for(int i = 0; i < config->items.size(); i++)
    {
        EvtItem * p = (EvtItem *)config->items[i];

        if(checkNameNotDuplicate(p->sName) == false)
        {
            CMsgInfo msgInfo( CMsgInfo::Enum_Application_Verify_Mode, CMsgInfo::CN_ERROR_MSG,
                              QString("%1表第%2条名称（%3）重复!")
                              .arg(config->sDesc).arg(p->wIndex+1).arg(p->sName) );
            m_MsgInfoList.append(msgInfo);
        }

        if(checkGSINCnnInfo(p->sCnnGSIn) == false)
        {
            CMsgInfo msgInfo( CMsgInfo::Enum_Application_Verify_Mode, CMsgInfo::CN_WARNNING_MSG,
                              QString("%1表第%2条关联远跳开入（%3）异常!")
                              .arg(config->sDesc).arg(p->wIndex+1).arg(p->sCnnGSIn) );
            m_MsgInfoList.append(msgInfo);
        }
    }
}

QString CDataBase::bindString(QString str1, QString str2)
{
    QString str;
    if(str1 == "")
        str = "-1/";
    else
        str = str1 +"/";

    if(str2 == "")
        str += "-1";
    else
        str += str2;

    return str;
}
