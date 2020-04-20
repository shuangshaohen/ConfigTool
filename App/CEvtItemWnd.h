#ifndef CEVTITEMWND_H
#define CEVTITEMWND_H

#include <CBaseItemWnd.h>
#include <QTableWidget>
#include "CDataBase.h"

enum _Enum_EvtTable_Type
{
    Enum_EvtTable_Type_Alm = 1,
    Enum_EvtTable_Type_Act,
    Enum_EvtTable_Type_Check,
    Enum_EvtTable_Type_Remote,
};

class CEvtItemWnd : public CBaseItemWnd
{
    Q_OBJECT
public:
    explicit CEvtItemWnd(QWidget *parent = nullptr);
    ~CEvtItemWnd();

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
    //列隐藏选择
    void            columnHidden() override;
};

#endif // CEVTITEMWND_H
