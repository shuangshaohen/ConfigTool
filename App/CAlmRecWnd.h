#ifndef CALMRECWND_H
#define CALMRECWND_H

#include <QWidget>
#include <QTableWidgetItem>
#include "CDataBase.h"

namespace Ui {
class CAlmRecWnd;
}

class CAlmRecWnd : public QWidget
{
    Q_OBJECT

public:
    explicit CAlmRecWnd(QWidget *parent = nullptr);
    ~CAlmRecWnd();

    void ClearTableModel();

    void ShowAlmInfo(CDataBase * pCDataBase);

    void AddOper();
    void DeleteOper();
    void CopyOper();
    void UpOper();
    void DownOper();

protected slots:
    void TableWidgetCellChangedSlot ( int row, int column );
    void TableWidgetCellDoubleClickedSlot( int row, int column );

private:
    Ui::CAlmRecWnd *ui;
    GseConfig * m_pGseConfig;
    QTableWidget * m_pTable;

    void SetNodeDataPoint(QTableWidgetItem * pItem , RecConfig * pNode);
    RecConfig * GetNodeDataPoint(QTableWidgetItem * pItem);

    void CreateRowOfTable(RecConfig * pNode, int row);

    void ChangeName(QTableWidgetItem * pItem);
    void ChangeAttr(QTableWidgetItem * pItem);
    void ChangeBO1(QTableWidgetItem * pItem);
    void ChangeBO2(QTableWidgetItem * pItem);
    void ChangeSignal(QTableWidgetItem * pItem);
    void ChangeLight(QTableWidgetItem * pItem);
    void ChangeDesc(QTableWidgetItem * pItem);

    QString InputNewName(QString old);
};

#endif // CALMRECWND_H