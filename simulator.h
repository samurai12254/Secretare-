#ifndef SIMULATOR_H
#define SIMULATOR_H

#pragma once
#include <QDateTime>
#include <QVector>
#include <QSet>
#include "calendarwindow.h"
#include "MailSystem.hpp"

class Simulator {
private:
    CalendarWindow* calendar;
    MailSystem* mailSystem;
    QDateTime currentTime;
    QDateTime endTime;
    int stepMinutes;
    bool RemindersEnabled;

    // --- Статистика симуляции ---
    int totalMessagesSent = 0;        ///< Общее количество отправленных сообщений
    int totalEventsProcessed = 0;     ///< Количество обработанных событий
    QSet<QString> uniqueParticipants; ///< Уникальные участники (по id)
    int totalParticipantsCount = 0;   ///< Суммарное количество участников всех событий

public:
    Simulator(CalendarWindow* cal, MailSystem* mail, QDateTime start, QDateTime end, int step);

    void run();

    // --- Методы для статистики ---    int getTotalMessagesSent() const { return totalMessagesSent; }

    int getTotalEventsProcessed() const { return totalEventsProcessed; }
    int getUniqueParticipantsCount() const { return uniqueParticipants.size(); }
    int getTotalParticipantsCount() const { return totalParticipantsCount; }
    int getTotalMessagesSent() {return totalMessagesSent;}

    void setCalendar(CalendarWindow* newCalendar) { calendar = newCalendar; }
    void setMailSystem(MailSystem* newMailSystem) { mailSystem = newMailSystem; }
    void setCurrentTime(const QDateTime& newTime) { currentTime = newTime; }
    void setEndTime(const QDateTime& newEndTime) { endTime = newEndTime; }
    void setStepMinutes(int newStepMinutes) { stepMinutes = newStepMinutes; }
    void setRemindersEnabled(bool isEnabled = false) {RemindersEnabled = isEnabled;}
    
    int GetCountConflicts(){return calendar->GetCountConflicts();};
    int GetCountPositiveEdit(){return calendar->GetCountPositiveEdit();};
    QVector <QString> GetAllDeleteEvents() {return calendar->GetAllDeleteEvents();};
    QVector <QString> GetAllLocation(){return calendar->GetAllLocation();};
    int getTotalMessagesSent() const {return mailSystem->getTotalMessagesSent();};
    void setMessagesSend(int count) {mailSystem->setMessagesSend(count);};


    void stepSimulation();

    QDateTime getCurrentTime() {return currentTime;}
    QDateTime getEndTime() {return endTime;}
    

private:
    void updateCalendar();
    void sendReminders();
};


#endif // SIMULATOR_H
