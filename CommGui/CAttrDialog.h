#ifndef CATTRDIALOG_H
#define CATTRDIALOG_H

#include <QDialog>
#include <QTableWidgetItem>
#include <QVector>
#include <QCheckBox>
#include "CommGui_global.h"

namespace Ui {
class CAttrDialog;
}

class COMMGUI_EXPORT CAttrDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CAttrDialog(QWidget *parent = nullptr, QTableWidgetItem * pItem = nullptr);
    ~CAttrDialog();

    void SetCheckBoxText(int num, QString text);
    void SetCheckBox(unsigned int dwAttr);
private:
    Ui::CAttrDialog *ui;
    QTableWidgetItem    *   m_pItem;
    QVector<QCheckBox *>    m_CheckBoxes;

    void CreateCheckBoxes();
protected slots:
    void DialogAcceptSlot();
};

#endif // CATTRDIALOG_H
