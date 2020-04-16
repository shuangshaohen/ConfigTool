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
        Enum_Ana_Node,								//		|-模拟量
                                                    //		|
        Enum_BI_Node,                               //		|-开关量
                                                    //		|
        Enum_GSBI_Node,                     		//		|-GS开入
                                                    //		|
        Enum_GSBO_Node,								//		|-GS开出
                                                    //		|
        Enum_TripMatrix_Node,						//		|-跳闸矩阵
                                                    //		|
        Enum_Setting_Node,                      	//		|-定值
                                                    //		|
        Enum_SoftYB_Node,                			//		|-告警
                                                    //		|
        Enum_Alarm_Node,        					//		|-告警
                                                    //		|
        Enum_Act_Node,      						//		|-动作
                                                    //		|
        Enum_Remote_Node,							//		|-远方跳闸
    };

    CResourceTreeWnd(QWidget *parent);

    void CreateIEDTreeNodes();
    void ClearTreeNode();

    void SetNodeType( QTreeWidgetItem *pTreeWidgetItem, Enum_Node_Type type );
    Enum_Node_Type GetNodeType(QTreeWidgetItem *pTreeWidgetItem);

    void ShowCurrentNodeInfoWnd();

private:
    CDataBase * m_pXerceXML;

    QTreeWidgetItem * m_pRootTreeWidgetItem;
    QTreeWidgetItem * m_pAnaTreeWidgetItem;
    QTreeWidgetItem * m_pBITreeWidgetItem;
    QTreeWidgetItem * m_pGSBITreeWidgetItem;
    QTreeWidgetItem * m_pGSBOTreeWidgetItem;
    QTreeWidgetItem * m_pTripMatrixTreeWidgetItem;
    QTreeWidgetItem * m_pSettingTreeWidgetItem;
    QTreeWidgetItem * m_pSoftYBTreeWidgetItem;
    QTreeWidgetItem * m_pAlarmTreeWidgetItem;
    QTreeWidgetItem * m_pActTreeWidgetItem;
    QTreeWidgetItem * m_pRemoteTreeWidgetItem;

private slots:
    void currentItemChangedSlot ( QTreeWidgetItem * current, QTreeWidgetItem * previous );

};

#endif // CRESOURCETREEWND_H
