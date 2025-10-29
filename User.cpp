#include "User.hpp"

// Конструктор
User::User(int id, const std::string& login_, const std::string& password_, 
           const std::string& role_, const std::string& department_)
    : user_id(id), login(login_), password(password_), role(role), department(department_) {}

// Проверка пароля
bool User::CheckPassword(const std::string& input_password) const {
    return password == input_password;
}

// Проверка возможности редактирования события
bool User::CanEdit(const Event& event) const {
    // Сотрудник не может редактировать события
    if (role == "сотрудник") {
        return false;
    }
    
    // Секретарь и руководитель могут редактировать
    // В будущем можно добавить дополнительную логику проверки события
    return true;
}

// Сериализация в словарь
std::unordered_map<std::string, std::string> User::ToDict() const {
    return {
        {"user_id", std::to_string(user_id)},
        {"login", login},
        {"password", password},
        {"role", role},
        {"department", department}
    };
}

// Геттеры
int User::GetId() const { 
    return user_id; 
}

std::string User::GetLogin() const { 
    return login; 
}

std::string User::GetRole() const { 
    return role; 
}

std::string User::GetDepartment() const { 
    return department; 
}



