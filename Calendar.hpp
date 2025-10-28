#ifndef CALENDAR_HPP
#define CALENDAR_HPP

#include <vector>
#include <memory>
#include <string>
#include <unordered_map>
//#include "Event.hpp"
#include "Department.hpp"
#include "User.hpp"

class Calendar {
private:
    std::vector<Event*> events;
    std::vector<Department*> departments;
    
    // Вспомогательные методы
    Event* find_event_by_id(int event_id) const;
    bool check_time_conflict(const Event& event1, const Event& event2) const;

public:
    // Конструктор
    Calendar() = default;
    
    // Основные методы управления событиями
    void add_event(std::shared_ptr<Event> event);
    void update_event(int event_id, const std::unordered_map<std::string, std::string>& updates);
    void remove_event(int event_id);
    
    // Методы получения событий
    std::vector<Event*> get_events_for_day(const std::string& date) const;
    std::vector<Event*> get_events_for_user(User* user) const;
    
    // Анализ событий
    std::vector<std::pair<Event*, Event*>> find_conflicts() const;
    void remove_past_events(const std::string& current_date);
    
    // Управление отделами
    void add_department(Department* department);
    std::vector<Department*> get_departments() const;
    
    // Статистика
    size_t get_total_events() const;
    size_t get_total_departments() const;
};

#endif // CALENDAR_HPP