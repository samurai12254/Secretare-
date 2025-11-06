#pragma once

#include <QMainWindow>
#include <QPushButton>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QWidget>

#include "simulatorwindow.h"
#include "mailwindow.h"
#include "calendarwindow.h"

#include "simulator.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void showSimulator();
    void showMail();
    void showCalendar();

private:
    QPushButton *simButton;
    QPushButton *mailButton;
    QPushButton *calendarButton;

    QStackedWidget *stack; // 👈 хранит все "экраны"
    SimulatorWindow *simulatorPage;
    MailWindow *mailPage;
    CalendarWindow *calendarPage;
};
