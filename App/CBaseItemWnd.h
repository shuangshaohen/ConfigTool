#ifndef CBASEITEMWND_H
#define CBASEITEMWND_H

#include <CDefaultWnd.h>
#include <QComboBox>
#include <QLineEdit>
#include <QTableWidgetItem>
#include <QMenu>

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

    void Clear() override;

public slots:
    void showToolTip(QTableWidgetItem *item);

protected slots:
    void currentIndexChangedSlot(int index);
    void textChangedSlot(const QString &text);

    virtual void itemChangedSlot(QTableWidgetItem *item);
    virtual void descTextChangedSlot(const QString &text);
    virtual void keyTextChangedSlot(const QString &text);

    virtual void AddOper() override{};
    virtual void DeleteOper() override{};
    virtual void PasteOper() override;
    virtual void CopyOper() override;

    void customContextMenuRequestedSlot(const QPoint &pos);
protected:
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

    virtual bool checkItemVal(QTableWidgetItem * item);

    bool event(QEvent *event) override;
private:
    Ui::CBaseItemWnd *ui;
};

#endif // CBASEITEMWND_H
