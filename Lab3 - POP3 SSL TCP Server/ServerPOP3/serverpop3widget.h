#ifndef SERVERPOP3WIDGET_H
#define SERVERPOP3WIDGET_H

#include <QWidget>
#include "serverpop3.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class ServerPOP3Widget;
}
QT_END_NAMESPACE

class ServerPOP3Widget : public QWidget
{
    Q_OBJECT

public:
    ServerPOP3Widget(QWidget *parent = nullptr);
    ~ServerPOP3Widget();

private:
    Ui::ServerPOP3Widget *ui;
};
#endif // SERVERPOP3WIDGET_H
