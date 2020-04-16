#include <QApplication>
#include "CCfgMainWnd.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CCfgMainWnd::Instance()->show();
    return a.exec();
}
