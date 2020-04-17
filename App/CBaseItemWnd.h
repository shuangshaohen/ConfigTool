#ifndef CBASEITEMWND_H
#define CBASEITEMWND_H

#include <CDefaultWnd.h>
#include <QComboBox>
#include <QLineEdit>
#include <QTableWidgetItem>

namespace Ui {
class CBaseItemWnd;
}

class AttrItem : public QTableWidgetItem
{
public:
    explicit AttrItem(const QString &text,int type = Type);

    QVariant data(int role) const;
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

private slots:
    void currentIndexChangedSlot(int index);
    void textChangedSlot(const QString &text);
protected:
    QTableWidget    *   m_table;
    QComboBox       *   m_comboBox;
    QLineEdit       *   m_lineEdit;

    void tableExchange(int fromRow, int toRow);
    void tableFilter(int column, QString key);
private:
    Ui::CBaseItemWnd *ui;
};

#endif // CBASEITEMWND_H
