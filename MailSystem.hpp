#pragma once

#include <QHash>
#include <QVector>
#include <QString>
#include <QDateTime>
#include "message.h"
#include "User.hpp"

class MailSystem
{
private:
    QHash<QString, QVector<Message>> inbox;  // ключ: логин пользователя
    QVector<Message> outbox;                 // исходящие письма
    int messages_sent;

public:
    // Конструктор
    MailSystem() = default;
    
    // Основные методы
    void sendMessage(User* user, const QString& subject, 
                    const QString& text, const QString& sender="System", const QDateTime& date = QDateTime::currentDateTime());
    void sendMessageToMultiple(const QVector<User*>& users, const QString& subject, 
                              const QString& text, const QString& sender="System", const QDateTime& date = QDateTime::currentDateTime());
    
    void sendReminder(User* user, Event event);

    // Работа с входящими сообщениями
    QVector<Message> getInbox(User* user) const;
    QVector<Message> getUnreadMessages(User* user) const;
    void markMessageAsRead(User* user, int messageIndex);
    
    // Управление сообщениями
    void clearOldMessages(const QDateTime& cutoffDate);
    void deleteMessage(User* user, int messageIndex);
    
    // Статистика
    int getMessageCount(User* user) const;
    int getUnreadCount(User* user) const;
    int getTotalMessagesSent() const {return messages_sent;};
    void setMessagesSend(int count) {messages_sent = count;};
    
    // Работа с исходящими
    QVector<Message> getOutbox() const;
};
