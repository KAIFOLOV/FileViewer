#ifndef FILE_TYPE_SCAN_STRATEGY_H
#define FILE_TYPE_SCAN_STRATEGY_H

#include "IScanStrategy.h"

#include <QVector>
#include <QDir>
#include <QFileInfoList>

class FileTypeScanStrategy : public IScanStrategy {

public:
    explicit FileTypeScanStrategy() = default;

    QVector<FileInfo> scan(const QString &path) override;
};

#endif // FILE_TYPE_SCAN_STRATEGY_H
