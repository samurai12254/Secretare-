#include "Calendar.hpp"
#include <QDebug>

// Конструктор уже определен в hpp

// Добавление события
void Calendar::addEvent(Event* event) {
    if (!event) return;
    
    events.append(event);
    
    // Добавляем событие в соответствующий отдел
    Department* event_dept = event->getLocation();
    if (event_dept) {
        event_dept->addEvent(event);
    }
}

// Обновление события
void Calendar::updateEvent(const QString& event_id, const QHash<QString, QString>& updates) {
    Event* event = findEventById(event_id);
    if (!event) {
        qWarning() << "Событие с ID" << event_id << "не найдено";
        return;
    }
    
    for (auto it = updates.begin(); it != updates.end(); ++it) {
        const QString& key = it.key();
        const QString& value = it.value();
        
        if (key == "title") {
            event->setTitle(value);
        } else if (key == "description") {
            event->setDescription(value);
        } else if (key == "importance") {
            event->setImportance(value);
        }
        // Для location и других сложных типов нужна дополнительная логика преобразования
    }
}

// Удаление события
void Calendar::removeEvent(const QString& event_id) {
    Event* event = findEventById(event_id);
    if (!event) {
        qWarning() << "Событие с ID" << event_id << "не найдено";
        return;
    }
    
    // Удаляем из отдела
    Department* event_dept = event->getLocation();
    if (event_dept) {
        event_dept->removeEvent(event);
    }
    
    // Удаляем из основного списка
    int index = events.indexOf(event);
    if (index != -1) {
        events.removeAt(index);
    }
}

// Получение событий на определенный день
QVector<Event*> Calendar::getEventsForDay(const QDateTime& date) const {
    QVector<Event*> result;
    QDate target_date = date.date();
    
    for (Event* event : events) {
        if (event && event->getStartTime().date() == target_date) {
            result.append(event);
        }
    }
    return result;
}

// Поиск конфликтов (пересечений по времени)
QVector<QPair<Event*, Event*>> Calendar::findConflicts() const {
    QVector<QPair<Event*, Event*>> conflicts;
    
    // Используем встроенную проверку конфликтов из класса Event
    for (int i = 0; i < events.size(); ++i) {
        for (int j = i + 1; j < events.size(); ++j) {
            if (events[i] && events[j] && events[i]->conflictsWith(*events[j])) {
                conflicts.append(qMakePair(events[i], events[j]));
            }
        }
    }
    
    return conflicts;
}

// Удаление прошедших событий
void Calendar::removePastEvents(const QDateTime& current_time) {
    QVector<Event*> to_remove;
    
    // Собираем события для удаления
    for (Event* event : events) {
        if (event && event->getEndTime() < current_time) {
            to_remove.append(event);
        }
    }
    
    // Удаляем собранные события
    for (Event* event : to_remove) {
        removeEvent(event->getId());
    }
    
    qDebug() << "Удалено прошедших событий:" << to_remove.size();
}

// Получение событий для пользователя
QVector<Event*> Calendar::getEventsForUser(User* user) const {
    QVector<Event*> user_events;
    
    if (!user) return user_events;
    
    QString user_dept = user->getDepartment();
    
    // Находим отдел пользователя
    for (Department* dept : departments) {
        if (dept && dept->getName() == user_dept) {
            user_events = dept->getAllEvents();
            break;
        }
    }
    
    return user_events;
}

// Добавление отдела
void Calendar::addDepartment(Department* department) {
    if (department) {
        departments.append(department);
    }
}

// Получение списка отделов
QVector<Department*> Calendar::getDepartments() const {
    return departments;
}

// Вспомогательный метод для поиска события по ID
Event* Calendar::findEventById(const QString& event_id) const {
    for (Event* event : events) {
        if (event && event->getId() == event_id) {
            return event;
        }
    }
    return nullptr;
}

// Проверка конфликта времени между двумя событиями
bool Calendar::checkTimeConflict(const Event* event1, const Event* event2) const {
    if (!event1 || !event2) return false;
    
    // Используем встроенную проверку конфликтов из Event
    return event1->conflictsWith(*event2);
}

// Статистика
int Calendar::getTotalEvents() const {
    return events.size();
}

int Calendar::getTotalDepartments() const {
    return departments.size();
}