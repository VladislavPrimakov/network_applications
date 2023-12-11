#ifndef SERVERPOP3_H
#define SERVERPOP3_H

#include <QSslServer>
#include <QSslSocket>
#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QScrollArea>
#include <QFont>
#include <QTextBrowser>
#include <QMap>
#include <QVector>
#include <iostream>
#include <QQueue>
#include <QSslCertificate>
#include <QSslKey>
#include <QFile>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

enum Commands{
    USER,
    PASS,
    QUIT,
    STAT,
    RETR,
    DELE,
    RSET,
};

enum TypeCommands{
    receive,
    transmit,
};

class User{
public:
    User();
    ~User();
    User& operator=(const User& other);

    void setSocket(QSslSocket *s);
    QSslSocket* getSocket();
    void setName(QString &name);
    QString& getName();
    void setIndex(int index);
    int getIndex();
    bool getIsAuth();
    void setNameLabel(QLabel *l);
    void setPassLabel(QLabel *l);
    void setLogTextBrowser(QTextBrowser *tb);
    QJsonArray readDB();
    void writeDB(QJsonArray users);
    void LogMessage(const QString s, TypeCommands t);

    void cUSER(QString &name);
    void cPASS(QString &pass);
    void cSTAT();
    void cRETR(QString &number);
    void cDELE(QString &number);
    void cRSET();
    void cSERVERREADY();
    void cQUIT();
private:
    QString nameUser;
    QSslSocket *socket;
    int index;
    QVector<int> checkedMessages;
    QLabel *nameLabel;
    QLabel *passLabel;
    QTextBrowser *log;
    bool isAuth;
};

class ServerPOP3 : public QSslServer
{
    Q_OBJECT
public:
    ServerPOP3(QWidget *w);
    void LogMessage(const QString s);
    QSslConfiguration sslConfiguration() const;
    void startServer();
    void userNewConnection();
    void HideUserLayout(int index);
    void HideLayout(QLayout *l);
    void userSocketReady();
    void userSocketDisconnected();

private:
    QWidget *w;
    QFont *font;
    QVBoxLayout *mainLayout;
    QTextBrowser *log;
    QMap<QSslSocket*, User> users;
    QSslConfiguration m_sslConfiguration;
    int index = 0;
    QSslConfiguration sslConf;
};

#endif // SERVERPOP3_H
