#include "logindialog.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QMessageBox>

LoginDialog::LoginDialog(QVector<User*> *allUsers, QWidget *parent)
    : QDialog(parent), usersList(allUsers)
{
    setWindowTitle("Login");

    QVBoxLayout *layout = new QVBoxLayout(this);

    layout->addWidget(new QLabel("Login:"));
    loginEdit = new QLineEdit(this);
    layout->addWidget(loginEdit);

    layout->addWidget(new QLabel("Password:"));
    passEdit = new QLineEdit(this);
    passEdit->setEchoMode(QLineEdit::Password);
    layout->addWidget(passEdit);

    QPushButton *loginBtn = new QPushButton("Log in", this);
    layout->addWidget(loginBtn);

    connect(loginBtn, &QPushButton::clicked, this, [=]() {
        QString login = loginEdit->text().trimmed();
        QString pass  = passEdit->text().trimmed();

        // ищем пользователя
        for (User *u : *usersList) {
            if (u->GetLogin() == login) {
                if (u->CheckPassword(pass)) {
                    loggedUser = u;
                    accept();
                    return;
                } else {
                    QMessageBox::warning(this, "Error", "Wrong password!");
                    return;
                }
            }
        }

        QMessageBox::warning(this, "Error", "User not found!");
    });
}
