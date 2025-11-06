#include "simulatorwindow.h"

SimulatorWindow::SimulatorWindow(QWidget *parent)
    : QWidget(parent)
{
    setupUI();
}

void SimulatorWindow::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // Заголовок
    QLabel *titleLabel = new QLabel("Параметры моделирования");
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 16px; font-weight: bold;");
    mainLayout->addWidget(titleLabel);

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

    mainLayout->addWidget(mainParamsGroup);

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

    mainLayout->addWidget(additionalParamsGroup);

    // Кнопка запуска
    startButton = new QPushButton("Запустить моделирование");
    startButton->setStyleSheet("QPushButton { background-color: #4CAF50; color: white; font-weight: bold; padding: 8px; }");
    mainLayout->addWidget(startButton);

    // Информационная панель
    QLabel *infoLabel = new QLabel("Моделирование будет выполнено для выбранного периода с указанным шагом.\n"
                                  "При включенных опциях будут генерироваться напоминания и разрешаться конфликты.");
    infoLabel->setWordWrap(true);
    infoLabel->setStyleSheet("background-color: #E3F2FD; padding: 10px; border-radius: 5px;");
    mainLayout->addWidget(infoLabel);

    mainLayout->addStretch();

    // Подключаем сигналы
    connect(startButton, &QPushButton::clicked, this, &SimulatorWindow::handleStartSimulation);
    connect(periodSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &SimulatorWindow::validateInput);
}

void SimulatorWindow::handleStartSimulation()
{
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
                                       "Разрешение конфликтов: %4")
                                .arg(periodSpinBox->value())
                                .arg(stepComboBox->currentText())
                                .arg(remindersCheckBox->isChecked() ? "Включены" : "Отключены")
                                .arg(conflictResolutionCheckBox->isChecked() ? "Включено" : "Отключено"),
                                QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        QMessageBox::information(this, "Успех", "Моделирование запущено с выбранными параметрами!");
        emit simulationStarted();
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