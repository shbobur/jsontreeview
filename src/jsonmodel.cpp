#include "jsonmodel.h"

#include <QDebug>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

const QHash<int, QByteArray> JsonModel::_roles = {
    { Qt::DisplayRole, "display" },
    { ValueRole, "value" },
    { KeyRole, "key" }
};

JsonModel::JsonModel(QObject *parent)
    : TreeModel{parent}
{

}

QHash<int, QByteArray> JsonModel::roleNames() const
{
    return _roles;
}

void JsonModel::loadJson(const QString &path)
{
    if (!QFile::exists(path)) {
        qCritical() << "error: File doesn't exists. Please check the file path:" << path;
        return;
    }

    QFile jsonFile {path};

    if (!jsonFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qCritical() << "error: cannot open JSON file, please check if the file formated correctly";
        return;
    }

    clear();

    QJsonParseError error;
    auto doc = QJsonDocument::fromJson(jsonFile.readAll(), &error);
    qDebug() << "Loading JSON file:" << error.errorString();

    auto root = doc.isArray() ? QJsonValue(doc.array()) : QJsonValue(doc.object());
    
    // Load directly into root item
    if (root.isObject()) {
        auto obj = root.toObject();
        for (auto it = obj.begin(); it != obj.end(); ++it) {
            const auto key = it.key();
            const auto value = it.value();
            
            auto child = new TreeItem();
            child->setData(key, KeyRole);
            child->setData(key, Qt::DisplayRole);
            
            if (value.isArray() || value.isObject()) {
                loadValue(value, child);
            } else {
                QString displayValue = value.toVariant().toString();
                child->setData(value.toVariant(), ValueRole);
                child->setData(key + ": " + displayValue, Qt::DisplayRole);
            }
            addTopLevelItem(child);
        }
    } else if (root.isArray()) {
        loadValue(root, rootItem());
    }
}


int JsonModel::role(const QString& roleName) const
{
    return _roles.key(roleName.toLatin1());
}

bool JsonModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    if (!index.isValid())
        return false;

    auto item = static_cast<TreeItem*>(index.internalPointer());
    if (!item)
        return false;

    QString strValue = value.toString();

    // Convert string to appropriate type
    QVariant newValue = value;
    if (strValue == "null") {
        newValue = QVariant();
    } else if (strValue == "true" || strValue == "false") {
        newValue = (strValue == "true");
    } else if (strValue.contains('.')) {
        bool ok;
        double d = strValue.toDouble(&ok);
        if (ok) newValue = d;
        else newValue = strValue;
    } else {
        bool ok;
        int i = strValue.toInt(&ok);
        if (ok) newValue = i;
        else newValue = strValue;
    }

    item->setData(newValue, ValueRole);

    // Update display text
    QString key = item->data(KeyRole).toString();
    QString displayText = key.isEmpty() ? strValue : key + ": " + strValue;
    item->setData(displayText, Qt::DisplayRole);

    emit dataChanged(index, index, {ValueRole, Qt::DisplayRole});
    return true;
}

Qt::ItemFlags JsonModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags flags = TreeModel::flags(index);
    flags |= Qt::ItemFlag::ItemIsEditable;
    return flags;
}

void JsonModel::loadValue(const QJsonValue &value, TreeItem *parent)
{
    if (value.isObject()) {
        auto object = value.toObject();
        for (auto it = object.begin(); it != object.end(); ++it) {
            const auto key = it.key();
            const auto value = it.value();

            auto child = new TreeItem();
            child->setData(key, KeyRole);
            child->setData(key, Qt::DisplayRole);

            if (value.isArray() || value.isObject()) {
                loadValue(value, child);
            } else {
                QString displayValue = value.toVariant().toString();
                child->setData(value.toVariant(), ValueRole);
                child->setData(key + ": " + displayValue, Qt::DisplayRole);
            }
            addItem(parent, child);
        }
    } else if (value.isArray()) {
        int index = 0;
        auto array = value.toArray();
        for (auto&& element : array) {
            auto child = new TreeItem();
            QString key = "[" + QString::number(index) + "]";
            child->setData(key, KeyRole);
            child->setData(key, Qt::DisplayRole);
            
            if (element.isArray() || element.isObject()) {
                loadValue(element, child);
            } else {
                QString displayValue = element.toVariant().toString();
                child->setData(element.toVariant(), ValueRole);
                child->setData(key + ": " + displayValue, Qt::DisplayRole);
            }
            addItem(parent, child);
            ++index;
        }
    } else {
        auto child = new TreeItem();
        QString displayValue = value.toVariant().toString();
        child->setData(displayValue, ValueRole);
        child->setData(displayValue, Qt::DisplayRole);
        addItem(parent, child);
    }
}
