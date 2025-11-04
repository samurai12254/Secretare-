#include "conflictresolver.h"

#include <QDebug>

ConflictResolver::ConflictResolver(Calendar* cal) : calendar(cal) {}

/**
 * @brief Находит все пары конфликтующих событий в календаре
 */
QVector<QPair<Event*, Event*>> ConflictResolver::findConflicts() const {
    QVector<Event*> events = calendar->getAllEvents();
    QVector<QPair<Event*, Event*>> conflicts;

    for (int i = 0; i < events.size(); ++i) {
        for (int j = i + 1; j < events.size(); ++j) {
            if (events[i]->conflictsWith(*events[j])) {
                conflicts.append(qMakePair(events[i], events[j]));
            }
        }
    }
    return conflicts;
}

/**
 * @brief Простое автоматическое разрешение конфликта:
 * Перенос события e2 на следующий свободный слот.
 */
bool ConflictResolver::resolveConflict(Event* e1, Event* e2) {
    if (!e1 || !e2) return false;

    // Ищем первый доступный час после e1->getEndTime()
    QDateTime newStart = e1->getEndTime();
    QDateTime newEnd = newStart.addSecs(e2->durationMinutes() * 60);

    e2->setStartTime(newStart);
    e2->setEndTime(newEnd);

    // Проверяем, решен ли конфликт
    if (!e1->conflictsWith(*e2)) {
        qDebug() << "Конфликт между событиями"
                 << e1->getTitle() << "и" << e2->getTitle()
                 << "разрешен автоматически.";
        return true;
    }

    // Если всё ещё конфликтует, вернуть false
    return false;
}

/**
 * @brief Предлагает варианты разрешения конфликта для пользователя
 */
QVector<QString> ConflictResolver::suggestResolutions(Event* e1, Event* e2) const {
    QVector<QString> suggestions;

    suggestions.append("Перенести новое событие на следующий свободный слот");
    suggestions.append("Перенести старое событие на следующий свободный слот");
    suggestions.append("Изменить место проведения нового события");
    suggestions.append("Изменить место проведения старого события");
    suggestions.append("Оставить конфликт и уведомить пользователей");

    return suggestions;
}
