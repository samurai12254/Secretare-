#ifndef SIMULATIONRUNWINDOW_H
#define SIMULATIONRUNWINDOW_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QTextEdit>
#include <QGroupBox>
#include <QTimer>
#include <QMessageBox>
#include "simulator.h"

class SimulationRunWindow : public QWidget
{
    Q_OBJECT

public:
    explicit SimulationRunWindow(Simulator* simulator, QWidget *parent = nullptr);

public slots:
    void handleNextStep();
    void updateDisplay();
    void handleStopSimulation();

signals:
    void simulationFinished();
    void returnToSettings();
    void handlePauseResume();

private:
    void setupUI();
    void updateStatistics();

    Simulator* now_simulator;
    QTimer* simulationTimer;
    bool isPaused;

    // UI элементы
    QLabel *timeLabel;
    QLabel *stepLabel;
    QLabel *statusLabel;
    QPushButton *nextStepButton;
    QPushButton *pauseResumeButton;
    QPushButton *stopButton;
    QTextEdit *logTextEdit;
    QTextEdit *statsTextEdit;
};

#endif // SIMULATIONRUNWINDOW_H