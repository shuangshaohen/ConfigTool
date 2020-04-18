#include "CDeviceParasWnd.h"
#include "CDataBase.h"
#include "CDelegate.h"
#include <QHeaderView>
#include <QVBoxLayout>

CDeviceParasWnd::CDeviceParasWnd(QWidget *parent) : CDefaultWnd(parent)
{
    m_table = new QTableWidget(this);

    m_table->setColumnCount(6);
    QStringList headerList;
    headerList << "模拟量数目" << "BI数目" << "SV数目" << "GS数目" << "BO数目" << "采样频率";
    m_table->setHorizontalHeaderLabels(headerList);
    //m_table->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    m_table->horizontalHeader()->setStretchLastSection(true);

    QVBoxLayout * layout = new QVBoxLayout (this);
    layout->addWidget(m_table);
    setLayout(layout);

    m_table->setItemDelegate(new CSpinBoxDelegate(this));
    m_data = NULL;
}

CDeviceParasWnd::~CDeviceParasWnd()
{
    delete m_table;
}

void CDeviceParasWnd::showInfo(void *pData)
{
    m_table->clearContents();
    if(NULL == pData)
    {
        m_data = NULL;
        return;
    }

    m_table->disconnect(SIGNAL(itemChanged(QTableWidgetItem *)));
    m_data = (CDataBase *)pData;
    DeviceParas * paras = &m_data->GetConfig()->deviceParas;

    m_table->setRowCount(1);

    m_table->setItem(0,Enum_DeviceParasTable_AnaNum_Col,new QTableWidgetItem(QString::number(paras->wMaxAnaNum)));
    m_table->setItem(0,Enum_DeviceParasTable_BiNum_Col,new QTableWidgetItem(QString::number(paras->wMaxBiNum)));
    m_table->setItem(0,Enum_DeviceParasTable_SvNum_Col,new QTableWidgetItem(QString::number(paras->wMaxSvNum)));
    m_table->setItem(0,Enum_DeviceParasTable_GsNum_Col,new QTableWidgetItem(QString::number(paras->wMaxGooseNum)));
    m_table->setItem(0,Enum_DeviceParasTable_BoNum_Col,new QTableWidgetItem(QString::number(paras->wMaxBoNum)));
    m_table->setItem(0,Enum_DeviceParasTable_SmpRate_Col,new QTableWidgetItem(QString::number(paras->wSmpRate)));

    setAlignment(m_table, Qt::AlignHCenter|Qt::AlignVCenter);

    connect(m_table,SIGNAL(itemChanged(QTableWidgetItem *)),this,SLOT(itemChangedSlot(QTableWidgetItem *)));
}

void CDeviceParasWnd::Clear()
{
    m_table->clearContents();
}

void CDeviceParasWnd::itemChangedSlot(QTableWidgetItem *item)
{
    setModified(true);

    switch (item->column())
    {
    case Enum_DeviceParasTable_AnaNum_Col:
        m_data->GetConfig()->deviceParas.wMaxAnaNum = item->text().toUInt();
        break;
    case Enum_DeviceParasTable_BiNum_Col:
        m_data->GetConfig()->deviceParas.wMaxBiNum = item->text().toUInt();
        break;
    case Enum_DeviceParasTable_SvNum_Col:
        m_data->GetConfig()->deviceParas.wMaxSvNum = item->text().toUInt();
        break;
    case Enum_DeviceParasTable_GsNum_Col:
        m_data->GetConfig()->deviceParas.wMaxGooseNum = item->text().toUInt();
        break;
    case Enum_DeviceParasTable_BoNum_Col:
        m_data->GetConfig()->deviceParas.wMaxBoNum = item->text().toUInt();
        break;
    case Enum_DeviceParasTable_SmpRate_Col:
        m_data->GetConfig()->deviceParas.wSmpRate = item->text().toUInt();
        break;
    default:
        break;
    }
}
