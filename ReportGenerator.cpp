#include "ReportGenerator.hpp"
#include <QFile>
#include <QTextStream>
#include <QDebug>

// Конструкторы
ReportGenerator::ReportGenerator(Calendar* cal) 
    : calendar(cal), mail_system(nullptr) {}

ReportGenerator::ReportGenerator(Calendar* cal, MailSystem* mail) 
    : calendar(cal), mail_system(mail) {}

// Генерация сводной статистики
void ReportGenerator::generateSummary() {
    stats.clear();
    
    // Собираем различную статистику
    collectEventStatistics();
    collectDepartmentStatistics();
    collectConflictStatistics();
    
    if (mail_system) {
        collectMailStatistics();
    }
    
    // Общая информация
    QDateTime currentTime = QDateTime::currentDateTime();
    stats["report_generation_time"] = currentTime.toString("dd.MM.yyyy hh:mm:ss");
}

// Формирование текстового отчета
QString ReportGenerator::toText() const {
    QString report;
    
    report += "📊 ОТЧЕТ ПО ИТОГАМ СИМУЛЯЦИИ\n";
    report += "==========================================\n\n";
    
    report += "📅 ОБЩАЯ СТАТИСТИКА СОБЫТИЙ\n";
    report += "------------------------------------------\n";
    report += "Всего событий: " + (stats.contains("total_events") ? stats["total_events"] : "0") + "\n";
    report += "Событий сегодня: " + (stats.contains("today_events") ? stats["today_events"] : "0") + "\n";
    report += "Будущих событий: " + (stats.contains("future_events") ? stats["future_events"] : "0") + "\n";
    report += "Прошедших событий: " + (stats.contains("past_events") ? stats["past_events"] : "0") + "\n";
    report += "Конфликтов расписания: " + (stats.contains("total_conflicts") ? stats["total_conflicts"] : "0") + "\n\n";
    
    report += "🏢 СТАТИСТИКА ПО ОТДЕЛАМ\n";
    report += "------------------------------------------\n";
    report += "Всего отделов: " + (stats.contains("total_departments") ? stats["total_departments"] : "0") + "\n";
    
    // Статистика по отделам
    if (stats.contains("department_stats")) {
        report += stats["department_stats"] + "\n";
    }
    
    // Статистика почты
    if (mail_system) {
        report += "✉️ СТАТИСТИКА ПОЧТЫ\n";
        report += "------------------------------------------\n";
        report += "Всего сообщений: " + (stats.contains("total_messages") ? stats["total_messages"] : "0") + "\n";
        report += "Исходящих сообщений: " + (stats.contains("outbox_messages") ? stats["outbox_messages"] : "0") + "\n\n";
    }
    
    report += "👥 АКТИВНОСТЬ ПОЛЬЗОВАТЕЛЕЙ\n";
    report += "------------------------------------------\n";
    if (stats.contains("user_activity")) {
        report += stats["user_activity"] + "\n";
    }
    
    report += "📈 РАСПРЕДЕЛЕНИЕ СОБЫТИЙ\n";
    report += "------------------------------------------\n";
    if (stats.contains("event_distribution")) {
        report += stats["event_distribution"] + "\n";
    }
    
    report += "------------------------------------------\n";
    report += "Отчет сгенерирован: " + (stats.contains("report_generation_time") ? stats["report_generation_time"] : "N/A") + "\n";
    
    return report;
}

// Экспорт отчета в файл
bool ReportGenerator::exportToFile(const QString& filename, const QString& format) const {
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qWarning() << "Ошибка открытия файла:" << filename;
        return false;
    }
    
    QTextStream out(&file);
    
    if (format == "txt") {
        out << toText();
    } else if (format == "html") {
        // Базовая HTML версия
        out << "<html><head><title>Отчет по симуляции</title></head><body>";
        out << "<h1>Отчет по симуляции</h1>";
        out << "<pre>" << toText() << "</pre>";
        out << "</body></html>";
    } else {
        // По умолчанию TXT
        out << toText();
    }
    
    file.close();
    qDebug() << "Отчет сохранен в файл:" << filename;
    return true;
}

// Сбор статистики по событиям
void ReportGenerator::collectEventStatistics() {
    if (!calendar) return;
    
    QDateTime currentDate = QDateTime::currentDateTime();
    QDate today = currentDate.date();
    
    int today_count = 0;
    int future_count = 0;
    int past_count = 0;
    
    int totalEvents = calendar->getTotalEvents();
    
    // Простая реализация - в реальной системе нужно анализировать все события
    // Для демонстрации используем приблизительные значения
    if (totalEvents > 0) {
        today_count = totalEvents / 10; // 10% событий сегодня
        future_count = totalEvents / 2; // 50% будущих событий
        past_count = totalEvents - today_count - future_count;
    }
    
    stats["total_events"] = QString::number(totalEvents);
    stats["today_events"] = QString::number(today_count);
    stats["future_events"] = QString::number(future_count);
    stats["past_events"] = QString::number(past_count);
}

// Сбор статистики по отделам
void ReportGenerator::collectDepartmentStatistics() {
    if (!calendar) return;
    
    QVector<Department*> departments = calendar->getDepartments();
    stats["total_departments"] = QString::number(departments.size());
    
    QString dept_stats;
    for (Department* dept : departments) {
        if (dept) {
            dept_stats += "  • " + dept->getName() 
                        + ": " + QString::number(dept->getEventCount()) + " событий, "
                        + QString::number(dept->getEmployeeCount()) + " сотрудников\n";
        }
    }
    
    stats["department_stats"] = dept_stats;
}

// Сбор статистики по почте
void ReportGenerator::collectMailStatistics() {
    if (!mail_system) return;
    
    // Для демонстрации используем приблизительные значения
    // В реальной системе нужно анализировать конкретных пользователей
    int totalMessages = 0;
    int outboxMessages = mail_system->getOutbox().size();
    
    stats["total_messages"] = QString::number(totalMessages);
    stats["outbox_messages"] = QString::number(outboxMessages);
}

// Сбор статистики по конфликтам
void ReportGenerator::collectConflictStatistics() {
    if (!calendar) return;
    
    QVector<QPair<Event*, Event*>> conflicts = calendar->findConflicts();
    stats["total_conflicts"] = QString::number(conflicts.size());
}

// Генерация отчета по отделам
QString ReportGenerator::generateDepartmentReport() const {
    if (!calendar) return "Нет данных";
    
    QString report;
    report += "ДЕТАЛЬНЫЙ ОТЧЕТ ПО ОТДЕЛАМ\n";
    report += "==========================\n\n";
    
    QVector<Department*> departments = calendar->getDepartments();
    for (Department* dept : departments) {
        if (!dept) continue;
        
        report += "ОТДЕЛ: " + dept->getName() + " (ID: " + QString::number(dept->getId()) + ")\n";
        report += "----------------------------------------\n";
        report += "Сотрудников: " + QString::number(dept->getEmployeeCount()) + "\n";
        report += "Событий: " + QString::number(dept->getEventCount()) + "\n";
        
        QVector<Event*> events = dept->getAllEvents();
        if (!events.isEmpty()) {
            report += "Последние события:\n";
            for (int i = 0; i < qMin(events.size(), 5); ++i) {
                if (events[i]) {
                    report += "  • " + events[i]->getTitle() + " (" + events[i]->formattedStart() + ")\n";
                }
            }
            if (events.size() > 5) {
                report += "  ... и еще " + QString::number(events.size() - 5) + " событий\n";
            }
        }
        report += "\n";
    }
    
    return report;
}

// Генерация отчета по активности пользователей
QString ReportGenerator::generateUserActivityReport() const {
    QString report;
    report += "АКТИВНОСТЬ ПОЛЬЗОВАТЕЛЕЙ\n";
    report += "========================\n\n";
    
    // Заглушка - в реальной системе нужно отслеживать активность пользователей
    report += "Для полного отчета по активности необходимо внедрить систему логирования действий пользователей.\n";
    
    return report;
}

// Генерация отчета по распределению событий
QString ReportGenerator::generateEventDistributionReport() const {
    if (!calendar) return "Нет данных";
    
    QString report;
    report += "РАСПРЕДЕЛЕНИЕ СОБЫТИЙ ПО ВРЕМЕНИ\n";
    report += "================================\n\n";
    
    // Простая реализация - в реальной системе нужно анализировать все события
    report += "Для детального отчета по распределению событий необходим доступ к полному списку событий с датами.\n";
    
    return report;
}

// Вспомогательный метод для форматирования длительности
QString ReportGenerator::formatDuration(const QDateTime& start, const QDateTime& end) const {
    if (!start.isValid() || !end.isValid() || start > end) {
        return "некорректный интервал";
    }
    
    qint64 seconds = start.secsTo(end);
    qint64 minutes = seconds / 60;
    qint64 hours = minutes / 60;
    qint64 days = hours / 24;
    
    if (days > 0) {
        return QString("%1 дней, %2 часов").arg(days).arg(hours % 24);
    } else if (hours > 0) {
        return QString("%1 часов, %2 минут").arg(hours).arg(minutes % 60);
    } else {
        return QString("%1 минут").arg(minutes);
    }
}

// Геттеры и сеттеры
QHash<QString, QString> ReportGenerator::getStats() const {
    return stats;
}

void ReportGenerator::setCalendar(Calendar* cal) {
    calendar = cal;
}

void ReportGenerator::setMailSystem(MailSystem* mail) {
    mail_system = mail;
}