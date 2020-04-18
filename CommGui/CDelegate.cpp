#include "CDelegate.h"
#include <HBaseSpinBox.h>
#include <QComboBox>

CSpinBoxDelegate::CSpinBoxDelegate(QObject *parent, int min, int max, int step, int base)
    : QItemDelegate(parent),
      m_min(min),
      m_max(max),
      m_step(step),
      m_base(base)
{

}

CSpinBoxDelegate::~CSpinBoxDelegate()
{

}

QWidget *CSpinBoxDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (index.isValid())
    {
        HBaseSpinBox *editor = new HBaseSpinBox(parent);
        editor->setBase(m_base);
        editor->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        editor->installEventFilter(const_cast<CSpinBoxDelegate *>(this));
        return editor;
    }
    else
    {
        return QItemDelegate::createEditor(parent, option, index);
    }
}

void CSpinBoxDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    if (index.isValid())
    {
        int value = index.model()->data(index, Qt::EditRole).toInt();
        HBaseSpinBox *box = static_cast<HBaseSpinBox *>(editor);
        box->setRange(m_min,m_max);
        box->setSingleStep(m_step);
        box->setValue(value);
    }
    else
    {
        QItemDelegate::setEditorData(editor, index);
    }
}

void CSpinBoxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    if (index.isValid())
    {
        HBaseSpinBox *box = static_cast<HBaseSpinBox *>(editor);
        model->setData(index, box->value());
    }
    else
    {
        QItemDelegate::setModelData(editor, model, index);
    }
}

CComboBoxDelegate::CComboBoxDelegate(const QStringList &labels, QObject *parent): QItemDelegate(parent)
{
    m_labels = labels;
}

CComboBoxDelegate::~CComboBoxDelegate()
{

}

QWidget *CComboBoxDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (index.isValid())
    {
        QComboBox *editor = new QComboBox(parent);
        editor->installEventFilter(const_cast<CComboBoxDelegate *>(this));
        return editor;
    }
    else
    {
        return QItemDelegate::createEditor(parent, option, index);
    }
}

void CComboBoxDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    if (index.isValid())
    {
        QString value = index.model()->data(index, Qt::EditRole).toString();
        QComboBox *box = static_cast<QComboBox *>(editor);
        box->addItems(m_labels);
        box->setEditable(true);
        box->setCurrentText(value);
    }
    else
    {
        QItemDelegate::setEditorData(editor, index);
    }
}

void CComboBoxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    if (index.isValid())
    {
        QComboBox *box = static_cast<QComboBox *>(editor);
        model->setData(index, box->currentText());
    }
    else
    {
        QItemDelegate::setModelData(editor, model, index);
    }
}
