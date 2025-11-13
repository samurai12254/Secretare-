#pragma once
#include <QWidget>
#include <QCalendarWidget>
#include <QMap>
#include <QVector>
#include <set>
#include "event.h"
#include "conflictresolver.h"


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
    ConflictResolver hasConflict(const Event& newEvent, const Event* ignoreEvent = nullptr);
    void DellPassEvents(QDateTime &finish);
    int GetCountConflicts();
    int GetCountPositiveEdit();
    QVector <QString> GetAllFinishEvents();
    QVector <QString> GetAllDeleteEvents();
    QVector <QString> GetAllLocation();

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
    int cnt_conflicts = 0;
    int cnt_good_edit = 0;
    QVector <QString> allFinishEvents;
    QVector <QString> allDelEvents;
    std::set <QString> allLocationFinish;
};
