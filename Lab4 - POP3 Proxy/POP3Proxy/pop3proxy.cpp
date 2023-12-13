#include "pop3proxy.h"

User::User(){
    index = -1;
    log = nullptr;
    client_socket = nullptr;
    server_socket = nullptr;
    adr = nullptr;
    adrLabel = nullptr;
    port = -1;
    clientSocketClosed = false;
    serverSocketClosed = false;
}

User::~User(){
    delete adrLabel;
    delete log;
    delete adr;
    delete client_socket;
    delete server_socket;
}

User& User::operator=(const User& other){
    index = other.index;
    log = other.log;
    client_socket = other.client_socket;
    server_socket = other.server_socket;
    adr = other.adr;
    adrLabel = other.adrLabel;
    port = other.port;
    clientSocketClosed = other.clientSocketClosed;
    serverSocketClosed = other.serverSocketClosed;
    return *this;
}

void User::setClientSocketClosed(bool s){
    clientSocketClosed = s;
}

bool User::getClientSocketClosed(){
    return clientSocketClosed;
}

void User::setServerSocketClosed(bool s){
    serverSocketClosed = s;
}

bool User::getServerSocketClosed(){
    return serverSocketClosed;
}

void User::setClientSocket(QSslSocket *s){
    client_socket = s;
}

QSslSocket* User::getClientSocket(){
    return client_socket;
}

void User::setServerSocket(QSslSocket *s){
    server_socket = s;
}

QSslSocket* User::getServerSocket(){
    return server_socket;
}

void User::setIndex(int index_){
    index = index_;
}

int User::getIndex(){
    return index;
}

void User::setAdr(QString adr_){
    adr = new QHostAddress(adr_);
}

QHostAddress* User::getAdr(){
    return adr;
}

void User::setAdrLabel(QLabel *l){
    adrLabel = l;
}

void User::setPort(int port_){
    port = port_;
}

int User::getPort(){
    return port;
}

void User::setLogTextBrowser(QTextBrowser *tb){
    log = tb;
}

void User::LogMessage(const QString s, TypeCommands t){
    if (log != nullptr){
        QString r;
        if (t == TypeCommands::receiveFromClient){
            r.append("Receive client < ");
        }
        if (t == TypeCommands::transmitToClient){
            r.append("Transmit client > ");
        }
        if (t == TypeCommands::receiveFromServer){
            r.append("Receive server < ");
        }
        if (t == TypeCommands::transmitToServer){
            r.append("Transmit server > ");
        }
        log->append(r + s);
    }
}

void User::reSendToClient(const QByteArray &ba){
    client_socket->write(ba);
    LogMessage(QString::fromLatin1(ba), TypeCommands::transmitToClient);
}

void User::reSendToServer(const QByteArray &ba){
    server_socket->write(ba);
    LogMessage(QString::fromLatin1(ba), TypeCommands::transmitToServer);
}

void User::cPROXYREADY(){
    QString command = "+OK POP3 proxy ready\r\n";
    client_socket->write(command.toLatin1());
    LogMessage(command, TypeCommands::transmitToClient);
}

void User::setAdr(QString adress_, QString port_){
    adr = new QHostAddress(adress_);
    port = port_.toInt();
    adrLabel->setText("Adress server: " + adr->toString() + ":" + QString::number(port));
}

void User::cOKPRXA(){
    QString command = "+OK Connection setted\r\n";
    client_socket->write(command.toLatin1());
    LogMessage(command, TypeCommands::transmitToClient);
}

void User::cERRPRXA(){
    QString command = "-ERR Connection not setted\r\n";
    client_socket->write(command.toLatin1());
    LogMessage(command, TypeCommands::transmitToClient);
}

POP3Proxy::POP3Proxy(QWidget *w){
    font = new QFont();
    font->setPointSize(14);
    w->setWindowTitle("POP3 Proxy Widget");
    w->showMaximized();

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

    usersLayout = new QVBoxLayout();
    mainLayout->addLayout(usersLayout);

    log = new QTextBrowser();
    log->setFont(*font);
    log->setMaximumHeight(150);
    scrollLayout->addWidget(log);

    showStartForm();
    usersClient = QMap<QSslSocket*, User*>();
    usersServer = QMap<QSslSocket*, User*>();
}

void POP3Proxy::showStartForm(){
    startLayout = new QVBoxLayout();

    QHBoxLayout *hostNameLayout = new QHBoxLayout();
    QLabel *hostNameLabel = new QLabel("Host name");
    hostNameLabel->setFont(*font);
    QHostAddress ard = QHostAddress::LocalHost;
    inputHostName = new QLineEdit(ard.toString());
    inputHostName->setFont(*font);
    hostNameLayout->addWidget(hostNameLabel);
    hostNameLayout->addWidget(inputHostName);
    startLayout->addLayout(hostNameLayout);

    QHBoxLayout *hostPortLayout = new QHBoxLayout();
    QLabel *hostPortLabel = new QLabel("Host port");
    hostPortLabel->setFont(*font);
    inputHostPort = new QLineEdit("1080");
    inputHostPort->setFont(*font);
    hostPortLayout->addWidget(hostPortLabel);
    hostPortLayout->addWidget(inputHostPort);
    startLayout->addLayout(hostPortLayout);

    QHBoxLayout *btnLayout = new QHBoxLayout();
    QPushButton *startBtn = new QPushButton("Start Server");
    startBtn->setFont(*font);
    connect(startBtn, &QPushButton::clicked, this, [this](bool){
        startProxy();
    });
    btnLayout->addWidget(startBtn);
    startLayout->addLayout(btnLayout);

    mainLayout->addLayout(startLayout);
}

void POP3Proxy::HideLayout(QLayout *l){
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

void POP3Proxy::HideUserLayout(QSslSocket *socket){
    User *user;
    user = usersServer[socket];
    if (user == nullptr){
        user = usersClient[socket];
    }
    // decrease indexes for other users
    for (auto usr : usersClient.values()){
        if (usr->getIndex() > user->getIndex()){
            usr->setIndex(usr->getIndex() - 1);
        }
    }
    QLayout *l = usersLayout->takeAt(user->getIndex())->layout();
    HideLayout(l);
    index--;
}

void POP3Proxy::startProxy(){
    auto config = QSslConfiguration::defaultConfiguration();

    // Load CA Certificates from the bundle file
    QFile caBundleFile(QStringLiteral("morality.pp.ua.ca-bundle"));
    if (caBundleFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QByteArray dataCABundle = caBundleFile.readAll();
        caBundleFile.close();
        QList<QSslCertificate> caCertificates = QSslCertificate::fromData(dataCABundle, QSsl::Pem);
        if (!caCertificates.isEmpty()) {
            config.setCaCertificates(caCertificates);
        } else {
            LogMessage("Failed to load CA certificates from the bundle file");
        }
    } else {
        LogMessage("Could not open CA bundle file");
    }
    // Load Server Certificate
    QFile certificateFile(QStringLiteral("morality.pp.ua.crt"));
    if (certificateFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QByteArray dataCertificate = certificateFile.readAll();
        certificateFile.close();
        QSslCertificate certificate(dataCertificate, QSsl::Pem);
        if (!certificate.isNull()) {
            config.setLocalCertificate(certificate);
        } else {
            LogMessage("Server certificate is not valid");
        }
    } else {
        LogMessage("Could not open server certificate file");
    }
    // Load Server Key
    QFile keyFile(QStringLiteral("morality.pp.ua.key"));
    if (keyFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QSslKey key(keyFile.readAll(), QSsl::Rsa, QSsl::Pem, QSsl::PrivateKey);
        keyFile.close();

        if (key.isNull()) {
            LogMessage("Server key is not valid");
        } else {
            config.setPrivateKey(key);
        }
    } else {
        LogMessage("Could not open server key file");
    }

    config.setPeerVerifyMode(QSslSocket::VerifyNone);
    setSslConfiguration(config);
    sslConf = config;

    qInfo() << QSslSocket::sslLibraryVersionString();
    qInfo() << QSslSocket::sslLibraryBuildVersionString();

    if(listen(QHostAddress(inputHostName->text()), inputHostPort->text().toUShort())){
        LogMessage("Listening: " + serverAddress().toString() + ":" + QString::number(serverPort()));

        connect(this, &QSslServer::pendingConnectionAvailable, this, &POP3Proxy::userNewConnection);

    } else{
        LogMessage("Error listening server");
    }

    HideLayout(startLayout);
}

void POP3Proxy::LogMessage(const QString s){
    log->append(s);
    qInfo() << s.toStdString() << Qt::endl;
}

void POP3Proxy::userNewConnection(){
    QSslSocket *userSocket = qobject_cast<QSslSocket *>(nextPendingConnection());

    connect(userSocket, &QSslSocket::disconnected, this, &POP3Proxy::userSocketDisconnected);
    connect(userSocket, &QSslSocket::readyRead, this, &POP3Proxy::userSocketReady);

    userSocket->setSslConfiguration(sslConf);
    User *u = new User();
    usersClient.insert(userSocket, u);
    usersClient[userSocket]->setClientSocket(userSocket);
    usersClient[userSocket]->setIndex(index);
    index++;

    LogMessage(QString("New user connection"));
    // create user interface
    QVBoxLayout *userLayout = new QVBoxLayout();

    QLabel *nameLabel = new QLabel("User " + QString::number(index));
    nameLabel->setFont(*font);
    userLayout->addWidget(nameLabel);

    QLabel *adrLabel = new QLabel("Adress server: Unsetted");
    adrLabel->setFont(*font);
    userLayout->addWidget(adrLabel);
    usersClient[userSocket]->setAdrLabel(adrLabel);

    QTextBrowser *userLog = new QTextBrowser();
    userLog->setFont(*font);
    userLog->setMaximumHeight(300);
    userLayout->addWidget(userLog);
    usersClient[userSocket]->setLogTextBrowser(userLog);

    usersLayout->addLayout(userLayout);

    usersClient[userSocket]->cPROXYREADY();
}

void POP3Proxy::userSocketReady(){
    QSslSocket *socket = qobject_cast<QSslSocket*>(sender());
    while(socket->canReadLine()){
        QByteArray byteArray = socket->readLine();
        QString s = QString::fromLatin1(byteArray).chopped(2);
        usersClient[socket]->LogMessage(s, TypeCommands::receiveFromClient);
        QList<QString> commands = s.split(' ');
        // unsetted POP3 server adress and connection
        if (usersClient[socket]->getServerSocket() == nullptr){
            QString c = commands.at(0);
            QString arg1;
            if (commands.size() > 1){
                arg1 = commands.at(1);
            }
            QString arg2;
            if (commands.size() > 2){
                arg2 = commands.at(2);
            }
            // command to set connection with server
            if (c == "PRXA" && !arg1.isEmpty() && !arg2.isEmpty()){
                usersClient[socket]->setAdr(arg1, arg2);

                QSslSocket *server_socket = new QSslSocket();
                auto config = QSslConfiguration::defaultConfiguration();
                config.setPeerVerifyMode(QSslSocket::VerifyNone);
                server_socket->setSslConfiguration(config);

                usersClient[socket]->setServerSocket(server_socket);
                usersServer.insert(server_socket, usersClient[socket]);

                connect(server_socket, &QSslSocket::disconnected, this, &POP3Proxy::serverSocketDisconnected);
                connect(server_socket, &QSslSocket::readyRead, this, &POP3Proxy::serverSocketReady);

                server_socket->connectToHostEncrypted(usersClient[socket]->getAdr()->toString(), usersClient[socket]->getPort());
                bool state = server_socket->waitForConnected(1000);
                if (state == true){
                    LogMessage("User " + QString::number(usersClient[socket]->getIndex()+1) + " connected to the POP3 server");
                } else {
                    LogMessage("User " + QString::number(usersClient[socket]->getIndex()+1) + " cannot connect to the POP3 server");
                    usersClient[socket]->cERRPRXA();
                    socket->close();
                }
            }
        }
        // resend data to server
        else{
            usersClient[socket]->reSendToServer(byteArray);
        }
    }
}

void POP3Proxy::userSocketDisconnected(){
    QSslSocket *socket = qobject_cast<QSslSocket*>(sender());
    usersClient[socket]->setClientSocketClosed(true);
    LogMessage("User " + QString::number(usersClient[socket]->getIndex()+1) + " client socket closed");
    if (usersClient[socket]->getServerSocket() != nullptr){
        if (usersClient[socket]->getServerSocketClosed() == false){
            usersClient[socket]->getServerSocket()->close();
            HideUserLayout(socket);
        }
    }
    usersClient.remove(socket);
}

void POP3Proxy::serverSocketReady(){
    QSslSocket *socket = qobject_cast<QSslSocket*>(sender());
    while(socket->canReadLine()){
        QByteArray byteArray = socket->readLine();
        QString s = QString::fromLatin1(byteArray).chopped(2);
        usersServer[socket]->LogMessage(s, TypeCommands::receiveFromServer);
        // First answer from server
        if (s.contains("+OK POP3 server ready")){
            usersServer[socket]->cOKPRXA();
            usersServer[socket]->reSendToClient(byteArray);
        }
        // resend data to client
        else{
            usersServer[socket]->reSendToClient(byteArray);
        }
    }
}

void POP3Proxy::serverSocketDisconnected(){
    QSslSocket *socket = qobject_cast<QSslSocket*>(sender());
    usersServer[socket]->setServerSocketClosed(true);
    LogMessage("User " + QString::number(usersServer[socket]->getIndex()+1) + " server socket closed");
    if (usersServer[socket]->getClientSocket() != nullptr){
        if (usersServer[socket]->getClientSocketClosed() == false){
            usersServer[socket]->getClientSocket()->close();
            HideUserLayout(socket);
        }
    }
    usersServer.remove(socket);
}

