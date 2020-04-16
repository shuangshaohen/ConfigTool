#ifndef CANAWND_H
#define CANAWND_H

#include <QWidget>
#include <QTableWidgetItem>
#include "CDataBase.h"

namespace Ui {
class CAnaWnd;
}

class CAnaWnd : public QWidget
{
    Q_OBJECT
public:
    explicit CAnaWnd(QWidget *parent = nullptr);
    ~CAnaWnd();

    void ClearTableModel();

    void ShowAnaInfo(CDataBase * pCDataBase);

    void AddOper();
    void DeleteOper();
    void CopyOper();
    void UpOper();
    void DownOper();

protected slots:
    void AnaSelectComboBoxCurrentIndexChangedSlot(int index);
    void AnaPropertyComboBoxCurrentIndexChangedSlot(int index);
    void AnaInfoLineEditTextChanged(const QString &text);
    void ShowYCPropertyCheckBoxStateChangedSlot(int state);
    void AnaTableWidgetCellChangedSlot ( int row, int column );
    void AnaTableWidgetCellDoubleClickedSlot( int row, int column );

    void AnaTableWidgetCurrentItemChangedSlot(QTableWidgetItem *current, QTableWidgetItem *previous);

    void AnaOtherComboBoxCurrentIndexChangedSlot(int index);
    void OtherInfoLineEditTextChanged(const QString &text);

    void AnaCnnPushButtonClicked(bool checked);
    void AnaDisPushButtonClicked(bool checked);
private:
    Ui::CAnaWnd *ui;
    GseConfig * m_pGseConfig;

    void SetNodeDataPoint(QTableWidgetItem * pItem , AnaConfig * pAna);
    AnaConfig * GetNodeDataPoint(QTableWidgetItem * pItem);

    void HideSomeRowOfAnaTableWidget();

    void ChangeName(QTableWidgetItem * pItem);
    void ChangeDesc(QTableWidgetItem * pItem);
    void ChangeAttr(QTableWidgetItem * pItem);
    void ChangeChan(QTableWidgetItem * pItem);
    void ChangeCoe(QTableWidgetItem * pItem);
    void ChangeWidth(QTableWidgetItem * pItem);
    void ChangeDotbit(QTableWidgetItem * pItem);
    void ChangeCapacity(QTableWidgetItem * pItem);
    void ChangeXuYCDft(QTableWidgetItem * pItem);
    void ChangeType(QTableWidgetItem *pItem);
    void ChangeUnit(QTableWidgetItem *pItem);
    void ChangeKUnit(QTableWidgetItem *pItem);

    bool CheckNodeIsADChan(AnaConfig * pAna);
    bool CheckNodeIsSVChan(AnaConfig * pAna);
    bool CheckNodeIsCalChan(AnaConfig * pAna);

    QString GetSettingDesc(QString name);
    QString GetSoftYBDesc(QString name);
    QString GetBIDesc(QString name);
    void    SetAnaCnnTable(int row);

    void ShowOtherTableWidget();
    void ShowOtherSettingTableWidget();
    void ShowOtherSoftYBTableWidget();
    void ShowOtherBITableWidget();
    void HideOtherTableWidget();

    void moveRow(QTableWidget * pTable, int fromRow, int toRow);

    QString InputNewName(QString old);

    void CreateRowOfAnaTableWidget(AnaConfig * pNode, int row);

    void RefreshCnnAnaName(QString oldName, QString newName);
};

#endif // CANAWND_H
