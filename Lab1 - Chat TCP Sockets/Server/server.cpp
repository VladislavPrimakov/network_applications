#include "server.h"

Server::Server(){}

User::User(QTcpSocket* socket, int id, int index){
    this->socket = socket;
    this->id = id;
    this->index = index;
}

QTcpSocket* User::getSocket(){
    return this->socket;
}

unsigned int User::getId(){
    return this->id;
}

unsigned int User::getIndex(){
    return this->index;
}

void User::setIndex(unsigned int index){
    this->index = index;
}

Data::Data(){}

Data::Data(Command command, QVector<QString> args, QString data){
    this->command = command;
    this->args = args;
    this->data = data;
}

Data::Data(QByteArray &byteArray){
    QDataStream stream(&byteArray, QDataStream::ReadOnly);
    int commandValue;
    stream >> commandValue;
    this->command = static_cast<Command>(commandValue);
    stream >> this->args;
    stream >> this->data;
}

QByteArray Data::Serialize(){
    QByteArray byteArray;
    QDataStream stream(&byteArray, QIODevice::WriteOnly);
    stream << static_cast<int>(this->command);
    stream << this->args;
    stream <<this->data;
    QByteArray sizeByteArray;
    QDataStream streamSize(&sizeByteArray, QIODevice::WriteOnly);
    quint16 objectSize = byteArray.size();
    streamSize << objectSize;
    byteArray.insert(0, sizeByteArray);
    return byteArray;
}

void Server::startServer(QWidget *serverWidget){
    this->log = serverWidget->findChild<QTextBrowser*>("Log");
    this->usersLayout = serverWidget->findChild<QVBoxLayout*>("usersLayout");
    this->tabChats = serverWidget->findChild<QTabWidget*>("tabWidget");
    this->tabChats->removeTab(1);
    this->tabChats->removeTab(0);
    this->users = QMap<QTcpSocket*, User*>();
    this->font = QFont();
    font.setPointSize(15);
    this->styleServer = "color: red;";
    this->styleUser = "color: blue;";
    // add general chat
    QScrollArea* scrollArea = new QScrollArea();
    scrollArea->setWidgetResizable(true);
    this->tabChats->addTab(scrollArea, "General Chat");
    QWidget *w = new QWidget;
    QVBoxLayout *vb = new QVBoxLayout();
    w->setLayout(vb);
    scrollArea->setWidget(w);

    if (this->listen(QHostAddress::LocalHost, 3000)){
        this->log->append("Listening : " + this->serverAddress().toString() + ":" + QString::number(this->serverPort()));
        connect(this, SIGNAL(newConnection()), this, SLOT(userNewConnection()));
    }
    else{
        this->log->append("Not listening");
    }
}

void Server::userNewConnection(){
    QTcpSocket* userSocket = nextPendingConnection();
    auto user = new User(userSocket, this->userId, this->userIndex);
    connect(userSocket, SIGNAL(disconnected()), this, SLOT(userSocketDisconnected()));
    connect(userSocket, SIGNAL(readyRead()), this, SLOT(userSockReady()));

    this->log->append("User " + QString::number(user->getId()) + " connected");

    // send other users to current user
    for (auto socket : this->users.keys()){
        Data data;
        data.command = Command::AddUser;
        data.data = QString::number(userId);
        socket->write(data.Serialize());
    }
    // send current user to other users
    for (auto user : this->users.values()){
        Data data;
        data.command = Command::AddUser;
        data.data = QString::number(user->getId());
        userSocket->write(data.Serialize());
    }
    // add new user to map
    this->users.insert(userSocket, user);

    // add label + button for new user
    auto tempHBoxLayout = new QHBoxLayout();
    auto tempLabel = new QLabel("User " + QString::number(user->getId()));
    tempLabel->setFont(this->font);
    auto tempButtonKickUser = new QPushButton("Kick user");
    tempButtonKickUser->setMinimumSize(100, 50);
    tempButtonKickUser->setMaximumSize(100, 50);
    tempButtonKickUser->setFont(this->font);
    tempHBoxLayout->addWidget(tempLabel, 0, Qt::AlignRight);
    tempHBoxLayout->addWidget(tempButtonKickUser, 0, Qt::AlignRight);
    this->usersLayout->addLayout(tempHBoxLayout);
    connect(tempButtonKickUser, &QPushButton::clicked,[this, userSocket] { this->userKick(userSocket); } );

    // add tab widget for new user
    QScrollArea* scrollArea = new QScrollArea();
    scrollArea->setWidgetResizable(true);
    this->tabChats->addTab(scrollArea, "User " + QString::number(user->getId()) + " Chat");
    QWidget *w = new QWidget;
    QVBoxLayout *vb = new QVBoxLayout();
    w->setLayout(vb);
    scrollArea->setWidget(w);

    // send user id
    Data data;
    data.command = Command::SendUserId;
    data.data = QString::number(this->userId);
    userSocket->write(data.Serialize());
    // send user log message
    Data data2;
    data2.command = Command::LogMessage;
    data2.data = "You are connected to the server";
    userSocket->write(data2.Serialize());
    // increment values
    this->userIndex++;
    this->userId++;
}

void Server::sendMessage(QString message){
    unsigned int index = this->tabChats->currentIndex();
    // Send general message to all users
    if (index == 0){
        Data data;
        data.command = Command::SendGeneralMessage;
        data.data = message;
        data.args.append(QString("server"));
        auto byteArray = data.Serialize();
        for (auto socket : this->users.keys()){
            socket->write(byteArray);
        }
    }
    // Send Private message
    else {
        Data data;
        data.command = Command::SendPrivateMessage;
        data.data = message;
        data.args.append(QString("server"));
        auto it = std::find_if(this->users.keyValueBegin(), this->users.keyValueEnd(), [index](const std::pair<QTcpSocket*, User*>& pair){
            return pair.second->getIndex() == index - 1;
        });
        if (it != this->users.keyValueEnd()){
            QTcpSocket *socket = it->first;
            data.args.append(QString("server"));
            socket->write(data.Serialize());
        }
    }
    // add label with message
    QLabel *messageLabel = new QLabel();
    messageLabel->setText("server: <br>" + message);
    messageLabel->setAlignment(Qt::AlignRight | Qt::AlignTop);
    messageLabel->setSizePolicy(QSizePolicy(QSizePolicy::Policy::MinimumExpanding, QSizePolicy::Policy::MinimumExpanding ));
    messageLabel->setFont(this->font);
    messageLabel->setStyleSheet(this->styleServer);
    QScrollArea *sa = (QScrollArea*)this->tabChats->widget(index);
    QWidget *w = sa->widget();
    QLayout *l = w->layout();
    l->addWidget(messageLabel);
}

void Server::userSockReady(){
    QTcpSocket *socket = qobject_cast<QTcpSocket*>(sender());
    QByteArray byteArray = socket->readAll();
    while(byteArray.size() > 0){
        // get size of block
        QByteArray sizeByteArray = byteArray.sliced(0, 2);
        byteArray.remove(0, 2);
        QDataStream dataStream(&sizeByteArray, QIODevice::ReadOnly);
        quint16 size;
        dataStream >> size;
        // get data
        QByteArray dataArray = byteArray.sliced(0, size);
        byteArray.remove(0, size);
        Data data(dataArray);

        switch (data.command) {
        case Command::SendGeneralMessage:
            this->receiveGeneralMessage(data);
            break;
        case Command::SendPrivateMessage:
            this->receivePrivateMessage(data);
            break;
        default:
            break;
        }
    }
}

QLabel* Server::receiveMessage(Data &data){
    QString message = data.data;
    QString sender = data.args[0];
    QLabel *messageLabel = new QLabel();
    messageLabel->setText("User " + sender + ":<br>" + message);
    messageLabel->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    messageLabel->setSizePolicy(QSizePolicy(QSizePolicy::Policy::MinimumExpanding, QSizePolicy::Policy::MinimumExpanding ));
    messageLabel->setFont(this->font);
    messageLabel->setStyleSheet(this->styleUser);
    return messageLabel;
}

void Server::receiveGeneralMessage(Data &data){
    QLabel* label = this->receiveMessage(data);
    QScrollArea *sa = (QScrollArea*)this->tabChats->widget(0);
    QWidget *w = sa->widget();
    QLayout *l = w->layout();
    l->addWidget(label);
}

void Server::receivePrivateMessage(Data &data){
    QString receiver = data.args[1];
    // receive private message for server
    if  (receiver == "server"){
        QLabel* label = this->receiveMessage(data);
        unsigned int senderId = data.args[0].toUInt();
        unsigned int senderIndex = 0;
        for (auto &usr : this->users.values()){
            if (usr->getId() == senderId){
                senderIndex = usr->getIndex();
                break;
            }
        }
        QScrollArea *sa = (QScrollArea*)this->tabChats->widget(senderIndex + 1);
        QWidget *w = sa->widget();
        QLayout *l = w->layout();
        l->addWidget(label);
    }
    // resend message to another user
    else {
        unsigned int receiverId = receiver.toUInt();
        for (auto &usr : this->users.values()){
            if (usr->getId() == receiverId){
                QTcpSocket *socket = usr->getSocket();
                socket->write(data.Serialize());
                break;
            }
        }
    }
}

void Server::userKick(QTcpSocket* socket){
    socket->close();
}

void Server::errorOccurred(QAbstractSocket::SocketError socketError){
    this->log->append("Socket error: " + QString::number(socketError));
}

void Server::userSocketDisconnected(){
    QTcpSocket *socket = qobject_cast<QTcpSocket*>(sender());
    auto index_user = this->users[socket]->getIndex();
    auto id_user = this->users[socket]->getId();

    // remove label and button
    QLayout *Layout = this->usersLayout->takeAt(index_user)->layout();
    for (int i = 0; i < Layout->count(); i++){
        QWidget *item = Layout->itemAt(i)->widget();
        delete item;
    }
    delete Layout;

    // remove tab widget
    this->tabChats->removeTab(index_user + 1);

    // delete user and socket
    this->log->append("User " + QString::number(this->users[socket]->getId()) + " disconnected");
    socket->deleteLater();
    delete users[socket];
    this->users.remove(socket);

    // decrease values
    for (auto usr : this->users){
        if (usr->getIndex() >= index_user){
            usr->setIndex(usr->getIndex()-1);
        }
    }
    this->userIndex--;

    // send remove user to others
    for (auto socket : this->users.keys()){
        Data data;
        data.command = Command::RemoveUser;
        data.data = QString::number(id_user);
        socket->write(data.Serialize());
    }
}
