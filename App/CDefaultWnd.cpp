#include "CDefaultWnd.h"
#include "ui_DefaultWnd.h"

CDefaultWnd::CDefaultWnd(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CDefaultWnd)
{
    ui->setupUi(this);
}

CDefaultWnd::~CDefaultWnd()
{
    delete ui;
}
