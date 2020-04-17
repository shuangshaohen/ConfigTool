#ifndef CCFGCENTERWND_H
#define CCFGCENTERWND_H

#include <QObject>
#include <QStackedWidget>
#include "CDataBase.h"

class CDefaultWnd;

class CCfgCenterWnd : public QStackedWidget
{
    Q_OBJECT
public:
    CCfgCenterWnd(QWidget *parent = nullptr);

    virtual ~CCfgCenterWnd(void);

    void ShowDefaultInfo(CDataBase * pData);
    void ShowDeviceInfo(CDataBase * pData);
    void ShowDeviceParas(CDataBase * pData);
    void ShowADAnaTable(CDataBase * pData);

    void AddInfo();
    void DeleteInfo();
    void CopyInfo();
    void UpInfo();
    void DownInfo();

    void ClearTableModel();
private:
    CDefaultWnd *   m_pDefaultWnd;
    CDefaultWnd *   m_pDeviceInfoWnd;
    CDefaultWnd *   m_pDeviceParasWnd;
    CDefaultWnd *   m_pADAnaParasWnd;
};

#endif // CCFGCENTERWND_H
