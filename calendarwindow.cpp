#include "calendarwindow.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QDialog>
#include <QInputDialog>
#include <QTextCharFormat>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QTimeEdit>
#include <QPushButton>
#include <QMessageBox>

CalendarWindow::CalendarWindow(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle("Calendar");
    QVBoxLayout *layout = new QVBoxLayout(this);

    QLabel *label = new QLabel("Select a date:", this);
    calendar = new QCalendarWidget(this);

    layout->addWidget(label);
    layout->addWidget(calendar);

    // пример: тестовое событие
    QDateTime now = QDateTime::currentDateTime();
    events.append(Event("1", "Meeting", nullptr, now, now.addSecs(3600), {}, "Средняя", "Обсудить проект"));

    highlightDates();

    connect(calendar, &QCalendarWidget::clicked, this, &CalendarWindow::onDateClicked);
}

void CalendarWindow::highlightDates()
{
    QTextCharFormat fmt;
    fmt.setBackground(Qt::yellow);

    QSet<QDate> dates;
    for(const Event &ev : events) {
        dates.insert(ev.getStartTime().date());
    }

    for(const QDate &d : dates) {
        calendar->setDateTextFormat(d, fmt);
    }
}

void CalendarWindow::onDateClicked(const QDate &date)
{
    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle("Events on " + date.toString());
    QVBoxLayout *layout = new QVBoxLayout(dialog);

    bool hasEvents = false;
    for(const Event &ev : events) {
        if(ev.getStartTime().date() == date) {
            layout->addWidget(new QLabel(ev.summary(), dialog));
            hasEvents = true;
        }
    }

    if(!hasEvents) {
        layout->addWidget(new QLabel("No events", dialog));
    }

    QPushButton *addBtn = new QPushButton("Add Event", dialog);
    layout->addWidget(addBtn);

    connect(addBtn, &QPushButton::clicked, [=]() {
        dialog->close();
        addEvent(date);
    });

    dialog->exec();
}

void CalendarWindow::addEvent(const QDate &date)
{
    // Создаём диалог для ввода нового события
    QDialog dialog(this);
    dialog.setWindowTitle("Add New Event");

    QVBoxLayout *mainLayout = new QVBoxLayout(&dialog);

    // --- Название события ---
    QLabel *titleLabel = new QLabel("Title:", &dialog);
    QLineEdit *titleEdit = new QLineEdit(&dialog);
    mainLayout->addWidget(titleLabel);
    mainLayout->addWidget(titleEdit);

    // --- Описание события ---
    QLabel *descLabel = new QLabel("Description:", &dialog);
    QLineEdit *descEdit = new QLineEdit(&dialog);
    mainLayout->addWidget(descLabel);
    mainLayout->addWidget(descEdit);

    // --- Время события ---
    QLabel *timeLabel = new QLabel("Start Time:", &dialog);
    QTimeEdit *timeEdit = new QTimeEdit(QTime::currentTime(), &dialog);
    timeEdit->setDisplayFormat("HH:mm");
    mainLayout->addWidget(timeLabel);
    mainLayout->addWidget(timeEdit);

    // --- Кнопки OK / Cancel ---
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    QPushButton *okBtn = new QPushButton("OK", &dialog);
    QPushButton *cancelBtn = new QPushButton("Cancel", &dialog);
    buttonLayout->addWidget(okBtn);
    buttonLayout->addWidget(cancelBtn);
    mainLayout->addLayout(buttonLayout);

    // --- Подключаем кнопки ---
    connect(okBtn, &QPushButton::clicked, &dialog, &QDialog::accept);
    connect(cancelBtn, &QPushButton::clicked, &dialog, &QDialog::reject);

    // Показываем диалог модально
    if(dialog.exec() == QDialog::Accepted)
    {
        QString title = titleEdit->text().trimmed();
        QString desc  = descEdit->text().trimmed();
        QTime startTime = timeEdit->time();

        if(title.isEmpty()) {
            QMessageBox::warning(this, "Error", "Title cannot be empty!");
            return;
        }

        QDateTime start(date, startTime);
        QDateTime end = start.addSecs(3600); // по умолчанию 1 час

        events.append(Event("", title, nullptr, start, end, {}, "Средняя", desc));

        highlightDates(); // обновляем подсветку
    }
}
