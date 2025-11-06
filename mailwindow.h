#ifndef MAILWINDOW_H
#define MAILWINDOW_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include <QLabel>
#include <QListWidget>
#include <QMessageBox>
#include <QMap>
#include <QList>
#include <QStackedWidget>
#include "MailSystem.hpp"
#include "User.hpp"


class MailWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MailWindow(MailSystem* MSystem_, QWidget *parent = nullptr);

private slots:
    void handleLogin();
    void handleLogout();
    void handleSendMessage();
    void handleInboxSelection();

private:
    void setupLoginUI();
    void setupMailUI();
    void showError(const QString &message);
    void clearMailFields();
    
    // UI элементы
    QStackedWidget *stack;
    QWidget *loginPage;
    QWidget *mailPage;
    
    // Логин элементы
    QLineEdit *usernameInput;
    QPushButton *loginButton;
    
    // Почта элементы
    QLabel *userInfoLabel;
    QPushButton *logoutButton;
    QLineEdit *recipientInput;
    QLineEdit *subjectInput;
    QTextEdit *messageInput;
    QPushButton *sendButton;
    QListWidget *inboxList;
    QTextEdit *messageDisplay;
    
    // Данные
    QString currentUser;
    MailSystem* MSystem;
    QHash<QString, User*> Users;
};

#endif // MAILWINDOW_H