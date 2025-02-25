#ifndef FOLDER_SCAN_STRATEGY_H
#define FOLDER_SCAN_STRATEGY_H

#include "IScanStrategy.h"

#include <QDir>
#include <QVector>

class FolderScanStrategy : public IScanStrategy {
public:
    explicit FolderScanStrategy() = default;

    QVector<FileInfo> scan(const QString &path) override;

private:
    double calculateFolderSize(const QString &folderPath);
};

#endif // FOLDER_SCAN_STRATEGY_H
