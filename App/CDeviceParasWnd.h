#ifndef CDEVICEPARASWND_H
#define CDEVICEPARASWND_H

#include <CDefaultWnd.h>
#include <QTableWidget>

class CDataBase;

class CDeviceParasWnd : public CDefaultWnd
{
    Q_OBJECT
public:
    explicit CDeviceParasWnd(QWidget *parent = nullptr);
    ~CDeviceParasWnd();

    void showInfo(void * pData) override;
    void Clear() override;
signals:

protected slots:
    void itemChangedSlot(QTableWidgetItem *item);
private:
    QTableWidget    *   m_table;
    CDataBase       *   m_data;
};

#endif // CDEVICEPARASWND_H
