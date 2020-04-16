#ifndef CTABLEWIDGETDIALOG_H
#define CTABLEWIDGETDIALOG_H

#include <QDialog>
#include <QTableWidget>
#include <GSEConfig.h>
#include "CommGui_global.h"

namespace Ui {
class CTableWidgetDialog;
}

class COMMGUI_EXPORT CTableWidgetDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CTableWidgetDialog(QWidget *parent = nullptr , QTableWidgetItem *pItem = nullptr);
    ~CTableWidgetDialog();

    QTableWidget * GetTableWidget();

    void SetConfig(GseConfig * pGseConfig);

    void ShowSetWithSecInType();
private:
    Ui::CTableWidgetDialog *ui;
    QTableWidgetItem    *   m_pItem;
    GseConfig           *   m_pGseConfig;

protected slots:
    void DialogAcceptSlot();
};

#endif // CTABLEWIDGETDIALOG_H
