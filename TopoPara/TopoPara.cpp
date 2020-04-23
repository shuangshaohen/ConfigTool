#include "TopoPara.h"
#include "CSavePara.h"
#include "InitalDialog.h"
#include "CBasePara.h"
#include "CParsePara.h"
#include <QFileDialog>

TopoPara* TopoPara::s_instance = NULL;

TopoPara::TopoPara()
{
    m_TreeModel = new ParaTreeModel();
    m_TableModel = new ParaTableModel();

    m_TableView = nullptr;
    m_TreeView = nullptr;

    QStringList header;
    header << "关键字" << "描述";
    m_TreeModel->setRootItem(&m_TopoPara,header);

    QStringList headerList;
    headerList << "关键词" << "描述" << "关联名";
    m_TableModel->setRootItem(m_TopoPara.ptPara,headerList);
}

void TopoPara::setTableView(QTableView *TableView)
{
    m_TableView = TableView;
    m_TableView->setModel(m_TableModel);
}

void TopoPara::setTreeView(QTreeView *TreeView)
{
    m_TreeView = TreeView;
    m_TreeView->setModel(m_TreeModel);
    connect(m_TreeView->selectionModel(),SIGNAL(currentRowChanged(QModelIndex, QModelIndex)),
            this,SLOT(currentRowChangedSlot(QModelIndex, QModelIndex)));
}

TopoPara::~TopoPara()
{
    delete m_TreeModel;
    delete m_TableModel;
}

TopoPara *TopoPara::GetInstance()
{
    if (s_instance == NULL)
    {
        s_instance = new TopoPara();
    }

    return s_instance;
}

void TopoPara::SavePara()
{
    CSavePara * pSave = new CSavePara();
    tdsTopoPara * ptTopoPara = &m_TopoPara;

    QString strProjectFilePathName;

    strProjectFilePathName = QFileDialog::getSaveFileName( nullptr, "Save...", "", "TopoPara (*.xml)" );

    if( strProjectFilePathName.length() <= 0 )
    {
        return;
    }

    if( !strProjectFilePathName.endsWith(".xml", Qt::CaseInsensitive) )
    {
        strProjectFilePathName.append(".xml");
    }

    pSave->Save(strProjectFilePathName,ptTopoPara);
    delete pSave;
}

void TopoPara::NewPara()
{
    InitalDialog * dialog = new InitalDialog();
    int res = dialog->exec();

    if(res == QDialog::Accepted)
    {
        CBasePara * m_pBasePara = new CBasePara();
        ClearPara();
        m_pBasePara->CreatePara(&m_TopoPara,
                                dialog->getOnlyDevice(),
                                dialog->getSourceNum(),
                                dialog->getBubarNum(),
                                dialog->getLineNum(),
                                dialog->getTransformerNum(),
                                dialog->getLoadNum(),
                                dialog->getRemoteNum(),
                                dialog->getSwitchNum());
        delete m_pBasePara;

//        QStringList header;
//        header << "关键字" << "描述";
//        m_TreeModel->setRootItem(&m_TopoPara,header);

        QStringList headerList;
        headerList << "关键词" << "描述" << "关联名";
        m_TableModel->setRootItem(m_TopoPara.ptPara,headerList);
        if(m_TreeView != NULL)
            m_TreeView->expand(m_TreeModel->index(0,0));
    }
    delete dialog;
}

void TopoPara::OpenPara()
{
    QString strProjectFilePathName;

    strProjectFilePathName = QFileDialog::getOpenFileName( nullptr, "Open...", "", "TopoPara (*.xml)" );

    if( strProjectFilePathName.length() <= 0 )
    {
        return;
    }

    ClearPara();
    CParsePara * parse = new CParsePara();
    parse->ParsePara(strProjectFilePathName.toStdString(),&m_TopoPara);
    delete parse;


    QStringList headerList;
    headerList << "关键词" << "描述" << "关联名";
    m_TableModel->setRootItem(m_TopoPara.ptPara,headerList);

    if(m_TreeView != NULL)
        m_TreeView->expand(m_TreeModel->index(0,0));
}

void TopoPara::ClearPara()
{
    m_TopoPara.ptPara->items.clear();
    m_TopoPara.ptPara->sons.clear();

    m_TableModel->clear();
}


void TopoPara::currentRowChangedSlot(const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(previous);
    tdsPara * para = m_TreeModel->itemFromIndex(current);
    QStringList headerList;
    headerList << "关键词" << "描述" << "关联名";
    m_TableModel->setRootItem(para,headerList);
}
