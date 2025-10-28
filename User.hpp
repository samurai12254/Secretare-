#pragma once

#include <string>
#include <unordered_map>

// Предварительное объявление класса Event
class Event;

class User {
private:
    int user_id;
    std::string login;
    std::string password;
    std::string role;
    std::string department;

public:
    // Конструктор
    User(int id, const std::string& l, const std::string& p, 
         const std::string& r, const std::string& d);

    // Проверка пароля
    bool check_password(const std::string& input_password) const;

    // Проверка возможности редактирования события
    bool can_edit(const Event& event) const;

    // Сериализация в словарь
    std::unordered_map<std::string, std::string> to_dict() const;

    // Геттеры
    int get_id() const;
    std::string get_login() const;
    std::string get_role() const;
    std::string get_department() const;
    
};

