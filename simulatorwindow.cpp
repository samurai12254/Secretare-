#include "simulatorwindow.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>

SimulatorWindow::SimulatorWindow(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle("Simulator");

    QVBoxLayout *layout = new QVBoxLayout(this);
    QLabel *label = new QLabel("Simulation Parameters:", this);
    QLineEdit *input = new QLineEdit(this);
    QPushButton *startBtn = new QPushButton("Start Simulation", this);

    layout->addWidget(label);
    layout->addWidget(input);
    layout->addWidget(startBtn);
}
