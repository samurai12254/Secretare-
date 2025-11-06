#include "calendarwindow.h"
#include <QVBoxLayout>
#include <QCalendarWidget>
#include <QLabel>

CalendarWindow::CalendarWindow(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle("Calendar");

    QVBoxLayout *layout = new QVBoxLayout(this);
    QLabel *label = new QLabel("Select a date:", this);
    QCalendarWidget *calendar = new QCalendarWidget(this);

    layout->addWidget(label);
    layout->addWidget(calendar);
}
