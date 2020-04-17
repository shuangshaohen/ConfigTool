#include "CDefaultWnd.h"
#include "ui_DefaultWnd.h"
#include <QTableWidget>

CDefaultWnd::CDefaultWnd(QWidget *parent) :
    QWidget(parent)
{

}

CDefaultWnd::~CDefaultWnd()
{

}

void CDefaultWnd::setAlignment(QTableWidget *pTable, Qt::Alignment flag)
{
    for (int row = 0; row < pTable->rowCount(); row++) {
        for (int column = 0; column < pTable->columnCount();column++) {
            pTable->item(row,column)->setTextAlignment(flag);
        }
    }
}
