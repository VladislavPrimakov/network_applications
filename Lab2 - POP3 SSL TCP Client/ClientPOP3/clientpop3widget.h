#ifndef CLIENTPOP3WIDGET_H
#define CLIENTPOP3WIDGET_H

#include <QWidget>
#include <QSslSocket>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QScrollArea>
#include <QLineEdit>
#include <QMessageBox>
#include <QTextBrowser>
#include <QByteArray>
#include <QQueue>
#include <QPair>
#include <QVector>
#include <QCheckBox>
#include <QStringDecoder>
#include <QStringEncoder>
#include "encoders.h"
#include <iostream>
#include <QSslConfiguration>

QT_BEGIN_NAMESPACE
namespace Ui { class ClientPOP3Widget; }
QT_END_NAMESPACE

class Message{
public:
    Message();
    void setId(int id_);
    int getId() const;
    void addMessage(const QString &message_);
    const QString& getMessage() const;
    void clear();
    Message& operator=(Message& other);
    void setCheck(bool check_);
    bool getCheck() const;
private:
    int id;
    QString message;
    bool isChecked;
};

class ClientPOP3Widget : public QWidget
{
    Q_OBJECT
public:
    enum Commands{
        USER,
        PASS,
        QUIT,
        STAT,
        RETR,
        DELE,
        RSET,
    };

    void cUSER();
    void aUSER(QString answer);
    void cPASS();
    void aPASS(QString answer);
    void cSTAT();
    void aSTAT(QString answer);
    void cRETR(int number);
    void aRETR(QString answer, QVector<QString> args);
    void cDELE(int number);
    void aDELE(QString answer, QVector<QString> args);
    void cRSET();
    void aRSET(QString answer);
    void cQUIT();
    void aQUIT(QString answer);

    ClientPOP3Widget(QWidget *parent = nullptr);
    ~ClientPOP3Widget();
    void ShowAuthForm();
    void HideAuthForm();
    void LogIn();
    void HideLayout(QLayout *l);
    void ShowEmailForm();
    void HideEmailForm();
    void ShowEmailsInRange();
    void ShowMessage(int index);
    QString FindTag(const QString& s, QString tag);
    void CheckMessage(int state, int index);
    void ShowPagination();
    void GoToNextPage();
    void GoToPrevPage();
    void GoToFirstPage();
    void GoToLastPage();
    void LogMessage(const QString &s);

public slots:
    void socketReady();
    void socketDisconnect();

private:
    Ui::ClientPOP3Widget *ui;
    QQueue<QPair<Commands, QVector<QString>>> *commandsQueue;
    QFont *font;
    QSslSocket *socket;
    QTextBrowser *log;
    QVBoxLayout *mainLayout;
    QVBoxLayout *authLayout;
    QVBoxLayout *stateLayout;
    QVBoxLayout *messagesLayout;
    QHBoxLayout *btnLayout;
    QHBoxLayout *paginationLayout;
    QLineEdit *emailLine;
    QLineEdit *passLine;
    QLineEdit *adressLine;
    QLineEdit *portLine;
    QString name;
    QVector<Message> messages;
    int messagesNumber;
    int messagesSize;
    int pageSize = 20;
    int currentPage = 1;
};
#endif // CLIENTPOP3WIDGET_H
