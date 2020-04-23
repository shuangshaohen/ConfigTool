#include "CBaseItemWnd.h"
#include "ui_BaseItemWnd.h"
#include <QToolTip>
#include <QDebug>
#include <algorithm>
#include <QClipboard>
#include <QKeyEvent>

CBaseItemWnd::CBaseItemWnd(QWidget *parent) :
    CDefaultWnd(parent),
    ui(new Ui::CBaseItemWnd)
{
    ui->setupUi(this);

    m_data = NULL;
    m_type = 0;

    m_table = ui->table;
    m_comboBox = ui->comboBox;
    m_lineEdit = ui->lineEdit;
    m_lineEditDesc = ui->lineEditDesc;
    m_lineEditKey = ui->lineEditKey;

    m_lineEditKey->setReadOnly(true);

    rightMenu = new QMenu(this);
    addAction = new QAction("新增",this);
    copyAction = new QAction("复制",this);
    pasteAction = new QAction("粘贴",this);
    deleteAction = new QAction("删除",this);

    rightMenu->addAction(addAction);
    rightMenu->addAction(deleteAction);
    rightMenu->addAction(copyAction);
    rightMenu->addAction(pasteAction);

    connect(addAction,SIGNAL(triggered()),this,SLOT(AddOper()));
    connect(deleteAction,SIGNAL(triggered()),this,SLOT(DeleteOper()));
    connect(copyAction,SIGNAL(triggered()),this,SLOT(CopyOper()));
    connect(pasteAction,SIGNAL(triggered()),this,SLOT(PasteOper()));

    //m_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_table->horizontalHeader()->setStretchLastSection(true);
    m_table->setContextMenuPolicy(Qt::CustomContextMenu);
    m_table->verticalHeader()->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(m_table,SIGNAL(customContextMenuRequested(QPoint)),
            this,SLOT(customContextMenuRequestedSlot(QPoint)));
    connect(m_table->verticalHeader(),SIGNAL(customContextMenuRequested(QPoint)),
            this,SLOT(customContextMenuRequestedSlot(QPoint)));

    connect(m_table,SIGNAL(itemDoubleClicked(QTableWidgetItem *)),this,SLOT(itemDoubleClickedSlot(QTableWidgetItem *)));
    connect(m_table,SIGNAL(itemChanged(QTableWidgetItem *)),this,SLOT(itemChangedSlot(QTableWidgetItem *)));
    connect(m_comboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(currentIndexChangedSlot(int)));
    connect(m_lineEdit,SIGNAL(textChanged(QString)),this,SLOT(textChangedSlot(QString)));
    connect(m_table,SIGNAL(itemClicked(QTableWidgetItem *)),this, SLOT(showToolTip(QTableWidgetItem *)));
    connect(m_lineEditDesc,SIGNAL(textChanged(QString)),this,SLOT(descTextChangedSlot(QString)));
    connect(m_lineEditKey,SIGNAL(textChanged(QString)),this,SLOT(keyTextChangedSlot(QString)));
}

CBaseItemWnd::~CBaseItemWnd()
{
    delete ui;
}

void CBaseItemWnd::setTableType(int type)
{
    m_type = type;
    columnHidden();
}

void CBaseItemWnd::showInfo(void *pData)
{
    m_table->clearContents();
    if(NULL == pData)
    {
        m_data = NULL;
        return;
    }

    m_table->disconnect(SIGNAL(itemChanged(QTableWidgetItem *)));
    m_data = (CDataBase *)pData;

    BaseTab * config = getConfig();

    if(NULL == config)
        return;

    m_lineEditDesc->setText(config->sDesc);
    m_lineEditKey->setText(config->sKey);

    m_table->setRowCount(config->items.size());
    for (int row = 0; row < config->items.size(); row++)
    {
        createItem(row,config->items[row]);
    }
    setAlignment(m_table, Qt::AlignHCenter|Qt::AlignVCenter);
    connect(m_table,SIGNAL(itemChanged(QTableWidgetItem *)),this,SLOT(itemChangedSlot(QTableWidgetItem *)));
    updateTableBackground();
}

void CBaseItemWnd::Clear()
{
    m_table->clearContents();
    m_lineEdit->clear();
//    m_comboBox->clear();
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

void CBaseItemWnd::itemDoubleClickedSlot(QTableWidgetItem *item)
{
    Q_UNUSED(item);
}

void CBaseItemWnd::itemChangedSlot(QTableWidgetItem *item)
{
    setModified(true);

    writeConfigVal(item);

    updateTableBackground();
}

void CBaseItemWnd::descTextChangedSlot(const QString &text)
{
    setModified(true);

    BaseTab * config = getConfig();

    if(NULL == config)
        return;

    config->sDesc = text;
}

void CBaseItemWnd::keyTextChangedSlot(const QString &text)
{
    setModified(true);

    BaseTab * config = getConfig();

    if(NULL == config)
        return;

    config->sKey = text;
}

void CBaseItemWnd::AddOper()
{
    setModified(true);

    BaseTab * config = getConfig();

    if(NULL == config)
        return;

    m_table->disconnect(SIGNAL(itemChanged(QTableWidgetItem *)));
    int row = m_table->rowCount();
    if(m_table->currentItem() != NULL)
        row = m_table->currentRow()+1;
    BaseItem * newItem = CreateNewItem(row);
    config->insertItem(row,newItem);

    m_table->insertRow(row);
    createItem(row,newItem);

    setAlignment(m_table, Qt::AlignHCenter|Qt::AlignVCenter);
    connect(m_table,SIGNAL(itemChanged(QTableWidgetItem *)),this,SLOT(itemChangedSlot(QTableWidgetItem *)));
    updateTableBackground();
}

void CBaseItemWnd::DeleteOper()
{
    setModified(true);

    BaseTab * config = getConfig();

    if(NULL == config)
        return;

    m_table->disconnect(SIGNAL(itemChanged(QTableWidgetItem *)));
    QList<int> rowList = getSelectedRows();

    for (int i = rowList.size() - 1;i >= 0; i--)
    {
        int row = rowList[i];

        config->deletItem(row);
        m_table->removeRow(row);
    }

    connect(m_table,SIGNAL(itemChanged(QTableWidgetItem *)),this,SLOT(itemChangedSlot(QTableWidgetItem *)));
}

void CBaseItemWnd::UpOper()
{
    setModified(true);

    BaseTab * config = getConfig();

    if(NULL == config)
        return;

    m_table->disconnect(SIGNAL(itemChanged(QTableWidgetItem *)));
    QList<QTableWidgetItem *> itemList = m_table->selectedItems();
    QList<int> rowList = getSelectedRows();

    for (int i = 0;i < rowList.size(); i++)
    {
        int row = rowList[i];

        if(row == 0)
            break;

        config->swapItem(row,row-1);
        tableExchange(row,row-1);
    }

    for (int i = 0; i < itemList.size(); i++) {
        itemList[i]->setSelected(true);
    }
    connect(m_table,SIGNAL(itemChanged(QTableWidgetItem *)),this,SLOT(itemChangedSlot(QTableWidgetItem *)));
}

void CBaseItemWnd::DownOper()
{
    setModified(true);

    BaseTab * config = getConfig();

    if(NULL == config)
        return;

    m_table->disconnect(SIGNAL(itemChanged(QTableWidgetItem *)));
    QList<QTableWidgetItem *> itemList = m_table->selectedItems();

    QList<int> rowList = getSelectedRows();

    for (int i = rowList.size() - 1;i >= 0; i--)
    {
        int row = rowList[i];

        if(row == config->items.size() -1)
            break;

        config->swapItem(row,row+1);
        tableExchange(row,row+2);
    }

    for (int i = 0; i < itemList.size(); i++) {
        itemList[i]->setSelected(true);
    }
    connect(m_table,SIGNAL(itemChanged(QTableWidgetItem *)),this,SLOT(itemChangedSlot(QTableWidgetItem *)));
}

void CBaseItemWnd::PasteOper()
{
    QString text_to_past = QApplication::clipboard()->text();
    QStringList table_row_data_list = text_to_past.split("\n", QString::SkipEmptyParts);

    QModelIndex current_index = m_table->currentIndex();
    for (int i = 0; i < m_table->rowCount() - current_index.row() && i < table_row_data_list.length(); ++i){
        QStringList row_data_list = table_row_data_list.at(i).split("\t");
        for(int k = 0; k < m_table->columnCount() - current_index.column() && k<row_data_list.length(); k++){
            QModelIndex temp_index = current_index.sibling(i+current_index.row(), k+current_index.column());
            QTableWidgetItem * item = m_table->item(temp_index.row(),temp_index.column());
            item->setText(row_data_list.at(k));
        }
    }
}

void CBaseItemWnd::CopyOper()
{
    QString copied_text;
    QModelIndexList current_selected_indexs = m_table->selectionModel()->selectedIndexes();
    if(current_selected_indexs.isEmpty())
        return;

    int current_row = current_selected_indexs.at(0).row();
    for(int i = 0; i < current_selected_indexs.count(); i++){
        if(current_row != current_selected_indexs.at(i).row()){
            current_row = current_selected_indexs.at(i).row();
            copied_text.append("\n");
            copied_text.append(current_selected_indexs.at(i).data().toString());
            continue;
        }
        if(0 != i){
            copied_text.append("\t");
        }
        copied_text.append(current_selected_indexs.at(i).data().toString());
    }
    copied_text.append("\n");
    QApplication::clipboard()->setText(copied_text);
}

void CBaseItemWnd::customContextMenuRequestedSlot(const QPoint &pos)
{
    Q_UNUSED(pos);

    rightMenu->exec(cursor().pos());
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
                m_table->setRowHidden(row,false);
            else
                m_table->setRowHidden(row,true);
        }
    }

}

void CBaseItemWnd::updateTableBackground()
{
    m_table->disconnect(SIGNAL(itemChanged(QTableWidgetItem *)));
    for (int row = 0; row < m_table->rowCount(); row++) {
        for (int column = 0; column < m_table->columnCount();column++) {
            if(checkItemVal(m_table->item(row,column)) == false)
            {
                m_table->item(row,column)->setBackground(QBrush(QColor(255, 0, 0)));
            }
            else
            {
                m_table->item(row,column)->setBackground(QBrush(QColor(255, 255, 255)));
            }
        }
    }
    connect(m_table,SIGNAL(itemChanged(QTableWidgetItem *)),this,SLOT(itemChangedSlot(QTableWidgetItem *)));
}

void CBaseItemWnd::columnHidden()
{

}

QList<int> CBaseItemWnd::getSelectedRows()
{
    QList<int> rowList;

    QList<QTableWidgetItem *> itemList = m_table->selectedItems();

    for (int i = 0; i< itemList.size(); i++)
    {
        rowList.push_back(itemList[i]->row());
    }
    std::sort(rowList.begin(),rowList.end());
    auto it= std::unique(rowList.begin(),rowList.end());   //去除容器内重复元素
    rowList.erase(it,rowList.end());

    return rowList;
}

bool CBaseItemWnd::checkItemVal(QTableWidgetItem *item)
{
    Q_UNUSED(item);
    return true;
}

bool CBaseItemWnd::event(QEvent *event)
{
    QWidget * fWidget = focusWidget();
    if(m_table != qobject_cast<QTableWidget *>(fWidget))
        return QWidget::event(event);

    if (event->type() == QEvent::KeyPress){
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        if (keyEvent->matches(QKeySequence::Paste)){
            PasteOper();
            event->accept();
            return true;
        }
        else if(keyEvent->matches(QKeySequence::Copy)){
            CopyOper();
            event->accept();
            return true;
        }
    }

    return QWidget::event(event);
}

void CBaseItemWnd::writeConfigVal(QTableWidgetItem *item)
{
    Q_UNUSED(item);
}

void CBaseItemWnd::createItem(int row, BaseItem *baseItem)
{
    Q_UNUSED(row);
    Q_UNUSED(baseItem);
}

BaseTab *CBaseItemWnd::getConfig()
{
    return NULL;
}

BaseItem *CBaseItemWnd::CreateNewItem(int row)
{
    return new BaseItem(row);
}

AttrItem::AttrItem(const QString &text, int type): QTableWidgetItem(text,type)
{

}

QVariant AttrItem::data(int role) const
{
    if (role == Qt::TextAlignmentRole)
        return (int)Qt::AlignCenter;
    if(role == Qt::DisplayRole){
        QString strVal = QTableWidgetItem::data(Qt::EditRole).toString();

        unsigned int val = strVal.toUInt();
        if(val == 0)
            return "0";

        QString s = "0x" + QString::number(val,16).toUpper();
        return s;
    }
    return QTableWidgetItem::data(role);
}

void AttrItem::setData(int role, const QVariant &value)
{
    //qDebug() << "AttrItem setData " << value.toString() << "role = " << role;
    if(role == Qt::DisplayRole)
    {
        QTableWidgetItem::setData(role,value);

        QString str = value.toString();

        if(str.startsWith("0x"))
        {
            int val = str.toUInt(nullptr,16);
            setData(Qt::EditRole,val);
        }
    }
    else
        QTableWidgetItem::setData(role,value);
}
