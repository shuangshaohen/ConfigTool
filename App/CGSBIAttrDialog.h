#ifndef CGSBIATTRDIALOG_H
#define CGSBIATTRDIALOG_H

#include <QDialog>
#include <QButtonGroup>
#include <QVector>
#include <QCheckBox>
#include <QTableWidgetItem>
#include <GSEConfig.h>

namespace Ui {
class CGSBIAttrDialog;
}

class CGSBIAttrDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CGSBIAttrDialog(QWidget *parent = nullptr, QTableWidgetItem * pItem = nullptr);
    ~CGSBIAttrDialog();

private:
    Ui::CGSBIAttrDialog *ui;
    QButtonGroup        *   m_pButtonGroup;
    QVector<QCheckBox *>    m_CheckBoxes;

    QTableWidgetItem    *   m_pItem;
    GooseBiConfig       *   m_pGSBI;

    void CreateCheckBoxes();
    void SetCheckBox(unsigned int dwAttr);
protected slots:
    void DialogAcceptSlot();
};

#endif // CGSBIATTRDIALOG_H
