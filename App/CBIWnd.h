#ifndef CBIWND_H
#define CBIWND_H

#include <QWidget>
#include <QTableWidgetItem>
#include "CDataBase.h"

namespace Ui {
class CBIWnd;
}

class CBIWnd : public QWidget
{
    Q_OBJECT

public:
    explicit CBIWnd(QWidget *parent = nullptr);
    ~CBIWnd();

    void ClearTableModel();

    void ShowBIInfo(CDataBase * pCDataBase);

    void AddOper();
    void DeleteOper();
    void CopyOper();
    void UpOper();
    void DownOper();

protected slots:
    void BITableWidgetCellChangedSlot ( int row, int column );
    void BITableWidgetCellDoubleClickedSlot(int row, int column);

    void BISelectComboBoxCurrentIndexChangedSlot(int index);
    void BIPropertyComboBoxCurrentIndexChangedSlot(int index);
    void BIInfoLineEditTextChanged(const QString &text);

private:
    Ui::CBIWnd *ui;
    GseConfig * m_pGseConfig;

    void HideSomeRowOfBITableWidget();

    void SetNodeDataPoint(QTableWidgetItem * pItem , BiConfig * pBI);
    BiConfig * GetNodeDataPoint(QTableWidgetItem * pItem);

    bool CheckNodeIsHardYX(BiConfig * pBI);
    bool CheckNodeIsSoftYX(BiConfig * pBI);
    bool CheckNodeIsDoubleYX(BiConfig * pBI);
    bool CheckNodeIsGOOSEYX(BiConfig * pBI);

    void ChangeName(QTableWidgetItem * pItem);
    void ChangeDesc(QTableWidgetItem * pItem);
    void ChangeAttr(QTableWidgetItem * pItem);
    void ChangeChan(QTableWidgetItem * pItem);
    void ChangeHoldTime(QTableWidgetItem * pItem);
    void ChangeIndexDPS(QTableWidgetItem * pItem);
    void ChangeType(QTableWidgetItem * pItem);

    void moveRow(QTableWidget * pTable, int fromRow, int toRow);

    QString InputNewName(QString old);

    void CreateRowOfBITableWidget(BiConfig * pNode, int row);

    void RefreshCnnBIName(QString oldName, QString newName);
};

#endif // CBIWND_H
