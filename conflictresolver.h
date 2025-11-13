#ifndef CONFLICTRESOLVER_H
#define CONFLICTRESOLVER_H

#pragma once
#include <QVector>

class ConflictResolver {
public:
    int has_conflict = 0;
    QVector<QString> busyUser,busyLocation;
};

#endif // CONFLICTRESOLVER_H
