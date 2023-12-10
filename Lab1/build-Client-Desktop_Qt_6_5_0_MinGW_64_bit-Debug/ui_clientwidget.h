/********************************************************************************
** Form generated from reading UI file 'clientwidget.ui'
**
** Created by: Qt User Interface Compiler version 6.5.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CLIENTWIDGET_H
#define UI_CLIENTWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ClientWidget
{
public:
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_2;
    QTabWidget *tabWidget;
    QWidget *TabGeneralChat;
    QWidget *tab_2;
    QVBoxLayout *verticalLayout_2;
    QLabel *labelUserList;
    QVBoxLayout *usersLayout;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label;
    QLineEdit *InputLine;
    QPushButton *EnterLine;
    QHBoxLayout *horizontalLayout_4;
    QLabel *labelLog;
    QTextBrowser *Log;
    QHBoxLayout *horizontalLayout;
    QPushButton *pushButton;
    QPushButton *pushButton_2;

    void setupUi(QWidget *ClientWidget)
    {
        if (ClientWidget->objectName().isEmpty())
            ClientWidget->setObjectName("ClientWidget");
        ClientWidget->resize(1000, 800);
        ClientWidget->setMinimumSize(QSize(1000, 800));
        ClientWidget->setMaximumSize(QSize(1000, 800));
        verticalLayoutWidget = new QWidget(ClientWidget);
        verticalLayoutWidget->setObjectName("verticalLayoutWidget");
        verticalLayoutWidget->setGeometry(QRect(10, 10, 981, 791));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(5);
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        horizontalLayout_2->setContentsMargins(5, 5, 5, 5);
        tabWidget = new QTabWidget(verticalLayoutWidget);
        tabWidget->setObjectName("tabWidget");
        tabWidget->setMinimumSize(QSize(700, 500));
        tabWidget->setMaximumSize(QSize(700, 500));
        TabGeneralChat = new QWidget();
        TabGeneralChat->setObjectName("TabGeneralChat");
        QFont font;
        font.setPointSize(15);
        TabGeneralChat->setFont(font);
        tabWidget->addTab(TabGeneralChat, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName("tab_2");
        tabWidget->addTab(tab_2, QString());

        horizontalLayout_2->addWidget(tabWidget);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(0);
        verticalLayout_2->setObjectName("verticalLayout_2");
        verticalLayout_2->setContentsMargins(0, 0, -1, -1);
        labelUserList = new QLabel(verticalLayoutWidget);
        labelUserList->setObjectName("labelUserList");
        labelUserList->setMinimumSize(QSize(0, 50));
        labelUserList->setMaximumSize(QSize(200, 50));
        labelUserList->setFont(font);
        labelUserList->setAlignment(Qt::AlignCenter);

        verticalLayout_2->addWidget(labelUserList, 0, Qt::AlignHCenter);

        usersLayout = new QVBoxLayout();
        usersLayout->setSpacing(0);
        usersLayout->setObjectName("usersLayout");
        usersLayout->setContentsMargins(0, 0, 0, 0);

        verticalLayout_2->addLayout(usersLayout);


        horizontalLayout_2->addLayout(verticalLayout_2);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(5);
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        horizontalLayout_3->setContentsMargins(5, 5, 5, 5);
        label = new QLabel(verticalLayoutWidget);
        label->setObjectName("label");
        label->setMinimumSize(QSize(100, 50));
        label->setMaximumSize(QSize(100, 50));
        label->setFont(font);

        horizontalLayout_3->addWidget(label, 0, Qt::AlignLeft|Qt::AlignVCenter);

        InputLine = new QLineEdit(verticalLayoutWidget);
        InputLine->setObjectName("InputLine");
        InputLine->setMinimumSize(QSize(700, 50));
        InputLine->setMaximumSize(QSize(700, 50));
        InputLine->setDragEnabled(false);

        horizontalLayout_3->addWidget(InputLine);

        EnterLine = new QPushButton(verticalLayoutWidget);
        EnterLine->setObjectName("EnterLine");
        EnterLine->setMinimumSize(QSize(100, 50));
        EnterLine->setMaximumSize(QSize(100, 50));
        EnterLine->setFont(font);

        horizontalLayout_3->addWidget(EnterLine);


        verticalLayout->addLayout(horizontalLayout_3);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(5);
        horizontalLayout_4->setObjectName("horizontalLayout_4");
        horizontalLayout_4->setContentsMargins(5, 5, 5, 5);
        labelLog = new QLabel(verticalLayoutWidget);
        labelLog->setObjectName("labelLog");
        labelLog->setMinimumSize(QSize(50, 100));
        labelLog->setMaximumSize(QSize(50, 100));
        labelLog->setFont(font);

        horizontalLayout_4->addWidget(labelLog, 0, Qt::AlignLeft|Qt::AlignVCenter);

        Log = new QTextBrowser(verticalLayoutWidget);
        Log->setObjectName("Log");
        Log->setMinimumSize(QSize(850, 100));
        Log->setMaximumSize(QSize(850, 100));

        horizontalLayout_4->addWidget(Log, 0, Qt::AlignLeft|Qt::AlignVCenter);


        verticalLayout->addLayout(horizontalLayout_4);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(50);
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalLayout->setContentsMargins(-1, 5, -1, 5);
        pushButton = new QPushButton(verticalLayoutWidget);
        pushButton->setObjectName("pushButton");
        QFont font1;
        font1.setPointSize(16);
        pushButton->setFont(font1);

        horizontalLayout->addWidget(pushButton);

        pushButton_2 = new QPushButton(verticalLayoutWidget);
        pushButton_2->setObjectName("pushButton_2");
        pushButton_2->setFont(font);

        horizontalLayout->addWidget(pushButton_2);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(ClientWidget);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(ClientWidget);
    } // setupUi

    void retranslateUi(QWidget *ClientWidget)
    {
        ClientWidget->setWindowTitle(QCoreApplication::translate("ClientWidget", "ClientWidget", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(TabGeneralChat), QCoreApplication::translate("ClientWidget", "General Chat", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QCoreApplication::translate("ClientWidget", "Tab 2", nullptr));
        labelUserList->setText(QCoreApplication::translate("ClientWidget", "User List", nullptr));
        label->setText(QCoreApplication::translate("ClientWidget", "Message", nullptr));
        EnterLine->setText(QCoreApplication::translate("ClientWidget", "Enter", nullptr));
        labelLog->setText(QCoreApplication::translate("ClientWidget", "Log", nullptr));
        pushButton->setText(QCoreApplication::translate("ClientWidget", "Connect", nullptr));
        pushButton_2->setText(QCoreApplication::translate("ClientWidget", "Disconnect", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ClientWidget: public Ui_ClientWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CLIENTWIDGET_H
