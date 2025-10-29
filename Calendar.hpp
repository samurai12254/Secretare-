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
    Event* FindEventById(int event_id) const;
    bool CheckTimeConflict(const Event& event1, const Event& event2) const;

public:
    // Конструктор
    Calendar() = default;
    
    // Основные методы управления событиями
    void AddEvent(Event* event);
    void UpdateEvent(int event_id, const std::unordered_map<std::string, std::string>& updates);
    void RemoveEvent(int event_id);
    
    // Методы получения событий
    std::vector<Event*> GetEventsForDay(const std::string& date) const;
    std::vector<Event*> GetEventsForUser(User* user) const;
    
    // Анализ событий
    std::vector<std::pair<Event*, Event*>> FindConflict() const;
    void RemovePastEvents(const std::string& current_date);
    
    // Статистика
    size_t GetTotalEvents() const;
};

#endif // CALENDAR_HPP