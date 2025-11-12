#ifndef SIMULATORWINDOW_H
#define SIMULATORWINDOW_H

#include <QWidget>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSpinBox>
#include <QComboBox>
#include <QCheckBox>
#include <QPushButton>
#include <QLabel>
#include <QGroupBox>
#include <QMessageBox>
#include <QLineEdit>
#include <QTableWidget>
#include <QHeaderView>

#include "simulator.h"
#include "User.hpp"
#include "simulationrunwindow.h"


class SimulatorWindow : public QWidget
{
    Q_OBJECT

public:
    explicit SimulatorWindow(QHash<QString, User*>* init_users_hash, QList<User*>* user_list_p, Simulator* init_simulator, QWidget *parent = nullptr);

    // Геттеры для получения параметров
    int getSimulationPeriod() const;
    QString getSimulationStep() const;
    bool getCalendarRemindersEnabled() const;
    bool getConflictResolutionEnabled() const;

    // Методы для работы с пользователями
    void addUser(User* user);
    QList<User*> getUsers() const;

signals:
    void simulationStarted(); // Сигнал о начале моделирования

private slots:
    void handleStartSimulation();
    void validateInput();
    void handleAddUser();
    void handleRemoveUser();
    void handleUserSelection();
    void toggleUsersManagement(); // Новый слот для переключения видимости
    void handleSimulationFinished();
    void handleReturnToSettings();
    void handlePause();
    void handleResume();

private:
    void setupUI();
    void setupUsersSection();
    void updateUsersTable();
    bool validateUserInput();
    void setupSettingsPage();

    // UI элементы
    QSpinBox *periodSpinBox;
    QComboBox *stepComboBox;
    QCheckBox *remindersCheckBox;
    QPushButton *startButton;
    QPushButton *manageUsersButton; // Новая кнопка для управления пользователями

    // Элементы для управления пользователями
    QGroupBox *usersGroup; // Группа управления пользователями
    QLineEdit *userIdInput;
    QLineEdit *userLoginInput;
    QLineEdit *userPasswordInput;
    QComboBox *userRoleComboBox;
    QLineEdit *userDepartmentInput;
    QPushButton *addUserButton;
    QPushButton *removeUserButton;
    QTableWidget *usersTable;

    Simulator* now_simulator;
    QList<User*>* usersList;
    QHash<QString, User*>* users_hash_table;
    bool usersSectionVisible; // Флаг видимости секции пользователей

    QStackedWidget *mainStack;
    QWidget *settingsPage;
    SimulationRunWindow *simulationPage;
};

#endif // SIMULATORWINDOW_H