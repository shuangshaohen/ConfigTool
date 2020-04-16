#ifndef CBIATTRDIALOG_H
#define CBIATTRDIALOG_H

#include <QDialog>
#include <QButtonGroup>
#include <QVector>
#include <QCheckBox>
#include <QTableWidgetItem>
#include <GSEConfig.h>

namespace Ui {
class CBIAttrDialog;
}

class CBIAttrDialog : public QDialog
{
    Q_OBJECT

public:
    enum _ENum_BIAttrGoup_Index{
        ENum_Free_BIAttrGoup = 0,
        ENum_RelayHard_BIAttrGoup,
        ENum_RelaySoft_BIAttrGoup,
        ENum_RelayGS_BIAttrGoup,
        ENum_CheckBI_BIAttrGoup,
        ENum_ResetBI_BIAttrGoup,
        ENum_RemoteOpBI_BIAttrGoup,
    };

    explicit CBIAttrDialog(QWidget *parent = nullptr , QTableWidgetItem * pItem = nullptr);
    ~CBIAttrDialog();

private:
    Ui::CBIAttrDialog *ui;
    QButtonGroup        *   m_pButtonGroup;
    QVector<QCheckBox *>    m_CheckBoxes;

    QTableWidgetItem    *   m_pItem;
    BiConfig            *   m_pBI;

    void CreateCheckBoxes();

    bool CheckModeRelayHard();
    bool CheckModeRelaySoft();
    bool CheckModeRelayGS();
    bool CheckModeCheckBI();
    bool CheckModeResetBI();
    bool CheckModeRemoteOpBI();

    void SetAllCheckBoxCheckable(bool b);
    void SetCheckBox(unsigned int dwAttr);
protected slots:
    void ButtonGroupClickedSlot(int index);
    void DialogAcceptSlot();

};

#endif // CBIATTRDIALOG_H
