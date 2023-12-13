#ifndef POP3PROXY_H
#define POP3PROXY_H

#include <QSslServer>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QTextBrowser>
#include <QLineEdit>
#include <QNetworkProxy>
#include <QScrollArea>
#include <QPushButton>
#include <QMap>
#include <QByteArray>
#include <QFile>
#include <QSslCertificate>
#include <QSslKey>

enum TypeCommands{
    receiveFromClient,
    transmitToClient,
    receiveFromServer,
    transmitToServer,
};

class User{
public:
    User();
    ~User();
    User& operator=(const User& other);

    void setClientSocketClosed(bool s);
    bool getClientSocketClosed();
    void setServerSocketClosed(bool s);
    bool getServerSocketClosed();
    void setClientSocket(QSslSocket *s);
    QSslSocket* getClientSocket();
    void setServerSocket(QSslSocket *s);
    QSslSocket* getServerSocket();
    void setIndex(int index);
    int getIndex();
    void setAdr(QString adr_);
    QHostAddress* getAdr();
    void setAdrLabel(QLabel *l);
    void setPort(int port_);
    int getPort();
    void setLogTextBrowser(QTextBrowser *tb);
    void LogMessage(const QString s, TypeCommands t);
    void reSendToClient(const QByteArray &ba);
    void reSendToServer(const QByteArray &ba);
    void setAdr(QString adress_, QString port_);
    void cPROXYREADY();
    void cOKPRXA();
    void cERRPRXA();
private:
    QSslSocket *client_socket;
    QSslSocket *server_socket;
    QLabel *adrLabel;
    int index;
    QTextBrowser *log;
    QHostAddress *adr;
    int port;
    bool clientSocketClosed;
    bool serverSocketClosed;
};

class POP3Proxy : public QSslServer
{
    Q_OBJECT

public:
    POP3Proxy(QWidget *w);
    void showStartForm();
    void startProxy();
    void HideLayout(QLayout *l);
    void HideUserLayout(QSslSocket *socket);
    void LogMessage(const QString s);
    void userNewConnection();
    void userSocketReady();
    void userSocketDisconnected();
    void serverSocketReady();
    void serverSocketDisconnected();


private:
    QMap<QSslSocket*, User*> usersClient;
    QMap<QSslSocket*, User*> usersServer;
    QTextBrowser *log;
    QFont *font;
    QVBoxLayout *mainLayout;
    QVBoxLayout *usersLayout;
    QVBoxLayout *startLayout;
    QLineEdit *inputHostName;
    QLineEdit *inputHostPort;
    QSslConfiguration sslConf;
    int index = 0;
};

#endif // POP3PROXY_H
