#include "CBaseItemWnd.h"
#include "ui_BaseItemWnd.h"
#include <QToolTip>
#include <QDebug>

CBaseItemWnd::CBaseItemWnd(QWidget *parent) :
    CDefaultWnd(parent),
    ui(new Ui::CBaseItemWnd)
{
    ui->setupUi(this);

    m_table = ui->table;
    m_comboBox = ui->comboBox;
    m_lineEdit = ui->lineEdit;

    //m_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_table->horizontalHeader()->setStretchLastSection(true);

    connect(m_comboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(currentIndexChangedSlot(int)));
    connect(m_lineEdit,SIGNAL(textChanged(QString)),this,SLOT(textChangedSlot(QString)));
    connect(m_table,SIGNAL(itemClicked(QTableWidgetItem *)),this, SLOT(showToolTip(QTableWidgetItem *)));
}

CBaseItemWnd::~CBaseItemWnd()
{
    delete ui;
}

void CBaseItemWnd::Clear()
{
    m_table->clearContents();
    m_lineEdit->clear();
    m_comboBox->clear();
}

void CBaseItemWnd::showToolTip(QTableWidgetItem * item)
{
    if(item == NULL)
    {
        return;
    }

    QToolTip::showText(QCursor::pos(),item->data(Qt::ToolTip).toString());
}

void CBaseItemWnd::currentIndexChangedSlot(int index)
{
    Q_UNUSED(index);
    tableFilter(m_comboBox->currentIndex(),m_lineEdit->text());
}

void CBaseItemWnd::textChangedSlot(const QString &text)
{
    Q_UNUSED(text);
    tableFilter(m_comboBox->currentIndex(),m_lineEdit->text());
}

void CBaseItemWnd::tableExchange(int fromRow, int toRow)
{
    if( m_table == NULL )
        return;

    m_table->setFocus();

    if( fromRow == toRow )
        return;

    if( fromRow < 0 || toRow < 0 )
        return;

    int nRowCount = m_table->rowCount();

    if( fromRow >= nRowCount  || toRow > nRowCount )
        return;

    if( toRow < fromRow )
        fromRow++;

    m_table->insertRow( toRow );
    int nCol = m_table->columnCount();

    for( int i=0; i<nCol; i++ )
    {
        m_table->setItem( toRow, i, m_table->takeItem( fromRow , i ) );
    }

    if( fromRow < toRow  )
        toRow--;

    m_table->removeRow( fromRow );
    m_table->selectRow( toRow );
}

void CBaseItemWnd::tableFilter(int column, QString key)
{
    for (int row = 0; row < m_table->rowCount(); row++)
    {
        if(("" == key)
                || (column >= m_table->columnCount() ))
        {
            m_table->setRowHidden(row,false);
        }
        else
        {
            if(m_table->item(row,column)->text().contains(key))
                m_table->setRowHidden(row,true);
            else
                m_table->setRowHidden(row,false);
        }
    }

}

AttrItem::AttrItem(const QString &text, int type): QTableWidgetItem(text,type)
{

}

QVariant AttrItem::data(int role) const
{
    if (role == Qt::TextAlignmentRole)
        return (int)Qt::AlignCenter;
    if(role == Qt::DisplayRole){
        int val = QTableWidgetItem::data(Qt::EditRole).toInt();

        if(val == 0)
            return "0";

        QString s = "0x" + QString::number(val,16).toUpper();
        return s;
    }
    return QTableWidgetItem::data(role);
}
