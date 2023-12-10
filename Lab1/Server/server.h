#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QTextBrowser>
#include <QMap>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFont>
#include <QPushButton>
#include <QTabWidget>
#include <utility>
#include <QScrollArea>

class User{
    unsigned int id;
    unsigned int index;
    QTcpSocket* socket;
public:
    User(QTcpSocket* socket, int id, int index);
    QTcpSocket* getSocket();
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

class Server : public QTcpServer
{
    Q_OBJECT
public:
    Server();
    void userKick(QTcpSocket* socket);
    void startServer(QWidget *serverWidget);
    void sendMessage(QString message);
    QLabel* receiveMessage(Data &data);
    void receiveGeneralMessage(Data &data);
    void receivePrivateMessage(Data &data);
public slots:
    void userNewConnection();
    void userSockReady();
    void userSocketDisconnected();
    void errorOccurred(QAbstractSocket::SocketError socketError);
private:
    QTabWidget *tabChats;
    QVBoxLayout *usersLayout;
    QTextBrowser *log;
    QFont font;
    QString styleServer;
    QString styleUser;
    QMap<QTcpSocket*, User*> users;
    int userIndex = 0;
    int userId = 1;
};

#endif // SERVER_H
