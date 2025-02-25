#ifndef FILE_SIZE_MODEL_H
#define FILE_SIZE_MODEL_H

#include "global.h"

#include <QAbstractTableModel >
#include <QObject>

class FileSizeModel : public QAbstractTableModel {
    Q_OBJECT

public:
    explicit FileSizeModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &) const override;
    int columnCount(const QModelIndex &) const override;

    QVariant data(const QModelIndex &index, int role) const override;

    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

    QVector<FileInfo> dataStorage() const;
    void setData(const QVector<FileInfo> &newData);

private:
    QVector<FileInfo> _dataStorage;
};

#endif // FILE_SIZE_MODEL_H
