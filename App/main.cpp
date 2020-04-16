#include <QApplication>
#include "CCfgMainWnd.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CCfgMainWnd::Instance()->show();
    QString s = "0x60000001";
    int t = -1;
    bool ok = false;
    if(s.startsWith("0x"))
        t = s.toLongLong(&ok, 16);
    else
        t = s.toInt();
    qDebug()<<"Run !" << t;
    return a.exec();
}
