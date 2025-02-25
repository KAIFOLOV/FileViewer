#include "file_type_scan_strategy.h"

QVector<FileInfo> FileTypeScanStrategy::scan(const QString &path) {
    QVector<FileInfo> result;
    QDir dir(path);
    QFileInfoList entries = dir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot);

    qint64 totalSize = 0;
    QMap<QString, qint64> sizeMap;

    for (const QFileInfo &entry : entries) {
        QString extension = entry.suffix().isEmpty() ? "Без расширения" : entry.suffix();
        sizeMap[extension] += entry.size();
        totalSize += entry.size();
    }

    for (auto it = sizeMap.begin(); it != sizeMap.end(); ++it) {
        double percentage = totalSize > 0 ? (it.value() * 100.0 / totalSize) : 0;
        result.append({it.key(), percentage});
    }
    return result;
}
