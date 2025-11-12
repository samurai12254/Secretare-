#include "simulationrunwindow.h"

SimulationRunWindow::SimulationRunWindow(Simulator* simulator, QWidget *parent)
    : QWidget(parent), now_simulator(simulator), isPaused(false)
{
    setupUI();
    updateDisplay();
}

void SimulationRunWindow::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // Заголовок
    QLabel *titleLabel = new QLabel("Выполнение симуляции");
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 16px; font-weight: bold; color: #2E8B57;");
    mainLayout->addWidget(titleLabel);

    // Группа информации о симуляции
    QGroupBox *infoGroup = new QGroupBox("Текущее состояние симуляции");
    QVBoxLayout *infoLayout = new QVBoxLayout(infoGroup);

    // Время и шаг
    QHBoxLayout *timeStepLayout = new QHBoxLayout();
    timeLabel = new QLabel();
    timeLabel->setStyleSheet("font-size: 14px; font-weight: bold;");
    stepLabel = new QLabel();
    stepLabel->setStyleSheet("font-size: 12px;");
    timeStepLayout->addWidget(timeLabel);
    timeStepLayout->addStretch();
    timeStepLayout->addWidget(stepLabel);
    infoLayout->addLayout(timeStepLayout);

    // Статус
    statusLabel = new QLabel("Симуляция запущена");
    statusLabel->setStyleSheet("font-size: 12px; color: #2E8B57;");
    infoLayout->addWidget(statusLabel);

    mainLayout->addWidget(infoGroup);

    // Группа управления
    QGroupBox *controlGroup = new QGroupBox("Управление симуляцией");
    QHBoxLayout *controlLayout = new QHBoxLayout(controlGroup);

    nextStepButton = new QPushButton("Следующий шаг");
    nextStepButton->setStyleSheet("QPushButton { background-color: #2196F3; color: white; font-weight: bold; padding: 8px; }");

    pauseResumeButton = new QPushButton("Пауза");
    pauseResumeButton->setStyleSheet("QPushButton { background-color: #FF9800; color: white; font-weight: bold; padding: 8px; }");

    stopButton = new QPushButton("Завершить");
    stopButton->setStyleSheet("QPushButton { background-color: #F44336; color: white; font-weight: bold; padding: 8px; }");

    controlLayout->addWidget(nextStepButton);
    controlLayout->addWidget(pauseResumeButton);
    controlLayout->addWidget(stopButton);

    mainLayout->addWidget(controlGroup);

    // Группа статистики
    QGroupBox *statsGroup = new QGroupBox("Статистика симуляции");
    QVBoxLayout *statsLayout = new QVBoxLayout(statsGroup);
    
    statsTextEdit = new QTextEdit();
    statsTextEdit->setReadOnly(true);
    statsTextEdit->setMaximumHeight(120);
    statsLayout->addWidget(statsTextEdit);
    
    mainLayout->addWidget(statsGroup);

    // Группа логов
    QGroupBox *logGroup = new QGroupBox("Лог симуляции");
    QVBoxLayout *logLayout = new QVBoxLayout(logGroup);
    
    logTextEdit = new QTextEdit();
    logTextEdit->setReadOnly(true);
    logLayout->addWidget(logTextEdit);
    
    mainLayout->addWidget(logGroup);

    // Таймер для автоматического обновления
    simulationTimer = new QTimer(this);
    simulationTimer->setInterval(1000); // Обновление каждую секунду

    // Подключаем сигналы
    connect(nextStepButton, &QPushButton::clicked, this, &SimulationRunWindow::handleNextStep);
    connect(pauseResumeButton, &QPushButton::clicked, this, &SimulationRunWindow::handlePauseResume);
    connect(stopButton, &QPushButton::clicked, this, &SimulationRunWindow::handleStopSimulation);
    connect(simulationTimer, &QTimer::timeout, this, &SimulationRunWindow::updateDisplay);

    // Запускаем таймер
    simulationTimer->start();
}

void SimulationRunWindow::handleNextStep()
{
    if (now_simulator->getCurrentTime() >= now_simulator->getEndTime()) {
        QMessageBox::information(this, "Симуляция завершена", "Достигнут конец периода симуляции.");
        emit simulationFinished();
        return;
    }

    // Выполняем один шаг симуляции
    now_simulator->stepSimulation();
    
    // Обновляем отображение
    updateDisplay();
    
    // Добавляем запись в лог
    logTextEdit->append(QString("[ШАГ] Время: %1")
                       .arg(now_simulator->getCurrentTime().toString("dd.MM.yyyy hh:mm:ss")));
}



void SimulationRunWindow::handleStopSimulation()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Завершение симуляции", 
                                "Вы уверены, что хотите завершить симуляцию?",
                                QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        now_simulator->run();
        emit simulationFinished();
    }
}

void SimulationRunWindow::updateDisplay()
{
    // Обновляем время
    timeLabel->setText(QString("Текущее время симуляции: <b>%1</b>")
                      .arg(now_simulator->getCurrentTime().toString("dd.MM.yyyy hh:mm:ss")));
    
    // Обновляем информацию о шаге
    QDateTime endTime = now_simulator->getEndTime();
    QDateTime currentTime = now_simulator->getCurrentTime();
    int totalSeconds = currentTime.secsTo(endTime);
    int days = totalSeconds / (24 * 3600);
    int hours = (totalSeconds % (24 * 3600)) / 3600;
    int minutes = (totalSeconds % 3600) / 60;
    
    stepLabel->setText(QString("Осталось: %1д %2ч %3м")
                      .arg(days).arg(hours).arg(minutes));
    
    // Обновляем статистику
    updateStatistics();
}

void SimulationRunWindow::updateStatistics()
{
    QString statsText = QString(
        "Сообщений отправлено: %1\n"
        "Событий обработано: %2\n"
        "Уникальных участников: %3\n"
        "Всего участников: %4")
        .arg(now_simulator->getTotalMessagesSent())
        .arg(now_simulator->getTotalEventsProcessed())
        .arg(now_simulator->getUniqueParticipantsCount())
        .arg(now_simulator->getTotalParticipantsCount());
    
    statsTextEdit->setText(statsText);
}