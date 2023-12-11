#include "serverpop3widget.h"
#include "ui_serverpop3widget.h"

ServerPOP3Widget::ServerPOP3Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ServerPOP3Widget)
{
    ui->setupUi(this);
    auto server = new ServerPOP3(this);
    server->startServer();
}

ServerPOP3Widget::~ServerPOP3Widget()
{
    delete ui;
}
