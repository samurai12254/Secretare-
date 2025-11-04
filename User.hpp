#pragma once

#include <string>
#include <unordered_map>
#include <QString>


// Предварительное объявление класса Event
class Event;

class User {
private:
    QString user_id;
    std::string login;
    std::string password;
    std::string role;
    std::string department;

public:
    // Конструктор
    User(int id, const std::string& l, const std::string& p, 
         const std::string& r, const std::string& d);

    // Проверка пароля
    bool CheckPassword(const std::string& input_password) const;

    // Проверка возможности редактирования события
    bool CanEdit(const Event& event) const;

    // Сериализация в словарь
    std::unordered_map<std::string, std::string> ToDict() const;

    // Геттеры
    QString GetId() const;
    std::string GetLogin() const;
    std::string GetRole() const;
    std::string GetDepartment() const;
    
};

