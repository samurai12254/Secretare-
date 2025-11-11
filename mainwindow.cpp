#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QWidget *central = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(central);

    // Кнопки навигации
    simButton = new QPushButton("Simulator");
    mailButton = new QPushButton("Mail");
    calendarButton = new QPushButton("Calendar");

    layout->addWidget(simButton);
    layout->addWidget(mailButton);
    layout->addWidget(calendarButton);

    // Стек для переключения страниц
    stack = new QStackedWidget();
    MailSystem* MSystem = new MailSystem();
    Simulator* now_simulator = new Simulator(nullptr, MSystem, QDateTime::currentDateTime(), QDateTime::currentDateTime(), 0);
    QVector<User*>* Users = new QVector<User*>;
    QHash<QString, User*>* Users_hash_table = nullptr;
    mailPage = new MailWindow(Users_hash_table,MSystem);
    simulatorPage = new SimulatorWindow(Users_hash_table,Users,now_simulator);
    calendarPage = new CalendarWindow(Users);

    // Добавляем страницы в стек
    stack->addWidget(simulatorPage);
    stack->addWidget(mailPage);
    stack->addWidget(calendarPage);

    layout->addWidget(stack);
    setCentralWidget(central);

    // Подключаем сигналы
    connect(simButton, &QPushButton::clicked, this, &MainWindow::showSimulator);
    connect(mailButton, &QPushButton::clicked, this, &MainWindow::showMail);
    connect(calendarButton, &QPushButton::clicked, this, &MainWindow::showCalendar);

    // По умолчанию показываем первую страницу
    stack->setCurrentWidget(simulatorPage);
}

MainWindow::~MainWindow() {}

void MainWindow::showSimulator()
{
    stack->setCurrentWidget(simulatorPage);
}

void MainWindow::showMail()
{
    stack->setCurrentWidget(mailPage);
}

void MainWindow::showCalendar()
{
    stack->setCurrentWidget(calendarPage);
}
