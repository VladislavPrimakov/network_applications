#include "clientpop3widget.h"
#include "ui_clientpop3widget.h"

Message::Message(){
    id = -1;
    isChecked = false;
}

void Message::setId(int id_){
    id = id_;
}

int Message::getId() const{
    return id;
}

void Message::addMessage(const QString &message_){
    message.append(message_);
}

const QString& Message::getMessage() const{
    return message;
}

void Message::clear(){
    id = -1;
    message.clear();
}

Message& Message::operator=(Message& rv){
    id = rv.id;
    message = rv.message;
    return *this;
}

void Message::setCheck(bool check_){
    isChecked = check_;
}

bool Message::getCheck() const{
    return isChecked;
}

ClientPOP3Widget::ClientPOP3Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ClientPOP3Widget)
{
    ui->setupUi(this);
    font = new QFont();
    font->setPointSize(14);
    setWindowTitle("Client POP3 Widget");

    QScrollArea* scrollArea = new QScrollArea();
    scrollArea->setWidgetResizable(true);
    scrollArea->resize(this->size());
    scrollArea->setSizePolicy(QSizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding));
    QVBoxLayout *scrollLayout = new QVBoxLayout();
    scrollLayout->setAlignment(Qt::AlignCenter);
    scrollLayout->addWidget(scrollArea);
    QWidget *w = new QWidget();
    QVBoxLayout *mainLayout_ = new QVBoxLayout();
    mainLayout_->setAlignment(Qt::AlignCenter);
    mainLayout = mainLayout_;
    w->setLayout(mainLayout);
    scrollArea->setWidget(w);
    setLayout(scrollLayout);

    log = new QTextBrowser();
    log->setFont(*font);
    log->setMaximumHeight(150);
    scrollLayout->addWidget(log);

    stateLayout = new QVBoxLayout();
    stateLayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    mainLayout->addLayout(stateLayout);

    btnLayout = new QHBoxLayout();
    btnLayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    mainLayout->addLayout(btnLayout);

    paginationLayout = new QHBoxLayout();
    paginationLayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    mainLayout->addLayout(paginationLayout);

    messagesLayout = new QVBoxLayout();
    messagesLayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    mainLayout->addLayout(messagesLayout);

    ShowAuthForm();

    socket = new QSslSocket(this);
    connect(socket, SIGNAL(readyRead()), this, SLOT(socketReady()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(socketDisconnect()));
    commandsQueue = new QQueue<QPair<Commands, QVector<QString>>>;
    messages = QVector<Message>(pageSize);
}

ClientPOP3Widget::~ClientPOP3Widget()
{
    delete ui;
}

void ClientPOP3Widget::socketReady(){
    while(socket->canReadLine()){
        QByteArray byteArray = socket->readLine();
        QString s = QString::fromLatin1(byteArray);
        if (s.contains("+OK") && s.contains("ready")){
            LogMessage(s.chopped(2), TypeCommands::receive);
            if (usedProxy == true && connectedToServer == false){
                cPRXA();
            }
            if (usedProxy == true && connectedToServer == true){
                cUSER();
            }
            if (usedProxy == false){
                cUSER();
            }
        } else{
            auto pair = commandsQueue->at(0);
            switch (pair.first) {
            case USER:
                aUSER(s);
                break;
            case PASS:
                aPASS(s);
                break;
            case QUIT:
                aQUIT(s);
                break;
            case STAT:
                aSTAT(s);
                break;
            case RETR:
                aRETR(s, pair.second);
                break;
            case DELE:
                aDELE(s, pair.second);
                break;
            case RSET:
                aRSET(s);
                break;
            case PRXA:
                aPRXA(s);
                break;
            }
        }
        s.clear();
    }
}

void ClientPOP3Widget::socketDisconnect(){
    LogMessage("Socket disconnect", TypeCommands::local);
    HideEmailForm();
    HideAuthForm();
    ShowAuthForm();
}

void ClientPOP3Widget::ShowAuthForm(){
    authLayout = new QVBoxLayout();
    authLayout->setAlignment(Qt::AlignCenter);

    QHostAddress localHost = QHostAddress::LocalHost;

    QHBoxLayout *adressLayout = new QHBoxLayout();
    QLabel *adressLabel = new QLabel("Adress");
    adressLabel->setFont(*font);
    adressLayout->addWidget(adressLabel);
    adressLine = new QLineEdit(localHost.toString());
    adressLine->setFont(*font);
    adressLayout->addWidget(adressLine);
    authLayout->addLayout(adressLayout);

    QHBoxLayout *portLayout = new QHBoxLayout();
    QLabel *portLabel = new QLabel("Port");
    portLabel->setFont(*font);
    portLayout->addWidget(portLabel);
    portLine = new QLineEdit("995");
    portLine->setFont(*font);
    portLayout->addWidget(portLine);
    authLayout->addLayout(portLayout);

    QHBoxLayout *emailLayout = new QHBoxLayout();
    QLabel *emailLabel = new QLabel("e-mail");
    emailLabel->setFont(*font);
    emailLayout->addWidget(emailLabel);
    emailLine = new QLineEdit();
    emailLine->setFont(*font);
    emailLayout->addWidget(emailLine);
    authLayout->addLayout(emailLayout);

    QHBoxLayout *passLayout = new QHBoxLayout();
    QLabel *passLabel = new QLabel("Password");
    passLabel->setFont(*font);
    passLayout->addWidget(passLabel);
    passLine = new QLineEdit();
    passLine->setEchoMode(QLineEdit::EchoMode::Password);
    passLine->setFont(*font);
    passLayout->addWidget(passLine);
    authLayout->addLayout(passLayout);

    QHBoxLayout *useProxyLayout = new QHBoxLayout();
    useProxyLayout->setAlignment(Qt::AlignLeft);
    QLabel *useProxyLabel = new QLabel("Use proxy?");
    useProxyLabel->setFont(*font);
    useProxyLayout->addWidget(useProxyLabel);
    useProxy = new QCheckBox();
    useProxyLayout->addWidget(useProxy);
    authLayout->addLayout(useProxyLayout);

    QHBoxLayout *adressProxyLayout = new QHBoxLayout();
    QLabel *adressProxyLabel = new QLabel("Adress proxy");
    adressProxyLabel->setFont(*font);
    adressProxyLayout->addWidget(adressProxyLabel);
    adressProxyLine = new QLineEdit(localHost.toString());
    adressProxyLine->setFont(*font);
    adressProxyLayout->addWidget(adressProxyLine);
    authLayout->addLayout(adressProxyLayout);

    QHBoxLayout *portProxyLayout = new QHBoxLayout();
    QLabel *portProxyLabel = new QLabel("Port proxy");
    portProxyLabel->setFont(*font);
    portProxyLayout->addWidget(portProxyLabel);
    portProxyLine = new QLineEdit("1080");
    portProxyLine->setFont(*font);
    portProxyLayout->addWidget(portProxyLine);
    authLayout->addLayout(portProxyLayout);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    QPushButton *button = new QPushButton("Log in");
    button->setFont(*font);
    buttonLayout->addWidget(button);
    connect(button, &QPushButton::clicked, this, &ClientPOP3Widget::LogIn);
    authLayout->addLayout(buttonLayout);

    mainLayout->addLayout(authLayout);
    showMaximized();
}

void ClientPOP3Widget::HideAuthForm(){
    HideLayout(authLayout);
}

void ClientPOP3Widget::ShowEmailForm(){
    QPushButton *uncheckBtn = new QPushButton("Uncheck all messages");
    uncheckBtn->setFont(*font);
    uncheckBtn->setMaximumWidth(300);
    connect(uncheckBtn, &QPushButton::clicked, this, &ClientPOP3Widget::cRSET);
    btnLayout->addWidget(uncheckBtn);

    QPushButton *deleteBtn = new QPushButton("QUIT (Delete marked messages)");
    deleteBtn->setFont(*font);
    deleteBtn->setMaximumWidth(300);
    connect(deleteBtn, &QPushButton::clicked, this, &ClientPOP3Widget::cQUIT);
    btnLayout->addWidget(deleteBtn);

    cSTAT();
    setWindowTitle("Client POP3 Widget " + name);
}

void ClientPOP3Widget::HideEmailForm(){
    HideLayout(stateLayout);
    HideLayout(btnLayout);
    HideLayout(messagesLayout);
    HideLayout(paginationLayout);
    for (auto &msg : messages){
        msg.clear();
    }
    messagesNumber = 0;
    messagesSize = 0;
    name = "";
    setWindowTitle("Client POP3 Widget");
    currentPage = 1;
    commandsQueue->clear();
    usedProxy = false;
    connectedToServer = false;
}

void ClientPOP3Widget::ShowPagination(){
    HideLayout(paginationLayout);
    QPushButton *firstPage = new QPushButton("<<");
    firstPage->setFont(*font);
    firstPage->setMinimumSize(50,50);
    firstPage->setMaximumSize(50,50);
    connect(firstPage, &QPushButton::clicked, this, &ClientPOP3Widget::GoToFirstPage);

    QPushButton *prevPage = new QPushButton("<");
    prevPage->setFont(*font);
    prevPage->setMinimumSize(50,50);
    prevPage->setMaximumSize(50,50);
    connect(prevPage, &QPushButton::clicked, this, &ClientPOP3Widget::GoToPrevPage);

    QLabel *labelPage = new QLabel(QString::number(currentPage));
    labelPage->setFont(*font);

    QPushButton *nextPage = new QPushButton(">");
    nextPage->setFont(*font);
    nextPage->setMinimumSize(50,50);
    nextPage->setMaximumSize(50,50);
    connect(nextPage, &QPushButton::clicked, this,  &ClientPOP3Widget::GoToNextPage);

    QPushButton *lastPage = new QPushButton(">>");
    lastPage->setFont(*font);
    lastPage->setMinimumSize(50,50);
    lastPage->setMaximumSize(50,50);
    connect(lastPage, &QPushButton::clicked, this, &ClientPOP3Widget::GoToLastPage);

    int minPage = 1;
    int maxPage = qCeil((double)(messagesNumber / (double)pageSize));
    if (currentPage > minPage + 1){
        paginationLayout->addWidget(firstPage);
    }
    if (currentPage > minPage){
        paginationLayout->addWidget(prevPage);
    }

    paginationLayout->addWidget(labelPage);

    if (currentPage < maxPage){
        paginationLayout->addWidget(nextPage);
    }
    if (currentPage < maxPage - 1){
        paginationLayout->addWidget(lastPage);
    }
    ShowEmailsInRange();
}

void ClientPOP3Widget::GoToNextPage(){
    currentPage++;
    ShowPagination();
}

void ClientPOP3Widget::GoToPrevPage(){
    currentPage--;
    ShowPagination();
}

void ClientPOP3Widget::GoToFirstPage(){
    currentPage = 1;
    ShowPagination();
}

void ClientPOP3Widget::GoToLastPage(){
    currentPage = qCeil((double)(messagesNumber / (double)pageSize));
    ShowPagination();
}

void ClientPOP3Widget::HideLayout(QLayout *l){
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

void ClientPOP3Widget::ShowEmailsInRange(){
    HideLayout(messagesLayout);
    for (int i = 0; i < messages.size(); i++){
        messages[i].clear();
    }
    int b = qMin(messagesNumber, currentPage*pageSize);
    int a = (currentPage - 1) * pageSize + 1;
    if (b > 0){
        for (int i = a; i <= b; i++){
            cRETR(i);
        }
    }
}

void ClientPOP3Widget::LogIn(){
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
            LogMessage("Failed to load CA certificates from the bundle file", TypeCommands::local);
        }
    } else {
        LogMessage("Could not open CA bundle file", TypeCommands::local);
    }

    config.setPeerVerifyMode(QSslSocket::VerifyNone);

    qInfo() << QSslSocket::sslLibraryVersionString();
    qInfo() << QSslSocket::sslLibraryBuildVersionString();

    socket->setSslConfiguration(config);
    usedProxy = useProxy->checkState() == Qt::Checked ? true : false;
    // use proxy
    if (usedProxy){
        socket->connectToHostEncrypted(adressProxyLine->text(), portProxyLine->text().toInt());
    }
    // use direct connection
    else {
        socket->connectToHostEncrypted(adressLine->text(), portLine->text().toInt());
    }
    bool state = socket->waitForConnected(1000);
    if (state == false){
        LogMessage("Cannot connect to the server", TypeCommands::local);
    }
}

void ClientPOP3Widget::ShowMessage(int index){
    QHBoxLayout *messageLayout = new QHBoxLayout();
    QCheckBox *CheckMessage = new QCheckBox();
    connect(CheckMessage, &QCheckBox::stateChanged, this, [this, index](int state){
        this->CheckMessage(state, index);
    });
    CheckMessage->setMinimumWidth(20);
    CheckMessage->setMaximumWidth(20);
    messageLayout->addWidget(CheckMessage);

    QLabel *labelMessage = new QLabel();
    labelMessage->setFont(*font);
    QString s = messages.at(index).getMessage();
    QString subj = FindTag(s, QString("Subject:"));
    QString from = FindTag(s, QString("From:"));
    QString date = FindTag(s, QString("Date:"));
    QString to = FindTag(s, QString("To:"));

    labelMessage->setText(subj + from + to + date);
    messageLayout->addWidget(labelMessage);

    messagesLayout->addLayout(messageLayout);
}

QString ClientPOP3Widget::FindTag(const QString& s, QString tag){
    tag = "\r\n" + tag;
    int b = s.indexOf(tag);
    if (b == -1){
        bool startWith = s.startsWith(tag.last(tag.size()-2));
        if (startWith){
            b = 0;
        } else {
            return QString();
        }
    } else{
        b += 2;
    }
    int e = s.indexOf("\r\n", b);
    QString result;
    while (1){
        QString r = s.sliced(b, (e-b) + 2);
        result += Imap::decodeRFC2047String(r.toLatin1());
        if ((e + 2) < s.size()){
            if (s.at(e + 2) == '\t' || s.at(e + 2) == ' '){
                b = e + 2;
                e = s.indexOf("\r\n", b);
            } else{
                break;
            }
        } else{
            break;
        }
    }
    return result;
}

void ClientPOP3Widget::CheckMessage(int state, int index){
    if (state){
        cDELE(messages.at(index).getId());
    }
}


void ClientPOP3Widget::LogMessage(const QString &s, TypeCommands t){
    QString result;
    if (t == TypeCommands::receive){
        result.append("Receive < ");
    }
    if (t == TypeCommands::transmit){
        result.append("Transmit > ");
    }
    result.append(s);
    log->append(result);
    qInfo() << result << Qt::endl;
}

void ClientPOP3Widget::cPRXA(){
    QString adrServer = adressLine->text();
    QString port = portLine->text();
    QString command = "PRXA " + adrServer + " " + port + "\r\n";
    socket->write(command.toLatin1());
    commandsQueue->append(QPair<Commands, QVector<QString>>(Commands::PRXA, NULL));
    LogMessage(command.chopped(2), TypeCommands::transmit);
}

void ClientPOP3Widget::aPRXA(QString answer){
    if (answer[0] == '+'){
        connectedToServer = true;
    }
    if (answer[0] == '-'){
        socket->close();
    }
    if (!commandsQueue->isEmpty())
        commandsQueue->removeFirst();
    LogMessage(answer.chopped(2), TypeCommands::receive);
}

void ClientPOP3Widget::cUSER(){
    QString name_ = emailLine->text();
    QString command = "USER " + name_ + "\r\n";
    name = name_;
    commandsQueue->append(QPair<Commands, QVector<QString>>(Commands::USER, NULL));
    socket->write(command.toLatin1());
    LogMessage(command.chopped(2), TypeCommands::transmit);
}

void ClientPOP3Widget::aUSER(QString answer){
    if (answer[0] == '+'){
        cPASS();
    }else{
        socket->close();
    }
    if (!commandsQueue->isEmpty())
        commandsQueue->removeFirst();
    LogMessage(answer.chopped(2), TypeCommands::receive);
}

void ClientPOP3Widget::cPASS(){
    QString pass_ = passLine->text();
    QString command = "PASS " + pass_ + "\r\n";
    commandsQueue->append(QPair<Commands, QVector<QString>>(Commands::PASS, NULL));
    socket->write(command.toLatin1());
    LogMessage("PASS ***", TypeCommands::transmit);
}

void ClientPOP3Widget::aPASS(QString answer){
    if (answer[0] == '+'){
        HideAuthForm();
        ShowEmailForm();
    }else{
        socket->close();
    }
    if (!commandsQueue->isEmpty())
        commandsQueue->removeFirst();
    LogMessage(answer.chopped(2), TypeCommands::receive);
}

void ClientPOP3Widget::cSTAT(){
    QString command = "STAT\r\n";
    commandsQueue->append(QPair<Commands, QVector<QString>>(Commands::STAT, NULL));
    socket->write(command.toLatin1());
    LogMessage(command.chopped(2), TypeCommands::transmit);
}

void ClientPOP3Widget::aSTAT(QString answer){
    if (answer[0] == '+'){
        auto arr = answer.split(' ');
        messagesNumber = arr[1].toInt();
        messagesSize = arr[2].toInt();
        QLabel *messagesCount_ = new QLabel("Messages count: " + QString::number(messagesNumber));
        messagesCount_->setFont(*font);
        QLabel *messagesSize_ = new QLabel("Messages size: " + QString::number(messagesSize / 1024 / 1024) + "MB");
        messagesSize_->setFont(*font);
        stateLayout->addWidget(messagesCount_);
        stateLayout->addWidget(messagesSize_);
        ShowPagination();
    }
    if (!commandsQueue->isEmpty())
        commandsQueue->removeFirst();
    LogMessage(answer.chopped(2), TypeCommands::receive);
}

void ClientPOP3Widget::cRETR(int number){
    QString command = "RETR " + QString::number(number) + "\r\n";
    commandsQueue->append(QPair<Commands, QVector<QString>>(Commands::RETR, QVector<QString>({QString::number(number)})));
    socket->write(command.toLatin1());
    LogMessage(command.chopped(2), TypeCommands::transmit);
}

void ClientPOP3Widget::aRETR(QString answer, QVector<QString> args){
    if (!answer.contains("-ERR")){
        int index = (args[0].toInt() - 1) % pageSize;
        // set id message for the first time
        if (messages.at(index).getId() == -1){
            messages[index].setId(args[0].toInt());
        }
        if (!answer.contains("message follows")){
            // if the end of messega
            if (answer.at(0) == '.' && answer.size() == 3){
                if (!commandsQueue->isEmpty())
                    commandsQueue->removeFirst();
                ShowMessage(index);
            }
            else{
                // remove two points at the begin
                if(answer.at(0) == '.'){
                    answer.removeFirst();
                }
                messages[index].addMessage(answer);
            }
        }
    }
    if (answer.contains("+OK")){
        LogMessage(answer.chopped(2), TypeCommands::receive);
    }
}

void ClientPOP3Widget::cDELE(int number){
    QString command = "DELE " + QString::number(number) + "\r\n";
    commandsQueue->append(QPair<Commands, QVector<QString>>(Commands::DELE, QVector<QString>({QString::number(number)})));
    socket->write(command.toLatin1());
    LogMessage(command.chopped(2), TypeCommands::transmit);
}

void ClientPOP3Widget::aDELE(QString answer, QVector<QString> args){
    int index = -1;
    // find index of message
    for (int i = 0; i < messages.size(); i++){
        if (messages[i].getId() == args[0].toInt()){
            index = i;
            break;
        }
    }
    // if message deleted block check box
    QLayoutItem *item = messagesLayout->itemAt(index);
    QLayout *layout = item->layout();
    QCheckBox *cb = (QCheckBox*)layout->itemAt(0)->widget();
    if (answer[0] == '+'){
        cb->setEnabled(false);
        messages[index].setCheck(true);
    }
    if (answer[0] == '-'){
        messages[index].setCheck(false);
        cb->setCheckState(Qt::Unchecked);
    }
    if (!commandsQueue->isEmpty())
        commandsQueue->removeFirst();
    LogMessage(answer.chopped(2), TypeCommands::receive);
}

void ClientPOP3Widget::cRSET(){
    QString command = "RSET\r\n";
    commandsQueue->append(QPair<Commands, QVector<QString>>(Commands::RSET, NULL));
    socket->write(command.toLatin1());
    LogMessage(command.chopped(2), TypeCommands::transmit);
}

void ClientPOP3Widget::aRSET(QString answer){
    if (answer[0] == '+'){
        // uncheck all messages
        for (int i = 0; i < messagesLayout->count(); i++){
            QLayoutItem *item = messagesLayout->itemAt(i);
            QLayout *layout = item->layout();
            QCheckBox *cb = (QCheckBox*)layout->itemAt(0)->widget();
            cb->setEnabled(true);
            cb->setCheckState(Qt::Unchecked);
            messages[i].setCheck(false);
        }
    }
    LogMessage(answer.chopped(2), TypeCommands::receive);
    if (!commandsQueue->isEmpty())
        commandsQueue->removeFirst();
}

void ClientPOP3Widget::cQUIT(){
    QString commandQUIT = "QUIT\r\n";
    socket->write(commandQUIT.toLatin1());
    commandsQueue->append(QPair<Commands, QVector<QString>>(Commands::QUIT, NULL));
    LogMessage(commandQUIT.chopped(2), TypeCommands::transmit);
}

void ClientPOP3Widget::aQUIT(QString answer){
    if (answer[0] == '+'){
        socket->close();
    }
    if (!commandsQueue->isEmpty())
        commandsQueue->removeFirst();
    LogMessage(answer.chopped(2), TypeCommands::receive);
}
