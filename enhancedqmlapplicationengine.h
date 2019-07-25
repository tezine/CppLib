#ifndef ENHANCEDQMLAPPLICATIONENGINE_H
#define ENHANCEDQMLAPPLICATIONENGINE_H

#pragma once
#include "cpplib_global.h"
#include <QQmlApplicationEngine>

class CPPLIB_EXPORT EnhancedQmlApplicationEngine : public QQmlApplicationEngine{
    Q_OBJECT

public:
    explicit EnhancedQmlApplicationEngine(QObject *parent = nullptr);
    Q_INVOKABLE void clearCache();

private slots:
    void slotWarnings(const QList<QQmlError> &warnings);
    void sloObjectCreated(QObject *object, const QUrl &url);
};
#endif
