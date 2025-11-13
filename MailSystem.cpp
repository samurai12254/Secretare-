#include "MailSystem.hpp"
#include <QDebug>
#include "event.h"

// Отправка сообщения пользователю
void MailSystem::sendMessage(User* user, const QString& subject, 
                            const QString& text, const QString& sender, const QDateTime& date) {
    if (!user) return;
    
    QString userLogin = user->GetLogin();
    Message message(sender, userLogin, subject, text, date);
    
    // Добавляем в inbox пользователя
    inbox[userLogin].append(message);
    
    // Добавляем в outbox для истории
    outbox.append(message);
    
    qDebug() << "Сообщение отправлено пользователю" << userLogin;
    messages_sent++;
}

// Отправка сообщения нескольким пользователям
void MailSystem::sendMessageToMultiple(const QVector<User*>& users, const QString& subject, 
                                      const QString& text, const QString& sender, const QDateTime& date) {
    for (User* user : users) {
        if (user) {
            sendMessage(user, subject, text, sender, date);
        }
    }
}

void MailSystem::sendReminder(User* user, Event event) {
    if (!user) {
        qWarning() << "Невозможно отправить напоминание: пользователь или событие не указаны";
        return;
    }
    
    QString userLogin = user->GetLogin();
    QString eventTitle = event.getTitle();
    QString eventStart = event.formattedStart();
    QString eventEnd = event.formattedEnd();
    
    // Получаем информацию о месте проведения
    QString locationName = "Не указано";
    QString location = event.getLocation();
    locationName = location;
    // Получаем информацию о важности
    QString importance = event.getImportance();
    QString importanceText;
    if (importance == "высокая") {
        importanceText = "🔴 ВАЖНОЕ СОБЫТИЕ";
    } else if (importance == "средняя") {
        importanceText = "🟡 Средняя важность";
    } else {
        importanceText = "🟢 Обычное событие";
    }
    
    // Формируем тему и текст напоминания
    QString subject = "🔔 Напоминание: " + eventTitle;
    QString body = 
        importanceText + "\n\n"
        "📅 Событие: " + eventTitle + "\n"
        "🕐 Время: " + eventStart + " - " + eventEnd + "\n"
        "🏢 Место: " + locationName + "\n";
    
    // Добавляем описание, если оно есть
    QString description = event.getDescription();
    if (!description.isEmpty()) {
        body += "📝 Описание: " + description + "\n";
    }
    
    // Добавляем информацию о участниках
    QVector<User*> participants = event.getParticipants();
    if (!participants.isEmpty()) {
        body += "👥 Участники: ";
        for (int i = 0; i < participants.size(); ++i) {
            if (participants[i]) {
                body += participants[i]->GetLogin();
                if (i < participants.size() - 1) {
                    body += ", ";
                }
            }
        }
        body += "\n";
    }
    
    body += "\nНе забудьте подготовиться к событию!";
    
    // Отправляем сообщение
    Message message("Система напоминаний", userLogin, subject, body, 
                   QDateTime::currentDateTime(), event.getId());
    
    // Добавляем в inbox пользователя
    inbox[userLogin].append(message);
    
    // Добавляем в outbox для истории
    outbox.append(message);
    
    qDebug() << "Напоминание отправлено пользователю" << userLogin << "о событии" << eventTitle;
}


// Получение входящих сообщений пользователя
QVector<Message> MailSystem::getInbox(User* user) const {
    if (!user) return QVector<Message>();
    
    QString userLogin = (user->GetLogin());
    return inbox.value(userLogin, QVector<Message>());
}

// Получение непрочитанных сообщений
QVector<Message> MailSystem::getUnreadMessages(User* user) const {
    QVector<Message> unreadMessages;
    if (!user) return unreadMessages;
    
    QString userLogin = (user->GetLogin());
    if (!inbox.contains(userLogin)) return unreadMessages;
    
    const QVector<Message>& userMessages = inbox[userLogin];
    for (const Message& message : userMessages) {
        if (!message.getIsRead()) {
            unreadMessages.append(message);
        }
    }
    
    return unreadMessages;
}

// Пометить сообщение как прочитанное
void MailSystem::markMessageAsRead(User* user, int messageIndex) {
    if (!user) return;
    
    QString userLogin = (user->GetLogin());
    if (inbox.contains(userLogin) && messageIndex >= 0 && messageIndex < inbox[userLogin].size()) {
        // Поскольку Message хранится по значению, нам нужно заменить его
        Message message = inbox[userLogin][messageIndex];
        message.markAsRead();
        inbox[userLogin][messageIndex] = message;
    } else {
        qWarning() << "Сообщение с индексом" << messageIndex << "не найдено для пользователя" << userLogin;
    }
}

// Удаление старых сообщений
void MailSystem::clearOldMessages(const QDateTime& cutoffDate) {
    // Очищаем inbox всех пользователей
    for (auto it = inbox.begin(); it != inbox.end(); ++it) {
        QVector<Message>& messages = it.value();
        QVector<Message> keptMessages;
        
        for (const Message& message : messages) {
            if (message.getTimestamp() >= cutoffDate) {
                keptMessages.append(message);
            }
        }
        
        messages = keptMessages;
    }
    
    // Очищаем outbox
    QVector<Message> keptOutbox;
    for (const Message& message : outbox) {
        if (message.getTimestamp() >= cutoffDate) {
            keptOutbox.append(message);
        }
    }
    outbox = keptOutbox;
    
    qDebug() << "Старые сообщения удалены (до" << cutoffDate.toString("dd.MM.yyyy hh:mm") << ")";
}

// Удаление конкретного сообщения
void MailSystem::deleteMessage(User* user, int messageIndex) {
    if (!user) return;
    
    QString userLogin = (user->GetLogin());
    if (inbox.contains(userLogin) && messageIndex >= 0 && messageIndex < inbox[userLogin].size()) {
        inbox[userLogin].removeAt(messageIndex);
        
        // Если у пользователя не осталось сообщений, удаляем запись
        if (inbox[userLogin].isEmpty()) {
            inbox.remove(userLogin);
        }
    }
}

void MailSystem::delUserInbox(const QString& username) {
    if (!inbox.contains(username)) return;
    inbox.erase(inbox.find(username));
}

// Статистика
int MailSystem::getMessageCount(User* user) const {
    if (!user) return 0;
    
    QString userLogin = (user->GetLogin());
    return inbox.value(userLogin, QVector<Message>()).size();
}

int MailSystem::getUnreadCount(User* user) const {
    return getUnreadMessages(user).size();
}



// Получение исходящих сообщений
QVector<Message> MailSystem::getOutbox() const {
    return outbox;
}
