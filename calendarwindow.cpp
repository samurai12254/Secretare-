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
#include <QComboBox>

CalendarWindow::CalendarWindow(QWidget *parent)
    : QWidget(parent)
{

}
QVector<Event> CalendarWindow::getALLEvents(){
    return events;
}
QVector<Event> CalendarWindow::getAllStartEvents(const QDate& start, const QDate& finish) const
{
    QVector<Event> result;
    // Для удобства создаём отсортированные диапазоны дат
    QDate startDate = start <= finish ? start : finish;
    QDate endDate   = start <= finish ? finish : start;
    for (Event ev : events) {
        QDate eventStartDate = ev.getStartTime().date();

        // Проверяем: дата начала события попадает в диапазон [startDate; endDate]
        if (eventStartDate >= startDate && eventStartDate <= endDate) {
            result.push_back(ev);
        }
    }
    return result;
}
void CalendarWindow::DellPassEvents(QDateTime& finish){
    QVector <Event> GoodEvents;
    for(Event ev : events){
        QDateTime eventFinishDate = ev.getEndTime();
        if(eventFinishDate > finish){
            GoodEvents.push_back(ev);
        }
    }
    events = GoodEvents;
}
CalendarWindow::CalendarWindow(QVector<User*> *users, QWidget *parent)
    : QWidget(parent), allUsers(users)
{
}
bool CalendarWindow::hasConflict(const Event& newEvent, const Event* ignoreEvent)
{
    for (const Event& existing : events)
    {
        // Если редактируем — игнорируем само себя
        if (ignoreEvent && existing.getId() == ignoreEvent->getId())
            continue;

        // --- Проверяем пересечение по времени ---
        bool overlap = newEvent.getStartTime() < existing.getEndTime() &&
                       newEvent.getEndTime() > existing.getStartTime();

        if (!overlap)
            continue;

        // --- Проверяем участников ---
        for (User* newUser : newEvent.getParticipants())
        {
            if (!newUser) continue;
            for (User* oldUser : existing.getParticipants())
            {
                if (!oldUser) continue;
                if (newUser->GetId() == oldUser->GetId())
                {
                    QMessageBox::warning(
                        this,
                        "Conflict Detected",
                        QString("Пользователь '%1' уже участвует в событии '%2' (%3 - %4)")
                            .arg(newUser->GetLogin())
                            .arg(existing.getTitle())
                            .arg(existing.getStartTime().time().toString("HH:mm"))
                            .arg(existing.getEndTime().time().toString("HH:mm"))
                        );
                    return true;
                }
            }
        }

        // --- Проверяем место ---
        if (newEvent.getLocation() != nullptr && existing.getLocation() != nullptr &&
            newEvent.getLocation() == existing.getLocation())
        {
            QMessageBox::warning(
                this,
                "Location Conflict",
                QString("Место '%1' уже занято другим событием '%2' (%3 - %4)")
                    .arg(newEvent.getLocation())
                    .arg(existing.getTitle())
                    .arg(existing.getStartTime().time().toString("HH:mm"))
                    .arg(existing.getEndTime().time().toString("HH:mm"))
                );
            return true;
        }
    }

    return false; // ✅ Нет конфликтов
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
bool CalendarWindow::runAuthorization()
{
    QString login = QInputDialog::getText(this, "Авторизация", "Логин:");
    if (login.isEmpty()) return false;

    QString pass = QInputDialog::getText(this, "Авторизация", "Пароль:", QLineEdit::Password);
    if (pass.isEmpty()) return false;

    for (User* u : *allUsers)
    {
        if (u->GetLogin() == login && u->CheckPassword(pass))
        {
            currentUser = u;
            // Если UI ещё не создан, создаём
            if (!uiBuilt) buildCalendarUI();
            return true;
        }
    }

    QMessageBox::warning(this, "Ошибка", "Неверный логин или пароль!");
    return false;
}

void CalendarWindow::buildCalendarUI()
{
    uiBuilt = true;
    setWindowTitle("Calendar");
    QVBoxLayout *layout = new QVBoxLayout(this);

    QLabel *label = new QLabel("Select a date:", this);
    calendar = new QCalendarWidget(this);

    layout->addWidget(label);
    layout->addWidget(calendar);

    highlightDates();

    connect(calendar, &QCalendarWidget::clicked, this, &CalendarWindow::onDateClicked);
}


void CalendarWindow::onDateClicked(const QDate &date)
{
    QDialog *dialog = new QDialog(this);
    dialog->setAttribute(Qt::WA_DeleteOnClose); // автоматически удаляется при закрытии
    dialog->setWindowTitle("Events on " + date.toString());
    QVBoxLayout *layout = new QVBoxLayout(dialog);

    bool hasEvents = false;

    // Перебираем все события
    Event need_ev;
    for ( Event &ev : events)
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
            if (currentUser->GetRole() == "Сотрудник") {
                editBtn->setVisible(false);
                deleteBtn->setVisible(false);
            }
            else if (currentUser->GetRole() == "Секретарь") {
                deleteBtn->setVisible(false);
            }
            eventLayout->addWidget(info);
            eventLayout->addWidget(editBtn);
            eventLayout->addWidget(deleteBtn);
            layout->addWidget(eventWidget);
            need_ev = ev;
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

                // --- Participants ---
                QLabel *participantsLabel = new QLabel("Participants (comma separated):", &editDialog);
                QLineEdit *participantsEdit = new QLineEdit(&editDialog);

                // заполняем строку логинов обратно
                QStringList logins;
                for (User* u : ev.getParticipants()) {
                    if (u) logins.append(u->GetLogin());
                }
                participantsEdit->setText(logins.join(", "));

                editLayout->addWidget(participantsLabel);
                editLayout->addWidget(participantsEdit);

                // --- Duration ---
                int oldDuration = ev.getStartTime().secsTo(ev.getEndTime()) / 60;
                QLabel *durLabel = new QLabel("Duration (minutes):", &editDialog);
                QLineEdit *durEdit = new QLineEdit(QString::number(oldDuration), &editDialog);
                editLayout->addWidget(durLabel);
                editLayout->addWidget(durEdit);
                // --- Важность ---
                QLabel *importanceLabel = new QLabel("Importance:", &editDialog);
                QComboBox *importanceBox = new QComboBox(&editDialog);
                importanceBox->addItems({"Низкая", "Средняя", "Высокая"});
                importanceBox->setCurrentText(ev.getImportance());
                editLayout->addWidget(importanceLabel);
                editLayout->addWidget(importanceBox);

                // --- Локация ---
                QLabel *locationLabel = new QLabel("Location:", &editDialog);
                QLineEdit *locationEdit = new QLineEdit(&editDialog);
                locationEdit->setText(ev.getLocation());
                editLayout->addWidget(locationLabel);
                editLayout->addWidget(locationEdit);

                QHBoxLayout *buttons = new QHBoxLayout();
                QPushButton *okBtn = new QPushButton("Save", &editDialog);
                QPushButton *cancelBtn = new QPushButton("Cancel", &editDialog);
                buttons->addWidget(okBtn);
                buttons->addWidget(cancelBtn);
                editLayout->addLayout(buttons);

                connect(okBtn, &QPushButton::clicked, &editDialog, &QDialog::accept);
                connect(cancelBtn, &QPushButton::clicked, &editDialog, &QDialog::reject);
                bool add_event = 0;
                while (!add_event && editDialog.exec() == QDialog::Accepted) {

                    Event newEvent;
                    newEvent.setTitle(titleEdit->text());
                    newEvent.setDescription(descEdit->text());

                    QDateTime newStart(date, timeEdit->time());
                    newEvent.setStartTime(newStart);

                    bool okDur;
                    int newDuration = durEdit->text().toInt(&okDur);
                    if (!okDur || newDuration <= 0) newDuration = 60;
                    newEvent.setEndTime(newStart.addSecs(newDuration * 60));
                    QString importance = importanceBox->currentText();
                    QString locName = locationEdit->text().trimmed();


                    newEvent.setImportance(importance);
                    newEvent.setLocation(locName);

                    newEvent.clearParticipants();
                    newEvent.setParticipantsFromString(participantsEdit->text(), *allUsers);

                    // Проверка конфликтов
                    if (newEvent.getTitle().isEmpty()) {
                        QMessageBox::warning(this, "Error", "Заголовок пуст!");
                        continue;
                    }else if(newEvent.getDescription().isEmpty()){
                        QMessageBox::warning(this, "Error", "Описание пусто");
                        continue;
                    }else if(locName.isEmpty()){
                        QMessageBox::warning(this, "Error", "Не указана локация проведения");
                        continue;
                    }
                    if (hasConflict(newEvent, &need_ev)) {
                        continue; // снова показать окно
                    }

                    // Обновляем существующее событие
                    for (Event &editable : events)
                    {
                        if (editable.getId() == eventId)
                        {
                            editable = newEvent; // или присвоить по полям
                            break;
                        }
                    }

                    add_event = true;
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
    // --- Важность ---
    QLabel *importanceLabel = new QLabel("Importance:", &dialog);
    QComboBox *importanceBox = new QComboBox(&dialog);
    importanceBox->addItems({"Низкая", "Средняя", "Высокая"});
    importanceBox->setCurrentText("Средняя");
    mainLayout->addWidget(importanceLabel);
    mainLayout->addWidget(importanceBox);

    // --- Локация ---
    QLabel *locationLabel = new QLabel("Location:", &dialog);
    QLineEdit *locationEdit = new QLineEdit(&dialog);
    mainLayout->addWidget(locationLabel);
    mainLayout->addWidget(locationEdit);

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
    bool add_event = 0;
    while (!add_event && dialog.exec() == QDialog::Accepted){
        QString title = titleEdit->text().trimmed();
        QString desc = descEdit->text().trimmed();
        QString participantsStr = participantsEdit->text().trimmed();
        QTime startTime = timeEdit->time();

        if (title.isEmpty()) {
            QMessageBox::warning(this, "Error", "Заголовок пуст!");
            return;
        }else if(desc.isEmpty()){
            QMessageBox::warning(this, "Error", "Описание пусто");
            return;
        }
        // --- Создаём событие ---
        bool ok;
        int durationMinutes = durationEdit->text().toInt(&ok);
        if (!ok || durationMinutes <= 0) durationMinutes = 60; // default 60

        QDateTime start(date, startTime);
        QDateTime end = start.addSecs(durationMinutes * 60); // вычисляем end

        QString importance = importanceBox->currentText();
        QString locName = locationEdit->text().trimmed();
        if(locName.isEmpty()){
            QMessageBox::warning(this, "Error", "Не указана локация проведения");
            return;
        }
        Event newEvent(
            title,
            locName,
            start,
            end,
            participantsStr,
            *allUsers,
            importance,
            desc
            );

        if (hasConflict(newEvent)) {
            continue; // Не добавляем, если нашли пересечение
        }


        events.append(newEvent);
        highlightDates();
        add_event = 1;
    }
}
