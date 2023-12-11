#include "serverpop3.h"

User::User(){
    index = -1;
    nameUser = "Unknow";
    isAuth = false;
    passLabel = nullptr;
    nameLabel = nullptr;
    log = nullptr;
    socket = nullptr;
}

User::~User(){}

User& User::operator=(const User& other){
    index = other.index;
    nameUser = other.nameUser;
    nameLabel = other.nameLabel;
    passLabel = other.passLabel;
    isAuth = other.isAuth;
    socket = other.socket;
    log = other.log;
    checkedMessages = other.checkedMessages;
    return *this;
}


void User::setSocket(QSslSocket *s){
    socket = s;
}

QSslSocket* User::getSocket(){
    return socket;
}

void User::setName(QString &name_){
    nameUser = name_;
}

QString& User::getName(){
    return nameUser;
}

void User::setIndex(int index_){
    index = index_;
}

int User::getIndex(){
    return index;
}

void User::setNameLabel(QLabel *l){
    nameLabel = l;
}

void User::setPassLabel(QLabel *l){
    passLabel = l;
}

void User::setLogTextBrowser(QTextBrowser *tb){
    log = tb;
}

bool User::getIsAuth(){
    return isAuth;
}

void User::LogMessage(const QString s, TypeCommands t){
    if (log != nullptr){
        QString r;
        if (t == TypeCommands::receive){
            r.append("Receive < ");
        }
        if (t == TypeCommands::transmit){
            r.append("Transmit > ");
        }
        log->append(r + s);
    }
}

QJsonArray User::readDB(){
    QFile file("database.json");
    if (!file.open(QIODevice::ReadOnly)){
        log->append("Error read dabase.json");
    } else{
        QByteArray data = file.readAll();
        file.close();
        QJsonParseError jsonError;
        QJsonDocument document = QJsonDocument::fromJson( data, &jsonError );
        if( jsonError.error != QJsonParseError::NoError ) {
            log->append("Error parse dabase.json");
        } else {
            QJsonArray arrUsers = document.object().value("users").toArray();
            return arrUsers;
        }
    }
    return QJsonArray();
}

void User::writeDB(QJsonArray users){
    QFile file("database.json");
    if (!file.open(QIODevice::WriteOnly)){
        log->append("Error read dabase.json");
    } else {
        QJsonObject obj;
        obj.insert("users", users);
        QJsonDocument document;
        document.setObject(obj);
        file.write(document.toJson());
        file.close();
    }
}

void User::cSERVERREADY(){
    QString command = "+OK POP3 server ready\r\n";
    socket->write(command.toLatin1());
    LogMessage(command, TypeCommands::transmit);
}

void User::cUSER(QString &name){
    QString command;
    auto users = readDB();
    bool isFound = false;
    if (nameUser == "Unknow"){
        for (auto usr : users){
            QString name_ = usr.toObject().value("UserName").toString();
            if (name == name_){
                isFound = true;
                command = "+OK " + name + "  is a real hoopy frood\r\n";
                nameUser = name;
                nameLabel->setText(nameUser);
                break;
            }
        }
    }
    if (isFound == false){
        command = "-ERR sorry, no mailbox for " + name + " here\r\n";
    }
    socket->write(command.toLatin1());
    LogMessage(command, TypeCommands::transmit);
}

void User::cPASS(QString &pass){
    QString command;
    auto users = readDB();
    bool isFound = false;
    if (nameUser != "Unknow"){
        for (auto usr : users){
            QString pass_ = usr.toObject().value("UserPassword").toString();
            if (pass == pass_){
                isFound = true;
                command = "+OK " + nameUser + "'s maildrop\r\n";
                isAuth = true;
                passLabel->setText("Setted password");
                break;
            }
        }
    }
    if (isFound == false){
        command = "-ERR maildrop already locked\r\n";
    }
    socket->write(command.toLatin1());
    LogMessage(command, TypeCommands::transmit);
}

void User::cSTAT(){
    QString command;
    auto users = readDB();
    int count = 0;
    int size = 0;
    for (auto usr : users){
        if (usr.toObject().value("UserName").toString() == nameUser){
            QJsonArray messages = usr.toObject().value("messages").toArray();
            for (auto msg : messages){
                count++;
                size += msg.toObject().value("text").toString().toLatin1().size();
            }
            break;
        }
    }
    command = "+OK " + QString::number(count) + " " + QString::number(size) + "\r\n";
    socket->write(command.toLatin1());
    LogMessage(command, TypeCommands::transmit);
}

void User::cRETR(QString &number){
    QString command;
    auto users = readDB();
    bool isFound = false;
    for (auto usr : users){
        if (usr.toObject().value("UserName").toString() == nameUser){
            QJsonArray messages = usr.toObject().value("messages").toArray();
            for (auto msg : messages){
                auto obj = msg.toObject().value("id");
                QString id = QString::number(msg.toObject().value("id").toInt());
                if (id == number){
                    command = "+OK message follows\r\n";
                    socket->write(QString("+OK message follows\r\n").toLatin1());
                    socket->write(QString(msg.toObject().value("text").toString() + ".\r\n").toLatin1());
                    isFound = true;
                    break;
                }
            }
        }
        if (isFound){
            break;
        }
    }
    if (isFound == false){
        command = "-ERR no such message\r\n";
        socket->write(QString("-ERR no such message\r\n").toLatin1());
    }
    LogMessage(command, TypeCommands::transmit);
}

void User::cDELE(QString &number){
    QString command;
    auto users = readDB();
    bool isFound = false;
    for (auto usr : users){
        if (usr.toObject().value("UserName").toString() == nameUser){
            QJsonArray messages = usr.toObject().value("messages").toArray();
            for (auto msg : messages){
                if (QString::number(msg.toObject().value("id").toInt()) == number){
                    command = "+OK message deleted\r\n";
                    isFound = true;
                    checkedMessages.append(number.toInt());
                    break;
                }
            }
        }
        if (isFound){
            break;
        }
    }
    if (isFound == false){
        command = "-ERR no such message\r\n";
    }
    socket->write(command.toLatin1());
    LogMessage(command, TypeCommands::transmit);
}

void User::cRSET(){
    QString command;
    checkedMessages.clear();
    command = "+OK\r\n";
    socket->write(command.toLatin1());
    LogMessage(command, TypeCommands::transmit);
}

void User::cQUIT(){
    QString command;
    QJsonArray users = readDB();
    bool isFound = false;
    bool isFoundUser = false;
    for (int itUser = 0; itUser < users.size(); ++itUser){
        QJsonObject user = users[itUser].toObject();
        if (user["UserName"].toString() == nameUser){
            isFoundUser = true;
            QJsonArray messages = user["messages"].toArray();
            int decreaseCount = 0;
            for (int i = 0; i < messages.size(); i++){
                bool isDeleted = false;
                QJsonObject msg = messages[i].toObject();
                for (auto &idDeleted : checkedMessages){
                    if (msg["id"].toInt() == idDeleted){
                        isFound = true;
                        isDeleted = true;
                        decreaseCount++;
                        break;
                    }
                }
                msg["id"] = msg["id"].toInt() - decreaseCount;
                messages[i] = msg;
                if (isDeleted == true){
                    messages.removeAt(i);
                    i--;
                }
            }
            user["messages"] = messages;
        }
        if (isFoundUser){
            users[itUser] = user;
            break;
        }
    }
    if (isFound == true){
        writeDB(users);
    }
    command = "+OK POP3 server signing off\r\n";
    socket->write(command.toLatin1());
    LogMessage(command, TypeCommands::transmit);
}

ServerPOP3::ServerPOP3(QWidget *w) {
    font = new QFont();
    font->setPointSize(14);
    w->setWindowTitle("Server POP3 Widget");

    QScrollArea* scrollArea = new QScrollArea();
    scrollArea->setWidgetResizable(true);
    scrollArea->setSizePolicy(QSizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding));
    QVBoxLayout *scrollLayout = new QVBoxLayout();
    scrollLayout->setAlignment(Qt::AlignCenter);
    scrollLayout->addWidget(scrollArea);
    QWidget *wScroll = new QWidget();
    mainLayout = new QVBoxLayout();
    mainLayout->setAlignment(Qt::AlignCenter);
    wScroll->setLayout(mainLayout);
    scrollArea->setWidget(wScroll);
    w->setLayout(scrollLayout);

    log = new QTextBrowser();
    log->setFont(*font);
    log->setMaximumHeight(150);
    scrollLayout->addWidget(log);

    users = QMap<QSslSocket*, User>();
}

void ServerPOP3::startServer(){
    auto config = QSslConfiguration::defaultConfiguration();
    config.setCaCertificates(QSslCertificate::fromPath(QStringLiteral("rootCA.csr")));

    QFile certificateFile(QStringLiteral("server.crt"));
    if (certificateFile.open(QIODevice::ReadOnly | QIODevice::Text))
        config.setLocalCertificate(QSslCertificate(certificateFile.readAll(), QSsl::Pem));
    else
        LogMessage("Could not open certificate file");

    QFile keyFile(QStringLiteral("server.key"));
    if (keyFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QSslKey key(keyFile.readAll(), QSsl::Rsa, QSsl::Pem, QSsl::PrivateKey);
        if (key.isNull())
            LogMessage("Key is not valid");
        config.setPrivateKey(key);
    } else {
        LogMessage("Could not open key file");
    }

    config.setPeerVerifyMode(QSslSocket::VerifyNone);
    setSslConfiguration(config);
    sslConf = config;


    if(listen(QHostAddress::LocalHost, 995)){
        LogMessage("Listening: " + serverAddress().toString() + ":" + QString::number(serverPort()));

        connect(this, &QSslServer::pendingConnectionAvailable, this, [this](){
            this->userNewConnection();
        });

    } else{
        LogMessage("Error listening server");
    }
}

void ServerPOP3::LogMessage(const QString s){
    log->append(s);
    std::cout << s.toStdString() << std::endl;
}

void ServerPOP3::userNewConnection(){
    QSslSocket *userSocket = qobject_cast<QSslSocket *>(nextPendingConnection());

    QSslConfiguration conf;
    conf.setPeerVerifyMode(QSslSocket::VerifyNone);
    userSocket->setSslConfiguration(conf);
    connect(userSocket, &QSslSocket::disconnected, this, &ServerPOP3::userSocketDisconnected);
    connect(userSocket, &QSslSocket::readyRead, this, &ServerPOP3::userSocketReady);

    userSocket->setSslConfiguration(sslConf);
    User u = User();
    users.insert(userSocket, u);
    users[userSocket].setSocket(userSocket);
    users[userSocket].setIndex(index);
    index++;

    LogMessage(QString("New user connection"));
    // create user interface
    QVBoxLayout *userLayout = new QVBoxLayout();

    QLabel *nameLabel = new QLabel("Name: Unknow name");
    nameLabel->setFont(*font);
    userLayout->addWidget(nameLabel);
    users[userSocket].setNameLabel(nameLabel);

    QLabel *passLabel = new QLabel("Password: Unseted password");
    passLabel->setFont(*font);
    userLayout->addWidget(passLabel);
    users[userSocket].setPassLabel(passLabel);

    QTextBrowser *userLog = new QTextBrowser();
    userLog->setFont(*font);
    userLog->setMaximumHeight(300);
    userLayout->addWidget(userLog);
    users[userSocket].setLogTextBrowser(userLog);

    mainLayout->addLayout(userLayout);

    users[userSocket].cSERVERREADY();
}

void ServerPOP3::userSocketReady(){
    QSslSocket *socket = qobject_cast<QSslSocket*>(sender());
    while(socket->canReadLine()){
        QByteArray byteArray = socket->readLine();
        QString s = QString::fromLatin1(byteArray).chopped(2);
        users[socket].LogMessage(s, TypeCommands::receive);
        QList<QString> commands = s.split(' ');
        if (commands.size() > 0){
            QString c = commands.at(0);
            QString arg1;
            if (commands.size() > 1){
                arg1 = commands.at(1);
            }
            if (c == "USER"){
                users[socket].cUSER(arg1);
            }
            if (c == "PASS"){
                users[socket].cPASS(arg1);
            }
            if (users[socket].getIsAuth()){
                if (c == "QUIT"){
                    users[socket].cQUIT();
                    users[socket].getSocket()->close();
                }
                if (c == "STAT"){
                    users[socket].cSTAT();
                }
                if (c == "RETR"){
                    users[socket].cRETR(arg1);
                }
                if (c == "DELE"){
                    users[socket].cDELE(arg1);
                }
                if (c == "RSET"){
                    users[socket].cRSET();
                }
            }

        }
    }
}

void ServerPOP3::userSocketDisconnected(){
    QSslSocket *socket = qobject_cast<QSslSocket*>(sender());
    int index_ = users[socket].getIndex();
    HideUserLayout(index_);
    LogMessage("Disconnect user " + users[socket].getName());
    users.remove(socket);
    index--;
    for (auto &usr : users){
        if (usr.getIndex() > index_){
            usr.setIndex(usr.getIndex()-1);
        }
    }
}

void ServerPOP3::HideUserLayout(int index){
    QLayout *l = mainLayout->takeAt(index)->layout();
    HideLayout(l);
}

void ServerPOP3::HideLayout(QLayout *l){
    while (l->count()) {
        QLayoutItem *item = l->takeAt(0);
        if (QWidget *widget = item->widget()) {
            delete widget;
        } else if (QLayout *childLayout = item->layout()) {
            HideLayout(childLayout);
        }
        delete item;
    }
}
