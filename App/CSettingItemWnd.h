#ifndef CSETTINGITEMWND_H
#define CSETTINGITEMWND_H

#include <CBaseItemWnd.h>
#include <QTableWidget>
#include "CDataBase.h"

enum _Enum_SetTable_Type
{
    Enum_SetTable_Type_SP = 1,
    Enum_SetTable_Type_SG,
};

class CSettingItemWnd : public CBaseItemWnd
{
    Q_OBJECT
public:
    explicit CSettingItemWnd(QWidget *parent = nullptr);

    ~CSettingItemWnd();

protected slots:
    void            itemDoubleClickedSlot(QTableWidgetItem *item) override;

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

#endif // CSETTINGITEMWND_H
