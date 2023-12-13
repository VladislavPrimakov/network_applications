#include "pop3proxywidget.h"
#include "ui_pop3proxywidget.h"

POP3ProxyWidget::POP3ProxyWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::POP3ProxyWidget)
{
    ui->setupUi(this);
    POP3Proxy *server = new POP3Proxy(this);
}

POP3ProxyWidget::~POP3ProxyWidget()
{
    delete ui;
}

