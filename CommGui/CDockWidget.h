#ifndef CDOCKWIDGET_H
#define CDOCKWIDGET_H

#include "CommGui_global.h"
#include <QDockWidget>

class QPainter;

class COMMGUI_EXPORT CDockWidget: public QDockWidget
{
    Q_OBJECT

public:
    //CDockWidget(&title, parent,flags) : QDockWidget(const QString &title, QWidget *parent = 0, Qt::WindowFlags flags = 0);
    CDockWidget(QWidget *parent = 0, Qt::WindowFlags flags = 0);

private:

protected:



};

#endif // CDOCKWIDGET_H
