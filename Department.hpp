#pragma once

#include <QString>
#include <QVector>
#include <QDateTime>

class User;
class Event;

class Department
{
private:
    int dept_id;
    QString name;
    QVector<User*> employees;
    QVector<Event*> events;

public:
    // Конструктор
    Department(int id, const QString& dept_name);
    
    // Основные методы
    void addEmployee(User* user);
    void addEvent(Event* event);
    void removeEvent(Event* event);
    
    // Метод get_events с фильтрацией по дате
    QVector<Event*> getEvents(const QDateTime& date = QDateTime()) const;
    
    // Геттеры
    int getId() const;
    QString getName() const;
    QVector<User*> getEmployees() const;
    QVector<Event*> getAllEvents() const;
    
    // Вспомогательные методы
    int getEmployeeCount() const;
    int getEventCount() const;
    bool hasEmployee(int user_id) const;
};
    