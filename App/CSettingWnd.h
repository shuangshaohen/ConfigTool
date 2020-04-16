#ifndef CSETTINGWND_H
#define CSETTINGWND_H

#include <QWidget>
#include <QTableWidgetItem>
#include "CDataBase.h"

namespace Ui {
class CSettingWnd;
}

class CSettingWnd : public QWidget
{
    Q_OBJECT

public:
    explicit CSettingWnd(QWidget *parent = nullptr);
    ~CSettingWnd();

    void ClearTableModel();

    void ShowSettingInfo(CDataBase * pCDataBase);

    void AddOper();
    void DeleteOper();
    void CopyOper();
    void UpOper();
    void DownOper();

protected slots:
    void SelectComboBoxCurrentIndexChangedSlot(int index);
    void TypeComboBoxCurrentIndexChangedSlot(int index);
    void InfoLineEditTextChanged(const QString &text);
    void TableWidgetCellChangedSlot ( int row, int column );
    void TableWidgetCellDoubleClickedSlot( int row, int column );
    void SettingGroupLineEditTextChanged(const QString &text);

private:
    Ui::CSettingWnd *ui;
    GseConfig * m_pGseConfig;

    void SetNodeDataPoint(QTableWidgetItem * pItem , SettingConfig * pNode);
    SettingConfig * GetNodeDataPoint(QTableWidgetItem * pItem);

    void ChangeName(QTableWidgetItem * pItem);
    void ChangeDesc(QTableWidgetItem * pItem);
    void ChangeAttr(QTableWidgetItem * pItem);
    void ChangeCoeRet(QTableWidgetItem * pItem);
    void ChangeMaxVal(QTableWidgetItem * pItem);
    void ChangeMinVal(QTableWidgetItem * pItem);
    void ChangeDftVal(QTableWidgetItem * pItem);
    void ChangeWidth(QTableWidgetItem * pItem);
    void ChangeDotBit(QTableWidgetItem * pItem);
    void ChangeUnit(QTableWidgetItem * pItem);
    void ChangeKunit(QTableWidgetItem * pItem);
    void ChangeType(QTableWidgetItem * pItem);
    void ChangeGroup(QTableWidgetItem * pItem);
    void ChangeSecIn(QTableWidgetItem * pItem);

    void HideSomeRowOfTableWidget();

    void CreateRowOfTable(SettingConfig *pNode, int row);

    QString InputNewName(QString old);

    void RefreshCnnSetName(QString oldName, QString newName);
};

#endif // CSETTINGWND_H
