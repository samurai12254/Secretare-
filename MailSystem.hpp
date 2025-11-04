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

public:
    // Конструктор
    MailSystem() = default;
    
    // Основные методы
    void sendMessage(User* user, const QString& subject, 
                    const QString& text, const QDateTime& date = QDateTime::currentDateTime());
    void sendMessageToMultiple(const QVector<User*>& users, const QString& subject, 
                              const QString& text, const QDateTime& date = QDateTime::currentDateTime());
    
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
    int getTotalMessages() const;
    
    // Работа с исходящими
    QVector<Message> getOutbox() const;
};
