#include "CDefaultWnd.h"
#include "ui_DefaultWnd.h"
#include <QTableWidget>

CDefaultWnd::CDefaultWnd(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CDefaultWnd)
{
    ui->setupUi(this);
}

CDefaultWnd::~CDefaultWnd()
{
    delete ui;
}

void CDefaultWnd::moveRow(QTableWidget *pTable, int fromRow, int toRow)
{
    if( pTable == NULL )
        return;

    pTable->setFocus();

    if( fromRow == toRow )
        return;

    if( fromRow < 0 || toRow < 0 )
        return;

    int nRowCount = pTable->rowCount();

    if( fromRow >= nRowCount  || toRow > nRowCount )
        return;

    if( toRow < fromRow )
        fromRow++;

    pTable->insertRow( toRow );
    int nCol = pTable->columnCount();

    for( int i=0; i<nCol; i++ )
    {
        pTable->setItem( toRow, i, pTable->takeItem( fromRow , i ) );
    }

    if( fromRow < toRow  )
        toRow--;

    pTable->removeRow( fromRow );
    pTable->selectRow( toRow );
}
