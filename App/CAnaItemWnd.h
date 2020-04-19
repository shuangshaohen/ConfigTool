#ifndef CANAITEMWND_H
#define CANAITEMWND_H

#include <CBaseItemWnd.h>
#include <QTableWidget>
#include "CDataBase.h"

enum _Enum_AnaTable_Type
{
    Enum_AnaTable_Type_AD = 1,
    Enum_AnaTable_Type_Derived,
    Enum_AnaTable_Type_SV,
    Enum_AnaTable_Type_GS,
    Enum_AnaTable_Type_Other,
};

class CAnaItemWnd : public CBaseItemWnd
{
    Q_OBJECT
public:
    explicit        CAnaItemWnd(QWidget *parent = nullptr );
    ~CAnaItemWnd();

private:
    //表格元素变化时需重写响应函数
    void            writeConfigVal(QTableWidgetItem * item) override;
    //检查表格值的正确性
    bool            checkItemVal(QTableWidgetItem *item) override;
    //创建新的一行表格
    void            createItem(int row, BaseItem *baseItem) override;
    //获取数据源
    BaseTab *       getConfig() override;
    //新建一行数据源
    BaseItem *      CreateNewItem(int row) override;
};

#endif // CANAITEMWND_H
