#ifndef CPARATABLE_H
#define CPARATABLE_H

#include "CDefaultWnd.h"
#include <QTableView>

class CParaTable : public CDefaultWnd
{
    Q_OBJECT
public:
    explicit CParaTable(QWidget *parent = nullptr);
    ~CParaTable();

signals:

private:
    QTableView * m_Table;
};

#endif // CPARATABLE_H
