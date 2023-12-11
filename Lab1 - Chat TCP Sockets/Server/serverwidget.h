#ifndef SERVERWIDGET_H
#define SERVERWIDGET_H

#include <QWidget>
#include "server.h"

QT_BEGIN_NAMESPACE
namespace Ui { class ServerWidget; }
QT_END_NAMESPACE

class ServerWidget : public QWidget
{
    Q_OBJECT

public:
    ServerWidget(QWidget *parent = nullptr);
    ~ServerWidget();
private slots:

    void on_EnterLine_clicked();

private:
    Ui::ServerWidget *ui;
    Server *server;

};
#endif // SERVERWIDGET_H

