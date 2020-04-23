#ifndef INITALDIALOG_H
#define INITALDIALOG_H

#include <QDialog>

namespace Ui {
class InitalDialog;
}

class InitalDialog : public QDialog
{
    Q_OBJECT

public:
    explicit InitalDialog(QWidget *parent = nullptr);
    ~InitalDialog();

    int getOnlyDevice();
    int getSourceNum();
    int getBubarNum();
    int getLineNum();
    int getTransformerNum();
    int getLoadNum();
    int getRemoteNum();
    int getSwitchNum();

private:
    Ui::InitalDialog *ui;
};

#endif // INITALDIALOG_H
