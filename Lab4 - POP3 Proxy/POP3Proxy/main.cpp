#include "pop3proxywidget.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    POP3ProxyWidget w;
    w.show();
    return a.exec();
}
