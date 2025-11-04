#include "event.h"

// --- Конструкторы ---
Event::Event() = default;

Event::Event(QString id,
             QString title,
             Department* location,
             QDateTime startTime,
             QDateTime endTime,
             QVector<User*> participants,
             QString importance,
             QString description,
             bool isInternal)
    : id(id),
      title(title),
      location(location),
      startTime(startTime),
      endTime(endTime),
      participants(participants),
      importance(importance),
      description(description),
      isInternal(isInternal)
{}

// --- Геттеры ---
QString Event::getId() const { return id; }
QString Event::getTitle() const { return title; }
Department* Event::getLocation() const { return location; }
QDateTime Event::getStartTime() const { return startTime; }
QDateTime Event::getEndTime() const { return endTime; }
QVector<User*> Event::getParticipants() const { return participants; }
QString Event::getImportance() const { return importance; }
QString Event::getDescription() const { return description; }
bool Event::getIsInternal() const { return isInternal; }

// --- Сеттеры ---
void Event::setTitle(const QString& newTitle) { title = newTitle; }
void Event::setLocation(Department* newLoc) { location = newLoc; }
void Event::setStartTime(const QDateTime& newStart) { startTime = newStart; }
void Event::setEndTime(const QDateTime& newEnd) { endTime = newEnd; }
void Event::setParticipants(const QVector<User*>& newParts) { participants = newParts; }
void Event::setImportance(const QString& newImportance) { importance = newImportance; }
void Event::setDescription(const QString& newDesc) { description = newDesc; }
void Event::setIsInternal(bool newValue) { isInternal = newValue; }

// --- Управление участниками ---
bool Event::addParticipant(User* user) {
    if (!user) return false;

    // Проверяем по ID, чтобы избежать дубликатов
    for (auto* existing : participants) {
        if (existing && existing->GetId() == user->GetId())
            return false;
    }

    participants.append(user);
    return true;
}

bool Event::removeParticipant(User* user) {
    if (!user) return false;
    QString targetId = user->GetId();

    for (int i = 0; i < participants.size(); ++i) {
        if (participants[i] && participants[i]->GetId() == targetId) {
            participants.removeAt(i);
            return true;
        }
    }
    return false;
}

// --- Проверка конфликтов ---
bool Event::conflictsWith(const Event& other) const {
    bool overlapTime = (startTime < other.endTime && endTime > other.startTime);

    bool sameParticipant = false;
    for (auto* p : participants) {
        if (!p) continue;
        for (auto* q : other.participants)
            if (q && p->GetId() == q->GetId()) {
                sameParticipant = true;
                break;
            }
        if (sameParticipant) break;
    }

    bool sameLocation = (location == other.location);

    return overlapTime && (sameParticipant || sameLocation);
}

// --- Вспомогательные методы ---
int Event::durationMinutes() const {
    return startTime.secsTo(endTime) / 60;
}

QString Event::formattedStart() const {
    return startTime.toString("dd.MM.yyyy hh:mm");
}

QString Event::formattedEnd() const {
    return endTime.toString("dd.MM.yyyy hh:mm");
}

QString Event::summary() const {
    QString locName = (location ? location->getName() : "Неизвестно");
    return QString("%1 [%2 — %3] (%4)")
        .arg(title)
        .arg(formattedStart())
        .arg(formattedEnd())
        .arg(locName);
}
    
