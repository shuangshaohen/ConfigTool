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
    //QByteArray utfData = DoTranUtf8(fileDat);
    file.close();

    //if (!bUtf8)
    //{
    //	QTextStream vReadStream(&file);
    //	QTextCodec* vCodec = QTextCodec::codecForName("UTF-8");
    //	vReadStream.setCodec(vCodec);
    //	QString vXmlDataStr = vReadStream.readAll();
    //	fileDat = vXmlDataStr.toUtf8();
    //	printf("file :%d\n", fileDat.size());
    //}

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

    //解析模拟量;
    //bool ok = false;
    QDomElement elemAna = elemRoot.firstChildElement("AnaConfig");
    while ( !elemAna.isNull() )
    {
        QDomElement elemItem = elemAna.firstChildElement("Item");
        while ( !elemItem.isNull() )
        {
            AnaConfig * item = new AnaConfig();
            item->sDesc = elemItem.attributeNode("Desc").value();
            item->sName = elemItem.attributeNode("Name").value();
            item->sUnit = elemItem.attributeNode("Unit").value();
            item->sKiloUnit = elemItem.attributeNode("KiloUnit").value();
            item->sAddr = elemItem.attributeNode("SAddr").value();

            item->wIndex = ChangeStringToInt(elemItem.attributeNode("Index").value());
            item->dwAttr = ChangeStringToInt(elemItem.attributeNode("Attr").value());
            item->dwXuYcDft = ChangeStringToInt(elemItem.attributeNode("XuYcDft").value());
            item->wCoe = ChangeStringToInt(elemItem.attributeNode("Coe").value());
            item->wChan = ChangeStringToInt(elemItem.attributeNode("Chan").value());
            item->wBoardChan = ChangeStringToInt(elemItem.attributeNode("BoardChan").value());
            item->dwRFVIndex = ChangeStringToInt(elemItem.attributeNode("RFVIndex").value());
            item->dwSecIn = ChangeStringToInt(elemItem.attributeNode("SecIn").value());
            item->dwMuSoft = ChangeStringToInt(elemItem.attributeNode("MuSoft").value());
            item->dwMuHard = ChangeStringToInt(elemItem.attributeNode("MuHard").value());
            item->wChanType = ChangeStringToInt(elemItem.attributeNode("ChanType").value());
            item->dwCapacity = ChangeStringToInt(elemItem.attributeNode("Capacity").value());
            item->byWidth = ChangeStringToInt(elemItem.attributeNode("Width").value());
            item->byDotBit = ChangeStringToInt(elemItem.attributeNode("DotBit").value());
            item->wPhaseName = ChangeStringToInt(elemItem.attributeNode("PhaseName").value());

            m_Config.anaConfig.push_back(item);

            elemItem = elemItem.nextSiblingElement("Item");
        }

        elemAna = elemAna.nextSiblingElement("AnaConfig");
    }

    QString strInfoAna = QString("解析模拟量完成:%1").arg(QString::number(m_Config.anaConfig.size()));
    CMsgInfo msgInfoAna( CMsgInfo::Enum_Application_Parse_Mode, CMsgInfo::CN_HINT_MSG, strInfoAna );
    m_MsgInfoList.append(msgInfoAna);

    //解析开入量;
    QDomElement elemBi = elemRoot.firstChildElement("BiConfig");
    while ( !elemBi.isNull() )
    {
        QDomElement elemItem = elemBi.firstChildElement("Item");
        while ( !elemItem.isNull() )
        {
            BiConfig * item = new BiConfig();
            item->sDesc = elemItem.attributeNode("Desc").value();
            item->sName = elemItem.attributeNode("Name").value();
            item->sEnName = elemItem.attributeNode("EnName").value();
            item->sAddr = elemItem.attributeNode("SAddr").value();

            item->wIndex = ChangeStringToInt(elemItem.attributeNode("Index").value());
            item->dwAttr = ChangeStringToInt(elemItem.attributeNode("Attr").value());
            item->wChan = ChangeStringToInt(elemItem.attributeNode("Chan").value());
            item->wBoardChan = ChangeStringToInt(elemItem.attributeNode("BoardChan").value());
            item->wIndexDPS = ChangeStringToInt(elemItem.attributeNode("IndexDPS").value());
            item->wHoldTime = ChangeStringToInt(elemItem.attributeNode("HoldTime").value());
            item->wYXType = ChangeStringToInt(elemItem.attributeNode("YXType").value());
            m_Config.biConfig.push_back(item);

            elemItem = elemItem.nextSiblingElement("Item");
        }

        elemBi = elemBi.nextSiblingElement("BiConfig");
    }
    QString strInfoBI = QString("解析开关量完成:%1").arg(QString::number(m_Config.biConfig.size()));
    CMsgInfo msgInfoBI( CMsgInfo::Enum_Application_Parse_Mode, CMsgInfo::CN_HINT_MSG, strInfoBI );
    m_MsgInfoList.append(msgInfoBI);

    //解析Goose开入量;
    QDomElement elemGsBi = elemRoot.firstChildElement("GooseBiConfig");
    while ( !elemGsBi.isNull() )
    {
        QDomElement elemItem = elemGsBi.firstChildElement("Item");
        while ( !elemItem.isNull() )
        {
            GooseBiConfig * item = new GooseBiConfig();
            item->sEnName = elemItem.attributeNode("EnName").value();
            item->sName = elemItem.attributeNode("Name").value();
            item->sAddr = elemItem.attributeNode("SAddr").value();

            item->wIndex = ChangeStringToInt(elemItem.attributeNode("Index").value());
            item->dwAttr = ChangeStringToInt(elemItem.attributeNode("Attr").value());
            item->HWChan = ChangeStringToInt(elemItem.attributeNode("HWChan").value());
            item->FWChan = ChangeStringToInt(elemItem.attributeNode("FWChan").value());

            m_Config.gsBiConfig.push_back(item);

            elemItem = elemItem.nextSiblingElement("Item");
        }

        elemGsBi = elemGsBi.nextSiblingElement("GooseBiConfig");
    }
    QString strInfoGSBI = QString("解析GS开入完成:%1").arg(QString::number(m_Config.gsBiConfig.size()));
    CMsgInfo msgInfoGSBI( CMsgInfo::Enum_Application_Parse_Mode, CMsgInfo::CN_HINT_MSG, strInfoGSBI );
    m_MsgInfoList.append(msgInfoGSBI);

    //解析Goose开出量;
    QDomElement elemGsBo = elemRoot.firstChildElement("GooseBoConfig");
    while ( !elemGsBo.isNull() )
    {
        QDomElement elemItem = elemGsBo.firstChildElement("Item");
        while ( !elemItem.isNull() )
        {
            GooseBoConfig * item = new GooseBoConfig();
            item->sEnName = elemItem.attributeNode("EnName").value();
            item->sName = elemItem.attributeNode("Name").value();
            item->sAddr = elemItem.attributeNode("SAddr").value();

            item->wIndex = ChangeStringToInt(elemItem.attributeNode("Index").value());
            item->dwAttr = ChangeStringToInt(elemItem.attributeNode("Attr").value());
            item->wSrcTab = ChangeStringToInt(elemItem.attributeNode("SrcTab").value());
            item->HWChan = ChangeStringToInt(elemItem.attributeNode("HWChan").value());
            item->FWChan = ChangeStringToInt(elemItem.attributeNode("FWChan").value());

            m_Config.gsBoConfig.push_back(item);

            elemItem = elemItem.nextSiblingElement("Item");
        }

        elemGsBo = elemGsBo.nextSiblingElement("GooseBoConfig");
    }
    QString strInfoGSBO = QString("解析GS开出完成:%1").arg(QString::number(m_Config.gsBoConfig.size()));
    CMsgInfo msgInfoGSBO( CMsgInfo::Enum_Application_Parse_Mode, CMsgInfo::CN_HINT_MSG, strInfoGSBO );
    m_MsgInfoList.append(msgInfoGSBO);

    //解析跳闸举证;
    QDomElement elemTrip = elemRoot.firstChildElement("TripMatrix");
    while ( !elemTrip.isNull() )
    {
        QDomElement elemItem = elemTrip.firstChildElement("Item");
        while ( !elemItem.isNull() )
        {
            TripMatrixConfig * item = new TripMatrixConfig();
            item->sDesc = elemItem.attributeNode("Desc").value();
            item->sName = elemItem.attributeNode("Name").value();
            item->sEnName = elemItem.attributeNode("EnName").value();

            item->wIndex = ChangeStringToInt(elemItem.attributeNode("Index").value());
            item->dwAttr = ChangeStringToInt(elemItem.attributeNode("Attr").value());
            item->Chan0 = ChangeStringToInt(elemItem.attributeNode("Chan0").value());
            item->IndexDPS = ChangeStringToInt(elemItem.attributeNode("IndexDPS").value());

            m_Config.tripMatrix.push_back(item);

            elemItem = elemItem.nextSiblingElement("Item");
        }

        elemTrip = elemTrip.nextSiblingElement("TripMatrix");
    }
    QString strInfoTrip = QString("解析跳闸矩阵完成:%1").arg(QString::number(m_Config.tripMatrix.size()));
    CMsgInfo msgInfoTrip( CMsgInfo::Enum_Application_Parse_Mode, CMsgInfo::CN_HINT_MSG, strInfoTrip );
    m_MsgInfoList.append(msgInfoTrip);

    //解析告警;
    QDomElement elemAlarm = elemRoot.firstChildElement("AlarmRec");
    while ( !elemAlarm.isNull() )
    {
        QDomElement elemItem = elemAlarm.firstChildElement("Item");
        while ( !elemItem.isNull() )
        {
            RecConfig * item = new RecConfig();
            item->sDesc = elemItem.attributeNode("Desc").value();
            item->sName = elemItem.attributeNode("Name").value();
            item->sAddr = elemItem.attributeNode("SAddr").value();

            item->wIndex = ChangeStringToInt(elemItem.attributeNode("Index").value());
            item->dwAttr = ChangeStringToInt(elemItem.attributeNode("Attr").value());
            item->dwBOConfig = ChangeStringToInt(elemItem.attributeNode("BoConfig").value());
            item->dwBOConfig2 = ChangeStringToInt(elemItem.attributeNode("BoConfig2").value());
            item->dwBOSignal = ChangeStringToInt(elemItem.attributeNode("BoSignal").value());
            item->dwLight = ChangeStringToInt(elemItem.attributeNode("Light").value());

            m_Config.alarmConfig.push_back(item);

            elemItem = elemItem.nextSiblingElement("Item");
        }

        elemAlarm = elemAlarm.nextSiblingElement("AlarmRec");
    }
    QString strInfoAlarmRec = QString("解析告警记录完成:%1").arg(QString::number(m_Config.alarmConfig.size()));
    CMsgInfo msgInfoAlarmRec( CMsgInfo::Enum_Application_Parse_Mode, CMsgInfo::CN_HINT_MSG, strInfoAlarmRec );
    m_MsgInfoList.append(msgInfoAlarmRec);

    //解析动作;
    QDomElement elemAct = elemRoot.firstChildElement("ActRec");
    while ( !elemAct.isNull() )
    {
        QDomElement elemItem = elemAct.firstChildElement("Item");
        while ( !elemItem.isNull() )
        {
            RecConfig * item = new RecConfig();
            item->sDesc = elemItem.attributeNode("Desc").value();
            item->sName = elemItem.attributeNode("Name").value();
            item->sAddr = elemItem.attributeNode("SAddr").value();

            item->wIndex = ChangeStringToInt(elemItem.attributeNode("Index").value());
            item->dwAttr = ChangeStringToInt(elemItem.attributeNode("Attr").value());
            item->dwBOConfig = ChangeStringToInt(elemItem.attributeNode("BoConfig").value());
            item->dwBOConfig2 = ChangeStringToInt(elemItem.attributeNode("BoConfig2").value());
            item->dwBOSignal = ChangeStringToInt(elemItem.attributeNode("BoSignal").value());
            item->dwLight = ChangeStringToInt(elemItem.attributeNode("Light").value());

            m_Config.actConfig.push_back(item);

            elemItem = elemItem.nextSiblingElement("Item");
        }

        elemAct = elemAct.nextSiblingElement("ActRec");
    }
    QString strInfoActRec = QString("解析动作记录完成:%1").arg(QString::number(m_Config.actConfig.size()));
    CMsgInfo msgInfoActRec( CMsgInfo::Enum_Application_Parse_Mode, CMsgInfo::CN_HINT_MSG, strInfoActRec );
    m_MsgInfoList.append(msgInfoActRec);

    //解析定值;
    QDomElement elemSet = elemRoot.firstChildElement("Setting");
    while ( !elemSet.isNull() )
    {
        QDomElement elemItem = elemSet.firstChildElement("Item");
        while ( !elemItem.isNull() )
        {
            SettingConfig * item = new SettingConfig();
            item->sDesc = elemItem.attributeNode("Desc").value();
            item->sName = elemItem.attributeNode("Name").value();
            item->sUnit = elemItem.attributeNode("Unit").value();
            item->sKiloUnit = elemItem.attributeNode("KiloUnit").value();
            item->sAddr = elemItem.attributeNode("SAddr").value();

            item->wIndex = ChangeStringToInt(elemItem.attributeNode("Index").value());
            item->dwAttr = ChangeStringToInt(elemItem.attributeNode("Attr").value());
            item->dwValMin = ChangeStringToInt(elemItem.attributeNode("ValMin").value());
            item->dwValMax = ChangeStringToInt(elemItem.attributeNode("ValMax").value());
            item->dwValDft = ChangeStringToInt(elemItem.attributeNode("ValDft").value());
            item->wSecIn = ChangeStringToInt(elemItem.attributeNode("SecIn").value());
            item->wCoeRet = ChangeStringToInt(elemItem.attributeNode("CoeRet").value());
            item->wType = ChangeStringToInt(elemItem.attributeNode("Type").value());
            item->byGroup = ChangeStringToInt(elemItem.attributeNode("Group").value());
            item->byWidth = ChangeStringToInt(elemItem.attributeNode("Width").value());
            item->byDotBit = ChangeStringToInt(elemItem.attributeNode("DotBit").value());

            m_Config.settingConfig.push_back(item);

            elemItem = elemItem.nextSiblingElement("Item");
        }

        elemSet = elemSet.nextSiblingElement("Setting");
    }
    QString strInfoSetting = QString("解析定值完成:%1").arg(QString::number(m_Config.settingConfig.size()));
    CMsgInfo msgInfoSetting( CMsgInfo::Enum_Application_Parse_Mode, CMsgInfo::CN_HINT_MSG, strInfoSetting );
    m_MsgInfoList.append(msgInfoSetting);

    //解析定值组;
    QDomElement elemGroup = elemRoot.firstChildElement("SettingGroup");
    if (!elemGroup.isNull())
    {
        m_Config.setGroup = elemGroup.attributeNode("number").value().toInt();
    }
    QString strInfoSettingGroup = QString("解析定值组数完成:%1").arg(QString::number(m_Config.setGroup));
    CMsgInfo msgInfoSettingGroup( CMsgInfo::Enum_Application_Parse_Mode, CMsgInfo::CN_HINT_MSG, strInfoSettingGroup );
    m_MsgInfoList.append(msgInfoSettingGroup);

    //解析软压板;
    QDomElement elemSoft = elemRoot.firstChildElement("SoftYb");
    while ( !elemSoft.isNull() )
    {
        QDomElement elemItem = elemSoft.firstChildElement("Item");
        while ( !elemItem.isNull() )
        {
            SoftYbConfig * item = new SoftYbConfig();
            item->sDesc = elemItem.attributeNode("Desc").value();
            item->sName = elemItem.attributeNode("Name").value();
            item->wIndex = ChangeStringToInt(elemItem.attributeNode("Index").value());
            item->dwAttr = ChangeStringToInt(elemItem.attributeNode("Attr").value());
            item->ValDft = ChangeStringToInt(elemItem.attributeNode("ValDft").value());

            m_Config.softYbConfig.push_back(item);

            elemItem = elemItem.nextSiblingElement("Item");
        }

        elemSoft = elemSoft.nextSiblingElement("SoftYb");
    }
    QString strInfoSoftYb = QString("解析软压板完成:%1").arg(QString::number(m_Config.softYbConfig.size()));
    CMsgInfo msgInfoSoftYb( CMsgInfo::Enum_Application_Parse_Mode, CMsgInfo::CN_HINT_MSG, strInfoSoftYb );
    m_MsgInfoList.append(msgInfoSoftYb);

    //解析远程跳闸配置;
    QDomElement elemRemote = elemRoot.firstChildElement("RemoteTrip");
    while ( !elemRemote.isNull() )
    {
        QDomElement elemItem = elemRemote.firstChildElement("Item");
        while ( !elemItem.isNull() )
        {
            RemoteTripConfig * item = new RemoteTripConfig();
            item->sDesc = elemItem.attributeNode("Desc").value();
            item->sName = elemItem.attributeNode("Name").value();
            item->sAddr = elemItem.attributeNode("SAddr").value();

            item->wIndex = ChangeStringToInt(elemItem.attributeNode("Index").value());
            item->dwAttr = ChangeStringToInt(elemItem.attributeNode("Attr").value());
            item->dwBiIndex = ChangeStringToInt(elemItem.attributeNode("BIIndex").value());
            item->dwBOConfig = ChangeStringToInt(elemItem.attributeNode("BoConfig").value());
            item->dwBOConfig2 = ChangeStringToInt(elemItem.attributeNode("BoConfig2").value());
            item->dwBOSignal = ChangeStringToInt(elemItem.attributeNode("BoSignal").value());
            item->dwLight = ChangeStringToInt(elemItem.attributeNode("Light").value());

            m_Config.remoteTrip.push_back(item);

            elemItem = elemItem.nextSiblingElement("Item");
        }

        elemRemote = elemRemote.nextSiblingElement("RemoteTrip");
    }
    QString strInfoRemoteTrip = QString("解析远跳完成:%1").arg(QString::number(m_Config.remoteTrip.size()));
    CMsgInfo msgInfoRemoteTrip( CMsgInfo::Enum_Application_Parse_Mode, CMsgInfo::CN_HINT_MSG, strInfoRemoteTrip);
    m_MsgInfoList.append(msgInfoRemoteTrip);

    this->CheckConfig();
    this->RefreshCnn();

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
    CheckCnn();

    QDomDocument document;  //QDomDocument类
    this->SaveInstruction(&document);
    this->SaveRoot(&document,"root");

    document.save(out, 4, QDomNode::EncodingFromTextStream);
    file.close();

    QString strInfo = QString(" 保存文件 %1 成功！")
                        .arg(fileName);
    CMsgInfo msgInfo( CMsgInfo::Enum_Application_Save_Mode, CMsgInfo::CN_HINT_MSG, strInfo );
    m_MsgInfoList.append(msgInfo);

    m_bModified = false;

    return "";
}

void CDataBase::SaveInstruction(QDomDocument *doc)
{
    QDomProcessingInstruction instruction = doc->createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\"");
    doc->appendChild(instruction);
}

void CDataBase::SaveRoot(QDomDocument *doc, QString str)
{
    QDomElement root = doc->createElement(str);//创建根节点  QDomElemet元素

    //DeviceInfo
    this->SaveDeviceInfo(doc,&root);

    //AnaConfig
    QDomElement anaConfig = doc->createElement("AnaConfig");
    anaConfig.setAttribute("number",QString::number(m_Config.anaConfig.size()));
    for(int i = 0 ; i < m_Config.anaConfig.size(); i++)
    {
        m_Config.anaConfig[i]->wIndex = i;
        m_Config.anaConfig[i]->sAddr = "ANA:B01." + QString::number(i);
        SaveAnaItem(doc,&anaConfig,m_Config.anaConfig[i]);
    }
    root.appendChild(anaConfig);

    //BiConfig
    QDomElement biConfig = doc->createElement("BiConfig");
    biConfig.setAttribute("number",QString::number(m_Config.biConfig.size()));
    for(int i = 0 ; i < m_Config.biConfig.size(); i++)
    {
        m_Config.biConfig[i]->wIndex = i;
        m_Config.biConfig[i]->sAddr = "BI:B01." + QString::number(i);
        SaveBiItem(doc,&biConfig,m_Config.biConfig[i]);
    }
    root.appendChild(biConfig);

    //GSBiConfig
    QDomElement gsBiConfig = doc->createElement("GooseBiConfig");
    gsBiConfig.setAttribute("number",QString::number(m_Config.gsBiConfig.size()));
    for(int i = 0 ; i < m_Config.gsBiConfig.size(); i++)
    {
        m_Config.gsBiConfig[i]->wIndex = i;
        m_Config.gsBiConfig[i]->sAddr = "GSBI:B01." + QString::number(i);
        SaveGsBiItem(doc,&gsBiConfig,m_Config.gsBiConfig[i]);
    }
    root.appendChild(gsBiConfig);

    //GSBoConfig
    QDomElement gsBoConfig = doc->createElement("GooseBoConfig");
    gsBoConfig.setAttribute("number",QString::number(m_Config.gsBoConfig.size()));
    for(int i = 0 ; i < m_Config.gsBoConfig.size(); i++)
    {
        m_Config.gsBoConfig[i]->wIndex = i;
        m_Config.gsBoConfig[i]->sAddr = "GSBO:B01." + QString::number(i);
        SaveGsBoItem(doc,&gsBoConfig,m_Config.gsBoConfig[i]);
    }
    root.appendChild(gsBoConfig);

    //TripMatrix
    QDomElement tripMatrix = doc->createElement("TripMatrix");
    tripMatrix.setAttribute("number",QString::number(m_Config.tripMatrix.size()));
    for(int i = 0 ; i < m_Config.tripMatrix.size(); i++)
    {
        m_Config.tripMatrix[i]->wIndex = i;
        SaveTripMatrixItem(doc,&tripMatrix,m_Config.tripMatrix[i]);
    }
    root.appendChild(tripMatrix);

    //Setting
    QDomElement setting = doc->createElement("Setting");
    setting.setAttribute("number",QString::number(m_Config.settingConfig.size()));
    for(int i = 0 ; i < m_Config.settingConfig.size(); i++)
    {
        m_Config.settingConfig[i]->wIndex = i;
        m_Config.settingConfig[i]->sAddr = "SET:B01." + QString::number(i);
        SaveSettingItem(doc,&setting,m_Config.settingConfig[i]);
    }
    root.appendChild(setting);

    //SettingGroup
    QDomElement settingGroup = doc->createElement("SettingGroup");
    settingGroup.setAttribute("number",QString::number(m_Config.setGroup));
    root.appendChild(settingGroup);

    //AlarmRec
    QDomElement alarmRec = doc->createElement("AlarmRec");
    alarmRec.setAttribute("number",QString::number(m_Config.alarmConfig.size()));
    for(int i = 0 ; i < m_Config.alarmConfig.size(); i++)
    {
        m_Config.alarmConfig[i]->wIndex = i;
        m_Config.alarmConfig[i]->sAddr = "ALM:B01." + QString::number(i);
        SaveRecItem(doc,&alarmRec,m_Config.alarmConfig[i]);
    }
    root.appendChild(alarmRec);

    //ActRec
    QDomElement actRec = doc->createElement("ActRec");
    actRec.setAttribute("number",QString::number(m_Config.actConfig.size()));
    for(int i = 0 ; i < m_Config.actConfig.size(); i++)
    {
        m_Config.actConfig[i]->wIndex = i;
        m_Config.actConfig[i]->sAddr = "EVT:B01." + QString::number(i);
        SaveRecItem(doc,&actRec,m_Config.actConfig[i]);
    }
    root.appendChild(actRec);

    //RemoteTrip
//    QDomElement remoteTrip = doc->createElement("RemoteTrip");
//    remoteTrip.setAttribute("number",QString::number(m_Config.remoteTrip.size()));
//    for(int i = 0 ; i < m_Config.remoteTrip.size(); i++)
//    {
//        m_Config.remoteTrip[i]->wIndex = i;
//        m_Config.remoteTrip[i]->sAddr = "RMT:B01." + QString::number(i);
//        SaveRemoteTripItem(doc,&remoteTrip,m_Config.remoteTrip[i]);
//    }
//    root.appendChild(remoteTrip);

    //SoftYb
    QDomElement softYb = doc->createElement("SoftYb");
    softYb.setAttribute("number",QString::number(m_Config.softYbConfig.size()));
    for(int i = 0 ; i < m_Config.softYbConfig.size(); i++)
    {
        m_Config.softYbConfig[i]->wIndex = i;
        SaveSoftYBItem(doc,&softYb,m_Config.softYbConfig[i]);
    }
    root.appendChild(softYb);

    doc->appendChild(root);//添加根节点
}

void CDataBase::SaveDeviceInfo(QDomDocument *doc, QDomElement *parentNode)
{
    QDomElement deviceInfo = doc->createElement("DeviceInfo");
    deviceInfo.setAttribute("Model", "3078");
    deviceInfo.setAttribute("Type", "GSE8000");
    deviceInfo.setAttribute("Name", "GSE8000");
    deviceInfo.setAttribute("Version", "1.10");
    QDateTime time = QDateTime::currentDateTime();
    QString str = time.toString("yyyy-MM-dd hh:mm:ss");
    deviceInfo.setAttribute("Date", str);
    deviceInfo.setAttribute("CRC", "1234ABCD");
    deviceInfo.setAttribute("SamRate", "40");
    parentNode->appendChild(deviceInfo);
}

void CDataBase::SaveAnaItem(QDomDocument *doc, QDomElement *parentNode, AnaConfig *p)
{
    QDomElement item = doc->createElement("Item");
    item.setAttribute("Index",       ChangeNumToDEC(p->wIndex));
    item.setAttribute("Attr",        ChangeNumToHEX(p->dwAttr));
    item.setAttribute("XuYcDft",     ChangeNumToDEC(p->dwXuYcDft));
    item.setAttribute("Coe",         ChangeNumToDEC(p->wCoe));
    item.setAttribute("Chan",        ChangeNumToDEC(p->wChan));
    item.setAttribute("BoardChan",   ChangeNumToDEC(p->wBoardChan));
    item.setAttribute("RFVIndex",    ChangeNumToDEC(p->dwRFVIndex));
    item.setAttribute("SecIn",       ChangeNumToDEC(p->dwSecIn));
    item.setAttribute("MuSoft",      ChangeNumToDEC(p->dwMuSoft));
    item.setAttribute("MuHard",      ChangeNumToDEC(p->dwMuHard));
    item.setAttribute("ChanType",    ChangeNumToDEC(p->wChanType));
    item.setAttribute("Capacity",    ChangeNumToDEC(p->dwCapacity));
    item.setAttribute("Name",        p->sName);
    item.setAttribute("Desc",        p->sDesc);
    item.setAttribute("Width",       ChangeNumToDEC(p->byWidth));
    item.setAttribute("DotBit",      ChangeNumToDEC(p->byDotBit));
    item.setAttribute("Unit",        p->sUnit);
    item.setAttribute("KiloUnit",    p->sKiloUnit);
    item.setAttribute("PhaseName",    ChangeNumToDEC(p->wPhaseName));
    item.setAttribute("SAddr",       p->sAddr);
    parentNode->appendChild(item);
}

void CDataBase::SaveBiItem(QDomDocument *doc, QDomElement *parentNode, BiConfig *p)
{
    QDomElement item = doc->createElement("Item");
    item.setAttribute("Index",       ChangeNumToDEC(p->wIndex));
    item.setAttribute("Attr",        ChangeNumToHEX(p->dwAttr));
    item.setAttribute("Chan",        ChangeNumToDEC(p->wChan));
    item.setAttribute("BoardChan",   ChangeNumToDEC(p->wBoardChan));
    item.setAttribute("IndexDPS",    ChangeNumToDEC(p->wIndexDPS));
    item.setAttribute("HoldTime",    ChangeNumToDEC(p->wHoldTime));
    item.setAttribute("YXType",      ChangeNumToHEX(p->wYXType));
    item.setAttribute("Name",        p->sName);
    item.setAttribute("Desc",        p->sDesc);
    item.setAttribute("EnName",      p->sEnName);
    item.setAttribute("SAddr",       p->sAddr);
    parentNode->appendChild(item);
}

void CDataBase::SaveGsBiItem(QDomDocument *doc, QDomElement *parentNode, GooseBiConfig *p)
{
    QDomElement item = doc->createElement("Item");
    item.setAttribute("Index",      ChangeNumToDEC(p->wIndex));
    item.setAttribute("Attr",       ChangeNumToHEX(p->dwAttr));
    item.setAttribute("HWChan",     ChangeNumToDEC(p->HWChan));
    item.setAttribute("FWChan",     ChangeNumToDEC(p->FWChan));
    item.setAttribute("Name",       p->sName);
    item.setAttribute("EnName",     p->sEnName);
    item.setAttribute("SAddr",      p->sAddr);
    parentNode->appendChild(item);
}

void CDataBase::SaveGsBoItem(QDomDocument *doc, QDomElement *parentNode, GooseBoConfig *p)
{
    QDomElement item = doc->createElement("Item");
    item.setAttribute("Index",      ChangeNumToDEC(p->wIndex));
    item.setAttribute("Attr",       ChangeNumToHEX(p->dwAttr));
    item.setAttribute("SrcTab",     ChangeNumToHEX(p->wSrcTab));
    item.setAttribute("HWChan",     ChangeNumToDEC(p->HWChan));
    item.setAttribute("FWChan",     ChangeNumToDEC(p->FWChan));
    item.setAttribute("Name",       p->sName);
    item.setAttribute("EnName",     p->sEnName);
    item.setAttribute("SAddr",      p->sAddr);
    parentNode->appendChild(item);
}

void CDataBase::SaveTripMatrixItem(QDomDocument *doc, QDomElement *parentNode, TripMatrixConfig *p)
{
    QDomElement item = doc->createElement("Item");
    item.setAttribute("Index",      ChangeNumToDEC(p->wIndex));
    item.setAttribute("Attr",       ChangeNumToHEX(p->dwAttr));
    item.setAttribute("Chan0",      ChangeNumToHEX(p->Chan0));
    item.setAttribute("IndexDPS",   ChangeNumToDEC(p->IndexDPS));
    item.setAttribute("Name",       p->sName);
    item.setAttribute("EnName",     p->sEnName);
    item.setAttribute("Desc",       p->sDesc);
    parentNode->appendChild(item);
}

void CDataBase::SaveSettingItem(QDomDocument *doc, QDomElement *parentNode, SettingConfig *p)
{
    QDomElement item = doc->createElement("Item");
    item.setAttribute("Index",      ChangeNumToDEC(p->wIndex));
    item.setAttribute("Attr",       ChangeNumToHEX(p->dwAttr));
    item.setAttribute("ValMin",     ChangeNumToDEC(p->dwValMin));
    item.setAttribute("ValMax",     ChangeNumToDEC(p->dwValMax));
    item.setAttribute("ValDft",     ChangeNumToDEC(p->dwValDft));
    item.setAttribute("SecIn",      ChangeNumToDEC(p->wSecIn));
    item.setAttribute("CoeRet",     ChangeShortToDEC(p->wCoeRet));
    item.setAttribute("Type",       ChangeNumToDEC(p->wType));
    item.setAttribute("Group",      ChangeNumToDEC(p->byGroup));
    item.setAttribute("Width",      ChangeNumToDEC(p->byWidth));
    item.setAttribute("DotBit",     ChangeNumToDEC(p->byDotBit));
    item.setAttribute("Name",       p->sName);
    item.setAttribute("Desc",       p->sDesc);
    item.setAttribute("Unit",       p->sUnit);
    item.setAttribute("KiloUnit",   p->sKiloUnit);
    item.setAttribute("sAddr",      p->sAddr);
    parentNode->appendChild(item);
}

void CDataBase::SaveRecItem(QDomDocument *doc, QDomElement *parentNode, RecConfig *p)
{
    QDomElement item = doc->createElement("Item");
    item.setAttribute("Index",      ChangeNumToDEC(p->wIndex));
    item.setAttribute("Attr",       ChangeNumToHEX(p->dwAttr));
    item.setAttribute("BoConfig",   ChangeNumToHEX(p->dwBOConfig));
    item.setAttribute("BoConfig2",  ChangeNumToHEX(p->dwBOConfig2));
    item.setAttribute("BoSignal",   ChangeNumToHEX(p->dwBOSignal));
    item.setAttribute("Light",      ChangeNumToHEX(p->dwLight));
    item.setAttribute("Name",       p->sName);
    item.setAttribute("Desc",       p->sDesc);
    item.setAttribute("sAddr",      p->sAddr);
    parentNode->appendChild(item);
}

void CDataBase::SaveRemoteTripItem(QDomDocument *doc, QDomElement *parentNode, RemoteTripConfig *p)
{
    QDomElement item = doc->createElement("Item");
    item.setAttribute("Index",      ChangeNumToDEC(p->wIndex));
    item.setAttribute("Attr",       ChangeNumToHEX(p->dwAttr));
    item.setAttribute("BIIndex",    ChangeNumToDEC(p->dwBiIndex));
    item.setAttribute("BoConfig",   ChangeNumToHEX(p->dwBOConfig));
    item.setAttribute("BoConfig2",  ChangeNumToHEX(p->dwBOConfig2));
    item.setAttribute("BoSignal",   ChangeNumToHEX(p->dwBOSignal));
    item.setAttribute("Light",      ChangeNumToHEX(p->dwLight));
    item.setAttribute("Name",       p->sName);
    item.setAttribute("Desc",       p->sDesc);
    item.setAttribute("sAddr",      p->sAddr);
    parentNode->appendChild(item);
}

void CDataBase::SaveSoftYBItem(QDomDocument *doc, QDomElement *parentNode, SoftYbConfig *p)
{
    QDomElement item = doc->createElement("Item");
    item.setAttribute("Index",      ChangeNumToDEC(p->wIndex));
    item.setAttribute("Attr",       ChangeNumToHEX(p->dwAttr));
    item.setAttribute("ValDft",    ChangeNumToDEC(p->ValDft));
    item.setAttribute("Name",       p->sName);
    item.setAttribute("Desc",       p->sDesc);
    parentNode->appendChild(item);
}

void CDataBase::CheckAnaConfig()
{
    //首先检查name的唯一性，index后期会因为增删操作导致变化不唯一，在最后导出配置时，统一更新
    int sum = m_Config.anaConfig.size();
    for(int i = 0; i < sum; i++ )
    {
        QString name = m_Config.anaConfig[i]->sName;
        for(int j = 0 ; j < sum; j++)
        {
            if((i != j)
                &&(name == m_Config.anaConfig[j]->sName))
            {
                m_Config.anaConfig[i]->sName = "DUPLICATE_ERROR_" + QString::number(i) + "_" + name;
                m_Config.anaConfig[j]->sName = "DUPLICATE_ERROR_" + QString::number(j) + "_" + name;
                m_bModified = true;

                QString msg = QString("模拟量表 行%1 与 行%2 都为%3，已自动更名，需确认修改！")
                                    .arg(QString::number(i))
                                    .arg(QString::number(j))
                                    .arg(name);
                CMsgInfo msgInfo( CMsgInfo::Enum_Application_Verify_Mode, CMsgInfo::CN_ERROR_MSG, msg);
                m_MsgInfoList.append(msgInfo);

                break;
            }
        }
    }
}

void CDataBase::CheckBIConfig()
{
    //首先检查name的唯一性，index后期会因为增删操作导致变化不唯一，在最后导出配置时，统一更新
    int sum = m_Config.biConfig.size();
    for(int i = 0; i < sum; i++ )
    {
        QString name = m_Config.biConfig[i]->sName;
        for(int j = 0 ; j < sum; j++)
        {
            if((i != j)
                &&(name == m_Config.biConfig[j]->sName))
            {
                m_Config.biConfig[i]->sName = "DUPLICATE_ERROR_" + QString::number(i) + "_" + name;
                m_Config.biConfig[j]->sName = "DUPLICATE_ERROR_" + QString::number(j) + "_" + name;
                m_bModified = true;

                QString msg = QString("开关量 行%1 与 行%2 都为%3，已自动更名，需确认修改！")
                                    .arg(QString::number(i))
                                    .arg(QString::number(j))
                                    .arg(name);
                CMsgInfo msgInfo( CMsgInfo::Enum_Application_Verify_Mode, CMsgInfo::CN_ERROR_MSG, msg);
                m_MsgInfoList.append(msgInfo);

                break;
            }
        }
    }
}

void CDataBase::CheckGSBIConfig()
{
    //首先检查name的唯一性，index后期会因为增删操作导致变化不唯一，在最后导出配置时，统一更新
    int sum = m_Config.gsBiConfig.size();
    for(int i = 0; i < sum; i++ )
    {
        QString name = m_Config.gsBiConfig[i]->sName;
        for(int j = 0 ; j < sum; j++)
        {
            if((i != j)
                &&(name == m_Config.gsBiConfig[j]->sName))
            {
                m_Config.gsBiConfig[i]->sName = "DUPLICATE_ERROR_" + QString::number(i) + "_" + name;
                m_Config.gsBiConfig[j]->sName = "DUPLICATE_ERROR_" + QString::number(j) + "_" + name;
                m_bModified = true;

                QString msg = QString("GS开入表 行%1 与 行%2 都为%3，已自动更名，需确认修改！")
                                    .arg(QString::number(i))
                                    .arg(QString::number(j))
                                    .arg(name);
                CMsgInfo msgInfo( CMsgInfo::Enum_Application_Verify_Mode, CMsgInfo::CN_ERROR_MSG, msg);
                m_MsgInfoList.append(msgInfo);

                break;
            }
        }
    }
}

void CDataBase::CheckGSBOConfig()
{
    //首先检查name的唯一性，index后期会因为增删操作导致变化不唯一，在最后导出配置时，统一更新
    int sum = m_Config.gsBoConfig.size();
    for(int i = 0; i < sum; i++ )
    {
        QString name = m_Config.gsBoConfig[i]->sName;
        for(int j = 0 ; j < sum; j++)
        {
            if((i != j)
                &&(name == m_Config.gsBoConfig[j]->sName))
            {
                m_Config.gsBoConfig[i]->sName = "DUPLICATE_ERROR_" + QString::number(i) + "_" + name;
                m_Config.gsBoConfig[j]->sName = "DUPLICATE_ERROR_" + QString::number(j) + "_" + name;
                m_bModified = true;

                QString msg = QString("GS开出表 行%1 与 行%2 都为%3，已自动更名，需确认修改！")
                                    .arg(QString::number(i))
                                    .arg(QString::number(j))
                                    .arg(name);
                CMsgInfo msgInfo( CMsgInfo::Enum_Application_Verify_Mode, CMsgInfo::CN_ERROR_MSG, msg);
                m_MsgInfoList.append(msgInfo);

                break;
            }
        }
    }
}

void CDataBase::CheckTripMatrixConfig()
{
    //首先检查name的唯一性，index后期会因为增删操作导致变化不唯一，在最后导出配置时，统一更新
    int sum = m_Config.tripMatrix.size();
    for(int i = 0; i < sum; i++ )
    {
        QString name = m_Config.tripMatrix[i]->sName;
        for(int j = 0 ; j < sum; j++)
        {
            if((i != j)
                &&(name == m_Config.tripMatrix[j]->sName))
            {
                m_Config.tripMatrix[i]->sName = "DUPLICATE_ERROR_" + QString::number(i) + "_" + name;
                m_Config.tripMatrix[j]->sName = "DUPLICATE_ERROR_" + QString::number(j) + "_" + name;
                m_bModified = true;

                QString msg = QString("GS开出表 行%1 与 行%2 都为%3，已自动更名，需确认修改！")
                                    .arg(QString::number(i))
                                    .arg(QString::number(j))
                                    .arg(name);
                CMsgInfo msgInfo( CMsgInfo::Enum_Application_Verify_Mode, CMsgInfo::CN_ERROR_MSG, msg);
                m_MsgInfoList.append(msgInfo);

                break;
            }
        }
    }
}

void CDataBase::CheckSettingConfig()
{
    //首先检查name的唯一性，index后期会因为增删操作导致变化不唯一，在最后导出配置时，统一更新
    int sum = m_Config.settingConfig.size();
    for(int i = 0; i < sum; i++ )
    {
        QString name = m_Config.settingConfig[i]->sName;
        for(int j = 0 ; j < sum; j++)
        {
            if((i != j)
                &&(name == m_Config.settingConfig[j]->sName))
            {
                m_Config.settingConfig[i]->sName = "DUPLICATE_ERROR_" + QString::number(i) + "_" + name;
                m_Config.settingConfig[j]->sName = "DUPLICATE_ERROR_" + QString::number(j) + "_" + name;
                m_bModified = true;

                QString msg = QString("GS开出表 行%1 与 行%2 都为%3，已自动更名，需确认修改！")
                                    .arg(QString::number(i))
                                    .arg(QString::number(j))
                                    .arg(name);
                CMsgInfo msgInfo( CMsgInfo::Enum_Application_Verify_Mode, CMsgInfo::CN_ERROR_MSG, msg);
                m_MsgInfoList.append(msgInfo);

                break;
            }
        }
    }
}

void CDataBase::CheckAlarmConfig()
{
    //首先检查name的唯一性，index后期会因为增删操作导致变化不唯一，在最后导出配置时，统一更新
    int sum = m_Config.alarmConfig.size();
    for(int i = 0; i < sum; i++ )
    {
        QString name = m_Config.alarmConfig[i]->sName;
        for(int j = 0 ; j < sum; j++)
        {
            if((i != j)
                &&(name == m_Config.alarmConfig[j]->sName))
            {
                m_Config.alarmConfig[i]->sName = "DUPLICATE_ERROR_" + QString::number(i) + "_" + name;
                m_Config.alarmConfig[j]->sName = "DUPLICATE_ERROR_" + QString::number(j) + "_" + name;
                m_bModified = true;

                QString msg = QString("GS开出表 行%1 与 行%2 都为%3，已自动更名，需确认修改！")
                                    .arg(QString::number(i))
                                    .arg(QString::number(j))
                                    .arg(name);
                CMsgInfo msgInfo( CMsgInfo::Enum_Application_Verify_Mode, CMsgInfo::CN_ERROR_MSG, msg);
                m_MsgInfoList.append(msgInfo);

                break;
            }
        }
    }
}

void CDataBase::CheckActConfig()
{
    //首先检查name的唯一性，index后期会因为增删操作导致变化不唯一，在最后导出配置时，统一更新
    int sum = m_Config.actConfig.size();
    for(int i = 0; i < sum; i++ )
    {
        QString name = m_Config.actConfig[i]->sName;
        for(int j = 0 ; j < sum; j++)
        {
            if((i != j)
                &&(name == m_Config.actConfig[j]->sName))
            {
                m_Config.actConfig[i]->sName = "DUPLICATE_ERROR_" + QString::number(i) + "_" + name;
                m_Config.actConfig[j]->sName = "DUPLICATE_ERROR_" + QString::number(j) + "_" + name;
                m_bModified = true;

                QString msg = QString("GS开出表 行%1 与 行%2 都为%3，已自动更名，需确认修改！")
                                    .arg(QString::number(i))
                                    .arg(QString::number(j))
                                    .arg(name);
                CMsgInfo msgInfo( CMsgInfo::Enum_Application_Verify_Mode, CMsgInfo::CN_ERROR_MSG, msg);
                m_MsgInfoList.append(msgInfo);

                break;
            }
        }
    }
}

void CDataBase::CheckRemoteTripConfig()
{
    //首先检查name的唯一性，index后期会因为增删操作导致变化不唯一，在最后导出配置时，统一更新
    int sum = m_Config.remoteTrip.size();
    for(int i = 0; i < sum; i++ )
    {
        QString name = m_Config.remoteTrip[i]->sName;
        for(int j = 0 ; j < sum; j++)
        {
            if((i != j)
                &&(name == m_Config.remoteTrip[j]->sName))
            {
                m_Config.remoteTrip[i]->sName = "DUPLICATE_ERROR_" + QString::number(i) + "_" + name;
                m_Config.remoteTrip[j]->sName = "DUPLICATE_ERROR_" + QString::number(j) + "_" + name;
                m_bModified = true;

                QString msg = QString("GS开出表 行%1 与 行%2 都为%3，已自动更名，需确认修改！")
                                    .arg(QString::number(i))
                                    .arg(QString::number(j))
                                    .arg(name);
                CMsgInfo msgInfo( CMsgInfo::Enum_Application_Verify_Mode, CMsgInfo::CN_ERROR_MSG, msg);
                m_MsgInfoList.append(msgInfo);

                break;
            }
        }
    }
}

unsigned int CDataBase::ChangeStringToInt(QString str)
{
    unsigned int num;
    bool ok = false;
    if(str.startsWith("0x"))
        num = str.toUInt(&ok,16);
    else
        num = str.toUInt(&ok,10);

    if(ok)
        return num;
    else
        return 65535;
}

QString CDataBase::ChangeNumToDEC(unsigned int num)
{
    if(num != 65535)
        return QString::number(num);
    else
        return "0xFFFF";
}

QString CDataBase::ChangeShortToDEC(unsigned int num)
{
    if(num != 255)
        return QString::number(num);
    else
        return "0xFF";
}

QString CDataBase::ChangeNumToHEX(unsigned int num)
{
    if(num != 0)
        return "0x" + QString::number(num,16).toUpper();
    else
        return "0";
}

void CDataBase::ClearConfig()
{
    m_Config.setGroup = 0;
    m_Config.anaConfig.clear();
    m_Config.biConfig.clear();
    m_Config.gsBiConfig.clear();
    m_Config.gsBoConfig.clear();
    m_Config.tripMatrix.clear();
    m_Config.settingConfig.clear();
    m_Config.softYbConfig.clear();
    m_Config.alarmConfig.clear();
    m_Config.actConfig.clear();
    m_Config.remoteTrip.clear();
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

void CDataBase::CheckConfig()
{
    CMsgInfo msgInfoStart( CMsgInfo::Enum_Application_Parse_Mode, CMsgInfo::CN_HINT_MSG, QString("表重名检测开始！"));
    m_MsgInfoList.append(msgInfoStart);

    CheckAnaConfig();
    CheckBIConfig();
    CheckGSBIConfig();
    CheckGSBOConfig();
    CheckTripMatrixConfig();
    CheckSettingConfig();
    CheckAlarmConfig();
    CheckActConfig();
    CheckRemoteTripConfig();

    CMsgInfo msgInfoEnd( CMsgInfo::Enum_Application_Parse_Mode, CMsgInfo::CN_HINT_MSG, QString("表重名检测完成！"));
    m_MsgInfoList.append(msgInfoEnd);
}

void CDataBase::CheckCnn()
{
    CMsgInfo msgInfoStart( CMsgInfo::Enum_Application_Parse_Mode, CMsgInfo::CN_HINT_MSG, QString("关联域检测开始！"));
    m_MsgInfoList.append(msgInfoStart);

    //ana
    for(int i = 0; i < m_Config.anaConfig.size(); i++)
    {
        AnaConfig * p = m_Config.anaConfig[i];

        QString headInfo = QString("模拟量表 行%1：%2").arg(p->wIndex).arg(p->sName);

        p->dwRFVIndex   = GetSetIndex(p->sRFVName, headInfo + "RFVIndex(关联的额定一次值定值号)");
        p->dwSecIn      = GetSetIndex(p->sSecInName, headInfo + "SecIn(关联的TA二次额定电流定值号)");
        p->dwMuSoft     = GetSoftIndex(p->sMuSoftName, headInfo + "MuSoft(关联的MU检修软压板号)");
        p->dwMuHard     = GetBIIndex(p->sMuHardName, headInfo + "SecIn(关联的MU检修硬压板号)");

        if((p->dwRFVIndex == 0xFFFF)&&(p->sRFVName != "NULL"))
            p->sRFVName = "ERROR";

        if((p->dwSecIn == 0xFFFF)&&(p->sSecInName != "NULL"))
            p->sSecInName = "ERROR";

        if((p->dwMuSoft == 0xFFFF)&&(p->sMuSoftName != "NULL"))
            p->sMuSoftName = "ERROR";

        if((p->dwMuHard == 0xFFFF)&&(p->sMuHardName != "NULL"))
            p->sMuHardName = "ERROR";
    }

    //bi
    for(int i = 0; i < m_Config.biConfig.size(); i++)
    {
        BiConfig * p = m_Config.biConfig[i];

        QString headInfo = QString("开关量表 行%1：%2").arg(p->wIndex).arg(p->sName);

        p->wIndexDPS     = GetBIIndex(p->sIndexDPS, headInfo + "IndexDPS(关联的双点状态配对常闭虚端子)");

        if((p->wIndexDPS == 0xFFFF)&&(p->sIndexDPS != "NULL"))
            p->sIndexDPS = "ERROR";
    }

    //gsbi
    for(int i = 0; i < m_Config.gsBiConfig.size(); i++)
    {
        GooseBiConfig * p = m_Config.gsBiConfig[i];

        QString headInfo = QString("GS开入表 行%1：%2").arg(p->wIndex).arg(p->sName);

        if(p->dwAttr < 0x4)
        {
            p->HWChan     = GetBIIndex(p->sHW, headInfo + "HWChan(关联的合位匹配的开关量号)");
            p->FWChan     = GetBIIndex(p->sFW, headInfo + "FWChan(关联的分位匹配的开关量号)");
        }
        else
        {
            p->HWChan     = GetAnaIndex(p->sHW, headInfo + "HWChan(关联的合位匹配的模拟量号)");
            p->FWChan     = GetAnaIndex(p->sFW, headInfo + "FWChan(关联的分位匹配的模拟量号)");
        }

        if((p->HWChan == 0xFFFF)&&(p->sHW != "NULL"))
            p->sHW = "ERROR";

        if((p->FWChan == 0xFFFF)&&(p->sFW != "NULL"))
            p->sFW = "ERROR";
    }

    //gsbo
    for(int i = 0; i < m_Config.gsBoConfig.size(); i++)
    {
        GooseBoConfig * p = m_Config.gsBoConfig[i];

        QString headInfo = QString("GS开出表 行%1：%2").arg(p->wIndex).arg(p->sName);

        if(p->wSrcTab == 1)
        {
            p->HWChan     = GetTripMatrixIndex(p->sHW, headInfo + "HWChan(关联的合位匹配的开关量号)");
            p->FWChan     = GetTripMatrixIndex(p->sFW, headInfo + "FWChan(关联的分位匹配的开关量号)");
        }
        else if(p->wSrcTab == 2)
        {
            p->HWChan     = GetBIIndex(p->sHW, headInfo + "HWChan(关联的合位匹配的开关量号)");
            p->FWChan     = GetBIIndex(p->sFW, headInfo + "FWChan(关联的分位匹配的开关量号)");
        }
        else if(p->wSrcTab == 3)
        {
            p->HWChan     = GetAnaIndex(p->sHW, headInfo + "HWChan(关联的合位匹配的模拟量号)");
            p->FWChan     = GetAnaIndex(p->sFW, headInfo + "FWChan(关联的分位匹配的模拟量号)");
        }
        else
        {
            p->HWChan = 0xFFFF;
            p->FWChan = 0xFFFF;

            QString msgAll = headInfo + QString("SrcTab(来源表) %1 错误 ！")
                                .arg(QString::number(p->wSrcTab));
            CMsgInfo msgInfo( CMsgInfo::Enum_Application_Verify_Mode, CMsgInfo::CN_ERROR_MSG, msgAll);
            m_MsgInfoList.append(msgInfo);
        }

        if((p->HWChan == 0xFFFF)&&(p->sHW != "NULL"))
            p->sHW = "ERROR";

        if((p->FWChan == 0xFFFF)&&(p->sFW != "NULL"))
            p->sFW = "ERROR";
    }

    //setting
    for(int i = 0; i < m_Config.settingConfig.size(); i++)
    {
        SettingConfig * p = m_Config.settingConfig[i];

        QString headInfo = QString("GS开出表 行%1：%2").arg(p->wIndex).arg(p->sName);

        p->wSecIn   = GetSetIndex(p->sSecIn, headInfo + "SecIn(关联的TA二次额定电流定值号)");

        if((p->wSecIn == 0xFFFF)&&(p->sSecIn != "NULL"))
            p->sSecIn = "ERROR";
    }

    //remoteTrip
    for(int i = 0; i < m_Config.remoteTrip.size(); i++)
    {
        RemoteTripConfig * p = m_Config.remoteTrip[i];

        QString headInfo = QString("远跳表 行%1：%2").arg(p->wIndex).arg(p->sName);

        p->dwBiIndex   = GetBIIndex(p->sBI, headInfo + "BIIndex(关联的开关量号)");

        if((p->dwBiIndex == 0xFFFF)&&(p->sBI != "NULL"))
            p->sBI = "ERROR";
    }

    CMsgInfo msgInfoEnd( CMsgInfo::Enum_Application_Parse_Mode, CMsgInfo::CN_HINT_MSG, QString("关联域检测完成！"));
    m_MsgInfoList.append(msgInfoEnd);
}

void CDataBase::RefreshCnn()
{
    CMsgInfo msgInfoStart( CMsgInfo::Enum_Application_Parse_Mode, CMsgInfo::CN_HINT_MSG, QString("关联域更新开始！"));
    m_MsgInfoList.append(msgInfoStart);

    //ana
    for(int i = 0; i < m_Config.anaConfig.size(); i++)
    {
        AnaConfig * p = m_Config.anaConfig[i];

        QString headInfo = QString("模拟量表 行%1：%2").arg(p->wIndex).arg(p->sName);

        p->sRFVName     = RefreshSet(p->dwRFVIndex, headInfo + "RFVIndex(关联的额定一次值定值号)");
        p->sSecInName   = RefreshSet(p->dwSecIn, headInfo + "SecIn(关联的TA二次额定电流定值号)");
        p->sMuSoftName  = RefreshSoftYB(p->dwMuSoft, headInfo + "MuSoft(关联的MU检修软压板号)");
        p->sMuHardName  = RefreshBI(p->dwMuHard, headInfo + "SecIn(关联的MU检修硬压板号)");
    }

    //bi
    for(int i = 0; i < m_Config.biConfig.size(); i++)
    {
        BiConfig * p = m_Config.biConfig[i];

        QString headInfo = QString("开关量表 行%1：%2").arg(p->wIndex).arg(p->sName);

        p->sIndexDPS     = RefreshBI(p->wIndexDPS, headInfo + "IndexDPS(关联的双点状态配对常闭虚端子)");
    }

    //gsbi
    for(int i = 0; i < m_Config.gsBiConfig.size(); i++)
    {
        GooseBiConfig * p = m_Config.gsBiConfig[i];

        QString headInfo = QString("GS开入表 行%1：%2").arg(p->wIndex).arg(p->sName);

        if(p->dwAttr < 0x4)
        {
            p->sHW     = RefreshBI(p->HWChan, headInfo + "HWChan(关联的合位匹配的开关量号)");
            p->sFW     = RefreshBI(p->FWChan, headInfo + "FWChan(关联的分位匹配的开关量号)");
        }
        else
        {
            p->sHW     = RefreshAna(p->HWChan, headInfo + "HWChan(关联的合位匹配的模拟量号)");
            p->sFW     = RefreshAna(p->FWChan, headInfo + "FWChan(关联的分位匹配的模拟量号)");
        }

    }

    //gsbo
    for(int i = 0; i < m_Config.gsBoConfig.size(); i++)
    {
        GooseBoConfig * p = m_Config.gsBoConfig[i];

        QString headInfo = QString("GS开出表 行%1：%2").arg(p->wIndex).arg(p->sName);

        if(p->wSrcTab == 1)
        {
            p->sHW     = RefreshTripMatrix(p->HWChan, headInfo + "HWChan(关联的合位匹配的开关量号)");
            p->sFW     = RefreshTripMatrix(p->FWChan, headInfo + "FWChan(关联的分位匹配的开关量号)");
        }
        else if(p->wSrcTab == 2)
        {
            p->sHW     = RefreshBI(p->HWChan, headInfo + "HWChan(关联的合位匹配的开关量号)");
            p->sFW     = RefreshBI(p->FWChan, headInfo + "FWChan(关联的分位匹配的开关量号)");
        }
        else if(p->wSrcTab == 3)
        {
            p->sHW     = RefreshAna(p->HWChan, headInfo + "HWChan(关联的合位匹配的模拟量号)");
            p->sFW     = RefreshAna(p->FWChan, headInfo + "FWChan(关联的分位匹配的模拟量号)");
        }
        else
        {
            p->sHW = "ERROR";
            p->sFW = "ERROR";

            QString msgAll = headInfo + QString("SrcTab(来源表) %1 错误 ！")
                                .arg(QString::number(p->wSrcTab));
            CMsgInfo msgInfo( CMsgInfo::Enum_Application_Verify_Mode, CMsgInfo::CN_ERROR_MSG, msgAll);
            m_MsgInfoList.append(msgInfo);
        }
    }

    //setting
    for(int i = 0; i < m_Config.settingConfig.size(); i++)
    {
        SettingConfig * p = m_Config.settingConfig[i];

        QString headInfo = QString("GS开出表 行%1：%2").arg(p->wIndex).arg(p->sName);

        p->sSecIn   = RefreshSet(p->wSecIn, headInfo + "SecIn(关联的TA二次额定电流定值号)");
    }

    //remoteTrip
    for(int i = 0; i < m_Config.remoteTrip.size(); i++)
    {
        RemoteTripConfig * p = m_Config.remoteTrip[i];

        QString headInfo = QString("远跳表 行%1：%2").arg(p->wIndex).arg(p->sName);

        p->sBI   = RefreshBI(p->dwBiIndex, headInfo + "BIIndex(关联的开关量号)");
    }

    CMsgInfo msgInfoEnd( CMsgInfo::Enum_Application_Parse_Mode, CMsgInfo::CN_HINT_MSG, QString("关联域更新完成！"));
    m_MsgInfoList.append(msgInfoEnd);
}

QString CDataBase::RefreshSet(int wIndex , QString msg)
{
    if(wIndex == 0xFFFF)
    {
        return "NULL";
    }
    else if((wIndex > m_Config.settingConfig.size())
            ||(wIndex < 0 ))
    {
        m_bModified = true;
        QString msgAll = msg + QString(" = %1 (0~%2)  溢出错误，修改为0xFFFF！")
                .arg(QString::number(wIndex)).arg(QString::number(m_Config.settingConfig.size()));
        CMsgInfo msgInfo( CMsgInfo::Enum_Application_Verify_Mode, CMsgInfo::CN_ERROR_MSG, msgAll);
        m_MsgInfoList.append(msgInfo);
        return "ERROR";
    }
    else
    {
        return m_Config.settingConfig[wIndex]->sName;
    }
}

QString CDataBase::RefreshSoftYB(int wIndex, QString msg)
{
    if(wIndex == 0xFFFF)
    {
        return "NULL";
    }
    else if((wIndex > m_Config.softYbConfig.size())
            ||(wIndex < 0 ))
    {
        m_bModified = true;
        QString msgAll = msg + QString(" = %1 (0~%2)  溢出错误，修改为0xFFFF！")
                .arg(QString::number(wIndex)).arg(QString::number(m_Config.softYbConfig.size()));
        CMsgInfo msgInfo( CMsgInfo::Enum_Application_Verify_Mode, CMsgInfo::CN_ERROR_MSG, msgAll);
        m_MsgInfoList.append(msgInfo);
        return "ERROR";
    }
    else
    {
        return m_Config.softYbConfig[wIndex]->sName;
    }
}

QString CDataBase::RefreshBI(int wIndex, QString msg)
{
    if(wIndex == 0xFFFF)
    {
        return "NULL";
    }
    else if((wIndex > m_Config.biConfig.size())
            ||(wIndex < 0 ))
    {
        m_bModified = true;
        QString msgAll = msg + QString(" = %1 (0~%2)  溢出错误，修改为0xFFFF！")
                .arg(QString::number(wIndex)).arg(QString::number(m_Config.biConfig.size()));
        CMsgInfo msgInfo( CMsgInfo::Enum_Application_Verify_Mode, CMsgInfo::CN_ERROR_MSG, msgAll);
        m_MsgInfoList.append(msgInfo);
        return "ERROR";
    }
    else
    {
        return m_Config.biConfig[wIndex]->sName;
    }
}

QString CDataBase::RefreshTripMatrix(int wIndex, QString msg)
{
    if(wIndex == 0xFFFF)
    {
        return "NULL";
    }
    else if((wIndex > m_Config.tripMatrix.size())
            ||(wIndex < 0 ))
    {
        m_bModified = true;
        QString msgAll = msg + QString(" = %1 (0~%2)  溢出错误，修改为0xFFFF！")
                .arg(QString::number(wIndex)).arg(QString::number(m_Config.tripMatrix.size()));
        CMsgInfo msgInfo( CMsgInfo::Enum_Application_Verify_Mode, CMsgInfo::CN_ERROR_MSG, msgAll);
        m_MsgInfoList.append(msgInfo);
        return "ERROR";
    }
    else
    {
        return m_Config.tripMatrix[wIndex]->sName;
    }
}

QString CDataBase::RefreshAna(int wIndex, QString msg)
{
    if(wIndex == 0xFFFF)
    {
        return "NULL";
    }
    else if((wIndex > m_Config.anaConfig.size())
            ||(wIndex < 0 ))
    {
        m_bModified = true;
        QString msgAll = msg + QString(" = %1 (0~%2)  溢出错误，修改为0xFFFF！")
                .arg(QString::number(wIndex)).arg(QString::number(m_Config.anaConfig.size()));
        CMsgInfo msgInfo( CMsgInfo::Enum_Application_Verify_Mode, CMsgInfo::CN_ERROR_MSG, msgAll);
        m_MsgInfoList.append(msgInfo);
        return "ERROR";
    }
    else
    {
        return m_Config.anaConfig[wIndex]->sName;
    }
}

unsigned int CDataBase::GetSetIndex(QString name , QString msg)
{
    if((name == "NULL")||(name == "ERROR"))
    {
        return 0xFFFF;
    }

    for(int i = 0 ; i  < m_Config.settingConfig.size(); i++)
    {
        if(name == m_Config.settingConfig[i]->sName)
        {
            return i;
        }
    }

    m_bModified = true;
    QString msgAll = msg + QString(" 未在定值表中找到名称为%1的条目，修改为0xFFFF！")
                        .arg(name);
    CMsgInfo msgInfo( CMsgInfo::Enum_Application_Verify_Mode, CMsgInfo::CN_ERROR_MSG, msgAll);
    m_MsgInfoList.append(msgInfo);

    return 0xFFFF;
}

unsigned int CDataBase::GetSoftIndex(QString name , QString msg)
{
    if((name == "NULL")||(name == "ERROR"))
    {
        return 0xFFFF;
    }

    m_bModified = true;
    for(int i = 0 ; i  < m_Config.softYbConfig.size(); i++)
    {
        if(name == m_Config.softYbConfig[i]->sName)
        {
            return i;
        }
    }

    m_bModified = true;
    QString msgAll = msg + QString(" 未在软压板表中找到名称为%1的条目，修改为0xFFFF！")
                        .arg(name);
    CMsgInfo msgInfo( CMsgInfo::Enum_Application_Verify_Mode, CMsgInfo::CN_ERROR_MSG, msgAll);
    m_MsgInfoList.append(msgInfo);

    return 0xFFFF;
}

unsigned int CDataBase::GetBIIndex(QString name , QString msg)
{
    if((name == "NULL")||(name == "ERROR"))
    {
        return 0xFFFF;
    }

    for(int i = 0 ; i  < m_Config.biConfig.size(); i++)
    {
        if(name == m_Config.biConfig[i]->sName)
        {
            return i;
        }
    }

    m_bModified = true;
    QString msgAll = msg + QString(" 未在开关量表中找到名称为%1的条目，修改为0xFFFF！")
                        .arg(name);
    CMsgInfo msgInfo( CMsgInfo::Enum_Application_Verify_Mode, CMsgInfo::CN_ERROR_MSG, msgAll);
    m_MsgInfoList.append(msgInfo);

    return 0xFFFF;
}

unsigned int CDataBase::GetTripMatrixIndex(QString name , QString msg)
{
    if((name == "NULL")||(name == "ERROR"))
    {
        return 0xFFFF;
    }

    for(int i = 0 ; i  < m_Config.tripMatrix.size(); i++)
    {
        if(name == m_Config.tripMatrix[i]->sName)
        {
            return i;
        }
    }

    m_bModified = true;
    QString msgAll = msg + QString(" 未在跳闸矩阵表中找到名称为%1的条目，修改为0xFFFF！")
                        .arg(name);
    CMsgInfo msgInfo( CMsgInfo::Enum_Application_Verify_Mode, CMsgInfo::CN_ERROR_MSG, msgAll);
    m_MsgInfoList.append(msgInfo);

    return 0xFFFF;
}

unsigned int CDataBase::GetAnaIndex(QString name, QString msg)
{
    if((name == "NULL")||(name == "ERROR"))
    {
        return 0xFFFF;
    }

    for(int i = 0 ; i  < m_Config.anaConfig.size(); i++)
    {
        if(name == m_Config.anaConfig[i]->sName)
        {
            return i;
        }
    }

    m_bModified = true;
    QString msgAll = msg + QString(" 未在开关量表中找到名称为%1的条目，修改为0xFFFF！")
                        .arg(name);
    CMsgInfo msgInfo( CMsgInfo::Enum_Application_Verify_Mode, CMsgInfo::CN_ERROR_MSG, msgAll);
    m_MsgInfoList.append(msgInfo);

    return 0xFFFF;
}
