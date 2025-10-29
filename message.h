#ifndef MESSAGE_H
#define MESSAGE_H
// message.h
#pragma once
#include <QString>
#include <QDateTime>
#include <optional>

class Message {
private:
    QString sender;         // логин или имя отправителя
    QString receiver;       // логин или имя получателя
    QString subject;        // тема письма
    QString body;           // текст письма
    QDateTime timestamp;    // время отправки
    bool isRead;            // статус письма
    QString relatedEventId; // ID события календаря (если сообщение о событии)

public:
    // --- Конструкторы ---
    Message() : isRead(false) {}

    Message(QString sender, QString receiver, QString subject, QString body,
            QDateTime timestamp = QDateTime::currentDateTime(),
            QString relatedEventId = "")
        : sender(sender),
        receiver(receiver),
        subject(subject),
        body(body),
        timestamp(timestamp),
        isRead(false),
        relatedEventId(relatedEventId)
    {}

    // --- Геттеры ---
    QString getSender() const { return sender; }
    QString getReceiver() const { return receiver; }
    QString getSubject() const { return subject; }
    QString getBody() const { return body; }
    QDateTime getTimestamp() const { return timestamp; }
    QString getRelatedEventId() const { return relatedEventId; }
    bool getIsRead() const { return isRead; }

    // --- Сеттеры ---
    void markAsRead() { isRead = true; }
    void markAsUnread() { isRead = false; }

    // --- Вспомогательные методы ---
    QString summary() const {
        QString status = isRead ? "+" : "-";
        return QString("[%1] %2 → %3 | %4")
            .arg(status)
            .arg(sender)
            .arg(receiver)
            .arg(subject);
    }

    QString formattedTimestamp() const {
        return timestamp.toString("dd.MM.yyyy hh:mm");
    }
};

#endif // MESSAGE_H
