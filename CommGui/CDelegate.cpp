#include "CDelegate.h"
#include <QSpinBox>

CSpinBoxDelegate::CSpinBoxDelegate(QObject *parent, int min, int max, int step)
    : QItemDelegate(parent),
      m_min(min),
      m_max(max),
      m_step(step)
{

}

CSpinBoxDelegate::~CSpinBoxDelegate()
{

}

QWidget *CSpinBoxDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (index.isValid())
    {
        QSpinBox *editor = new QSpinBox(parent);
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
        QSpinBox *box = static_cast<QSpinBox *>(editor);
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
        QSpinBox *box = static_cast<QSpinBox *>(editor);
        model->setData(index, box->value());
    }
    else
    {
        QItemDelegate::setModelData(editor, model, index);
    }
}
