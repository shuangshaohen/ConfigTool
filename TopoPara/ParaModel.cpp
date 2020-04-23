#include "ParaModel.h"

ParaTreeModel::ParaTreeModel(QObject *parent) : QAbstractItemModel(parent)
{
    mRootItem = NULL;
}

ParaTreeModel::~ParaTreeModel()
{

}

void ParaTreeModel::setRootItem(tdsTopoPara *root, QStringList header)
{
    beginResetModel();
    mRootItem = root;
    mHeaders = header;
    endResetModel();
}

QVariant ParaTreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    Q_UNUSED(orientation);
    if (role != Qt::DisplayRole) {
        return QVariant();
    }

    if((orientation == Qt::Horizontal)
            &&(section < mHeaders.size()))
        return mHeaders[section];

    if(orientation == Qt::Vertical)
        return QString::number(section);

    return QVariant();
}

QVariant ParaTreeModel::data(const QModelIndex &index, int role) const
{
    // 节点内容：左对齐，显示行列号
    if (role == Qt::TextAlignmentRole)
    {
        return int(Qt::AlignLeft | Qt::AlignVCenter);
    }
    else if (role == Qt::DisplayRole)
    {
        if((!index.isValid())
                ||(index.internalPointer() == nullptr))
        {
            return QString("row=%1,col=%2 invalid").arg(index.row()).arg(index.column());
        }
        else
        {
            tdsPara* pNode = reinterpret_cast<tdsPara*>(index.internalPointer());
//            QString keyPlus;
//            if(numWithKeyFromParent(index) != 0)
//                keyPlus = QString::number(numWithKeyFromParent(index));
            if(index.column() == 0)
                return QString::fromStdString(pNode->key) /*+ keyPlus*/;
            else
                return QString::fromStdString(pNode->desc);
        }
    }
    else
    {
        return QVariant();
    }
}

QModelIndex ParaTreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if (parent.row() == -1 && parent.column() == -1)
    {
        // 首层节点绑定关系
        if ((itemRoot() != NULL)
                &&(row < 1))
        {
            tdsPara * p = itemRoot()->ptPara;

            return createIndex(row, column, p);
        }
    }
    else
    {
        // 其它层节点绑定关系
        if (parent.internalPointer() != nullptr)
        {
            tdsPara * par = itemFromIndex(parent);
            if (row < (int)par->sons.size())
            {
                tdsPara * p = par->sons[row];

                return createIndex(row, column, p);
            }
        }
    }
    return QModelIndex();
}

QModelIndex ParaTreeModel::parent(const QModelIndex &index) const
{
    if (index.internalPointer() != nullptr)
    {
        tdsPara * pNode = itemFromIndex(index);
        if (pNode->parent != NULL)
        {
            if(pNode->parent->parent == NULL)
            {
                if(itemRoot() == NULL)
                {
                    return QModelIndex();
                }

                return createIndex(0, 0, pNode->parent);
            }
            else
            {

                for(int i = 0; i < (int)pNode->parent->parent->sons.size(); i++)
                {
                    if(pNode->parent == pNode->parent->parent->sons[i])
                    {
                        return createIndex(i, 0, pNode->parent);
                    }
                }
            }
        }
    }
    return QModelIndex();
}

int ParaTreeModel::rowCount(const QModelIndex &parent) const
{
    if(mRootItem == NULL)
        return 0;

    if(parent.internalPointer() == NULL)
    {
        return 1;
    }
    else
    {
        return itemFromIndex(parent)->sons.size();
    }
}

int ParaTreeModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 2;
}

bool ParaTreeModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(QAbstractItemModel::setData(index,value,role) == false)
        return false;

    return true;
}

tdsPara *ParaTreeModel::itemFromIndex(const QModelIndex &index) const
{
    tdsPara* pNode = reinterpret_cast<tdsPara*>(index.internalPointer());
    return pNode;
}

tdsTopoPara *ParaTreeModel::itemRoot() const
{
    return mRootItem;
}

void ParaTreeModel::clear()
{
    mRootItem = nullptr;
}

int ParaTreeModel::numWithKeyFromParent(const QModelIndex &index) const
{
    tdsPara * pNode = itemFromIndex(index);

    if(pNode->parent == NULL)
        return 0;

    int num = 0;
    int sum = 0;
    for(int i = 0; i < (int)pNode->parent->sons.size(); i++)
    {
        if(pNode->parent->sons[i]->key == pNode->key)
        {
            sum++;
        }

        if(pNode->parent->sons[i] == pNode)
            num = sum;
    }

    if(sum  < 2)
        return 0;
    else
        return num;

}

ParaTableModel::ParaTableModel(QObject *parent): QAbstractItemModel(parent)
{
    mRootItem = NULL;
    mHeaders.clear();
}

ParaTableModel::~ParaTableModel()
{

}

QVariant ParaTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    Q_UNUSED(orientation);
    if (role != Qt::DisplayRole) {
        return QVariant();
    }

    if((orientation == Qt::Horizontal)
            &&(section < mHeaders.size()))
        return mHeaders[section];

    if(orientation == Qt::Vertical)
        return QString::number(section);

    return QVariant();
}

void ParaTableModel::setRootItem(tdsPara *root , QStringList header)
{
    beginResetModel();
    mRootItem = root;
    mHeaders = header;
    endResetModel();
}

QVariant ParaTableModel::data(const QModelIndex &index, int role) const
{
    // 节点内容：居中，显示行列号
    if (role == Qt::TextAlignmentRole)
    {
        return int(Qt::AlignLeft | Qt::AlignVCenter);
    }
    else if ((role == Qt::DisplayRole)
             ||(role == Qt::EditRole))
    {
        if((!index.isValid())
                ||(index.internalPointer() == nullptr))
        {
            return QString("row=%1,col=%2 invalid").arg(index.row()).arg(index.column());
        }
        else
        {
            tdsItem* pNode = reinterpret_cast<tdsItem*>(index.internalPointer());

            if(index.column() == 0)
                return QString::fromStdString(pNode->key);
            else if(index.column() == 1)
                return QString::fromStdString(pNode->desc);
            else
                return QString::fromStdString(pNode->name);
        }
    }
    else
    {
        return QVariant();
    }
}

Qt::ItemFlags ParaTableModel::flags(const QModelIndex &index) const
{
    return QAbstractItemModel::flags(index)|Qt::ItemIsEditable;
}

QModelIndex ParaTableModel::index(int row, int column, const QModelIndex &parent) const
{
    if (parent.row() == -1 && parent.column() == -1)
    {
        // 首层节点绑定关系
        if ((itemRoot() != NULL)
                &&(row < (int)itemRoot()->items.size()))
        {
            tdsItem * p = itemRoot()->items[row];

            return createIndex(row, column, p);
        }
    }

    return QModelIndex();
}

QModelIndex ParaTableModel::parent(const QModelIndex &index) const
{
    Q_UNUSED(index);
    return QModelIndex();
}

int ParaTableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    if(mRootItem == NULL)
            return 0;
    else
        return mRootItem->items.size();
}

int ParaTableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 3;
}

bool ParaTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role == Qt::EditRole)
    {
        if((!index.isValid())
                ||(index.internalPointer() == nullptr))
        {
            return false;
        }
        else
        {
            tdsItem* pNode = reinterpret_cast<tdsItem*>(index.internalPointer());

            QString str = value.toString();

            if(index.column() == 0)
                pNode->key = str.toStdString();
            else if(index.column() == 1)
                pNode->desc = str.toStdString();
            else
                pNode->name = str.toStdString();

            return true;
        }
    }
    else
        return QAbstractItemModel::setData(index,value,role);
}

tdsItem *ParaTableModel::itemFromIndex(const QModelIndex &index) const
{
    tdsItem* pNode = reinterpret_cast<tdsItem*>(index.internalPointer());
    return pNode;
}

tdsPara *ParaTableModel::itemRoot() const
{
    return mRootItem;
}

void ParaTableModel::clear()
{
    beginResetModel();
    mRootItem = nullptr;
    endResetModel();
}
