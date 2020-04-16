#ifndef CANAATTRDIALOG_H
#define CANAATTRDIALOG_H

#include <QDialog>
#include <QButtonGroup>
#include <QVector>
#include <QCheckBox>
#include <QTableWidgetItem>
#include <GSEConfig.h>

namespace Ui {
class CAnaAttrDialog;
}

class CAnaAttrDialog : public QDialog
{
    Q_OBJECT

public:
    enum _ENum_AnaAttrGoup_Index{
        ENum_Free_AnaAttrGoup = 0,
        ENum_ADRelay_AnaAttrGoup,
        ENum_SVRelay_AnaAttrGoup,
        ENum_CalRelay_AnaAttrGoup,
        ENum_DevInfo_AnaAttrGoup,
    };

    explicit CAnaAttrDialog(QWidget *parent = nullptr , QTableWidgetItem * pItem = nullptr);
    ~CAnaAttrDialog();

private:
    Ui::CAnaAttrDialog  *   ui;
    QButtonGroup        *   m_pButtonGroup;
    QVector<QCheckBox *>    m_CheckBoxes;

    QTableWidgetItem    *   m_pItem;
    AnaConfig           *   m_pAna;

    void CreateCheckBoxes();

    bool CheckModeADRelay();
    bool CheckModeSVRelay();
    bool CheckModeCalRelay();
    bool CheckModeDevInfo();

    void SetAllCheckBoxCheckable(bool b);
    void SetCheckBox(unsigned int dwAttr);
protected slots:
    void ButtonGroupClickedSlot(int index);
    void DialogAcceptSlot();
};

#endif // CANAATTRDIALOG_H
