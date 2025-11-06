#include "mailwindow.h"
#include <QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QTextEdit>
#include <QLabel>

MailWindow::MailWindow(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle("Mail");

    QVBoxLayout *layout = new QVBoxLayout(this);

    QLabel *toLabel = new QLabel("To:", this);
    QLineEdit *toField = new QLineEdit(this);

    QLabel *subjectLabel = new QLabel("Subject:", this);
    QLineEdit *subjectField = new QLineEdit(this);

    QLabel *bodyLabel = new QLabel("Message:", this);
    QTextEdit *bodyField = new QTextEdit(this);

    QPushButton *sendBtn = new QPushButton("Send", this);

    layout->addWidget(toLabel);
    layout->addWidget(toField);
    layout->addWidget(subjectLabel);
    layout->addWidget(subjectField);
    layout->addWidget(bodyLabel);
    layout->addWidget(bodyField);
    layout->addWidget(sendBtn);
}
