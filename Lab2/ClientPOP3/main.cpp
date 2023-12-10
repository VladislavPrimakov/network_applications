#include "clientpop3widget.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ClientPOP3Widget w;
    w.show();
    return a.exec();
}
