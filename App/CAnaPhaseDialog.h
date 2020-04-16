#ifndef CANAPHASEDIALOG_H
#define CANAPHASEDIALOG_H

#include <QDialog>
#include <QButtonGroup>
#include <QVector>
#include <QRadioButton>
#include <QTableWidgetItem>
#include <GSEConfig.h>

namespace Ui {
class CAnaPhaseDialog;
}

class CAnaPhaseDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CAnaPhaseDialog(QWidget *parent = nullptr , QTableWidgetItem * pItem = nullptr);
    ~CAnaPhaseDialog();

private:
    Ui::CAnaPhaseDialog *ui;

    QButtonGroup                *   m_pButtonGroup;
    QVector<QRadioButton *>         m_RadioButtons;

    QTableWidgetItem    *   m_pItem;
    AnaConfig           *   m_pAna;
    int                 *   m_Select;

protected slots:
    void DialogAcceptSlot();
};

#endif // CANAPHASEDIALOG_H
