#include "User.hpp"

// Конструктор
User::User(QString id, const QString& login_, const QString& password_, 
           const QString& role_, const QString& department_)
    : user_id(id), login(login_), password(password_), role(role), department(department_) {}

// Проверка пароля
bool User::CheckPassword(const QString& input_password) const {
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
QHash<QString, QString> User::ToDict() const {
    return {
        {"user_id", user_id},
        {"login", login},
        {"password", password},
        {"role", role},
        {"department", department}
    };
}

// Геттеры
QString User::GetId() const { 
    return user_id; 
}

QString User::GetLogin() const { 
    return login; 
}

QString User::GetRole() const { 
    return role; 
}

QString User::GetDepartment() const { 
    return department; 
}



