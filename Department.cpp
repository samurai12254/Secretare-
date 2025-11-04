#include "Department.hpp"
#include "User.hpp"
#include "event.h"

// Конструктор
Department::Department(int id, const QString& dept_name)
    : dept_id(id), name(dept_name) {}

// Добавление сотрудника
void Department::addEmployee(User* user) {
    if (!user) return;
    
    // Проверяем, не добавлен ли уже этот сотрудник
    if (!hasEmployee(user->GetId())) {
        employees.append(user);
    }
}

// Добавление события
void Department::addEvent(Event* event) {
    if (event) {
        events.append(event);
    }
}

// Удаление события
void Department::removeEvent(Event* event) {
    if (event) {
        int index = events.indexOf(event);
        if (index != -1) {
            events.removeAt(index);
        }
    }
}

// Получение событий с фильтрацией по дате
QVector<Event*> Department::getEvents(const QDateTime& date) const {
    if (!date.isValid()) {
        // Если дата не указана, возвращаем все события
        return events;
    } else {
        // Фильтруем события по дате (сравниваем только дату, без времени)
        QVector<Event*> filtered_events;
        for (Event* event : events) {
            if (event && event->getStartTime().date() == date.date()) {
                filtered_events.append(event);
            }
        }
        return filtered_events;
    }
}

// Геттеры
int Department::getId() const {
    return dept_id;
}

QString Department::getName() const {
    return name;
}

QVector<User*> Department::getEmployees() const {
    return employees;
}

QVector<Event*> Department::getAllEvents() const {
    return events;
}

// Вспомогательные методы
int Department::getEmployeeCount() const {
    return employees.size();
}

int Department::getEventCount() const {
    return events.size();
}

bool Department::hasEmployee(int user_id) const {
    for (User* employee : employees) {
        if (employee && employee->GetId() == user_id) {
            return true;
        }
    }
    return false;
}
