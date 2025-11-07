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
    //events.append(Event("1", "Meeting", nullptr, now, now.addSecs(3600), {}, "Средняя", "Обсудить проект"));

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
    dialog->setAttribute(Qt::WA_DeleteOnClose); // автоматически удаляется при закрытии
    dialog->setWindowTitle("Events on " + date.toString());
    QVBoxLayout *layout = new QVBoxLayout(dialog);

    bool hasEvents = false;

    // Перебираем все события
    for (const Event &ev : events)
    {
        if (ev.getStartTime().date() == date)
        {
            hasEvents = true;

            QWidget *eventWidget = new QWidget(dialog);
            QHBoxLayout *eventLayout = new QHBoxLayout(eventWidget);

            QLabel *info = new QLabel(
                QString("%1 — %2 (%3)")
                    .arg(ev.getStartTime().time().toString("HH:mm"))
                    .arg(ev.getTitle())
                    .arg(ev.getImportance()),
                eventWidget);

            QPushButton *editBtn = new QPushButton("Edit", eventWidget);
            QPushButton *deleteBtn = new QPushButton("Delete", eventWidget);

            eventLayout->addWidget(info);
            eventLayout->addWidget(editBtn);
            eventLayout->addWidget(deleteBtn);
            layout->addWidget(eventWidget);

            QString eventId = ev.getId(); // сохраняем ID события для лямбды

            // --- Редактирование события ---
            connect(editBtn, &QPushButton::clicked, this, [=]() {
                QDialog editDialog(dialog);
                editDialog.setWindowTitle("Edit Event");

                QVBoxLayout *editLayout = new QVBoxLayout(&editDialog);

                QLabel *titleLabel = new QLabel("Title:", &editDialog);
                QLineEdit *titleEdit = new QLineEdit(ev.getTitle(), &editDialog);
                editLayout->addWidget(titleLabel);
                editLayout->addWidget(titleEdit);

                QLabel *descLabel = new QLabel("Description:", &editDialog);
                QLineEdit *descEdit = new QLineEdit(ev.getDescription(), &editDialog);
                editLayout->addWidget(descLabel);
                editLayout->addWidget(descEdit);

                QLabel *timeLabel = new QLabel("Start Time:", &editDialog);
                QTimeEdit *timeEdit = new QTimeEdit(ev.getStartTime().time(), &editDialog);
                timeEdit->setDisplayFormat("HH:mm");
                editLayout->addWidget(timeLabel);
                editLayout->addWidget(timeEdit);

                QHBoxLayout *buttons = new QHBoxLayout();
                QPushButton *okBtn = new QPushButton("Save", &editDialog);
                QPushButton *cancelBtn = new QPushButton("Cancel", &editDialog);
                buttons->addWidget(okBtn);
                buttons->addWidget(cancelBtn);
                editLayout->addLayout(buttons);

                connect(okBtn, &QPushButton::clicked, &editDialog, &QDialog::accept);
                connect(cancelBtn, &QPushButton::clicked, &editDialog, &QDialog::reject);

                if (editDialog.exec() == QDialog::Accepted)
                {
                    for (Event &editable : events)
                    {
                        if (editable.getId() == eventId)
                        {
                            editable.setTitle(titleEdit->text());
                            editable.setDescription(descEdit->text());
                            QDateTime newStart(date, timeEdit->time());
                            editable.setStartTime(newStart);
                            editable.setEndTime(newStart.addSecs(3600));
                            break;
                        }
                    }

                    highlightDates();
                    dialog->close();
                    QMetaObject::invokeMethod(this, "onDateClicked", Qt::QueuedConnection, Q_ARG(QDate, date));
                }
            });

            // --- Удаление события ---
            connect(deleteBtn, &QPushButton::clicked, this, [this, dialog, date, eventId]() {
                // Удаляем по уникальному ID
                auto it = std::remove_if(events.begin(), events.end(),
                                         [&](const Event &e) { return e.getId() == eventId; });
                events.erase(it, events.end());

                highlightDates(); // обновляем подсветку

                // Закрываем текущий диалог и снова открываем список событий
                dialog->close();
                QMetaObject::invokeMethod(this, "onDateClicked", Qt::QueuedConnection, Q_ARG(QDate, date));
            });
        }
    }

    if (!hasEvents)
    {
        layout->addWidget(new QLabel("No events", dialog));
    }

    QPushButton *addBtn = new QPushButton("Add Event", dialog);
    layout->addWidget(addBtn);
    connect(addBtn, &QPushButton::clicked, this, [=]() {
        dialog->close();
        addEvent(date);
    });

    dialog->setLayout(layout);
    dialog->exec();
}



void CalendarWindow::addEvent(const QDate &date)
{
    // --- Создаём окно для добавления события ---
    QDialog dialog(this);
    dialog.setWindowTitle("Add New Event");

    QVBoxLayout *mainLayout = new QVBoxLayout(&dialog);

    // --- Название ---
    QLabel *titleLabel = new QLabel("Title:", &dialog);
    QLineEdit *titleEdit = new QLineEdit(&dialog);
    mainLayout->addWidget(titleLabel);
    mainLayout->addWidget(titleEdit);

    // --- Описание ---
    QLabel *descLabel = new QLabel("Description:", &dialog);
    QLineEdit *descEdit = new QLineEdit(&dialog);
    mainLayout->addWidget(descLabel);
    mainLayout->addWidget(descEdit);

    // --- Время ---
    QLabel *timeLabel = new QLabel("Start Time:", &dialog);
    QTimeEdit *timeEdit = new QTimeEdit(QTime::currentTime(), &dialog);
    timeEdit->setDisplayFormat("HH:mm");
    mainLayout->addWidget(timeLabel);
    mainLayout->addWidget(timeEdit);

    QLabel *durationLabel = new QLabel("Duration (minutes):", &dialog);
    QLineEdit *durationEdit = new QLineEdit(&dialog);
    durationEdit->setText("60"); // по умолчанию 60 минут
    mainLayout->addWidget(durationLabel);
    mainLayout->addWidget(durationEdit);

    // --- 🔹 Участники (через запятую) ---
    QLabel *participantsLabel = new QLabel("Participants (comma separated logins):", &dialog);
    QLineEdit *participantsEdit = new QLineEdit(&dialog);
    mainLayout->addWidget(participantsLabel);
    mainLayout->addWidget(participantsEdit);

    // --- Кнопки ---
    QHBoxLayout *buttons = new QHBoxLayout();
    QPushButton *okBtn = new QPushButton("OK", &dialog);
    QPushButton *cancelBtn = new QPushButton("Cancel", &dialog);
    buttons->addWidget(okBtn);
    buttons->addWidget(cancelBtn);
    mainLayout->addLayout(buttons);

    connect(okBtn, &QPushButton::clicked, &dialog, &QDialog::accept);
    connect(cancelBtn, &QPushButton::clicked, &dialog, &QDialog::reject);

    // --- Ожидаем ответ пользователя ---
    if (dialog.exec() == QDialog::Accepted)
    {
        QString title = titleEdit->text().trimmed();
        QString desc = descEdit->text().trimmed();
        QString participantsStr = participantsEdit->text().trimmed();
        QTime startTime = timeEdit->time();

        if (title.isEmpty()) {
            QMessageBox::warning(this, "Error", "Title cannot be empty!");
            return;
        }

        // --- Создаём событие ---
        bool ok;
        int durationMinutes = durationEdit->text().toInt(&ok);
        if (!ok || durationMinutes <= 0) durationMinutes = 60; // default 60

        QDateTime start(date, startTime);
        QDateTime end = start.addSecs(durationMinutes * 60); // вычисляем end

        // ✅ Используем новый конструктор с participantsString
        Event newEvent(
            title,
            nullptr,                // location (можно добавить позже)
            start,
            end,
            participantsStr,        // строка логинов
            allUsers,               // список всех пользователей
            "Средняя",
            desc
            );

        events.append(newEvent);

        highlightDates();
    }
}
