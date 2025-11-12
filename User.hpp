#pragma once

#include <QString>
#include <QHash>

// Предварительное объявление класса Event
class Event;

class User {
private:
    QString user_id;
    QString login;
    QString password;
    QString role;
    QString department;

public:
    // Конструктор
    User(QString id, const QString l, const QString p, 
         const QString r, const QString d);

    // Проверка пароля
    bool CheckPassword(const QString& input_password) const;

    // Проверка возможности редактирования события
    bool CanEdit(const Event& event) const;

    // Сериализация в словарь
    QHash<QString, QString> ToDict() const;

    // Геттеры
    QString GetId() const;
    QString GetLogin() const;
    QString GetRole() const;
    QString GetDepartment() const;
    QString GetPassword() const;
    
};

