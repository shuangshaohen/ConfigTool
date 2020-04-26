#include "CDataSQL.h"
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QVariantList>
#include <QSqlRecord>
#include <QSqlTableModel>
#include <QMessageBox>
#include "CDataBase.h"
#include <QDebug>
#include "CMsgInfo.h"

CDataSQL::CDataSQL(QObject *parent) : QObject(parent)
{
    m_data = nullptr;
}

CDataSQL::~CDataSQL()
{

}

void CDataSQL::save(QString dbName, CDataBase *data)
{
    m_data = data;
    if(m_data == nullptr)
        return;

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbName);

    if (!db.open()) {
        QMessageBox::critical(nullptr, QObject::tr("Cannot open database"),
            QObject::tr("Unable to establish a database connection.\n"
                        "This example needs SQLite support. Please read "
                        "the Qt SQL driver documentation for information how "
                        "to build it.\n\n"
                        "Click Cancel to exit."), QMessageBox::Cancel);
        return;
    }

    for(int i = 0 ; i < db.tables().size(); i++)
    {
        qDebug() << db.tables().at(i);
    }

    //首先清除所有老数据
    deletDB();
    //创建索引表
    creatMasterIndex();

    createDeviceInfo();
    createDeviceParas();

    createAnaConfig("AnaConfig");
    createAnaConfig("SvConfig");
    createAnaConfig("GsAnaConfig");
    createAnaConfig("OtherConfig");
    createAnaConfig("DerivedConfig");

    createBiConfig("GeneralBiConfig");
    createBiConfig("GooseBiConfig");
    createBiConfig("SignalConfig");
    creatSoftYB();

    createGooseBoConfig();
    createTripMatrixConfig();

    createSetConfig("SPSetConfig");
    createSetConfig("SGSetConfig");

    createEvtConfig("EvtAlmConfig");
    createEvtConfig("EvtActConfig");
    createEvtConfig("EvtCheckConfig");
    createEvtConfig("RemoteConfig");

    db.close();
}

void CDataSQL::deletDB()
{
    QSqlDatabase db = QSqlDatabase::database(); //建立数据库连接
    QSqlQuery query(db);

    QStringList tables = db.tables();
    for(int i = 0 ; i < tables.size(); i++)
    {
        QString deleteStr = "DROP TABLE " + tables.at(i);
        query.exec(deleteStr);
    }
}

void CDataSQL::createDeviceInfo()
{
    QSqlDatabase db = QSqlDatabase::database(); //建立数据库连接
    QSqlQuery query(db);
    query.exec("CREATE TABLE DeviceInfo ("
                                         "ID  INTEGER PRIMARY KEY AUTOINCREMENT,"
                                         "Type  TEXT,"
                                         "Desc  TEXT,"
                                         "Version  TEXT,"
                                         "Date  TEXT,"
                                         "CRC TEXT");

    query.prepare("INSERT INTO DeviceInfo (Type, Desc, Version, Date, CRC) "
                      "VALUES (:Type, :Desc, :Version, :Date, :CRC)");

    if(m_data == nullptr)
        return;

    DeviceInfo * info = &m_data->GetConfig()->deviceInfo;

    query.bindValue(":Type", info->sType);
    query.bindValue(":Desc", info->sDesc);
    query.bindValue(":Version", info->sVersion);
    query.bindValue(":Date", info->sDate);
    query.bindValue(":CRC", info->sCRC);
    query.exec();
}

void CDataSQL::createDeviceParas()
{
    QSqlDatabase db = QSqlDatabase::database(); //建立数据库连接
    QSqlQuery query(db);
    query.exec("CREATE TABLE DeviceParas ("
                                         "ID  INTEGER PRIMARY KEY AUTOINCREMENT,"
                                         "MaxAnaNum  INTEGER,"
                                         "MaxBiNum  INTEGER,"
                                         "MaxSvNum  INTEGER,"
                                         "MaxGooseNum  INTEGER,"
                                         "MaxBoNum INTEGER,"
                                         "SmpRate INTEGER,"
                                         "ZoneNum INTEGER");

    query.prepare("INSERT INTO DeviceInfo (MaxAnaNum, MaxBiNum, MaxSvNum, MaxGooseNum, MaxBoNum, SmpRate, ZoneNum) "
                      "VALUES (:MaxAnaNum, :MaxBiNum, :MaxSvNum, :MaxGooseNum, :MaxBoNum, :SmpRate, :ZoneNum)");

    if(m_data == nullptr)
        return;

    DeviceParas * para = &m_data->GetConfig()->deviceParas;

    query.bindValue(":MaxAnaNum", para->wMaxAnaNum);
    query.bindValue(":MaxBiNum", para->wMaxBiNum);
    query.bindValue(":MaxSvNum", para->wMaxSvNum);
    query.bindValue(":MaxGooseNum", para->wMaxGooseNum);
    query.bindValue(":MaxBoNum", para->wMaxBoNum);
    query.bindValue(":SmpRate", para->wSmpRate);
    query.bindValue(":ZoneNum", para->wZoneNum);
    query.exec();
}

void CDataSQL::creatMasterIndex()
{
    QSqlDatabase db = QSqlDatabase::database(); //建立数据库连接
    QSqlQuery query(db);
    query.exec("CREATE TABLE MasterIndex ("
                                         "ID  INTEGER PRIMARY KEY AUTOINCREMENT,"
                                         "Name  TEXT,"
                                         "Desc  TEXT,"
                                         "Sublist  TEXT,"
                                         "SublistID  INTEGER)");
}

void CDataSQL::createAnaConfig(QString tableType)
{
    if(m_data == nullptr)
        return;

    BaseTab * baseTable = nullptr;
    if(tableType == "AnaConfig")
        baseTable = &m_data->GetConfig()->adAnaConfig;
    else if(tableType == "SvConfig")
            baseTable = &m_data->GetConfig()->svConfig;
    else if(tableType == "GsAnaConfig")
            baseTable = &m_data->GetConfig()->gsAnaConfig;
    else if(tableType == "OtherConfig")
            baseTable = &m_data->GetConfig()->otherAnaConfig;
    else if(tableType == "DerivedConfig")
            baseTable = &m_data->GetConfig()->derivedConfig;
    else
        return;

    QSqlDatabase db = QSqlDatabase::database(); //建立数据库连接
    QSqlQuery query(db);

    query.exec(QString("CREATE TABLE %1 ("
                                       "ID INTEGER PRIMARY KEY, "
                                       "Desc  TEXT,"
                                       "Name  TEXT UNIQUE,"
                                       "ChanAttr  INTEGER,"
                                       "ChanType  TEXT,"
                                       "ChanFlag  TEXT,"
                                       "VirtualDft  INTEGER,"
                                       "Coe  INTEGER,"
                                       "PrimarySetIndex  TEXT,"
                                       "SecondarySetIndex  TEXT,"
                                       "RecSoftYbIndex  TEXT,"
                                       "RecHardYbIndex  TEXT,"
                                       "DataAttrWidth  INTEGER,"
                                       "DataAttrDotBit  INTEGER,"
                                       "Unit  TEXT,"
                                       "KiloUnit  TEXT,"
                                       "Amplitude  REAL,"
                                       "phase  REAL)").arg(tableType));

    query.prepare(QString("INSERT INTO %1 (ID, Desc, Name, ChanAttr, ChanType, ChanFlag,"
                  " VirtualDft, Coe, PrimarySetIndex, SecondarySetIndex, RecSoftYbIndex, "
                  "RecHardYbIndex, DataAttrWidth, DataAttrDotBit, Unit, KiloUnit, Amplitude, phase) "
                      "VALUES (:ID, :Desc, :Name, :ChanAttr, :ChanType, :ChanFlag,"
                  " :VirtualDft, :Coe, :PrimarySetIndex, :SecondarySetIndex, :RecSoftYbIndex,"
                  " :RecHardYbIndex, :DataAttrWidth, :DataAttrDotBit, :Unit, :KiloUnit, :Amplitude, :phase)").arg(tableType));

    for(int i = 0; i < baseTable->items.size(); i++)
    {
        AnaItem * base = (AnaItem *)baseTable->items[i];
        query.bindValue(":ID", base->wIndex);
        query.bindValue(":Desc", base->sDesc);
        query.bindValue(":Name", base->sName);
        query.bindValue(":ChanAttr", base->dwAttr);
        query.bindValue(":ChanType", base->sChanType);
        query.bindValue(":ChanFlag", base->sChanFlag);
        query.bindValue(":VirtualDft", base->iXuYCDft);
        query.bindValue(":Coe", base->wCoe);
        query.bindValue(":PrimarySetIndex", base->sRateSetP);
        query.bindValue(":SecondarySetIndex", base->sRateSetS);
        query.bindValue(":RecSoftYbIndex", base->sRecSYb);
        query.bindValue(":RecHardYbIndex", base->sRecHYb);
        query.bindValue(":DataAttrWidth", base->byWidth);
        query.bindValue(":DataAttrDotBit", base->byDotBit);
        query.bindValue(":Unit", base->sUnit);
        query.bindValue(":KiloUnit", base->sKiloUnit);
        query.bindValue(":Amplitude", 0);
        query.bindValue(":phase", 0);
        query.exec();
    }

    query.prepare("INSERT INTO MasterIndex (Name, Desc, Sublist, SublistID) "
                      "VALUES (:Name, :Desc, :Sublist, :SublistID)");

    for(int i = 0; i < baseTable->items.size(); i++)
    {
        BaseItem * base = (BaseItem *)baseTable->items[i];
        query.bindValue(":Name", base->sName);
        query.bindValue(":Desc", base->sDesc);
        query.bindValue(":Sublist", tableType);
        query.bindValue(":SublistID", base->wIndex);
        query.exec();
    }
}

void CDataSQL::createBiConfig(QString tableType)
{
    if(m_data == nullptr)
        return;

    BaseTab * baseTable = nullptr;
    if(tableType == "GeneralBiConfig")
        baseTable = &m_data->GetConfig()->generalBiConfig;
    else if(tableType == "GooseBiConfig")
            baseTable = &m_data->GetConfig()->gooseBiConfig;
    else if(tableType == "SignalConfig")
            baseTable = &m_data->GetConfig()->signalConfig;
    else
        return;

    QSqlDatabase db = QSqlDatabase::database(); //建立数据库连接
    QSqlQuery query(db);

    query.exec(QString("CREATE TABLE %1 ("
                                       "ID INTEGER PRIMARY KEY, "
                                       "Desc  TEXT,"
                                       "Name  TEXT UNIQUE,"
                                       "ChanAttr  INTEGER,"
                                       "ChanType  TEXT,"
                                       "IndexDPS  TEXT,"
                                       "IndexAna  TEXT,"
                                       "HoldTime  INTEGER,"
                                       "AlmLevel  TEXT,"
                                       "Value  INTEGER)").arg(tableType));

    query.prepare(QString("INSERT INTO %1 (ID, Desc, Name, ChanAttr, ChanType, IndexDPS,"
                  " IndexAna, HoldTime, AlmLevel, Value) "
                      "VALUES (:ID, :Desc, :Name, :ChanAttr, :ChanType, :IndexDPS,"
                  " :IndexAna, :HoldTime, :AlmLevel, :Value)").arg(tableType));

    for(int i = 0; i < baseTable->items.size(); i++)
    {
        BiItem * base = (BiItem *)baseTable->items[i];
        query.bindValue(":ID", base->wIndex);
        query.bindValue(":Desc", base->sDesc);
        query.bindValue(":Name", base->sName);
        query.bindValue(":ChanAttr", base->dwAttr);
        query.bindValue(":ChanType", base->sChanType);
        query.bindValue(":IndexDPS", base->sIndexDPS);
        query.bindValue(":IndexAna", base->sIndexAna);
        query.bindValue(":HoldTime", base->wHoldTime);
        query.bindValue(":AlmLevel", base->sAlmLevel);
        query.bindValue(":Value", 0);
        query.exec();
    }

    query.prepare("INSERT INTO MasterIndex (Name, Desc, Sublist, SublistID) "
                      "VALUES (:Name, :Desc, :Sublist, :SublistID)");

    for(int i = 0; i < baseTable->items.size(); i++)
    {
        BaseItem * base = (BaseItem *)baseTable->items[i];
        query.bindValue(":Name", base->sName);
        query.bindValue(":Desc", base->sDesc);
        query.bindValue(":Sublist", tableType);
        query.bindValue(":SublistID", base->wIndex);
        query.exec();
    }
}

void CDataSQL::creatSoftYB()
{
    if(m_data == nullptr)
        return;

    BaseTab * baseTable = &m_data->GetConfig()->softYBConfig;
    QString tableType = "SoftYBConfig";

    QSqlDatabase db = QSqlDatabase::database(); //建立数据库连接
    QSqlQuery query(db);

    query.exec(QString("CREATE TABLE %1 ("
                                       "ID INTEGER PRIMARY KEY, "
                                       "Desc  TEXT,"
                                       "Name  TEXT UNIQUE,"
                                       "ChanAttr  INTEGER,"
                                       "DftVal  INTEGER,"
                                       "Value  INTEGER)").arg(tableType));

    query.prepare(QString("INSERT INTO %1 (ID, Desc, Name, ChanAttr, DftVal, Value) "
                      "VALUES (:ID, :Desc, :Name, :ChanAttr, :DftVal, :Value)").arg(tableType));

    for(int i = 0; i < baseTable->items.size(); i++)
    {
        SoftYBItem * base = (SoftYBItem *)baseTable->items[i];
        query.bindValue(":ID", base->wIndex);
        query.bindValue(":Desc", base->sDesc);
        query.bindValue(":Name", base->sName);
        query.bindValue(":ChanAttr", base->dwAttr);
        query.bindValue(":DftVal", base->wDftVal);
        query.bindValue(":Value", 0);
        query.exec();
    }

    query.prepare("INSERT INTO MasterIndex (Name, Desc, Sublist, SublistID) "
                      "VALUES (:Name, :Desc, :Sublist, :SublistID)");

    for(int i = 0; i < baseTable->items.size(); i++)
    {
        BaseItem * base = (BaseItem *)baseTable->items[i];
        query.bindValue(":Name", base->sName);
        query.bindValue(":Desc", base->sDesc);
        query.bindValue(":Sublist", tableType);
        query.bindValue(":SublistID", base->wIndex);
        query.exec();
    }
}

void CDataSQL::createGooseBoConfig()
{
    if(m_data == nullptr)
        return;

    BaseTab * baseTable = &m_data->GetConfig()->gooseBoConfig;
    QString tableType = "GooseBoConfig";

    QSqlDatabase db = QSqlDatabase::database(); //建立数据库连接
    QSqlQuery query(db);

    query.exec(QString("CREATE TABLE %1 ("
                                       "ID INTEGER PRIMARY KEY, "
                                       "Desc  TEXT,"
                                       "Name  TEXT UNIQUE,"
                                       "ChanAttr  INTEGER,"
                                       "HWChan  TEXT,"
                                       "FWChan  TEXT)").arg(tableType));

    query.prepare(QString("INSERT INTO %1 (ID, Desc, Name, ChanAttr, HWChan, FWChan) "
                      "VALUES (:ID, :Desc, :Name, :ChanAttr, :HWChan, :FWChan)").arg(tableType));

    for(int i = 0; i < baseTable->items.size(); i++)
    {
        GooseBOItem * base = (GooseBOItem *)baseTable->items[i];
        query.bindValue(":ID", base->wIndex);
        query.bindValue(":Desc", base->sDesc);
        query.bindValue(":Name", base->sName);
        query.bindValue(":ChanAttr", base->dwAttr);
        query.bindValue(":HWChan", base->sHWChan);
        query.bindValue(":FWChan", base->sFWChan);
        query.exec();
    }

    query.prepare("INSERT INTO MasterIndex (Name, Desc, Sublist, SublistID) "
                      "VALUES (:Name, :Desc, :Sublist, :SublistID)");

    for(int i = 0; i < baseTable->items.size(); i++)
    {
        BaseItem * base = (BaseItem *)baseTable->items[i];
        query.bindValue(":Name", base->sName);
        query.bindValue(":Desc", base->sDesc);
        query.bindValue(":Sublist", tableType);
        query.bindValue(":SublistID", base->wIndex);
        query.exec();
    }
}

void CDataSQL::createTripMatrixConfig()
{
    if(m_data == nullptr)
        return;

    BaseTab * baseTable = &m_data->GetConfig()->tripMaxtrixConfig;
    QString tableType = "TripMatrixConfig";

    QSqlDatabase db = QSqlDatabase::database(); //建立数据库连接
    QSqlQuery query(db);

    query.exec(QString("CREATE TABLE %1 ("
                                       "ID INTEGER PRIMARY KEY, "
                                       "Desc  TEXT,"
                                       "Name  TEXT UNIQUE,"
                                       "ChanAttr  INTEGER,"
                                       "BoardIndex  INTEGER,"
                                       "BOFlag  INTEGER)").arg(tableType));

    query.prepare(QString("INSERT INTO %1 (ID, Desc, Name, ChanAttr, wBoardIndex, BOFlag) "
                      "VALUES (:ID, :Desc, :Name, :ChanAttr, :BoardIndex, :BOFlag)").arg(tableType));

    for(int i = 0; i < baseTable->items.size(); i++)
    {
        TripMatrixItem * base = (TripMatrixItem *)baseTable->items[i];
        query.bindValue(":ID", base->wIndex);
        query.bindValue(":Desc", base->sDesc);
        query.bindValue(":Name", base->sName);
        query.bindValue(":ChanAttr", base->dwAttr);
        query.bindValue(":BoardIndex", base->wBoardIndex);
        query.bindValue(":BOFlag", base->wBOFlag);
        query.exec();
    }

    query.prepare("INSERT INTO MasterIndex (Name, Desc, Sublist, SublistID) "
                      "VALUES (:Name, :Desc, :Sublist, :SublistID)");

    for(int i = 0; i < baseTable->items.size(); i++)
    {
        BaseItem * base = (BaseItem *)baseTable->items[i];
        query.bindValue(":Name", base->sName);
        query.bindValue(":Desc", base->sDesc);
        query.bindValue(":Sublist", tableType);
        query.bindValue(":SublistID", base->wIndex);
        query.exec();
    }
}

void CDataSQL::createSetConfig(QString tableType)
{
    if(m_data == nullptr)
        return;

    BaseTab * baseTable = nullptr;
    if(tableType == "SPSetConfig")
        baseTable = &m_data->GetConfig()->settingSPConifg;
    else if(tableType == "SGSetConfig")
            baseTable = &m_data->GetConfig()->settingSGConifg;
    else
        return;

    QSqlDatabase db = QSqlDatabase::database(); //建立数据库连接
    QSqlQuery query(db);

    query.exec(QString("CREATE TABLE %1 ("
                                       "ID INTEGER PRIMARY KEY, "
                                       "Desc  TEXT,"
                                       "Name  TEXT UNIQUE,"
                                       "ChanAttr  INTEGER,"
                                       "ValMin  INTEGER,"
                                       "ValMax  INTEGER,"
                                       "ValDft  INTEGER,"
                                       "ValStep  INTEGER,"
                                       "SecIn  TEXT,"
                                       "CoeRet  INTEGER,"
                                       "Type  TEXT,"
                                       "Width INTEGER,"
                                       "DotBit INTEGER,"
                                       "Unit  TEXT,"
                                       "KiloUnit  TEXT,"
                                       "ValueInt  INTEGER,"
                                       "ValueFloat REAL)").arg(tableType));

    query.prepare(QString("INSERT INTO %1 (ID, Desc, Name, ChanAttr, ValMin, ValMax,"
                  " ValDft, ValStep, SecIn, CoeRet, Type, Width, DotBit, Unit, KiloUnit, ValueInt, ValueFloat) "
                      "VALUES (:ID, :Desc, :Name, :ChanAttr, :ValMin, :ValMax,"
                          " :ValDft, :ValStep, :SecIn, :CoeRet, :Type, :Width, :DotBit,"
                          " :Unit, :KiloUnit, :ValueInt, :ValueFloat) ").arg(tableType));

    for(int i = 0; i < baseTable->items.size(); i++)
    {
        SettingItem * base = (SettingItem *)baseTable->items[i];

        float fVal = (float)base->wValDft;
        for(unsigned int i = 0 ; i < base->byDotBit ; i++)
        {
            fVal = fVal/10;
        }

        query.bindValue(":ID", base->wIndex);
        query.bindValue(":Desc", base->sDesc);
        query.bindValue(":Name", base->sName);
        query.bindValue(":ChanAttr", base->dwAttr);
        query.bindValue(":ValMin", base->wValMin);
        query.bindValue(":ValMax", base->wValMax);
        query.bindValue(":ValDft", base->wValDft);
        query.bindValue(":ValStep", base->wValStep);
        query.bindValue(":SecIn", base->sSecIn);
        query.bindValue(":CoeRet", base->wCoeRet);
        query.bindValue(":Type", base->sType);
        query.bindValue(":Width", base->byWidth);
        query.bindValue(":DotBit", base->byDotBit);
        query.bindValue(":Unit", base->sUnit);
        query.bindValue(":KiloUnit", base->sKiloUnit);
        query.bindValue(":ValueInt", base->wValDft);
        query.bindValue(":ValueFloat", fVal);
        query.exec();
    }

    query.prepare("INSERT INTO MasterIndex (Name, Desc, Sublist, SublistID) "
                      "VALUES (:Name, :Desc, :Sublist, :SublistID)");

    for(int i = 0; i < baseTable->items.size(); i++)
    {
        BaseItem * base = (BaseItem *)baseTable->items[i];
        query.bindValue(":Name", base->sName);
        query.bindValue(":Desc", base->sDesc);
        query.bindValue(":Sublist", tableType);
        query.bindValue(":SublistID", base->wIndex);
        query.exec();
    }
}

void CDataSQL::createEvtConfig(QString tableType)
{
    if(m_data == nullptr)
        return;

    BaseTab * baseTable = nullptr;
    if(tableType == "EvtAlmConfig")
        baseTable = &m_data->GetConfig()->evtAlmConfig;
    else if(tableType == "EvtActConfig")
            baseTable = &m_data->GetConfig()->evtActConfig;
    else if(tableType == "EvtCheckConfig")
            baseTable = &m_data->GetConfig()->evtCheckConfig;
    else if(tableType == "RemoteConfig")
            baseTable = &m_data->GetConfig()->remoteTripConfig;
    else
        return;

    QSqlDatabase db = QSqlDatabase::database(); //建立数据库连接
    QSqlQuery query(db);

    query.exec(QString("CREATE TABLE %1 ("
                                       "ID INTEGER PRIMARY KEY, "
                                       "Desc  TEXT,"
                                       "Name  TEXT UNIQUE,"
                                       "ChanAttr  INTEGER,"
                                       "boConfig1  INTEGER,"
                                       "boConfig2  INTEGER,"
                                       "boSignal  INTEGER,"
                                       "boLight  INTEGER,"
                                       "CnnGSIn  TEXT,"
                                       "Value  INTEGER)").arg(tableType));

    query.prepare(QString("INSERT INTO %1 (ID, Desc, Name, ChanAttr, boConfig1, boConfig2,"
                  " boSignal, boLight, CnnGSIn, Value) "
                      "VALUES (:ID, :Desc, :Name, :ChanAttr, :boConfig1, :boConfig2,"
                          " :boSignal, :boLight, :CnnGSIn, :Value) ").arg(tableType));

    for(int i = 0; i < baseTable->items.size(); i++)
    {
        EvtItem * base = (EvtItem *)baseTable->items[i];

        query.bindValue(":ID", base->wIndex);
        query.bindValue(":Desc", base->sDesc);
        query.bindValue(":Name", base->sName);
        query.bindValue(":ChanAttr", base->dwAttr);
        query.bindValue(":boConfig1", base->boConfig1);
        query.bindValue(":boConfig2", base->boConfig2);
        query.bindValue(":boSignal", base->boSignal);
        query.bindValue(":boLight", base->boLight);
        query.bindValue(":CnnGSIn", base->sCnnGSIn);
        query.bindValue(":Value", 0);
        query.exec();
    }

    query.prepare("INSERT INTO MasterIndex (Name, Desc, Sublist, SublistID) "
                      "VALUES (:Name, :Desc, :Sublist, :SublistID)");

    for(int i = 0; i < baseTable->items.size(); i++)
    {
        BaseItem * base = (BaseItem *)baseTable->items[i];
        query.bindValue(":Name", base->sName);
        query.bindValue(":Desc", base->sDesc);
        query.bindValue(":Sublist", tableType);
        query.bindValue(":SublistID", base->wIndex);
        query.exec();
    }
}

