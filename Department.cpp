#include "Department.hpp"
#include <algorithm>
#include <iostream>

// Предполагаем, что у Event есть метод get_date()
// Временная заглушка для компиляции
class Event {
public:
    int get_date() const { return 0; }
};

// Конструктор
Department::Department(int id, const std::string& dept_name)
    : dept_id(id), name(dept_name) {}

// Добавление сотрудника
void Department::AddEmployee(User* user) {
    // Проверяем, не добавлен ли уже этот сотрудник
    if (!HasEmployee(user->GetId())) {
        employees.push_back(user);
    }
}

// Добавление события
void Department::AddEvent(Event* event) {
    events.push_back(event);
}

// Удаление события
void Department::RemoveEvent(Event* event) {
    auto it = std::find(events.begin(), events.end(), event);
    if (it != events.end()) {
        events.erase(it);
    }
}

// Получение событий с фильтрацией по дате
std::vector<Event*> Department::GetEvents(const int date_start, const int date_end) const {
    if (date_start == -1) {
        // Если дата не указана, возвращаем все события
        return events;
    } else {
        // Фильтруем события по дате
        std::vector<Event*> filtered_events;
        for (const auto& event : events) {
            // Предполагаем, что у Event есть метод get_date()
            if (event->get_date() >=  date_start && event->get_date() <= date_end) {
                filtered_events.push_back(event);
            }
        }
        return filtered_events;
    }
}

// Геттеры
int Department::GetId() const {
    return dept_id;
}

std::string Department::GetName() const {
    return name;
}

std::vector<User*> Department::GetEmployees() const {
    return employees;
}

// Вспомогательные методы
size_t Department::GetEmployeeCount() const {
    return employees.size();
}

size_t Department::GetEventCount() const {
    return events.size();
}

bool Department::HasEmployee(int user_id) const {
    for (const auto& employee : employees) {
        if (employee->GetId() == user_id) {
            return true;
        }
    }
    return false;
}