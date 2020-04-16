#ifndef CGSBOWND_H
#define CGSBOWND_H

#include <QWidget>
#include <QTableWidgetItem>
#include "CDataBase.h"

namespace Ui {
class CGSBOWnd;
}

class CGSBOWnd : public QWidget
{
    Q_OBJECT

public:
    explicit CGSBOWnd(QWidget *parent = nullptr);
    ~CGSBOWnd();

    void ClearTableModel();

    void ShowGSBOInfo(CDataBase * pCDataBase);

    void AddOper();
    void DeleteOper();
    void CopyOper();
    void UpOper();
    void DownOper();

protected slots:
    void GSBOTableWidgetCellChangedSlot ( int row, int column );
    void GSBOTableWidgetCellDoubleClickedSlot(int row, int column);
    void GSBOTableWidgetCellClickedSlot(int row, int column);

    void GSBOInfoComboBoxCurrentIndexChangedSlot(int index);
    void GSBOInfoLineEditTextChanged(const QString &text);

    void CnnComboBoxCurrentIndexChangedSlot(int index);
    void CnnInfoLineEditTextChanged(const QString &text);

    void CnnPushButtonClicked(bool checked);
    void DisPushButtonClicked(bool checked);

private:
    Ui::CGSBOWnd *ui;
    GseConfig * m_pGseConfig;

    void HideSomeRowOfGSBOTableWidget();

    void SetNodeDataPoint(QTableWidgetItem * pItem , GooseBoConfig * pGSBO);
    GooseBoConfig * GetNodeDataPoint(QTableWidgetItem * pItem);

    void CreateRowOfGSBOTableWidget(GooseBoConfig * pNode, int row);

    void ShowCnnBITable();
    void ShowCnnAnaTable();
    void showCnnTZJZTable();

    void HideSomeRowOfCnnTable();

    void ChangeName(QTableWidgetItem * pItem);
    void ChangeAttr(QTableWidgetItem * pItem);
    void ChangeSrcTab(QTableWidgetItem * pItem);

    QString InputNewName(QString old);
};

#endif // CGSBOWND_H
