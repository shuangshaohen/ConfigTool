#ifndef CTYPEDIALOG_H
#define CTYPEDIALOG_H

#include <QDialog>
#include <QButtonGroup>
#include <QVector>
#include <QRadioButton>
#include <QTableWidgetItem>
#include "CommGui_global.h"

#define CN_MAX_TYPE_NUM (24)

namespace Ui {
class CTypeDialog;
}

class COMMGUI_EXPORT CTypeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CTypeDialog(QWidget *parent = nullptr, QTableWidgetItem * pItem = nullptr);
    ~CTypeDialog();

    void SetRadioButtonText(int num, QString text);
    void SetType(unsigned int type);
private:
    Ui::CTypeDialog *ui;
    QButtonGroup                *   m_pButtonGroup;
    QVector<QRadioButton *>         m_RadioButtons;

    QTableWidgetItem    *   m_pItem;

protected slots:
    void DialogAcceptSlot();
};

#endif // CTYPEDIALOG_H
