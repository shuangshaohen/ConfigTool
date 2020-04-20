#ifndef GSECONFIG_H
#define GSECONFIG_H

#include <QString>
#include <QVector>
#include <QDateTime>

#define CN_AD_I_COE (153)
#define CN_AD_U_COE (154)
#define CN_SV_I_COE (1000)
#define CN_SV_U_COE (100)
#define CN_CAL_COE  (100)

#define CN_VISABLE_ATTR     (0x1)       //0x01:可见标志
#define CN_SEND_ATTR        (0x2)       //0x02:信号上送标志
#define CN_COMTRAD_ATTR     (0x4)       //0x04:录波标志

enum _Enum_Table_Type
{
    Enum_Table_Type_Ana = 0,
    Enum_Table_Type_Bi,
    Enum_Table_Type_SoftYB,
    Enum_Table_Type_GsBo,
    Enum_Table_Type_TripMatrix,
    Enum_Table_Type_Yk,
    Enum_Table_Type_Set,
    Enum_Table_Type_Evt,
};

enum _Enum_DeviceInfoTable_Column
{
    Enum_DeviceInfoTable_Type_Col = 0,      //装置类型
    Enum_DeviceInfoTable_Desc_Col,          //描述
    Enum_DeviceInfoTable_Version_Col,       //版本
    Enum_DeviceInfoTable_Date_Col,          //配置文件日期
    Enum_DeviceInfoTable_CRC_Col,           //配置文件校验码
};

enum _Enum_DeviceParasTable_Column
{
    Enum_DeviceParasTable_AnaNum_Col = 0,       //
    Enum_DeviceParasTable_BiNum_Col,            //
    Enum_DeviceParasTable_SvNum_Col,            //
    Enum_DeviceParasTable_GsNum_Col,            //
    Enum_DeviceParasTable_BoNum_Col,            //
    Enum_DeviceParasTable_SmpRate_Col,          //
    Enum_DeviceParasTable_ZoneNum_Col,
};

enum _Enum_PubTable_Column
{
//    Enum_PubTable_Index_Col = 0,    //索引
    Enum_PubTable_Desc_Col = 0,     //描述
    Enum_PubTable_Name_Col,         //名字
    Enum_PubTable_Attr_Col,         //通道属性

    Enum_PubTable_End = Enum_PubTable_Attr_Col,
};

enum _Enum_AnaTable_Column
{
    Enum_AnaTable_ChanType_Col = Enum_PubTable_End + 1,     //通道类型,AC交流；DC直流；Other其它。缺省AC
    Enum_AnaTable_ChanFlag_Col,                             //通道标识,U：电压，I：电流，A、B、C：相别，如：UA、UB、UC、UX、UL、3U0、U1、U2； IA、IB、IC、IX、IL、3I0、I1、I2；Other
    Enum_AnaTable_XuYCDft_Col,                              //虚遥测默认值
    Enum_AnaTable_Coe_Col,                                  //通道系数(放大倍数)
    Enum_AnaTable_RateSetP_Col,
    Enum_AnaTable_RateSetS_Col,                             //关联的一次额定值索引号/二次额定值索引号
    Enum_AnaTable_RecSYb_Col,
    Enum_AnaTable_RecHYb_Col,                               //是否接收sv软压板号/硬压板号
    Enum_AnaTable_DataWidth_Col,
    Enum_AnaTable_DataDot_Col,                              //数据属性Width/DotBit
    Enum_AnaTable_Unit_Col,
    Enum_AnaTable_KUnit_Col,                                //一次值单位/二次值单位
};

enum _Enum_BiTable_Column
{
    Enum_BiTable_ChanType_Col = Enum_PubTable_End + 1,     //通道类型,SPS：单点，DPS：双点，INS：整型，FLS：浮点数
    Enum_BiTable_IndexDPS_Col,                             //关联的双点状态配对常闭虚端子
    Enum_BiTable_IndexAna_Col,                             //浮点值关联的模拟量索引
    Enum_BiTable_HoldTime_Col,                             //防抖时间（ms）
    Enum_BiTable_AlmLevel_Col,                             //告警级别,A类、B类、C类，缺省A类
};

enum _Enum_SoftYBTable_Column
{
    Enum_SoftYBTable_ValDft_Col = Enum_PubTable_End + 1,     //缺省默认值
};

enum _Enum_GSBOTable_Column
{
    Enum_GSBOTable_HWChan_Col = Enum_PubTable_End + 1,     //合位通道索引
    Enum_GSBOTable_FWChan_Col,                              //分位通道索引
};

enum _Enum_TripMatrixTable_Column
{
    Enum_TripMatrixTable_Baod_Col = Enum_PubTable_End + 1,     //出口槽位
    Enum_TripMatrixTable_Flag_Col,                              //出口标记
};

enum _Enum_SetTable_Column
{
    Enum_SetTable_Min_Col = Enum_PubTable_End + 1,     //最大值
    Enum_SetTable_Max_Col,                              //最小值
    Enum_SetTable_Dft_Col,                              //缺省值
    Enum_SetTable_SecIn_Col,                            //二次额定值索引
    Enum_SetTable_CoeRet_Col,                            //返回系数
    Enum_SetTable_Type_Col,                            //定值类型
    Enum_SetTable_Width_Col,                            //位宽
    Enum_SetTable_Dotbit_Col,                            //精度
    Enum_SetTable_Unit_Col,                            //二次单位
    Enum_SetTable_KUnit_Col,                            //一次单位
};

enum _Enum_EvtTable_Column
{
    Enum_EvtTable_GSIN_Col = Enum_PubTable_End + 1,     //GOOSE开入
    Enum_EvtTable_BO1_Col,                              //出口1
    Enum_EvtTable_BO2_Col,                              //出口2
    Enum_EvtTable_SIGNAL_Col,                           //信号
    Enum_EvtTable_Light_Col,                            //灯
};

class DeviceInfo
{
public:
    QString                 sType;
    QString                 sDesc;
    QString                 sVersion;
    QString                 sDate;
    QString                 sCRC;

    DeviceInfo()
    {
        sType = "GSE8500";
        sDesc = "区域保护控制系统";
        sVersion = "V1.00";
        sDate = QDateTime::currentDateTime().toString();
        sCRC = "1234ABCD";
    }

    void clear()
    {
        sType = "";
        sDesc = "";
        sVersion = "";
        sDate = "";
        sCRC = "";
    }
};

class DeviceParas
{
public:
    unsigned int            wMaxAnaNum;
    unsigned int            wMaxBiNum;
    unsigned int            wMaxSvNum;
    unsigned int            wMaxGooseNum;
    unsigned int            wMaxBoNum;
    unsigned int            wSmpRate;
    unsigned int            wZoneNum;

    DeviceParas()
    {
        wMaxAnaNum = 0;
        wMaxBiNum = 0;
        wMaxSvNum = 0;
        wMaxGooseNum = 0;
        wMaxBoNum = 0;
        wSmpRate = 4000;
        wZoneNum= 20;
    }

    void clear()
    {
        wMaxAnaNum = 0;
        wMaxBiNum = 0;
        wMaxSvNum = 0;
        wMaxGooseNum = 0;
        wMaxBoNum = 0;
        wSmpRate = 0;
        wZoneNum= 0;
    }
};

class BaseItem
{
public:
    unsigned int            wIndex;
    QString                 sDesc;
    QString                 sName;
    unsigned int            dwAttr;

    BaseItem(unsigned int index)
    {
        wIndex = index;
        sDesc = QString("缺省%1").arg(index);
        sName = QString("item%1").arg(index);
        dwAttr = 0;
    }

};

class BaseTab
{
public:
    QString                 sDesc;
    QString                 sKey;
    QVector<BaseItem *>     items;

    void clear()
    {
        sDesc = "";
        sKey = "";

        for(int i = 0 ; i < items.size(); i++)
        {
            delete items[i];
        }

        items.clear();
    }

    void insertItem(int row , BaseItem * item)
    {
        items.insert(row,item);
        for (int i = row + 1; i < items.size(); i++)
        {
            items[i]->wIndex += 1;
        }
    }

    void deletItem(int row)
    {
        for (int i = row + 1; i < items.size(); i++)
        {
            items[i]->wIndex -= 1;
        }

        items.remove(row);
    }

    void swapItem(int row1, int row2)
    {
        if((row1 < items.size())
                &&(row2 < items.size()))
        {
            int tempIndex = items[row1]->wIndex;
            items[row1]->wIndex = items[row2]->wIndex;
            items[row2]->wIndex = tempIndex;
            items.swapItemsAt(row1,row2);
        }
    }

    int getNameCounts(QString name)
    {
        int num = 0;
        for (int i = 0; i < items.size(); i++)
        {
            if(items[i]->sName == name)
                num++;
        }
        return num;
    }

    bool matchKeyAndRowExit(QString info)
    {
        if((info == "")
                ||(info == "-1"))
            return true;

        QStringList list = info.split(":");
        if(list.size() != 2)
            return false;

        if(list[0] != sKey)
            return false;

        bool ok;
        int row = list[1].toUInt(&ok);
        if(ok == false)
            return false;

        if(row >= items.size() )
            return false;

        return true;
    }
};

class AnaItem:public BaseItem
{
public:
    QString                 sChanType;
    QString                 sChanFlag;
    int                     iXuYCDft;
    unsigned int            wCoe;
    QString                 sRateSetP;
    QString                 sRateSetS;
    QString                 sRecSYb;
    QString                 sRecHYb;
    unsigned int            byWidth;
    unsigned int            byDotBit;
    QString                 sUnit;
    QString                 sKiloUnit;

    AnaItem(unsigned int index):BaseItem(index)
    {
        sChanType = QString("Other");
        sChanFlag = QString("Other");
        iXuYCDft = -1;
        wCoe = 1;
        sRateSetP = "-1";
        sRateSetS = "-1";
        sRecSYb = "-1";
        sRecHYb = "-1";
        byWidth = 7;
        byDotBit = 3;
        sUnit = "N";
        sKiloUnit = "N";
    }
};

class BiItem:public BaseItem
{
public:
    QString                 sChanType;
    QString                 sIndexDPS;
    QString                 sIndexAna;
    unsigned int            wHoldTime;
    QString                 sAlmLevel;

    BiItem(unsigned int index):BaseItem(index)
    {
        sChanType = QString("SPS");
        sIndexDPS = "-1";
        sIndexAna = "-1";
        wHoldTime = 40;
        sAlmLevel = "A";
    }
};

class SoftYBItem:public BaseItem
{
public:
    unsigned int            wDftVal;

    SoftYBItem(unsigned int index):BaseItem(index)
    {
        wDftVal = 0;
    }
};

class GooseBOItem:public BaseItem
{
public:
    QString                 sHWChan;
    QString                 sFWChan;

    GooseBOItem(unsigned int index):BaseItem(index)
    {
        sHWChan = "-1";
        sFWChan = "-1";
    }
};

class TripMatrixItem:public BaseItem
{
public:
    unsigned int            wBoardIndex;
    unsigned int            wBOFlag;

    TripMatrixItem(unsigned int index):BaseItem(index)
    {
        wBoardIndex = 0;
        wBOFlag = 0;
    }
};

class SettingItem:public BaseItem
{
public:
    unsigned int            wValMin;
    unsigned int            wValMax;
    unsigned int            wValDft;

    QString                 sSecIn;
    unsigned int            wCoeRet;
    QString                 sType;
    unsigned int            byWidth;
    unsigned int            byDotBit;
    QString                 sUnit;
    QString                 sKiloUnit;

    SettingItem(unsigned int index):BaseItem(index)
    {
        wValMin = 0;
        wValMax = 0;
        wValDft = 0;

        sSecIn = "-1";
        wCoeRet = 100;
        sType = "NULL";
        byWidth = 7;
        byDotBit = 3;
        sUnit = "N";
        sKiloUnit = "N";
    }
};

class EvtItem:public BaseItem
{
public:
    unsigned int            boConfig1;
    unsigned int            boConfig2;
    unsigned int            boSignal;
    unsigned int            boLight;

    QString                 sCnnGSIn;

    EvtItem(unsigned int index):BaseItem(index)
    {
        boConfig1 = 0;
        boConfig2 = 0;
        boSignal = 0;
        boLight = 0;

        sCnnGSIn = "-1";
    }
};

struct GseConfig
{
    DeviceInfo              deviceInfo;
    DeviceParas             deviceParas;

    //模拟量表
    BaseTab                 adAnaConfig;
    BaseTab                 derivedConfig;
    BaseTab                 svConfig;
    BaseTab                 gsAnaConfig;
    BaseTab                 otherAnaConfig;

    //开关量表
    BaseTab                 generalBiConfig;
    BaseTab                 signalConfig;
    BaseTab                 gooseBiConfig;
    BaseTab                 softYBConfig;

    //开出表
    BaseTab                 gooseBoConfig;
    BaseTab                 tripMaxtrixConfig;
    //YK表后期添加
    BaseTab                 ykConfig;

    //定值表
    BaseTab                 settingSPConifg;
    BaseTab                 settingSGConifg;

    //事件表
    BaseTab                 evtAlmConfig;
    BaseTab                 evtActConfig;
    BaseTab                 evtCheckConfig;
    BaseTab                 remoteTripConfig;

    void clear()
    {
        deviceInfo.clear();
        deviceParas.clear();

        adAnaConfig.clear();
        derivedConfig.clear();
        svConfig.clear();
        gsAnaConfig.clear();
        otherAnaConfig.clear();

        generalBiConfig.clear();
        signalConfig.clear();
        gooseBiConfig.clear();
        softYBConfig.clear();

        gooseBoConfig.clear();
        tripMaxtrixConfig.clear();

        ykConfig.clear();


        settingSPConifg.clear();
        settingSGConifg.clear();


        evtAlmConfig.clear();
        evtActConfig.clear();
        evtCheckConfig.clear();
        remoteTripConfig.clear();
    }
};


#endif // GSECONFIG_H
