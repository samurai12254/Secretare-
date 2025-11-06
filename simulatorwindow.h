#ifndef SIMULATIONSETTINGSWINDOW_H
#define SIMULATIONSETTINGSWINDOW_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSpinBox>
#include <QComboBox>
#include <QCheckBox>
#include <QPushButton>
#include <QLabel>
#include <QGroupBox>
#include <QMessageBox>

class SimulatorWindow : public QWidget
{
    Q_OBJECT

public:
    explicit SimulatorWindow(QWidget *parent = nullptr);

    // Геттеры для получения параметров
    int getSimulationPeriod() const;
    QString getSimulationStep() const;
    bool getCalendarRemindersEnabled() const;
    bool getConflictResolutionEnabled() const;

signals:
    void simulationStarted(); // Сигнал о начале моделирования

private slots:
    void handleStartSimulation();
    void validateInput();

private:
    void setupUI();

    // UI элементы
    QSpinBox *periodSpinBox;
    QComboBox *stepComboBox;
    QCheckBox *remindersCheckBox;
    QCheckBox *conflictResolutionCheckBox;
    QPushButton *startButton;
};

#endif // SIMULATIONSETTINGSWINDOW_H