#ifndef CDEFAULTWND_H
#define CDEFAULTWND_H

#include <QWidget>

class QTableWidget;

namespace Ui {
class CDefaultWnd;
}

class CDefaultWnd : public QWidget
{
    Q_OBJECT

public:
    explicit CDefaultWnd(QWidget *parent = nullptr);
    ~CDefaultWnd();

    virtual void showInfo(void * pData){Q_UNUSED(pData);};
    virtual void AddOper(){};
    virtual void DeleteOper(){};
    virtual void CopyOper(){};
    virtual void UpOper(){};
    virtual void DownOper(){};

    virtual void Clear(){};

protected:
    void moveRow(QTableWidget *pTable, int fromRow, int toRow);
private:
    Ui::CDefaultWnd *ui;
};

#endif // CDEFAULTWND_H
