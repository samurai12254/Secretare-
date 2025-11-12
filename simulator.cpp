#include "simulator.h"

#include "simulator.h"
#include <QDebug>

Simulator::Simulator(CalendarWindow* cal, MailSystem* mail, QDateTime start, QDateTime end, int step)
    : calendar(cal), mailSystem(mail), currentTime(start), endTime(end), stepMinutes(step)
{}

void Simulator::run() {
    while (currentTime <= endTime) {
        //updateCalendar();
        //checkConflicts();
        stepSimulation();
    }
    QDateTime infinity = QDateTime::currentDateTime().addYears(200000);
    mailSystem->clearOldMessages(infinity);
    calendar->DellPassEvents(infinity);
    qDebug() << "Симуляция завершена!";
    qDebug() << "Общее количество отправленных сообщений:" << totalMessagesSent;
    qDebug() << "Общее количество обработанных событий:" << totalEventsProcessed;
    qDebug() << "Уникальных участников:" << getUniqueParticipantsCount();
    qDebug() << "Общее количество участников всех событий:" << totalParticipantsCount;
}

void Simulator::updateCalendar() {
    calendar->DellPassEvents(currentTime);
}



void Simulator::sendReminders() {
    if (currentTime != currentTime.date().startOfDay()) return;
    QVector<Event> events = {};
    QVector<QVector<QString>> impotances = {{},{"Низкая"},{"Низкая","Средняя"}};
    for (int i = 1;i <= 3; ++i){
        events = calendar->getAllStartEvents(currentTime.date().addDays(i), currentTime.date().addDays(i));
        for (auto e : events) {
            if (impotances[i-1].contains(e.getImportance())) continue;
            for (auto* user : e.getParticipants()) {
                if (!user) continue;
                mailSystem->sendReminder(user, e);
                totalMessagesSent++;        // учитываем каждое сообщение
                uniqueParticipants.insert(user->GetId());
            }
            totalEventsProcessed++;         // каждый обработанный event
            totalParticipantsCount += e.getParticipants().size();
        }
    }
    
}

void Simulator::stepSimulation() {
    currentTime = currentTime.addSecs(stepMinutes * 60);
    updateCalendar();
    if (RemindersEnabled) sendReminders();
}
