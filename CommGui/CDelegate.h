#ifndef CDELEGATE_H
#define CDELEGATE_H

#include <QStyledItemDelegate>
#include <QItemDelegate>
#include <QModelIndex>
#include <QPainter>
#include <QWidget>
#include "CommGui_global.h"

class COMMGUI_EXPORT CSpinBoxDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    explicit CSpinBoxDelegate(QObject *parent = nullptr, int min = 0, int max = 65535, int step = 1);
    ~CSpinBoxDelegate();

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                                        const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                                    const QModelIndex &index) const;
signals:

private:
    int m_min;
    int m_max;
    int m_step;
};

class COMMGUI_EXPORT CComboBoxDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    explicit CComboBoxDelegate(const QStringList &labels,QObject *parent = nullptr);
    ~CComboBoxDelegate();

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                                        const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                                    const QModelIndex &index) const;
signals:

private:
    QStringList m_labels;
};

#endif // CDELEGATE_H
