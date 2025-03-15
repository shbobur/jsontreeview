#include "treemodel.h"

TreeModel::TreeModel(QObject* parent)
    : QAbstractItemModel(parent)
    , _rootItem{new TreeItem()}
{}

TreeModel::~TreeModel()
{
    delete _rootItem;
}

int TreeModel::rowCount(const QModelIndex& parent) const
{
    if (!parent.isValid()) {
        return _rootItem->childCount();
    }

    return internalPointer(parent)->childCount();
}

int TreeModel::columnCount(const QModelIndex& /*parent*/) const
{
    // This is basically flatten as a list model
    return 1;
}

QModelIndex TreeModel::index(const int row, const int column, const QModelIndex& parent) const
{
    if (!hasIndex(row, column, parent)) {
        return {};
    }

    TreeItem* parentItem;
    if (!parent.isValid()) {
        parentItem = _rootItem;
    } else {
        parentItem = static_cast<TreeItem*>(parent.internalPointer());
    }

    TreeItem* childItem = parentItem->child(row);
    if (childItem) {
        return createIndex(row, column, childItem);
    }

    return {};
}

QModelIndex TreeModel::parent(const QModelIndex& index) const
{
    if (!index.isValid()) {
        return {};
    }

    TreeItem* childItem = static_cast<TreeItem*>(index.internalPointer());
    TreeItem* parentItem = childItem->parentItem();

    if (parentItem == _rootItem || !parentItem) {
        return {};
    }

    return createIndex(parentItem->row(), 0, parentItem);
}

QVariant TreeModel::data(const QModelIndex& index, const int role) const
{
    if (!index.isValid()) {
        return {};
    }

    return internalPointer(index)->data(role);
}

bool TreeModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    if (!index.isValid()) {
        return false;
    }

    auto item = internalPointer(index);
    if (item && (role != Qt::DisplayRole)) {
        item->setData(value, role);
        emit dataChanged(index, index, {role});
    }

    return false;
}

void TreeModel::addTopLevelItem(TreeItem* child)
{
    if (child) {
        addItem(_rootItem, child);
    }
}

void TreeModel::addItem(TreeItem* parent, TreeItem* child)
{
    if (!child || !parent) {
        return;
    }

    emit layoutAboutToBeChanged();

    if (child->parentItem()) {
        beginRemoveRows(QModelIndex(), qMax(parent->childCount() - 1, 0), qMax(parent->childCount(), 0));
        child->parentItem()->removeChild(child);
        endRemoveRows();
    }

    beginInsertRows(QModelIndex(), qMax(parent->childCount() - 1, 0), qMax(parent->childCount() - 1, 0));
    child->setParentItem(parent);
    parent->appendChild(child);
    endInsertRows();

    emit layoutChanged();
}

void TreeModel::removeItem(TreeItem* item)
{
    if (!item) {
        return;
    }

    emit layoutAboutToBeChanged();

    if (item->parentItem()) {
        beginRemoveRows(QModelIndex(), qMax(item->childCount() - 1, 0), qMax(item->childCount(), 0));
        item->parentItem()->removeChild(item);
        endRemoveRows();
    }

    emit layoutChanged();
}

TreeItem* TreeModel::rootItem() const
{
    return _rootItem;
}

QModelIndex TreeModel::rootIndex()
{
    return {};
}

int TreeModel::depth(const QModelIndex& index) const
{
    int count = 0;
    auto anchestor = index;
    if (!index.isValid()) {
        return 0;
    }
    while (anchestor.parent().isValid()) {
        anchestor = anchestor.parent();
        ++count;
    }

    return count;
}

void TreeModel::clear()
{
    emit layoutAboutToBeChanged();
    beginResetModel();
    delete _rootItem;
    _rootItem = new TreeItem();
    endResetModel();
    emit layoutChanged();
}

TreeItem* TreeModel::internalPointer(const QModelIndex& index) const
{
    return static_cast<TreeItem*>(index.internalPointer());
}

QHash<int, QByteArray> TreeModel::roleNames() const 
{
    QHash<int, QByteArray> roles;
    roles[Qt::DisplayRole] = "display";
    return roles;
}
