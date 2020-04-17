#ifndef CDEVICEINFOWND_H
#define CDEVICEINFOWND_H

#include <CDefaultWnd.h>
#include <QTableWidget>

class CDataBase;

class CDeviceInfoWnd : public CDefaultWnd
{
    Q_OBJECT
public:
    explicit CDeviceInfoWnd(QWidget *parent = nullptr);
    ~CDeviceInfoWnd();

    void showInfo(void * pData) override;

    void Clear() override;
signals:

protected slots:
    void itemChangedSlot(QTableWidgetItem *item);

private:
    QTableWidget    *   m_table;
    CDataBase       *   m_data;
};

#endif // CDEVICEINFOWND_H
