#pragma once

#include <QVector>
#include <QString>
#include <QDateTime>
#include <QHash>
#include "event.h"
#include "Department.hpp"
#include "User.hpp"

class Calendar
{
private:
    QVector<Event*> events;
    QVector<Department*> departments;
    
    // Вспомогательные методы
    Event* findEventById(const QString& event_id) const;
    bool checkTimeConflict(const Event* event1, const Event* event2) const;

public:
    // Конструктор
    Calendar() = default;
    
    // Основные методы управления событиями
    void addEvent(Event* event);
    void updateEvent(const QString& event_id, const QHash<QString, QString>& updates);
    void removeEvent(const QString& event_id);
    
    // Методы получения событий
    QVector<Event*> getEventsForDay(const QDateTime& date) const;
    QVector<Event*> getEventsForUser(User* user) const;
    
    // Анализ событий
    QVector<QPair<Event*, Event*>> findConflicts() const;
    void removePastEvents(const QDateTime& current_time);
    
    // Управление отделами
    void addDepartment(Department* department);
    QVector<Department*> getDepartments() const;
    
    // Статистика
    int getTotalEvents() const;
    int getTotalDepartments() const;
};
