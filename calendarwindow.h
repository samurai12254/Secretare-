#pragma once
#include <QWidget>
#include <QCalendarWidget>
#include <QMap>
#include <QVector>
#include "event.h"

class CalendarWindow : public QWidget
{
    Q_OBJECT
public:
    explicit CalendarWindow(QWidget *parent = nullptr);
    explicit CalendarWindow(QVector<User*> *users, QWidget *parent = nullptr);
    QVector<Event> getAllStartEvents(const QDate& start, const QDate& finish) const;
    QVector<Event> getALLEvents();
    bool runAuthorization();
    void buildCalendarUI();
    bool hasConflict(const Event& newEvent, const Event* ignoreEvent = nullptr);
    void DellPassEvents(QDateTime &finish);

private slots:
    void onDateClicked(const QDate &date);
    void addEvent(const QDate &date);

private:
    QCalendarWidget *calendar;
    QVector<Event> events; // храним все события как Event
    QVector<User*> *allUsers = nullptr;
    User *currentUser = nullptr;
    void highlightDates();
    bool uiBuilt = false;

};
