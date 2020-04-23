#include "CParaTable.h"
#include <QTableView>
#include "TopoPara.h"
#include <QVBoxLayout>
#include <QHeaderView>

CParaTable::CParaTable(QWidget *parent) : CDefaultWnd(parent)
{
    m_Table = new QTableView(this);

    m_Table->horizontalHeader()->setStretchLastSection(true);

    TopoPara::GetInstance()->setTableView(m_Table);

    QVBoxLayout * layout = new QVBoxLayout (this);
    layout->addWidget(m_Table);
    setLayout(layout);
}

CParaTable::~CParaTable()
{
    delete m_Table;
}
