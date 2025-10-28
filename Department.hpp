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
    void add_employee(User* user);
    void add_event(Event* event);
    void remove_event(Event* event);
    
    // Метод get_events с фильтрацией по дате
    std::vector<Event*> get_events(const int& date = -1) const; //пока ХЗ как храним дату, потом поредачу
    
    // Геттеры
    int get_id() const;
    std::string get_name() const;
    std::vector<User*> get_employees() const;
    
    
    // Вспомогательные методы
    size_t get_employee_count() const;
    size_t get_event_count() const;
    bool has_employee(int user_id) const;
};

