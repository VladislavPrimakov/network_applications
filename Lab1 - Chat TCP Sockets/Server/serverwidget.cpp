#include "serverwidget.h"
#include "ui_serverwidget.h"
#include "server.h"

ServerWidget::ServerWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ServerWidget)
{
    ui->setupUi(this);
    Server *server = new Server();
    server->startServer(this);
    this->server = server;
}

ServerWidget::~ServerWidget()
{
    delete ui;
}



void ServerWidget::on_EnterLine_clicked()
{
    QLineEdit *InputLine = this->findChild<QLineEdit*>("InputLine");
    QString message = InputLine->text();
    InputLine->clear();
    this->server->sendMessage(message);
}

