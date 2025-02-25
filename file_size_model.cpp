#include "file_size_model.h"

FileSizeModel::FileSizeModel(QObject *parent) : QAbstractTableModel(parent) {}

void FileSizeModel::setData(const QVector<FileInfo> &newData) {
    beginResetModel();
    _dataStorage = newData;
    endResetModel();
}

int FileSizeModel::rowCount(const QModelIndex &) const { return _dataStorage.size(); }

int FileSizeModel::columnCount(const QModelIndex &) const { return 2; }

QVariant FileSizeModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || index.row() >= _dataStorage.size()) return QVariant();

    if (role == Qt::DisplayRole) {
        if (index.column() == 0) return _dataStorage[index.row()].name;
        if (index.column() == 1) return QString::number(_dataStorage[index.row()].percentage, 'f', 2) + "%";
    }
    return QVariant();
}

QVariant FileSizeModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        return (section == 0) ? "Название" : "Размер (%)";
    }
    return QVariant();
}

QVector<FileInfo> FileSizeModel::dataStorage() const
{
    return _dataStorage;
}
