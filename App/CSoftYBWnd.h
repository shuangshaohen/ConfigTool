#ifndef CSOFTYBWND_H
#define CSOFTYBWND_H

#include <QWidget>
#include <QTableWidgetItem>
#include "CDataBase.h"

namespace Ui {
class CSoftYBWnd;
}

class CSoftYBWnd : public QWidget
{
    Q_OBJECT

public:
    explicit CSoftYBWnd(QWidget *parent = nullptr);
    ~CSoftYBWnd();

    void ClearTableModel();

    void ShowSoftYBInfo(CDataBase * pCDataBase);

    void AddOper();
    void DeleteOper();
    void CopyOper();
    void UpOper();
    void DownOper();

protected slots:
    void TableWidgetCellChangedSlot ( int row, int column );
    void TableWidgetCellDoubleClickedSlot( int row, int column );

private:
    Ui::CSoftYBWnd *ui;
    GseConfig * m_pGseConfig;
    QTableWidget * m_pTable;

    void SetNodeDataPoint(QTableWidgetItem * pItem , SoftYbConfig * pNode);
    SoftYbConfig * GetNodeDataPoint(QTableWidgetItem * pItem);

    void CreateRowOfTable(SoftYbConfig *pNode, int row);

    void ChangeName(QTableWidgetItem * pItem);
    void ChangeAttr(QTableWidgetItem * pItem);
    void ChangeDftVal(QTableWidgetItem * pItem);
    void ChangeDesc(QTableWidgetItem * pItem);

    QString InputNewName(QString old);

    void RefreshCnnSoftYBName(QString oldName, QString newName);
};

#endif // CSOFTYBWND_H
