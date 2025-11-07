#ifndef EVENT_H
#define EVENT_H

#pragma once
#include <QString>
#include <QDateTime>
#include <QVector>
#include "User.hpp"
#include "Department.hpp"

/**
 * @brief Класс Event представляет одно календарное событие фирмы.
 *
 * Содержит информацию о времени, месте, участниках и важности события.
 * Поддерживает проверку конфликтов и операции над списком участников.
 */
class Event {
private:
    QString id;                      ///< Уникальный идентификатор события
    QString title;                   ///< Название события
    Department* location;            ///< Место проведения (указатель на объект Department)
    QDateTime startTime;             ///< Время начала
    QDateTime endTime;               ///< Время окончания
    QVector<User*> participants;     ///< Участники события
    QString importance;              ///< Важность ("низкая", "средняя", "высокая")
    QString description;             ///< Дополнительная информация
    bool isInternal;                 ///< Внутреннее ли мероприятие

public:
    // --- Конструкторы ---
    Event();
    Event(QString title,
          Department* location,
          QDateTime startTime,
          QDateTime endTime,
          QString& participantsString,   // "ivan, petr, anna"
          QVector<User*>& allUsers,      // список всех пользователей системы
          QString importance,
          QString description = "",
          bool isInternal = true);
    Event(QString title,
          Department* location,
          QDateTime startTime,
          QDateTime endTime,
          QVector<User*> participants,
          QString importance,
          QString description = "",
          bool isInternal = true);
    Event(bool z);
    // --- Геттеры ---
    QString getId() const;
    QString getTitle() const;
    Department* getLocation() const;
    QDateTime getStartTime() const;
    QDateTime getEndTime() const;
    QVector<User*> getParticipants() const;
    QString getImportance() const;
    QString getDescription() const;
    bool getIsInternal() const;

    // --- Сеттеры ---
    void setTitle(const QString& newTitle);
    void setLocation(Department* newLoc);
    void setStartTime(const QDateTime& newStart);
    void setEndTime(const QDateTime& newEnd);
    void setParticipants(const QVector<User*>& newParts);
    void setImportance(const QString& newImportance);
    void setDescription(const QString& newDesc);
    void setIsInternal(bool newValue);

    // --- Управление участниками ---
    bool addParticipant(User* user);
    bool removeParticipant(User* user);
    void setParticipantsFromString(const QString& participantsString, const QVector<User*>& allUsers);

    // --- Проверка конфликтов ---
    bool conflictsWith(const Event& other) const;

    // --- Вспомогательные методы ---
    int durationMinutes() const;
    QString formattedStart() const;
    QString formattedEnd() const;
    QString summary() const;
};


#endif // EVENT_H
