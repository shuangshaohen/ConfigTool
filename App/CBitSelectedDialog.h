#ifndef CBITSELECTEDDIALOG_H
#define CBITSELECTEDDIALOG_H

#include <QDialog>
#include <QCheckBox>

namespace Ui {
class CBitSelectedDialog;
}

class CBitSelectedDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CBitSelectedDialog(unsigned int val ,QStringList list,QWidget *parent = nullptr);
    ~CBitSelectedDialog();

    unsigned int getVal();
private:
    Ui::CBitSelectedDialog *ui;
    QVector<QCheckBox *> m_boxes;
};

#endif // CBITSELECTEDDIALOG_H
