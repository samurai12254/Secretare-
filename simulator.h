#ifndef SIMULATOR_H
#define SIMULATOR_H

#pragma once
#include <QDateTime>
#include <QVector>
#include <QSet>
#include "Calendar.hpp"
#include "MailSystem.hpp"

class Simulator {
private:
    Calendar* calendar;
    MailSystem* mailSystem;
    QDateTime currentTime;
    QDateTime endTime;
    int stepMinutes;

    // --- Статистика симуляции ---
    int totalMessagesSent = 0;        ///< Общее количество отправленных сообщений
    int totalEventsProcessed = 0;     ///< Количество обработанных событий
    QSet<QString> uniqueParticipants; ///< Уникальные участники (по id)
    int totalParticipantsCount = 0;   ///< Суммарное количество участников всех событий

public:
    Simulator(Calendar* cal, MailSystem* mail, QDateTime start, QDateTime end, int step);

    void run();

    // --- Методы для статистики ---    int getTotalMessagesSent() const { return totalMessagesSent; }

    int getTotalEventsProcessed() const { return totalEventsProcessed; }
    int getUniqueParticipantsCount() const { return uniqueParticipants.size(); }
    int getTotalParticipantsCount() const { return totalParticipantsCount; }

    void setCalendar(Calendar* newCalendar) { calendar = newCalendar; }
    void setMailSystem(MailSystem* newMailSystem) { mailSystem = newMailSystem; }
    void setCurrentTime(const QDateTime& newTime) { currentTime = newTime; }
    void setEndTime(const QDateTime& newEndTime) { endTime = newEndTime; }
    void setStepMinutes(int newStepMinutes) { stepMinutes = newStepMinutes; }

    QDateTime getCurrentTime() {return currentTime;}

private:
    void updateCalendar();
    void checkConflicts();
    void sendReminders();
    void stepSimulation();
};


#endif // SIMULATOR_H
