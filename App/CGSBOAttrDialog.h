#ifndef CGSBOATTRDIALOG_H
#define CGSBOATTRDIALOG_H

#include <QDialog>
#include <QButtonGroup>
#include <QVector>
#include <QCheckBox>
#include <QTableWidgetItem>
#include <GSEConfig.h>

namespace Ui {
class CGSBOAttrDialog;
}

class CGSBOAttrDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CGSBOAttrDialog(QWidget *parent = nullptr, QTableWidgetItem * pItem = nullptr);
    ~CGSBOAttrDialog();

private:
    Ui::CGSBOAttrDialog *ui;
    QButtonGroup        *   m_pButtonGroup;
    QVector<QCheckBox *>    m_CheckBoxes;

    QTableWidgetItem    *   m_pItem;
    GooseBoConfig       *   m_pGSBO;

    void CreateCheckBoxes();
    void SetCheckBox(unsigned int dwAttr);
protected slots:
    void DialogAcceptSlot();
};

#endif // CGSBOATTRDIALOG_H
