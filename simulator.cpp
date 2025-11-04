#include "simulator.h"

#include "simulator.h"
#include <QDebug>

Simulator::Simulator(Calendar* cal, MailSystem* mail, QDateTime start, QDateTime end, int step)
    : calendar(cal), mailSystem(mail), currentTime(start), endTime(end), stepMinutes(step)
{}

void Simulator::run() {
    while (currentTime <= endTime) {
        updateCalendar();
        checkConflicts();
        sendReminders();
        stepSimulation();
    }

    qDebug() << "Симуляция завершена!";
    qDebug() << "Общее количество отправленных сообщений:" << totalMessagesSent;
    qDebug() << "Общее количество обработанных событий:" << totalEventsProcessed;
    qDebug() << "Уникальных участников:" << getUniqueParticipantsCount();
    qDebug() << "Общее количество участников всех событий:" << totalParticipantsCount;
}

void Simulator::updateCalendar() {
    QVector<Event*> pastEvents = calendar->getPastEvents(currentTime);
    for (auto* e : pastEvents) {
        calendar->removeEvent(e->getId());
        qDebug() << "Удалено прошедшее событие:" << e->summary();
        totalEventsProcessed++;  // учёт события
        // учёт участников
        totalParticipantsCount += e->getParticipants().size();
        for (auto* u : e->getParticipants()) {
            if (u) uniqueParticipants.insert(u->GetId());
        }
    }
}

void Simulator::checkConflicts() {
    QVector<Event*> events = calendar->getEventsForDay(currentTime.date());
    for (int i = 0; i < events.size(); ++i) {
        for (int j = i + 1; j < events.size(); ++j) {
            if (events[i]->conflictsWith(*events[j])) {
                qDebug() << "⚠️ Конфликт между событиями:"
                         << events[i]->summary() << "и" << events[j]->summary();
            }
        }
    }
}

void Simulator::sendReminders() {
    QVector<Event*> events = calendar->getEventsForPeriod(currentTime, currentTime.addSecs(stepMinutes * 60));
    for (auto* e : events) {
        for (auto* user : e->getParticipants()) {
            if (!user) continue;
            mailSystem->sendReminder(user, e);
            totalMessagesSent++;        // учитываем каждое сообщение
            uniqueParticipants.insert(user->GetId());
        }
        totalEventsProcessed++;         // каждый обработанный event
        totalParticipantsCount += e->getParticipants().size();
    }
}

void Simulator::stepSimulation() {
    currentTime = currentTime.addSecs(stepMinutes * 60);
}
