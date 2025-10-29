#ifndef CONFLICTRESOLVER_H
#define CONFLICTRESOLVER_H

#pragma once
#include <QVector>
#include "event.h"
#include "calendar.h"

/**
 * @brief Класс ConflictResolver отвечает за обнаружение и разрешение конфликтов между событиями.
 */
class ConflictResolver {
private:
    Calendar* calendar;

public:
    ConflictResolver(Calendar* cal);

    /**
     * @brief Проверяет все события в календаре на конфликты.
     * @return Список пар конфликтующих событий
     */
    QVector<QPair<Event*, Event*>> findConflicts() const;

    /**
     * @brief Автоматически пытается разрешить конфликт между двумя событиями.
     * Простая стратегия: перенос нового события на следующий свободный слот.
     * @param e1 Первое событие
     * @param e2 Второе событие
     * @return true, если конфликт успешно разрешен
     */
    bool resolveConflict(Event* e1, Event* e2);

    /**
     * @brief Предлагает несколько вариантов разрешения конфликта.
     * Можно выводить в GUI или консоль для выбора пользователем.
     */
    QVector<QString> suggestResolutions(Event* e1, Event* e2) const;
};

#endif // CONFLICTRESOLVER_H
