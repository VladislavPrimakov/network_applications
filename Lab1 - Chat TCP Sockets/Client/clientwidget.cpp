#include "clientwidget.h"
#include "ui_clientwidget.h"

User::User(int id, int index){
    this->id = id;
    this->index = index;
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

ClientWidget::ClientWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ClientWidget)
{
    ui->setupUi(this);
    this->log = this->findChild<QTextBrowser*>("Log");
    this->tabChats = this->findChild<QTabWidget*>("tabWidget");
    this->usersLayout = this->findChild<QVBoxLayout*>("usersLayout");
    tabChats->removeTab(1);
    tabChats->removeTab(0);

    // set general chat
    QScrollArea* scrollArea = new QScrollArea();
    scrollArea->setWidgetResizable(true);
    this->tabChats->addTab(scrollArea, "General Chat");
    QWidget *w = new QWidget;
    QVBoxLayout *vb = new QVBoxLayout();
    w->setLayout(vb);
    scrollArea->setWidget(w);
    this->font = QFont();
    font.setPointSize(15);
    this->styleServer = "color: red;";
    this->styleUser = "color: blue;";

    // set socket
    socket = new QTcpSocket(this);
    connect(socket, SIGNAL(readyRead()), this, SLOT(sockReady()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(sockDisc()));
    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)),this, SLOT(sockError(QAbstractSocket::SocketError)));
    this->isConnect = false;
}

ClientWidget::~ClientWidget(){
    delete ui;
}

void ClientWidget::sockError(QAbstractSocket::SocketError socketError){
    this->log->append("Socket error: " + QString::number(socketError));
}

void ClientWidget::on_pushButton_clicked()
{
    if (this->isConnect == false){
        QHostAddress adr = QHostAddress::LocalHost;
        unsigned short port = 3000;
        socket->connectToHost(adr, port);
        bool state = socket->waitForConnected(1000);
        if (state == true){
            this->log->append("Connected to the server " + adr.toString() + ":" + QString::number(port) + ". Status: " + QString::number(socket->state()));
            this->isConnect = true;
            // set server chat
            QScrollArea* scrollArea = new QScrollArea();
            scrollArea->setWidgetResizable(true);
            this->tabChats->addTab(scrollArea, "Server Chat");
            QWidget *w = new QWidget;
            QVBoxLayout *vb = new QVBoxLayout();
            w->setLayout(vb);
            scrollArea->setWidget(w);
        } else {
            this->log->append("Cannot connect to the server " + adr.toString() + ":" + QString::number(port) + ". Status: " + QString::number(socket->state()));
        }
    }
}

void ClientWidget::on_pushButton_2_clicked()
{
    this->socket->close();
}

void ClientWidget::sockDisc(){
    // close socket
    socket->close();
    this->isConnect = false;
    // remove users map
    for (auto id : this->users.keys()){
        this->removeUser(id);
    }
    // remove chats
    for (auto i = this->tabChats->count() - 1; i > 0; i--){
        this->tabChats->removeTab(i);
    }
    this->log->append("Disconnected from the server. Status: " + QString::number(socket->state()));
    this->setWindowTitle("Client Widget");
}

void ClientWidget::sockReady(){
    QByteArray byteArray = this->socket->readAll();
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
        case Command::LogMessage:
            this->log->append(data.data);
            break;
        case Command::AddUser:
            this->addUser(data.data.toUInt());
            break;
        case Command::RemoveUser:
            this->removeUser(data.data.toUInt());
            break;
        case Command::SendGeneralMessage:
            this->receiveGeneralMessage(data);
            break;
        case Command::SendPrivateMessage:
            this->receivePrivateMessage(data);
            break;
        case Command::SendUserId:
            this->receiveUserId(data);
            break;
        }
    }
}

void ClientWidget::receiveUserId(Data &data){
    this->userId = data.data.toUInt();
    this->setWindowTitle("Client Widget - User " + QString::number(this->userId));
}

QLabel* ClientWidget::receiveMessage(Data &data){
    QString message = data.data;
    QString sender = data.args[0];
    QLabel *messageLabel = new QLabel();
    messageLabel->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    messageLabel->setSizePolicy(QSizePolicy(QSizePolicy::Policy::MinimumExpanding, QSizePolicy::Policy::MinimumExpanding ));
    messageLabel->setFont(this->font);
    if (sender == "server"){
        messageLabel->setStyleSheet(this->styleServer);
        messageLabel->setText(sender + ":<br>" + message);
    }
    else{
        messageLabel->setStyleSheet(this->styleUser);
        messageLabel->setText("User " + sender + ":<br>" + message);
    }
    return messageLabel;
}

void ClientWidget::receiveGeneralMessage(Data &data){
    QLabel* label = this->receiveMessage(data);
    QScrollArea *sa = (QScrollArea*)this->tabChats->widget(0);
    QWidget *w = sa->widget();
    QLayout *l = w->layout();
    l->addWidget(label);
}

void ClientWidget::receivePrivateMessage(Data &data){
    QLabel* label = this->receiveMessage(data);
    QString sender = data.args[0];
    int index = 0;
    if  (sender == "server"){
        index = 1;
    } else{
        for (auto &usr : this->users.values()){
            if (usr->getId() == sender.toUInt()){
                index = usr->getIndex();
                break;
            }
        }
    }
    QScrollArea *sa = (QScrollArea*)this->tabChats->widget(index + 2);
    QWidget *w = sa->widget();
    QLayout *l = w->layout();
    l->addWidget(label);
}

void ClientWidget::sendMessage(QString message){
    unsigned int index = this->tabChats->currentIndex();
    // Send general message to all users
    if (index == 0){
        Data data;
        data.command = Command::SendGeneralMessage;
        data.data = message;
        data.args.append(QString::number(this->userId));
        this->socket->write(data.Serialize());
    }
    // Send Private message
    else {
        Data data;
        data.command = Command::SendPrivateMessage;
        data.data = message;
        unsigned int idUserReciever = 1;
        for (auto &usr : this->users){
            if (usr->getIndex() == index - 2){
                idUserReciever = usr->getId();
                break;
            }
        }
        data.args.append(QString::number(this->userId));
        if (index == 1){
            data.args.append("server");

        } else{
            data.args.append(QString::number(idUserReciever));
        }
        this->socket->write(data.Serialize());
    }
    // Add label with message
    QLabel *messageLabel = new QLabel();
    messageLabel->setText("User " + QString::number(this->userId) + ": <br>" + message);
    messageLabel->setAlignment(Qt::AlignRight | Qt::AlignTop);
    messageLabel->setSizePolicy(QSizePolicy(QSizePolicy::Policy::MinimumExpanding, QSizePolicy::Policy::MinimumExpanding ));
    messageLabel->setFont(this->font);
    messageLabel->setStyleSheet(this->styleUser);
    QScrollArea *sa = (QScrollArea*)this->tabChats->widget(index);
    QWidget *w = sa->widget();
    QLayout *l = w->layout();
    l->addWidget(messageLabel);
}

void ClientWidget::on_EnterLine_clicked()
{
    QLineEdit *InputLine = this->findChild<QLineEdit*>("InputLine");
    QString message = InputLine->text();
    InputLine->clear();
    this->sendMessage(message);
}

void ClientWidget::addUser(unsigned int id){
    // add label for new user
    auto tempHBoxLayout = new QHBoxLayout();
    auto tempLabel = new QLabel("User " + QString::number(id));
    tempLabel->setFont(this->font);
    tempHBoxLayout->addWidget(tempLabel, 0, Qt::AlignRight);
    this->usersLayout->addLayout(tempHBoxLayout);

    // add new tab widget for new user
    QScrollArea* scrollArea = new QScrollArea();
    scrollArea->setWidgetResizable(true);
    this->tabChats->addTab(scrollArea, "User " + QString::number(id) + " Chat");
    QWidget *w = new QWidget;
    QVBoxLayout *vb = new QVBoxLayout();
    w->setLayout(vb);
    scrollArea->setWidget(w);

    // add new user to map
    User *user = new User(id, this->userIndex);
    this->users.insert(id, user);
}

void ClientWidget::removeUser(unsigned int id){
    auto index_user = this->users[id]->getIndex();
    QLayout *Layout = this->usersLayout->takeAt(index_user)->layout();

    // remove label
    for (int i = 0; i < Layout->count(); i++){
        QWidget *item = Layout->itemAt(i)->widget();
        delete item;
    }
    delete Layout;

    // remove tab widget
    this->tabChats->removeTab(index_user + 2);


    // remove user from map
    this->log->append("User " + QString::number(id) + " disconnected");
    delete users[id];
    this->users.remove(id);

    // decrease values
    for (auto usr : this->users){
        if (usr->getIndex() >= index_user){
            usr->setIndex(usr->getIndex()-1);
        }
    }
    this->userIndex--;
}



