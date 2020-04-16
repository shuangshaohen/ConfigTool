#ifndef CTRIPMATRIXWND_H
#define CTRIPMATRIXWND_H

#include <QWidget>
#include <QTableWidgetItem>
#include "CDataBase.h"

#define CN_MAX_BOARD_BO_NUM (16)

namespace Ui {
class CTripMatrixWnd;
}

class CTripMatrixWnd : public QWidget
{
    Q_OBJECT

public:
    explicit CTripMatrixWnd(QWidget *parent = nullptr);
    ~CTripMatrixWnd();

    void ClearTableModel();

    void ShowTripMatrixInfo(CDataBase * pCDataBase);

    void AddOper();
    void DeleteOper();
    void CopyOper();
    void UpOper();
    void DownOper();

protected slots:
    void TableWidgetCellChangedSlot ( int row, int column );
    void TableWidgetCellDoubleClickedSlot(int row, int column);
private:
    Ui::CTripMatrixWnd *ui;
    GseConfig * m_pGseConfig;

    void SetNodeDataPoint(QTableWidgetItem * pItem , TripMatrixConfig * pNode);
    TripMatrixConfig * GetNodeDataPoint(QTableWidgetItem * pItem);

    void CreateRowOfTableWidget(TripMatrixConfig * pNode, int row);

    void ChangeName(QTableWidgetItem * pItem);
    void ChangeAttr(QTableWidgetItem * pItem);
    void ChangeBoardNum(QTableWidgetItem * pItem);
    void ChangeChanNum(QTableWidgetItem * pItem);
    void ChangeDesc(QTableWidgetItem * pItem);

    QString InputNewName(QString old);

    void RefreshCnnTripMatrixName(QString oldName, QString newName);
};

#endif // CTRIPMATRIXWND_H
