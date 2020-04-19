#ifndef CBASEITEMWND_H
#define CBASEITEMWND_H

#include <CDefaultWnd.h>
#include <QComboBox>
#include <QLineEdit>
#include <QTableWidgetItem>
#include <QMenu>
#include "CDataBase.h"

namespace Ui {
class CBaseItemWnd;
}

class AttrItem : public QTableWidgetItem
{
public:
    explicit AttrItem(const QString &text,int type = Type);

    QVariant data(int role) const;

    void setData(int role, const QVariant &value);

signals:

private:
};

class CBaseItemWnd : public CDefaultWnd
{
    Q_OBJECT

public:
    explicit CBaseItemWnd(QWidget *parent = nullptr);
    ~CBaseItemWnd();

    void            setTableType(int type) override;
    void            showInfo(void * pData) override;

    void            Clear() override;

public slots:
    void showToolTip(QTableWidgetItem *item);

protected slots:
    void currentIndexChangedSlot(int index);
    void textChangedSlot(const QString &text);

    void itemChangedSlot(QTableWidgetItem *item);
    void descTextChangedSlot(const QString &text);
    void keyTextChangedSlot(const QString &text);

    void AddOper() override;
    void DeleteOper() override;
    void UpOper() override;
    void DownOper() override;
    void PasteOper() override;
    void CopyOper() override;

    void customContextMenuRequestedSlot(const QPoint &pos);
protected:
    CDataBase       *   m_data;
    int                 m_type;

    QTableWidget    *   m_table;
    QComboBox       *   m_comboBox;
    QLineEdit       *   m_lineEdit;
    QLineEdit       *   m_lineEditDesc;
    QLineEdit       *   m_lineEditKey;

    QMenu           *   rightMenu;     //右键菜单
    QAction         *   addAction;     //新增
    QAction         *   deleteAction;  //删除
    QAction         *   copyAction;    //复制
    QAction         *   pasteAction;   //粘贴

    void tableExchange(int fromRow, int toRow);
    void tableFilter(int column, QString key);
    void updateTableBackground();

    QList<int> getSelectedRows();

    bool event(QEvent *event) override;

    //以下部分需要重写
    //表格元素变化时需重写响应函数
    virtual void writeConfigVal(QTableWidgetItem * item);
    //检查表格值的正确性
    virtual bool checkItemVal(QTableWidgetItem * item);
    //创建新的一行表格
    virtual void createItem(int row, BaseItem *baseItem);
    //获取数据源
    virtual BaseTab * getConfig();
    //新建一行数据源
    virtual BaseItem * CreateNewItem(int row);
private:
    Ui::CBaseItemWnd *ui;
};

#endif // CBASEITEMWND_H
