#ifndef GSECONFIG_H
#define GSECONFIG_H

#include <QString>
#include <QVector>

#define CN_AD_I_COE (153)
#define CN_AD_U_COE (154)
#define CN_SV_I_COE (1000)
#define CN_SV_U_COE (100)
#define CN_CAL_COE  (1)

#define CN_AD_RELAY_ANA_ATTR (0xF)
#define CN_SV_RELAY_ANA_ATTR (0x80000007)
#define CN_CAL_RELAY_ANA_ATTR (0x20023)
#define CN_DEVINFO_ANA_ATTR (0x140001)

#define CN_AD_ANA_BIT (0x8)
#define CN_SV_ANA_BIT (0x80000000)
#define CN_GS_ANA_BIT (0x40000000)
#define CN_CAL_ANA_BIT (0x30)   //DB4|DB5

#define CN_HARD_BI_BIT (0x10)
#define CN_SOFT_BI_BIT (0x60)
#define CN_DOUBLE_BI_BIT (0x80)
#define CN_GOOSE_BI_BIT (0x80000000)

#define CN_RELAYHARD_BI_ATTR (0x1F)
#define CN_RELAYSOFT_BI_ATTR (0x43)
#define CN_RELAYGOOSE_BI_ATTR (0x8000000F)
#define CN_CHECK_BI_ATTR (0x021F)
#define CN_RESET_BI_ATTR (0x041F)
#define CN_REMOTEOP_BI_ATTR (0x401F)

#define CN_GSIN_SP_ATTR (0x1)
#define CN_GSIN_DP_ATTR (0x2)

#define CN_NOGROUP_SET_BIT (0x4)

enum _Enum_AnaTable_Column
{
    Enum_AnaTable_Name_Col=0,
    Enum_AnaTable_Type_Col,
    Enum_AnaTable_Attr_Col,
    Enum_AnaTable_Chan_Col,
    Enum_AnaTable_Coe_Col,
    Enum_AnaTable_Desc_Col,
    Enum_AnaTable_Phase_Col,
    Enum_AnaTable_Width_Col,
    Enum_AnaTable_Dot_Col,
    Enum_AnaTable_Capacity_Col,
    Enum_AnaTable_XuYCDft_Col,
    Enum_AnaTable_Unit_Col,
    Enum_AnaTable_KUnit_Col,
};

enum _Enum_BITable_Column
{
    Enum_BITable_Name_Col=0,
    Enum_BITable_Attr_Col,
    Enum_BITable_Chan_Col,
    Enum_BITable_HoldTime_Col,
    Enum_BITable_IndexDPS_Col,
    Enum_BITable_Type_Col,
    Enum_BITable_Desc_Col,
};

enum _Enum_GSBITable_Column
{
    Enum_GSBITable_Name_Col=0,
    Enum_GSBITable_Attr_Col,
    Enum_GSBITable_HWChan_Col,
    Enum_GSBITable_FWChan_Col,
};

enum _Enum_GSBOTable_Column
{
    Enum_GSBOTable_Name_Col=0,
    Enum_GSBOTable_Attr_Col,
    Enum_GSBOTable_SrcTab_Col,
    Enum_GSBOTable_HWChan_Col,
    Enum_GSBOTable_FWChan_Col,
};

enum _Enum_TripMatrixTable_Column
{
    Enum_TripMatrixTable_Name_Col=0,
    Enum_TripMatrixTable_Attr_Col,
    Enum_TripMatrixTable_BoardNum_Col,
    Enum_TripMatrixTable_ChanNum_Col,
    Enum_TripMatrixTable_Desc_Col,
};

enum _Enum_RecTable_Column
{
    Enum_RecTable_Name_Col=0,
    Enum_RecTable_Attr_Col,
    Enum_RecTable_BO1_Col,
    Enum_RecTable_BO2_Col,
    Enum_RecTable_Sign_Col,
    Enum_RecTable_Light_Col,
    Enum_RecTable_Desc_Col,
};

enum _Enum_SoftYBTable_Column
{
    Enum_SoftYBTable_Name_Col=0,
    Enum_SoftYBTable_Attr_Col,
    Enum_SoftYBTable_DftVal_Col,
    Enum_SoftYBTable_Desc_Col,
};

enum _Enum_SettingTable_Column
{
    Enum_SettingTable_Name_Col=0,
    Enum_SettingTable_Attr_Col,
    Enum_SettingTable_Type_Col,
    Enum_SettingTable_SecIn_Col,
    Enum_SettingTable_CoeRet_Col,
    Enum_SettingTable_MaxVal_Col,
    Enum_SettingTable_MinVal_Col,
    Enum_SettingTable_DftVal_Col,
    Enum_SettingTable_Width_Col,
    Enum_SettingTable_Dot_Col,
    Enum_SettingTable_Desc_Col,
    Enum_SettingTable_Group_Col,
    Enum_SettingTable_Unit_Col,
    Enum_SettingTable_KUnit_Col,
};


enum _ANA_TYP_                              // 模拟量类型定义
{
    EN_ANA_TYP_NULL = 0,                    // 未定义
    EN_ANA_TYP_I,                           // 电流
    EN_ANA_TYP_U,                           // 电压
    EN_ANA_TYP_U_S,                         // 站用变电压
    EN_ANA_TYP_P,                           // 有功功率
    EN_ANA_TYP_Q,                           // 无功功率
    EN_ANA_TYP_S,                           // 视在功率
    EN_ANA_TYP_COS,                         // 功率因素
    EN_ANA_TYP_DC_U,                        // 直流电压
    EN_ANA_TYP_DC_I,                        // 直流电流
    EN_ANA_TYP_GEAR,                        // 档位
    EN_ANA_TYP_FR,                          // 频率
    EN_ANA_TYP_T,                           // 温度
    EN_ANA_TYP_DU,                          // 直流
    EN_ANA_TYP_MC,                          // 脉冲
    EN_ANA_TYP_AMP,
    EN_ANA_TYP_ST,                          // 设备态
    EN_ANA_TYP_IMP,                         // 阻抗
    EN_ANA_TYP_FLT_U,                       // 故障电压
    EN_ANA_TYP_FLT_I,                       // 故障电流
    EN_ANA_TYP_FLT_ZROA,                    // 零序电流
    EN_ANA_TYP_FLT_DIFA,                    // 差动电流
    EN_ANA_TYP_FLT_DISKM,                   // 故障测距
    EN_ANA_TYP_FLT_ZROV,                    // 零序差压
    EN_ANA_TYP_END
};

enum   _ENUM_SET_TYPE_                                          // 定值类型
{
    EN_NO_DEFINE,                                               ///< 无类型
    EN_CONTROL_TYPE,                                            ///< 控制字
    EN_CURRENT_TYPE,                                            ///< 电流型
    EN_VOL_TYPE,                                                ///< 电压型
    EN_IMPEDANCE_TYPE,                                          ///< 阻抗型
    EN_ANGLE_TYPE,                                              ///< 角度型
    EN_TIME_TYPE,                                               ///< 时间型
    EN_CALCULATE_TYPE,                                          ///< 计算型
    EN_PTCT_TYPE,                                               ///< PTCT型
    EN_PT_TYPE,                                                 ///< PT型
    EN_CT_TYPE,                                                 ///< CT型
    EN_CT1N_TYPE,                                               ///< CT1次额定值
    EN_CT2N_TYPE,                                               ///< CT2次额定值
    EN_KR_TYPE,                                                 ///< 比率系数
    EN_SET_DIF_VOL,                                             ///< 电压滑差
    EN_FONT_TYPE,                                               ///< 字符串型
    EN_SETZONE_TYPE,                                            ///< 定值区号
    EN_SET_PARA_TYPE,                                           ///< 系统参数
    EN_LENTH_TYPE,                                              ///< 长度型
    EN_FR_TYPE,                                                 ///< 频率型
    EN_FR_DIF_TYPE,                                             ///< 频率滑差型
    EN_SET_TRIP_TYPE,                                           ///< 出口定值
    EN_ANGLE_DISTANCE_TYPE,                                      ///< 距离保护用角度
    EN_CURRENT_STABLE_TYPE										///< zhaoli@2014-10-6 电流定值的整定范围不受CT二次值影响的，区别于那些受二次值影响的电流型定值
};

enum	_ENUM_SET_GROUP_TYPE_
{
    EN_SIDE_START,                  /// 备用侧
    EN_SIDE_BASIC,                  /// 基本信息
    EN_SIDE_DEVINF,                 /// 装置参数
    EN_SIDE_COP,                    /// 内部定值
    EN_SIDE_MATRIX,                 /// 内部定值 出口矩阵
    EN_SIDE_ALL,                  	/// 全侧
    EN_SIDE_HIGH,                   /// 高压侧
    EN_SIDE_MED1,                   /// 中压侧
    EN_SIDE_MED2,                   /// 中压侧
    EN_SIDE_LOW1,                   /// 低压侧
    EN_SIDE_LOW2,                   /// 低压侧
    EN_SIDE_LK,                     /// 电抗器
    EN_SIDE_Z,                      /// Z变
    EN_SIDE_DEF,                    /// 自定义
    EN_SIDE_END,
    EN_SIDE_NONE=0xFF               /// 不存在
};

struct AnaConfig
{
    unsigned int            wIndex;
    unsigned int            dwAttr;
    unsigned int            dwXuYcDft;
    unsigned int            wCoe;
    unsigned int            wChan;
    unsigned int            wBoardChan;
    unsigned int            dwRFVIndex;
    unsigned int            dwSecIn;
    unsigned int            dwMuSoft;
    unsigned int            dwMuHard;
    unsigned int            wChanType;
    unsigned int            dwCapacity;
    unsigned int            byWidth;
    unsigned int            byDotBit;
    unsigned int            wPhaseName;
    QString                 sUnit;
    QString                 sKiloUnit;
    QString                 sName;
    QString                 sDesc;
    QString                 sAddr;

    //关联关系,用name具有唯一性
    QString                 sRFVName;
    QString                 sSecInName;
    QString                 sMuSoftName;
    QString                 sMuHardName;
};

struct BiConfig
{
    unsigned int            wIndex;
    unsigned int            dwAttr;
    unsigned int            wChan;
    unsigned int            wBoardChan;
    unsigned int            wIndexDPS;
    unsigned int            wHoldTime;
    unsigned int            wYXType;
    QString                 sName;
    QString                 sDesc;
    QString                 sEnName;
    QString                 sAddr;

    //关联关系,用name具有唯一性
    QString                 sIndexDPS;
};

struct GooseBiConfig
{
    unsigned int            wIndex;
    unsigned int            dwAttr;
    unsigned int            HWChan;
    unsigned int            FWChan;
    QString                 sName;
    QString                 sEnName;
    QString                 sAddr;

    //关联关系,用name具有唯一性
    QString                 sHW;
    QString                 sFW;
};

struct GooseBoConfig
{
    unsigned int            wIndex;
    unsigned int            dwAttr;
    unsigned int            wSrcTab;
    unsigned int            HWChan;
    unsigned int            FWChan;
    QString                 sName;
    QString                 sEnName;
    QString                 sAddr;

    //关联关系,用name具有唯一性
    QString                 sHW;
    QString                 sFW;
};

struct TripMatrixConfig
{
    unsigned int            wIndex;
    unsigned int            dwAttr;
    unsigned int            Chan0;
    unsigned int            IndexDPS;
    QString                 sName;
    QString                 sDesc;
    QString                 sEnName;
};

struct SettingConfig
{
    unsigned int            wIndex;
    unsigned int            dwAttr;
    unsigned int            dwValMin;
    unsigned int            dwValMax;
    unsigned int            dwValDft;
    unsigned int            wSecIn;
    unsigned int            wCoeRet;
    unsigned int            wType;
    unsigned int            byGroup;
    unsigned int            byWidth;
    unsigned int            byDotBit;
    QString                 sUnit;
    QString                 sKiloUnit;
    QString                 sName;
    QString                 sDesc;
    QString                 sAddr;

    //关联关系,用name具有唯一性
    QString                 sSecIn;
};

struct RecConfig
{
    unsigned int            wIndex;
    unsigned int            dwAttr;
    unsigned int            dwBOConfig;
    unsigned int            dwBOConfig2;
    unsigned int            dwBOSignal;
    unsigned int            dwLight;
    QString                 sName;
    QString                 sDesc;
    QString                 sAddr;
};

struct SoftYbConfig
{
    unsigned int            wIndex;
    unsigned int            dwAttr;
    unsigned int            ValDft;
    QString                 sName;
    QString                 sDesc;
};

struct RemoteTripConfig
{
    unsigned int            wIndex;
    unsigned int            dwAttr;
    unsigned int            dwBiIndex;
    unsigned int            dwBOConfig;
    unsigned int            dwBOConfig2;
    unsigned int            dwBOSignal;
    unsigned int            dwLight;
    QString                 sName;
    QString                 sDesc;
    QString                 sAddr;

    //关联关系,用name具有唯一性
    QString     sBI;
};

struct GseConfig
{
    unsigned int                    setGroup;
    QVector<AnaConfig *>			anaConfig;
    QVector<BiConfig *>             biConfig;
    QVector<GooseBiConfig *>		gsBiConfig;
    QVector<GooseBoConfig *>		gsBoConfig;
    QVector<TripMatrixConfig *>     tripMatrix;
    QVector<SettingConfig *>		settingConfig;
    QVector<SoftYbConfig *>         softYbConfig;
    QVector<RecConfig *>			alarmConfig;
    QVector<RecConfig *>			actConfig;
    QVector<RemoteTripConfig *>     remoteTrip;
};


#endif // GSECONFIG_H
