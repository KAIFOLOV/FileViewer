#ifndef ISCANSTRATEGY_H
#define ISCANSTRATEGY_H

#include "global.h"

class IScanStrategy {

public:
    virtual ~IScanStrategy() = default;
    virtual QVector<FileInfo> scan(const QString &path) = 0;
};

#endif // ISCANSTRATEGY_H
