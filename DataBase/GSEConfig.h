#ifndef GSECONFIG_H
#define GSECONFIG_H

#include <QString>
#include <QVector>

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

class DeviceInfo
{
public:
    QString                 sType;
    QString                 sDesc;
    QString                 sVersion;
    QString                 sDate;
    QString                 sCRC;

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

    void clear()
    {
        wMaxAnaNum = 0;
        wMaxBiNum = 0;
        wMaxSvNum = 0;
        wMaxGooseNum = 0;
        wMaxBoNum = 0;
        wSmpRate = 0;
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

//struct AnaConfig
//{
//    QString                 sDesc;
//    QString                 sKey;
//    QVector<AnaItem *>      items;

//    void clear()
//    {
//        sDesc = "";
//        sKey = "";

//        for(int i = 0 ; i < items.size(); i++)
//        {
//            delete items[i];
//        }

//        items.clear();
//    }

//    void insertItem(int row , AnaItem * item)
//    {
//        items.insert(row,item);
//        for (int i = row + 1; i < items.size(); i++)
//        {
//            items[i]->wIndex += 1;
//        }
//    }
//};

struct BiItem
{
    unsigned int            wIndex;
    QString                 sDesc;
    QString                 sName;
    unsigned int            dwAttr;

    QString                 sChanType;
    QString                 sIndexDPS;
    QString                 sIndexAna;
    unsigned int            wHoldTime;
    QString                 sAlmLevel;

    BiItem(unsigned int index)
    {
        wIndex = index;
        sDesc = QString("缺省%1").arg(index);
        sName = QString("item%1").arg(index);
        dwAttr = 0;

        sChanType = QString("SPS");
        sIndexDPS = "-1";
        sIndexAna = "-1";
        wHoldTime = 40;
        sAlmLevel = "A";
    }
};

struct BiConfig
{
    QString                 sDesc;
    QString                 sKey;
    QVector<BiItem *>       items;

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
};

struct SoftYBItem
{
    unsigned int            wIndex;
    QString                 sDesc;
    QString                 sName;
    unsigned int            dwAttr;

    unsigned int            wDftVal;

    SoftYBItem(unsigned int index)
    {
        wIndex = index;
        sDesc = QString("缺省%1").arg(index);
        sName = QString("item%1").arg(index);
        dwAttr = 0;

        wDftVal = 0;
    }
};

struct SoftYBConfig
{
    QString                 sDesc;
    QString                 sKey;
    QVector<SoftYBItem *>       items;

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
};

struct GseConfig
{
    DeviceInfo              deviceInfo;
    DeviceParas             deviceParas;

    BaseTab                 adAnaConfig;
    BaseTab                 derivedConfig;
    BaseTab                 svConfig;
    BaseTab                 gsAnaConfig;
    BaseTab                 otherAnaConfig;

    BiConfig                generalBiConfig;
    BiConfig                signalConfig;
    BiConfig                GooseBiConfig;
    SoftYBConfig            softYBConfig;

    void clear()
    {
        deviceInfo.clear();
        deviceParas.clear();

        adAnaConfig.clear();
        derivedConfig.clear();
        svConfig.clear();
        gsAnaConfig.clear();
        otherAnaConfig.clear();
    }
};


#endif // GSECONFIG_H
