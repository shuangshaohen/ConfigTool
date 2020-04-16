#ifndef CDEFAULTWND_H
#define CDEFAULTWND_H

#include <QWidget>

namespace Ui {
class CDefaultWnd;
}

class CDefaultWnd : public QWidget
{
    Q_OBJECT

public:
    explicit CDefaultWnd(QWidget *parent = nullptr);
    ~CDefaultWnd();

private:
    Ui::CDefaultWnd *ui;
};

#endif // CDEFAULTWND_H
