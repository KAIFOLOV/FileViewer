#include "folder_scan_strategy.h"

QVector<FileInfo> FolderScanStrategy::scan(const QString &path) {
    QVector<FileInfo> result;
    QDir dir(path);
    QFileInfoList entries = dir.entryInfoList(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot);

    double totalSize = 0;
    QMap<QString, double> sizeMap;

    for (const QFileInfo  &entry : entries) {
        double size = entry.isDir() ? calculateFolderSize(entry.filePath()) : entry.size();
        totalSize += size;
        sizeMap[entry.fileName()] = size;
    }

    for (auto it = sizeMap.begin(); it != sizeMap.end(); ++it) {
        double percentage = totalSize > 0 ? (it.value() * 100.0 / totalSize) : 0;
        result.append({it.key(), percentage});
    }
    return result;
}

double FolderScanStrategy::calculateFolderSize(const QString &folderPath) {
    qint64 size = 0;
    QDir dir(folderPath);
    QFileInfoList entries = dir.entryInfoList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot);
    for (const QFileInfo &entry : entries) {
        size += entry.isDir() ? calculateFolderSize(entry.filePath()) : entry.size();
    }
    return size;
}
