#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include "User.hpp"

class LoginDialog : public QDialog {
    Q_OBJECT

public:
    explicit LoginDialog(QVector<User*> *allUsers, QWidget *parent = nullptr);

    User* getLoggedUser() const { return loggedUser; }

private:
    QVector<User*> *usersList;
    User *loggedUser = nullptr;

    QLineEdit *loginEdit;
    QLineEdit *passEdit;
};
#endif // LOGINDIALOG_H
