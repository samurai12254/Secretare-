#include "simulatorwindow.h"


SimulatorWindow::SimulatorWindow(QHash<QString, User*>* init_users_hash, QList<User*>* user_list_p, Simulator* init_simulator, QWidget *parent)
    : QWidget(parent), now_simulator(init_simulator), usersList(user_list_p), users_hash_table(init_users_hash), usersSectionVisible(false)
{
    setupUI();
}

void SimulatorWindow::setupUI()
{
    // Создаем основной стек для переключения между настройками и симуляцией
    mainStack = new QStackedWidget(this);
    
    // Страница настроек
    settingsPage = new QWidget();
    setupSettingsPage();
    
    // Страница выполнения симуляции
    simulationPage = new SimulationRunWindow(now_simulator);
    
    // Добавляем страницы в стек
    mainStack->addWidget(settingsPage);
    mainStack->addWidget(simulationPage);
    
    // Устанавливаем основной layout
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(mainStack);
    
    // По умолчанию показываем страницу настроек
    mainStack->setCurrentWidget(settingsPage);
    
    // Подключаем сигналы от simulationPage

    connect(simulationPage, &SimulationRunWindow::simulationFinished, this, &SimulatorWindow::handleSimulationFinished);
    connect(simulationPage, &SimulationRunWindow::returnToSettings, this, &SimulatorWindow::handleReturnToSettings);
    connect(simulationPage, &SimulationRunWindow::handlePauseResume, this, &SimulatorWindow::handlePause);
}

void SimulatorWindow::setupSettingsPage()
{
    QVBoxLayout *settingsLayout = new QVBoxLayout(settingsPage);

    // Заголовок
    QLabel *titleLabel = new QLabel("Параметры моделирования");
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 16px; font-weight: bold;");
    settingsLayout->addWidget(titleLabel);

    // Группа основных параметров
    QGroupBox *mainParamsGroup = new QGroupBox("Основные параметры моделирования");
    QVBoxLayout *mainParamsLayout = new QVBoxLayout(mainParamsGroup);

    // Период моделирования
    QHBoxLayout *periodLayout = new QHBoxLayout();
    periodLayout->addWidget(new QLabel("Период моделирования (дни):"));
    periodSpinBox = new QSpinBox();
    periodSpinBox->setRange(7, 30);
    periodSpinBox->setValue(7);
    periodSpinBox->setSuffix(" дней");
    periodLayout->addWidget(periodSpinBox);
    periodLayout->addStretch();
    mainParamsLayout->addLayout(periodLayout);

    // Шаг моделирования
    QHBoxLayout *stepLayout = new QHBoxLayout();
    stepLayout->addWidget(new QLabel("Шаг моделирования:"));
    stepComboBox = new QComboBox();
    stepComboBox->addItem("30 минут");
    stepComboBox->addItem("1 час");
    stepLayout->addWidget(stepComboBox);
    stepLayout->addStretch();
    mainParamsLayout->addLayout(stepLayout);

    settingsLayout->addWidget(mainParamsGroup);

    // Группа дополнительных параметров
    QGroupBox *additionalParamsGroup = new QGroupBox("Дополнительные параметры");
    QVBoxLayout *additionalParamsLayout = new QVBoxLayout(additionalParamsGroup);

    // Напоминания о событиях
    remindersCheckBox = new QCheckBox("Включить напоминания о событиях календаря");
    remindersCheckBox->setChecked(true);
    additionalParamsLayout->addWidget(remindersCheckBox);

    // Разрешение конфликтов
    conflictResolutionCheckBox = new QCheckBox("Включить автоматическое разрешение конфликтов");
    conflictResolutionCheckBox->setChecked(true);
    additionalParamsLayout->addWidget(conflictResolutionCheckBox);

    settingsLayout->addWidget(additionalParamsGroup);

    // Кнопка управления пользователями
    manageUsersButton = new QPushButton("Управление пользователями");
    manageUsersButton->setStyleSheet("QPushButton { background-color: #2196F3; color: white; font-weight: bold; padding: 8px; }");
    settingsLayout->addWidget(manageUsersButton);

    // Секция управления пользователями (изначально скрыта)
    setupUsersSection();
    usersGroup->setVisible(false);

    // Кнопка запуска
    startButton = new QPushButton("Запустить моделирование");
    startButton->setStyleSheet("QPushButton { background-color: #4CAF50; color: white; font-weight: bold; padding: 8px; }");
    settingsLayout->addWidget(startButton);

    // Информационная панель
    QLabel *infoLabel = new QLabel("Моделирование будет выполнено для выбранного периода с указанным шагом.\n"
                                  "При включенных опциях будут генерироваться напоминания и разрешаться конфликты.");
    infoLabel->setWordWrap(true);
    infoLabel->setStyleSheet("background-color: #EE204D; padding: 10px; border-radius: 5px;");
    settingsLayout->addWidget(infoLabel);

    settingsLayout->addStretch();

    // Подключаем сигналы
    connect(startButton, &QPushButton::clicked, this, &SimulatorWindow::handleStartSimulation);
    connect(periodSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &SimulatorWindow::validateInput);
    connect(manageUsersButton, &QPushButton::clicked, this, &SimulatorWindow::toggleUsersManagement);
}

void SimulatorWindow::handleStartSimulation()
{
    // Проверяем, что есть хотя бы один пользователь
    if (usersList->isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Добавьте хотя бы одного пользователя перед запуском моделирования");
        return;
    }

    // Проверяем валидность введенных данных
    if (periodSpinBox->value() < 7 || periodSpinBox->value() > 30) {
        QMessageBox::warning(this, "Ошибка", "Период моделирования должен быть от 7 до 30 дней");
        return;
    }

    // Показываем подтверждение
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Подтверждение", 
                                QString("Начать моделирование?\n\n"
                                       "Период: %1 дней\n"
                                       "Шаг: %2\n"
                                       "Напоминания: %3\n"
                                       "Разрешение конфликтов: %4\n"
                                       "Пользователей: %5")
                                .arg(periodSpinBox->value())
                                .arg(stepComboBox->currentText())
                                .arg(remindersCheckBox->isChecked() ? "Включены" : "Отключены")
                                .arg(conflictResolutionCheckBox->isChecked() ? "Включено" : "Отключено")
                                .arg(usersList->size()),
                                QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        // Устанавливаем текущее время как время начала симуляции
        QDateTime currentTime = QDateTime::currentDateTime().date().startOfDay();
        now_simulator->setCurrentTime(currentTime);
        
        // Сохраняем настройки в симулятор
        int currentPeriod = periodSpinBox->value();
        QString currentStep = stepComboBox->currentText();
        if (currentStep == "30 минут") {
            now_simulator->setStepMinutes(30);
        } else if (currentStep == "1 час") {
            now_simulator->setStepMinutes(60);
        }
        now_simulator->setEndTime(currentTime.addDays(currentPeriod));
        
        QMessageBox::information(this, "Успех", 
                                QString("Моделирование запущено с параметрами:\n"
                                       "Период: %1 дней\n"
                                       "Шаг: %2\n"
                                       "Пользователей: %3\n"
                                       "Время начала: %4")
                                .arg(currentPeriod)
                                .arg(currentStep)
                                .arg(usersList->size())
                                .arg(currentTime.toString("dd.MM.yyyy hh:mm:ss")));
        
        // Переключаемся на страницу выполнения симуляции
        mainStack->setCurrentWidget(simulationPage);
        emit simulationStarted();
    }
}

void SimulatorWindow::handleSimulationFinished()
{
    QMessageBox::information(this, "Симуляция завершена", 
                            "Симуляция успешно завершена. Возврат к настройкам.");
    mainStack->setCurrentWidget(settingsPage);
}

void SimulatorWindow::handleReturnToSettings()
{
    mainStack->setCurrentWidget(settingsPage);
}

void SimulatorWindow::handlePause() {
    mainStack->setCurrentWidget(settingsPage);
    startButton->setText("Продолжить симуляцию");
    disconnect(startButton, &QPushButton::clicked, this, &SimulatorWindow::handleStartSimulation);
    connect(startButton, &QPushButton::clicked, this, &SimulatorWindow::handleResume);
}

void SimulatorWindow::handleResume(){
    mainStack->setCurrentWidget(simulationPage);
    startButton->setText("Запустить моделирование");
    disconnect(startButton, &QPushButton::clicked, this, &SimulatorWindow::handleResume);
    connect(startButton, &QPushButton::clicked, this, &SimulatorWindow::handleStartSimulation);
}

void SimulatorWindow::setupUsersSection()
{
    usersGroup = new QGroupBox("Управление пользователями");
    QVBoxLayout *usersLayout = new QVBoxLayout(usersGroup);

    // Форма добавления пользователя
    QGridLayout *formLayout = new QGridLayout();
    
    formLayout->addWidget(new QLabel("ID:"), 0, 0);
    userIdInput = new QLineEdit();
    formLayout->addWidget(userIdInput, 0, 1);
    
    formLayout->addWidget(new QLabel("Логин:"), 0, 2);
    userLoginInput = new QLineEdit();
    formLayout->addWidget(userLoginInput, 0, 3);
    
    formLayout->addWidget(new QLabel("Пароль:"), 1, 0);
    userPasswordInput = new QLineEdit();
    userPasswordInput->setEchoMode(QLineEdit::Password);
    formLayout->addWidget(userPasswordInput, 1, 1);
    
    formLayout->addWidget(new QLabel("Роль:"), 1, 2);
    userRoleComboBox = new QComboBox();
    userRoleComboBox->addItems({"сотрудник", "секретарь", "руководитель"});
    formLayout->addWidget(userRoleComboBox, 1, 3);
    
    formLayout->addWidget(new QLabel("Отдел:"), 2, 0);
    userDepartmentInput = new QLineEdit();
    formLayout->addWidget(userDepartmentInput, 2, 1);
    
    usersLayout->addLayout(formLayout);

    // Кнопки управления пользователями
    QHBoxLayout *buttonsLayout = new QHBoxLayout();
    addUserButton = new QPushButton("Добавить пользователя");
    removeUserButton = new QPushButton("Удалить выбранного");
    removeUserButton->setEnabled(false);
    
    buttonsLayout->addWidget(addUserButton);
    buttonsLayout->addWidget(removeUserButton);
    buttonsLayout->addStretch();
    
    usersLayout->addLayout(buttonsLayout);

    // Таблица пользователей
    usersTable = new QTableWidget();
    usersTable->setColumnCount(5);
    usersTable->setHorizontalHeaderLabels({"ID", "Логин", "Пароль", "Роль", "Отдел"});
    usersTable->horizontalHeader()->setStretchLastSection(true);
    usersTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    usersTable->setSelectionMode(QAbstractItemView::SingleSelection);
    usersTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    
    usersLayout->addWidget(usersTable);

    // Подключаем сигналы
    connect(addUserButton, &QPushButton::clicked, this, &SimulatorWindow::handleAddUser);
    connect(removeUserButton, &QPushButton::clicked, this, &SimulatorWindow::handleRemoveUser);
    connect(usersTable, &QTableWidget::itemSelectionChanged, this, &SimulatorWindow::handleUserSelection);

    // Добавляем группу в основной layout
    QVBoxLayout *mainLayout = qobject_cast<QVBoxLayout*>(settingsPage->layout());
    if (mainLayout) {
        // Вставляем после кнопки управления пользователями
        mainLayout->insertWidget(3, usersGroup);
    }
}


void SimulatorWindow::toggleUsersManagement()
{
    usersSectionVisible = !usersSectionVisible;
    usersGroup->setVisible(usersSectionVisible);
    
    // Обновляем текст кнопки в зависимости от состояния
    if (usersSectionVisible) {
        manageUsersButton->setText("Скрыть управление пользователями");
        // Обновляем таблицу при открытии
        updateUsersTable();
    } else {
        manageUsersButton->setText("Управление пользователями");
    }
}



void SimulatorWindow::validateInput()
{
    // Автоматическая валидация ввода
    int value = periodSpinBox->value();
    if (value < 7) {
        periodSpinBox->setValue(7);
    } else if (value > 30) {
        periodSpinBox->setValue(30);
    }
}

void SimulatorWindow::handleAddUser()
{
    if (!validateUserInput()) {
        return;
    }

    // Создаем нового пользователя
    User* new_user_p = new User(
        userIdInput->text().trimmed(),
        userLoginInput->text().trimmed(),
        userPasswordInput->text().trimmed(),
        userRoleComboBox->currentText(),
        userDepartmentInput->text().trimmed()
    );

    // Проверяем уникальность ID
    for (const User* user : *(usersList)) {
        if (user->GetId() == new_user_p->GetId()) {
            QMessageBox::warning(this, "Ошибка", "Пользователь с таким ID уже существует");
            return;
        }
        if (user->GetLogin() == new_user_p->GetLogin()) {
            QMessageBox::warning(this, "Ошибка", "Пользователь с таким логином уже существует");
            return;
        }
    }
    
    // Добавляем пользователя в список
    usersList->append(new_user_p);
    updateUsersTable();
    users_hash_table->insert(new_user_p->GetLogin(), new_user_p);

    // Очищаем поля ввода
    userIdInput->clear();
    userLoginInput->clear();
    userPasswordInput->clear();
    userDepartmentInput->clear();
    
    QMessageBox::information(this, "Успех", "Пользователь успешно добавлен");
}

void SimulatorWindow::handleRemoveUser()
{
    int currentRow = usersTable->currentRow();
    if (currentRow >= 0 && currentRow < usersList->size()) {
        QString userId = (*usersList)[currentRow]->GetId();
        QString user_login = (*usersList)[currentRow]->GetLogin();
        usersList->removeAt(currentRow);
        users_hash_table->erase(users_hash_table->find(user_login));
        updateUsersTable();
        QMessageBox::information(this, "Успех", QString("Пользователь %1 удален").arg(userId));
    }
}

void SimulatorWindow::handleUserSelection()
{
    removeUserButton->setEnabled(usersTable->currentRow() >= 0);
}

bool SimulatorWindow::validateUserInput()
{
    if (userIdInput->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Введите ID пользователя");
        return false;
    }
    if (userLoginInput->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Введите логин пользователя");
        return false;
    }
    if (userPasswordInput->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Введите пароль пользователя");
        return false;
    }
    if (userDepartmentInput->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Введите отдел пользователя");
        return false;
    }
    return true;
}

void SimulatorWindow::updateUsersTable()
{
    usersTable->setRowCount(usersList->size());
    
    for (int i = 0; i < usersList->size(); ++i) {
        const User& user = *((*usersList)[i]);
        usersTable->setItem(i, 0, new QTableWidgetItem(user.GetId()));
        usersTable->setItem(i, 1, new QTableWidgetItem(user.GetLogin()));
        usersTable->setItem(i, 2, new QTableWidgetItem("******")); // Скрываем пароль
        usersTable->setItem(i, 3, new QTableWidgetItem(user.GetRole()));
        usersTable->setItem(i, 4, new QTableWidgetItem(user.GetDepartment()));
    }
    
    usersTable->resizeColumnsToContents();
}

void SimulatorWindow::addUser(User* user)
{
    usersList->append(user);
    updateUsersTable();
}

QList<User*> SimulatorWindow::getUsers() const
{
    return *usersList;
}

// Геттеры для получения параметров
int SimulatorWindow::getSimulationPeriod() const
{
    return periodSpinBox->value();
}

QString SimulatorWindow::getSimulationStep() const
{
    return stepComboBox->currentText();
}

bool SimulatorWindow::getCalendarRemindersEnabled() const
{
    return remindersCheckBox->isChecked();
}

bool SimulatorWindow::getConflictResolutionEnabled() const
{
    return conflictResolutionCheckBox->isChecked();
}