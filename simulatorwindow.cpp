#include "simulatorwindow.h"
#include <QPushButton>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QLabel>

SimulatorWindow::SimulatorWindow(QWidget *parent) : QWidget(parent)
{
    setWindowTitle("Simulator");

    QVBoxLayout *layout = new QVBoxLayout(this);

    QLabel *label = new QLabel("Simulator Input:", this);
    QLineEdit *input = new QLineEdit(this);
    QPushButton *button = new QPushButton("Start", this);

    layout->addWidget(label);
    layout->addWidget(input);
    layout->addWidget(button);

    setLayout(layout);
}
