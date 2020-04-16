#ifndef CCOMBOBOX_H
#define CCOMBOBOX_H

#include <QComboBox>
#include <QTableWidgetItem>
#include "CDataBase.h"
#include "CommGui_global.h"

enum _ENum_ComboBox_Type{
    Enum_DefaultComboBox_Type=0,
    Enum_AnaChanTypeComboBox_Type,
};

class COMMGUI_EXPORT CComboBox : public QComboBox
{
    Q_OBJECT
public:
    CComboBox(QWidget *parent = nullptr , _ENum_ComboBox_Type type = Enum_DefaultComboBox_Type, void * p = nullptr);
    ~CComboBox();

private:
    AnaConfig           * m_pAna;

    void SetAnaList();

    void SetAnaNodePoint(AnaConfig * pAna);

    void ConnectAnaTypeChanged(bool b);

protected slots:
    void AnaTypeCurrentIndexChangedSlot(int index);

};

#endif // CCOMBOBOX_H
