#include "mailsystem.h"
#include <QDebug>

// Отправка сообщения пользователю
void MailSystem::sendMessage(User* user, const QString& subject, 
                            const QString& text, const QDateTime& date) {
    if (!user) return;
    
    QString userLogin = user->getLogin();
    Message message("System", userLogin, subject, text, date);
    
    // Добавляем в inbox пользователя
    inbox[userLogin].append(message);
    
    // Добавляем в outbox для истории
    outbox.append(message);
    
    qDebug() << "Сообщение отправлено пользователю" << userLogin;
}

// Отправка сообщения нескольким пользователям
void MailSystem::sendMessageToMultiple(const QVector<User*>& users, const QString& subject, 
                                      const QString& text, const QDateTime& date) {
    for (User* user : users) {
        if (user) {
            sendMessage(user, subject, text, date);
        }
    }
}

// Получение входящих сообщений пользователя
QVector<Message> MailSystem::getInbox(User* user) const {
    if (!user) return QVector<Message>();
    
    QString userLogin = user->getLogin();
    return inbox.value(userLogin, QVector<Message>());
}

// Получение непрочитанных сообщений
QVector<Message> MailSystem::getUnreadMessages(User* user) const {
    QVector<Message> unreadMessages;
    if (!user) return unreadMessages;
    
    QString userLogin = user->getLogin();
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
    
    QString userLogin = user->getLogin();
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
    
    QString userLogin = user->getLogin();
    if (inbox.contains(userLogin) && messageIndex >= 0 && messageIndex < inbox[userLogin].size()) {
        inbox[userLogin].removeAt(messageIndex);
        
        // Если у пользователя не осталось сообщений, удаляем запись
        if (inbox[userLogin].isEmpty()) {
            inbox.remove(userLogin);
        }
    }
}

// Статистика
int MailSystem::getMessageCount(User* user) const {
    if (!user) return 0;
    
    QString userLogin = user->getLogin();
    return inbox.value(userLogin, QVector<Message>()).size();
}

int MailSystem::getUnreadCount(User* user) const {
    return getUnreadMessages(user).size();
}

int MailSystem::getTotalMessages() const {
    int total = 0;
    for (const QVector<Message>& messages : inbox) {
        total += messages.size();
    }
    return total;
}

// Получение исходящих сообщений
QVector<Message> MailSystem::getOutbox() const {
    return outbox;
}