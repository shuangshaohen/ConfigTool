#include "InitalDialog.h"
#include "ui_InitalDialog.h"
#include <QButtonGroup>

InitalDialog::InitalDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InitalDialog)
{
    ui->setupUi(this);
    setWindowTitle("初始化Topo参数配置");
    QButtonGroup * group = new QButtonGroup(this);
    group->addButton(ui->rBDevice);
    group->addButton(ui->rBRegion);
}

InitalDialog::~InitalDialog()
{
    delete ui;
}

int InitalDialog::getOnlyDevice()
{
    if(ui->rBDevice->isChecked())
        return 1;
    else
        return 0;
}

int InitalDialog::getSourceNum()
{
    return ui->spinBoxSoure->value();
}

int InitalDialog::getBubarNum()
{
    return ui->spinBoxBusbar->value();
}

int InitalDialog::getLineNum()
{
    return ui->spinBoxLine->value();
}

int InitalDialog::getTransformerNum()
{
    return ui->spinBoxTransformer->value();
}

int InitalDialog::getLoadNum()
{
    return ui->spinBoxLoad->value();
}

int InitalDialog::getRemoteNum()
{
    return ui->spinBoxRemote->value();
}

int InitalDialog::getSwitchNum()
{
    return ui->spinBoxSwitch->value();
}
