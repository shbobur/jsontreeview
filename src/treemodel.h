#ifndef QML_TREEVIEW_TREE_MODEL_H
#define QML_TREEVIEW_TREE_MODEL_H

#include "treeitem.h"

#include <QAbstractItemModel>

/*!
 * The Tree Model works as List Model, using one column and using the row information
 * referred to the parent node.
 */
class TreeModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit TreeModel(QObject* parent = nullptr);
    ~TreeModel() override;

public:
    int rowCount(const QModelIndex& index) const override;
    int columnCount(const QModelIndex& index) const override;

    QModelIndex index(int row, int column, const QModelIndex& parent) const override;
    QModelIndex parent(const QModelIndex& childIndex) const override;

    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override;

    QHash<int, QByteArray> roleNames() const override;

public:
    //! Add an item to the top level.
    void addTopLevelItem(TreeItem* child);

    //! Add a child to the parent item.
    void addItem(TreeItem* parent, TreeItem* child);

    //! Remove the item from the model.
    void removeItem(TreeItem* item);

    //! Return the root item of the model.
    TreeItem* rootItem() const;

    //! Return the depth for the given index
    Q_INVOKABLE int depth(const QModelIndex& index) const;

    //! Clear the model.
    Q_INVOKABLE void clear();

    /*!
    *  Return the root item to the QML Side.
    *  This method is not meant to be used in client code.
    */
    Q_INVOKABLE QModelIndex rootIndex();

private:
    TreeItem* internalPointer(const QModelIndex& index) const;

private:
    TreeItem* _rootItem;
};

#endif // QML_TREEVIEW_TREE_MODEL_H
