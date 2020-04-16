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

enum _Enum_PubTable_Column
{
    Enum_PubTable_Index_Col = 0,    //索引
    Enum_PubTable_Desc_Col,         //描述
    Enum_AnaTable_Name_Col,         //名字
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
    Enum_AnaTable_DataWith_Col,
    Enum_AnaTable_DataDot_Col,                              //数据属性Width/DotBit
    Enum_AnaTable_Unit_Col,
    Enum_AnaTable_KUnit_Col,                                //一次值单位/二次值单位
};

struct DeviceInfo
{
    QString                 sType;
    QString                 sDesc;
    QString                 sVersion;
    QString                 sDate;
    QString                 sCRC;
};

struct DeviceParas
{
    unsigned int            wMaxAnaNum;
    unsigned int            wMaxBiNum;
    unsigned int            wMaxSvNum;
    unsigned int            wMaxGooseNum;
    unsigned int            wMaxBoNum;
    unsigned int            wSmpRate;
};

struct AnaItem
{
    unsigned int            wIndex;
    QString                 sDesc;
    QString                 sName;
    unsigned int            dwAttr;

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

    AnaItem(unsigned int index)
    {
        wIndex = index;
        sDesc = QString("条目%1").arg(index);
        sName = QString("item%1").arg(index);
        dwAttr = 0;

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
        sUnit = "";
        sKiloUnit = "";
    }
};

struct AnaConfig
{
    QString                 sDesc;
    QString                 sKey;
    QVector<AnaItem *>      items;

    void clear()
    {
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

    AnaConfig               adAnaConfig;
    AnaConfig               derivedConfig;
    AnaConfig               svConfig;
    AnaConfig               gsAnaConfig;
    AnaConfig               otherAnaConfig;

    void clear()
    {
        adAnaConfig.clear();
        derivedConfig.clear();
        svConfig.clear();
        gsAnaConfig.clear();
        otherAnaConfig.clear();
    }
};


#endif // GSECONFIG_H
