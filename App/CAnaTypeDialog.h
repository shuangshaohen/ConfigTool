#ifndef CANATYPEDIALOG_H
#define CANATYPEDIALOG_H

#include <QDialog>
#include <QButtonGroup>
#include <QVector>
#include <QRadioButton>
#include <QTableWidgetItem>
#include <GSEConfig.h>

namespace Ui {
class CAnaTypeDialog;
}

class CAnaTypeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CAnaTypeDialog(QWidget *parent = nullptr , QTableWidgetItem * pItem = nullptr);
    ~CAnaTypeDialog();

private:
    Ui::CAnaTypeDialog *ui;
    QButtonGroup                *   m_pButtonGroup;
    QVector<QRadioButton *>         m_RadioButtons;

    QTableWidgetItem    *   m_pItem;
    AnaConfig           *   m_pAna;
    int                 *   m_Select;

protected slots:
    void DialogAcceptSlot();
};

#endif // CANATYPEDIALOG_H
