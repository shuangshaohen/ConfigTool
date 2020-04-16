#ifndef CGSBOSRCTABDIALOG_H
#define CGSBOSRCTABDIALOG_H

#include <QDialog>
#include <QButtonGroup>
#include <QTableWidgetItem>
#include <GSEConfig.h>

namespace Ui {
class CGSBOSrcTabDialog;
}

class CGSBOSrcTabDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CGSBOSrcTabDialog(QWidget *parent = nullptr, QTableWidgetItem * pItem = nullptr);
    ~CGSBOSrcTabDialog();

private:
    Ui::CGSBOSrcTabDialog *ui;
    QButtonGroup        *   m_pButtonGroup;

    QTableWidgetItem    *   m_pItem;
    GooseBoConfig       *   m_pGSBO;

    void SetCheckBox(unsigned int srcTab);
protected slots:
    void DialogAcceptSlot();
};

#endif // CGSBOSRCTABDIALOG_H
