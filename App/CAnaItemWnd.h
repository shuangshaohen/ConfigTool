#ifndef CANAITEMWND_H
#define CANAITEMWND_H

#include <CBaseItemWnd.h>
#include <QTableWidget>
#include "CDataBase.h"

enum _Enum_AnaTable_Type
{
    Enum_AnaTable_Type_AD = 0,
    Enum_AnaTable_Type_Derived,
    Enum_AnaTable_Type_SV,
    Enum_AnaTable_Type_GS,
    Enum_AnaTable_Type_Other,
};

class CAnaItemWnd : public CBaseItemWnd
{
    Q_OBJECT
public:
    explicit CAnaItemWnd(QWidget *parent = nullptr );
    ~CAnaItemWnd();

    void setTableType(int type) override;
    void showInfo(void * pData) override;

    void AddOper() override;
    void DeleteOper() override;
    void UpOper() override;
    void DownOper() override;
signals:

protected:
    bool checkItemVal(QTableWidgetItem *item) override;

protected slots:
    void itemChangedSlot(QTableWidgetItem *item) override;
    void keyTextChangedSlot(const QString &text) override;
    void descTextChangedSlot(const QString &text) override;
private:
    CDataBase       *   m_data;
    int                 m_type;

    void createItem(int row, AnaItem * item);

    AnaConfig *     getConfig();
};

#endif // CANAITEMWND_H
