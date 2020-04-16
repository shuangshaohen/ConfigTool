#ifndef CBITYPEDIALOG_H
#define CBITYPEDIALOG_H

#include <QDialog>
#include <QButtonGroup>
#include <QVector>
#include <QRadioButton>
#include <QTableWidgetItem>
#include <GSEConfig.h>

namespace Ui {
class CBITypeDialog;
}

class CBITypeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CBITypeDialog(QWidget *parent = nullptr , QTableWidgetItem * pItem = nullptr);
    ~CBITypeDialog();

private:
    Ui::CBITypeDialog *ui;

    QButtonGroup                *   m_pButtonGroup;
    QVector<QRadioButton *>         m_RadioButtons;

    QTableWidgetItem    *   m_pItem;
    BiConfig            *   m_pBI;
    int                 *   m_Select;

protected slots:
    void DialogAcceptSlot();
};

#endif // CBITYPEDIALOG_H
