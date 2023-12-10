#ifndef CLIENTWIDGET_H
#define CLIENTWIDGET_H

#include "qboxlayout.h"
#include <QWidget>
#include <QTcpSocket>
#include <QMessageBox>
#include <QDebug>
#include <QTextBrowser>
#include <QTabWidget>
#include <QScrollArea>

QT_BEGIN_NAMESPACE
namespace Ui { class ClientWidget; }
QT_END_NAMESPACE

class User{
    unsigned int id;
    unsigned int index;
public:
    User(int id, int index);
    unsigned int getId();
    unsigned int getIndex();
    void setIndex(unsigned int index);
};

enum Command{
    LogMessage = 1,
    AddUser = 2,
    RemoveUser = 3,
    SendGeneralMessage = 4,
    SendPrivateMessage = 5,
    SendUserId = 6
};

struct Data{
    Command command;
    QVector<QString> args;
    QString data;
    Data();
    Data(Command command, QVector<QString> args, QString data);
    Data(QByteArray &byteArray);
    QByteArray Serialize();
};

class ClientWidget : public QWidget
{
    Q_OBJECT

public:
    ClientWidget(QWidget *parent = nullptr);
    ~ClientWidget();
    QTcpSocket *socket;
    void addUser(unsigned int  id);
    void extracted(unsigned int &index_user);
    void removeUser(unsigned int id);
    QLabel* receiveMessage(Data &data);
    void receiveGeneralMessage(Data &data);
    void receivePrivateMessage(Data &data);
    void sendMessage(QString message);
    void receiveUserId(Data &data);
public slots:
    void sockReady();
    void sockDisc();
    void sockError(QAbstractSocket::SocketError socketError);
private slots:
    void on_pushButton_clicked();
    void on_EnterLine_clicked();
    void on_pushButton_2_clicked();

private:
    Ui::ClientWidget *ui;
    bool isConnect;
    QTextBrowser *log;
    QTabWidget *tabChats;
    QVBoxLayout *usersLayout;
    QFont font;
    QString styleServer;
    QString styleUser;
    unsigned int userIndex = 0;
    unsigned int userId;
    // map <userId, User>
    QMap<unsigned int, User*> users;
};
#endif // CLIENTWIDGET_H
