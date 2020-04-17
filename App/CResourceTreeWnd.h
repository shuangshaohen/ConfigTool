#ifndef CRESOURCETREEWND_H
#define CRESOURCETREEWND_H

#include <QTreeWidget>

class CDataBase;

class CResourceTreeWnd : public QTreeWidget
{
    Q_OBJECT
public:
    enum Enum_Node_Type
    {
        Enum_Invalid_Node = 0,

        Enum_Root_Node,								//	|-root
                                                    //	    |
        Enum_DeviceInfo_Node,						//		|-装置信息
                                                    //  	|
        Enum_DeviceParas_Node,						//		|-装置参数
                                                    //  	|
        Enum_Ana_Node,								//		|-模拟量
                                                    //		|
        Enum_Ana_AD_Node,                           //      |           |-AD模拟量
                                                    //      |           |
        Enum_Ana_Derived_Node,                      //      |           |-衍生模拟量
                                                    //      |           |
        Enum_Ana_SV_Node,                           //      |           |-SV模拟量
                                                    //      |           |
        Enum_Ana_GS_Node,                           //      |           |-GS模拟量
                                                    //      |           |
        Enum_Ana_Other_Node,                        //      |           |-其他模拟量
                                                    //      |           |
        Enum_BI_Node,                               //		|-开关量
                                                    //		|
        Enum_BI_General_Node,                       //      |           |-硬开入
                                                    //      |           |
        Enum_BI_Signal_Node,                        //      |           |-软遥信
                                                    //      |           |
        Enum_GSBI_Node,                     		//		|           |-GS开入
                                                    //		|           |
        Enum_SoftYB_Node,                			//		|           |-软压板
                                                    //		|           |
        Enum_BO_Node,								//		|-开出
                                                    //		|
        Enum_GSBO_Node,								//		|           |-GS开出
                                                    //		|           |
        Enum_TripMatrix_Node,						//		|           |-跳闸矩阵
                                                    //		|           |
        Enum_BO_YK_Node,                            //		|           |-遥控
                                                    //		|           |
        Enum_Setting_Node,                      	//		|-定值
                                                    //		|
        Enum_SPSet_Node,                            //		|           |-不分组定值
                                                    //		|           |
        Enum_SGSet_Node,                            //		|           |-分组定值
                                                    //		|           |
        Enum_Evt_Node,                              //		|-告警
                                                    //		|
        Enum_Alarm_Node,        					//		|           |-告警
                                                    //		|           |
        Enum_Act_Node,      						//		|           |-动作
                                                    //		|           |
        Enum_Remote_Node,							//		|           |-远方跳闸
                                                    //		|           |
        Enum_Check_Node,							//		|           |-自检
    };

    CResourceTreeWnd(QWidget *parent);
    ~CResourceTreeWnd();

    void CreateTreeNodes(CDataBase *pXercesXML);
    void RefreshTreeNodes();
    void ClearTreeNode();

    void SetNodeType( QTreeWidgetItem *pTreeWidgetItem, Enum_Node_Type type );
    Enum_Node_Type GetNodeType(QTreeWidgetItem *pTreeWidgetItem);

    void ShowCurrentNodeInfoWnd();

private:
    CDataBase * m_pXerceXML;

    QTreeWidgetItem * m_pRootTreeWidgetItem;
    QTreeWidgetItem * m_pInfoTreeWidgetItem;
    QTreeWidgetItem * m_pParasTreeWidgetItem;
    QTreeWidgetItem * m_pSmpTreeWidgetItem;
    QTreeWidgetItem * m_pAnaTreeWidgetItem;
    QTreeWidgetItem * m_pDerivedTreeWidgetItem;
    QTreeWidgetItem * m_pSvTreeWidgetItem;
    QTreeWidgetItem * m_pGsAnaTreeWidgetItem;
    QTreeWidgetItem * m_pOherAnaTreeWidgetItem;
    QTreeWidgetItem * m_pBITreeWidgetItem;
    QTreeWidgetItem * m_pGeneralBITreeWidgetItem;
    QTreeWidgetItem * m_pSignalTreeWidgetItem;
    QTreeWidgetItem * m_pGSBITreeWidgetItem;
    QTreeWidgetItem * m_pSoftYBTreeWidgetItem;
    QTreeWidgetItem * m_pBOTreeWidgetItem;
    QTreeWidgetItem * m_pGSBOTreeWidgetItem;
    QTreeWidgetItem * m_pTripMatrixTreeWidgetItem;
    QTreeWidgetItem * m_pYKTreeWidgetItem;
    QTreeWidgetItem * m_pSetTreeWidgetItem;
    QTreeWidgetItem * m_pSPSetTreeWidgetItem;
    QTreeWidgetItem * m_pSGSetTreeWidgetItem;
    QTreeWidgetItem * m_pEvtTreeWidgetItem;
    QTreeWidgetItem * m_pAlmEvtTreeWidgetItem;
    QTreeWidgetItem * m_pActEvtTreeWidgetItem;
    QTreeWidgetItem * m_pRemoteTripTreeWidgetItem;
    QTreeWidgetItem * m_pCheckEvtTreeWidgetItem;
private slots:
    void currentItemChangedSlot ( QTreeWidgetItem * current, QTreeWidgetItem * previous );

};

#endif // CRESOURCETREEWND_H
