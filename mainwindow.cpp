#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QLineEdit>
#include <QWidgetAction>
bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    QMenu *menu = qobject_cast<QMenu*>(obj);
    if (menu) {
        if (event->type() == QEvent::HoverEnter) {
            // Блокируем автоматическое открытие меню при наведении
            return true;
        }
        else if (event->type() == QEvent::MouseButtonPress) {
            // Открываем меню вручную по клику
            menu->popup(menu->parentWidget()->mapToGlobal(menu->pos()));
            return true; // событие обработано
        }
    }
    return QMainWindow::eventFilter(obj, event);
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QLineEdit *lineEdit = new QLineEdit(this);
    QWidgetAction *lineEditAction = new QWidgetAction(this);
    lineEditAction->setDefaultWidget(lineEdit);

    // Добавляем в меню Mail
    ui->menuMail->addAction(lineEditAction);
    // Перебираем все меню в menubar
    foreach (QAction *action, ui->menubar->actions()) {
        QMenu *menu = action->menu();
        if (menu) {
            menu->installEventFilter(this); // устанавливаем фильтр событий
        }
    }


}

MainWindow::~MainWindow()
{
    delete ui;
}
