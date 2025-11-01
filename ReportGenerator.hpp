#pragma once

#include <QString>
#include <QHash>
#include <QVector>
#include <QDateTime>
#include "calendar.h"
#include "mailsystem.h"

class ReportGenerator
{
private:
    Calendar* calendar;
    MailSystem* mail_system;
    QHash<QString, QString> stats;
    
    // Вспомогательные методы
    void collectEventStatistics();
    void collectDepartmentStatistics();
    void collectMailStatistics();
    void collectConflictStatistics();
    QString formatDuration(const QDateTime& start, const QDateTime& end) const;

public:
    // Конструкторы
    ReportGenerator(Calendar* cal);
    ReportGenerator(Calendar* cal, MailSystem* mail);
    
    // Основные методы
    void generateSummary();
    QString toText() const;
    bool exportToFile(const QString& filename, const QString& format = "txt") const;
    
    // Дополнительные методы отчетов
    QString generateDepartmentReport() const;
    QString generateUserActivityReport() const;
    QString generateEventDistributionReport() const;
    
    // Геттеры
    QHash<QString, QString> getStats() const;
    void setCalendar(Calendar* cal);
    void setMailSystem(MailSystem* mail);
};
