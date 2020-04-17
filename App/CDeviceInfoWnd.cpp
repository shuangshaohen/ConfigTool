#include "CDeviceInfoWnd.h"
#include "CDataBase.h"
#include <QHeaderView>
#include <QVBoxLayout>

CDeviceInfoWnd::CDeviceInfoWnd(QWidget *parent) : CDefaultWnd(parent)
{
    m_table = new QTableWidget(this);

    m_table->setColumnCount(5);
    QStringList headerList;
    headerList << "型号" << "描述" << "版本" << "日期" << "校验码";
    m_table->setHorizontalHeaderLabels(headerList);
    //m_table->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    m_table->horizontalHeader()->setStretchLastSection(true);

    QVBoxLayout * layout = new QVBoxLayout (this);
    layout->addWidget(m_table);
    setLayout(layout);

    m_data = NULL;
}

CDeviceInfoWnd::~CDeviceInfoWnd()
{
    delete m_table;
}

void CDeviceInfoWnd::showInfo(void *pData)
{
    m_table->clearContents();
    if(NULL == pData)
    {
        m_data = NULL;
        return;
    }

    m_table->disconnect(SIGNAL(itemChanged(QTableWidgetItem *)));
    m_data = (CDataBase *)pData;
    DeviceInfo * info = &m_data->GetConfig()->deviceInfo;

    m_table->setRowCount(1);

    m_table->setItem(0,Enum_DeviceInfoTable_Type_Col,new QTableWidgetItem(info->sType));
    m_table->setItem(0,Enum_DeviceInfoTable_Desc_Col,new QTableWidgetItem(info->sDesc));
    m_table->setItem(0,Enum_DeviceInfoTable_Version_Col,new QTableWidgetItem(info->sVersion));
    m_table->setItem(0,Enum_DeviceInfoTable_Date_Col,new QTableWidgetItem(info->sDate));
    m_table->setItem(0,Enum_DeviceInfoTable_CRC_Col,new QTableWidgetItem(info->sCRC));

    setAlignment(m_table, Qt::AlignHCenter|Qt::AlignVCenter);
    connect(m_table,SIGNAL(itemChanged(QTableWidgetItem *)),this,SLOT(itemChangedSlot(QTableWidgetItem *)));
}

void CDeviceInfoWnd::Clear()
{
    m_table->clearContents();
}

void CDeviceInfoWnd::itemChangedSlot(QTableWidgetItem *item)
{
    m_data->SetModified(true);

    switch (item->column())
    {
    case Enum_DeviceInfoTable_Type_Col:
        m_data->GetConfig()->deviceInfo.sType = item->text();
        break;
    case Enum_DeviceInfoTable_Desc_Col:
        m_data->GetConfig()->deviceInfo.sDesc = item->text();
        break;
    case Enum_DeviceInfoTable_Version_Col:
        m_data->GetConfig()->deviceInfo.sVersion = item->text();
        break;
    case Enum_DeviceInfoTable_Date_Col:
        m_data->GetConfig()->deviceInfo.sDate = item->text();
        break;
    case Enum_DeviceInfoTable_CRC_Col:
        m_data->GetConfig()->deviceInfo.sCRC = item->text();
        break;
    default:
        break;
    }
}
