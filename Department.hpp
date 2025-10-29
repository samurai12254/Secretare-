#pragma once


#include <string>
#include <vector>
#include <memory>
#include "User.hpp"

// Предварительное объявление класса Event
class Event;

class Department {
private:
    int dept_id;
    std::string name;
    std::vector<User*> employees;
    std::vector<Event*> events;

public:
    // Конструктор
    Department(int id, const std::string& dept_name);
    
    // Основные методы
    void AddEmployee(User* user);
    void AddEvent(Event* event);
    void RemoveEvent(Event* event);
    
    // Метод get_events с фильтрацией по дате
    std::vector<Event*> GetEvents(const int date_start = -1, const int date_end = -1) const; //пока ХЗ как храним дату, потом поредачу
    
    // Геттеры
    int GetId() const;
    std::string GetName() const;
    std::vector<User*> GetEmployees() const;
    
    
    // Вспомогательные методы
    size_t GetEmployeeCount() const;
    size_t GetEventCount() const;
    bool HasEmployee(int user_id) const;
};

