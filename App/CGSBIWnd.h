#ifndef CGBIWND_H
#define CGBIWND_H

#include <QWidget>
#include <QTableWidgetItem>
#include "CDataBase.h"

namespace Ui {
class CGSBIWnd;
}

class CGSBIWnd : public QWidget
{
    Q_OBJECT

public:
    explicit CGSBIWnd(QWidget *parent = nullptr);
    ~CGSBIWnd();

    void ClearTableModel();

    void ShowGSBIInfo(CDataBase * pCDataBase);

    void AddOper();
    void DeleteOper();
    void CopyOper();
    void UpOper();
    void DownOper();

protected slots:
    void GSBITableWidgetCellChangedSlot ( int row, int column );
    void GSBITableWidgetCellDoubleClickedSlot(int row, int column);
    void GSBITableWidgetCellClickedSlot(int row, int column);

    void GSBIInfoComboBoxCurrentIndexChangedSlot(int index);
    void GSBIInfoLineEditTextChanged(const QString &text);

    void CnnComboBoxCurrentIndexChangedSlot(int index);
    void CnnInfoLineEditTextChanged(const QString &text);

    void CnnPushButtonClicked(bool checked);
    void DisPushButtonClicked(bool checked);
private:
    Ui::CGSBIWnd *ui;
    GseConfig * m_pGseConfig;

    void HideSomeRowOfGSBITableWidget();

    void SetNodeDataPoint(QTableWidgetItem * pItem , GooseBiConfig * pGSBI);
    GooseBiConfig * GetNodeDataPoint(QTableWidgetItem * pItem);

    void CreateRowOfGSBITableWidget(GooseBiConfig * pNode, int row);

    void ShowCnnBITable();
    void ShowCnnAnaTable();

    void HideSomeRowOfCnnTable();

    void ChangeName(QTableWidgetItem * pItem);
    void ChangeAttr(QTableWidgetItem * pItem);

    QString InputNewName(QString old);

};

#endif // CGBIWND_H
