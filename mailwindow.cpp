#include "mailwindow.h"

MailWindow::MailWindow(MailSystem* MSystem_, QWidget *parent)
    : QWidget(parent), MSystem(MSystem_)
{
    setupLoginUI();
    setupMailUI();
    
    // Создаем стек для переключения между логином и почтой
    stack = new QStackedWidget(this);
    stack->addWidget(loginPage);
    stack->addWidget(mailPage);
    

    User* a = new User("1", "user1", "", "", "");
    User* b = new User("2", "user2", "", "", "");
    Users["user1"] = a;
    Users["user2"] = b;
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(stack);
    // Подключаем сигналы
    connect(loginButton, &QPushButton::clicked, this, &MailWindow::handleLogin);
    connect(sendButton, &QPushButton::clicked, this, &MailWindow::handleSendMessage);
    connect(inboxList, &QListWidget::itemSelectionChanged, this, &MailWindow::handleInboxSelection);
    connect(logoutButton, &QPushButton::clicked, this, &MailWindow::handleLogout);
    // Добавляем тестовых пользователей
    
}

void MailWindow::setupLoginUI()
{
    loginPage = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(loginPage);
    
    QLabel *titleLabel = new QLabel("Вход в почту");
    titleLabel->setAlignment(Qt::AlignCenter);
    
    usernameInput = new QLineEdit();
    usernameInput->setPlaceholderText("Имя пользователя");
    
    loginButton = new QPushButton("Войти");
    
    layout->addWidget(titleLabel);
    layout->addWidget(usernameInput);
    layout->addWidget(loginButton);
    layout->addStretch();
}


void MailWindow::setupMailUI()
{
    mailPage = new QWidget();
    QVBoxLayout *mainLayout = new QVBoxLayout(mailPage);
    
    // Заголовок с информацией о пользователе и кнопкой выхода
    QHBoxLayout *headerLayout = new QHBoxLayout();
    userInfoLabel = new QLabel("Почта");
    userInfoLabel->setAlignment(Qt::AlignLeft);
    logoutButton = new QPushButton("Выйти");
    headerLayout->addWidget(userInfoLabel);
    headerLayout->addStretch();
    headerLayout->addWidget(logoutButton);
    mainLayout->addLayout(headerLayout);
    
    // Форма отправки сообщения
    QHBoxLayout *recipientLayout = new QHBoxLayout();
    recipientLayout->addWidget(new QLabel("Кому:"));
    recipientInput = new QLineEdit();
    recipientLayout->addWidget(recipientInput);
    
    QHBoxLayout *subjectLayout = new QHBoxLayout();
    subjectLayout->addWidget(new QLabel("Тема:"));
    subjectInput = new QLineEdit();
    subjectLayout->addWidget(subjectInput);
    
    QLabel *messageLabel = new QLabel("Сообщение:");
    messageInput = new QTextEdit();
    messageInput->setMaximumHeight(100);
    
    sendButton = new QPushButton("Отправить");
    
    // Входящие сообщения
    QLabel *inboxLabel = new QLabel("Входящие:");
    inboxList = new QListWidget();
    messageDisplay = new QTextEdit();
    messageDisplay->setReadOnly(true);
    
    // Разделяем отправку и просмотр
    QFrame *separator = new QFrame();
    separator->setFrameShape(QFrame::HLine);
    separator->setFrameShadow(QFrame::Sunken);
    
    mainLayout->addLayout(recipientLayout);
    mainLayout->addLayout(subjectLayout);
    mainLayout->addWidget(messageLabel);
    mainLayout->addWidget(messageInput);
    mainLayout->addWidget(sendButton);
    mainLayout->addWidget(separator);
    mainLayout->addWidget(inboxLabel);
    mainLayout->addWidget(inboxList);
    mainLayout->addWidget(new QLabel("Просмотр сообщения:"));
    mainLayout->addWidget(messageDisplay);
}


void MailWindow::handleLogout()
{
    currentUser.clear();
    clearMailFields();
    stack->setCurrentWidget(loginPage);
    userInfoLabel->setText("Почта");
}

void MailWindow::handleLogin()
{
    QString username = usernameInput->text().trimmed();
    
    if (username.isEmpty()) {
        showError("Введите имя пользователя");
        return;
    }
    
    if (!Users.contains(username)) {
        showError("Пользователь не найден");
        return;
    }
    
    currentUser = username;
    stack->setCurrentWidget(mailPage);
    
    // Обновляем список входящих
    inboxList->clear();
    const QList<Message> &messages = MSystem->getInbox(Users[currentUser]);
    for (const Message &msg : messages) {
        inboxList->addItem(msg.getSubject() + " (от: " + msg.getSender() + ")");
    }
}

void MailWindow::handleSendMessage()
{
    QString recipient = recipientInput->text().trimmed();
    QString subject = subjectInput->text().trimmed();
    QString body = messageInput->toPlainText().trimmed();
    
    if (recipient.isEmpty() || subject.isEmpty() || body.isEmpty()) {
        showError("Заполните все поля");
        return;
    }
    
    if (!Users.contains(recipient)) {
        showError("Получатель не найден");
        return;
    }
    
    MSystem->sendMessage(Users[recipient], subject, body, currentUser);
    QMessageBox::information(this, "Успех", "Сообщение отправлено!");
    
    // Если отправили себе, обновляем список входящих
    if (recipient == currentUser) {
        inboxList->addItem(subject + " (от: " + currentUser + ")");
    }
}

void MailWindow::handleInboxSelection()
{
    int currentRow = inboxList->currentRow();
    if (currentRow >= 0 && currentRow < MSystem->getInbox(Users[currentUser]).size()) {
        const Message &selectedMessage = MSystem->getInbox(Users[currentUser])[currentRow];
        QString messageText = QString("От: %1\nКому: %2\nТема: %3\n\n%4")
                                .arg(selectedMessage.getSender())
                                .arg(selectedMessage.getReceiver())
                                .arg(selectedMessage.getSubject())
                                .arg(selectedMessage.getBody());
        messageDisplay->setText(messageText);
    }
}

void MailWindow::clearMailFields()
{
    recipientInput->clear();
    subjectInput->clear();
    messageInput->clear();
    inboxList->clear();
    messageDisplay->clear();
}


void MailWindow::showError(const QString &message)
{
    QMessageBox::warning(this, "Ошибка", message);
}