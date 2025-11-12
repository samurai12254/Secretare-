#include "event.h"
#include <random>
#include <chrono>
#include <string>
using dist = std::uniform_int_distribution<int>;
std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());
// --- Конструкторы ---
Event::Event() = default;
Event::Event(QString title,
             QString location,
             QDateTime startTime,
             QDateTime endTime,
             QString& participantsString,
             QVector<User*>& allUsers,
             QString importance,
             QString description,
             bool isInternal)
    :
    title(title),
    location(location),
    startTime(startTime),
    endTime(endTime),
    importance(importance),
    description(description),
    isInternal(isInternal)
{
    std::string id_now;
    for(int i = 0;i < 20;i++){
        id_now += (char)dist(35,94)(rng);
    }
    id = QString::fromStdString(id_now);
    setParticipantsFromString(participantsString, allUsers);
}

Event::Event(
             QString title,
             QString location,
             QDateTime startTime,
             QDateTime endTime,
             QVector<User*> participants,
             QString importance,
             QString description,
             bool isInternal)
    : title(title),
      location(location),
      startTime(startTime),
      endTime(endTime),
      participants(participants),
      importance(importance),
      description(description),
      isInternal(isInternal)
{
    std::string id_now;
    for(int i = 0;i < 20;i++){
        id_now += (char)dist(35,94)(rng);
    }
    id = QString::fromStdString(id_now);
}

// --- Геттеры ---
QString Event::getId() const { return id; }
QString Event::getTitle() const { return title; }
QString Event::getLocation() const { return location; }
QDateTime Event::getStartTime() const { return startTime; }
QDateTime Event::getEndTime() const { return endTime; }
QVector<User*> Event::getParticipants() const { return participants; }
QString Event::getImportance() const { return importance; }
QString Event::getDescription() const { return description; }
bool Event::getIsInternal() const { return isInternal; }
void Event::clearParticipants(){
    participants.clear();
}

// --- Сеттеры ---
void Event::setTitle(const QString& newTitle) { title = newTitle; }
void Event::setLocation(QString newLoc) { location = newLoc; }
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
    QString locName = location;
    return QString("%1 [%2 — %3] (%4)")
        .arg(title)
        .arg(formattedStart())
        .arg(formattedEnd())
        .arg(locName);
}

void Event::setParticipantsFromString(const QString& participantsString, const QVector<User*> &allUsers)
{
    QStringList logins = participantsString.split(',', Qt::SkipEmptyParts);
    for (QString login : logins) {
        login = login.trimmed(); // убираем пробелы

        for (User* user : allUsers) {
            if (user && user->GetLogin() == login) {
                addParticipant(user);
                break;
            }
        }
    }
}
