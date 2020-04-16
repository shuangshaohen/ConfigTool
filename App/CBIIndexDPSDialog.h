#ifndef CBIINDEXDPSDIALOG_H
#define CBIINDEXDPSDIALOG_H

#include <QDialog>
#include <QTableWidgetItem>
#include "CDataBase.h"

namespace Ui {
class CBIIndexDPSDialog;
}

class CBIIndexDPSDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CBIIndexDPSDialog(QWidget *parent = nullptr, QTableWidgetItem * pItem = nullptr , GseConfig * pGseConfig = nullptr);
    ~CBIIndexDPSDialog();

private:
    Ui::CBIIndexDPSDialog *ui;
    QTableWidgetItem    *   m_pItem;
    BiConfig            *   m_pBI;
    GseConfig           *   m_pGseConfig;

protected slots:
    void DialogAcceptSlot();
};

#endif // CBIINDEXDPSDIALOG_H
