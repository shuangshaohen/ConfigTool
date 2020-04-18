#ifndef CDEFAULTWND_H
#define CDEFAULTWND_H

#include <QWidget>
#include "CMsgInfo.h"

class QTableWidget;

class CDefaultWnd : public QWidget
{
    Q_OBJECT

public:
    explicit CDefaultWnd(QWidget *parent = nullptr);
    virtual ~CDefaultWnd();

    virtual void setTableType(int type){Q_UNUSED(type)};
    virtual void showInfo(void * pData){Q_UNUSED(pData);};

    virtual void AddOper(){};
    virtual void DeleteOper(){};
    virtual void UpOper(){};
    virtual void DownOper(){};

    virtual void CopyOper(){};
    virtual void PasteOper(){};

    virtual void Clear(){};

protected:
    void setAlignment(QTableWidget *pTable,Qt::Alignment flag);

    void outPutMsgInfo(const CMsgInfo &msgInfo);

    void setModified(bool bModified);
    void updateTree();
private:
};

#endif // CDEFAULTWND_H
