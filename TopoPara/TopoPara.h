#ifndef TOPOPARA_H
#define TOPOPARA_H

#include "TopoPara_global.h"
#include "ParaStruct.h"
#include "ParaModel.h"
#include <QTreeView>
#include <QTableView>

class TOPOPARA_EXPORT TopoPara:public QObject
{
        Q_OBJECT

public:
    ~TopoPara();
    //获取单例对象;
    static TopoPara* GetInstance();

    void SavePara();
    void NewPara();
    void OpenPara();
    void ClearPara();

    void SaveConfig();
    void SaveICD();

    void setTreeView(QTreeView *TreeView);

    void setTableView(QTableView *TableView);

public slots:
    void currentRowChangedSlot(const QModelIndex &current, const QModelIndex &previous);

private:
    TopoPara();
    //实际单例对象;
    static TopoPara* s_instance;

    tdsTopoPara         m_TopoPara;
    ParaTreeModel   *   m_TreeModel;
    ParaTableModel  *   m_TableModel;

    QTreeView       *   m_TreeView;
    QTableView      *   m_TableView;
};

#endif // TOPOPARA_H
