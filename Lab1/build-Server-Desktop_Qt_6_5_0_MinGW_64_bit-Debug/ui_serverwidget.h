/********************************************************************************
** Form generated from reading UI file 'serverwidget.ui'
**
** Created by: Qt User Interface Compiler version 6.5.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SERVERWIDGET_H
#define UI_SERVERWIDGET_H

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

class Ui_ServerWidget
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

    void setupUi(QWidget *ServerWidget)
    {
        if (ServerWidget->objectName().isEmpty())
            ServerWidget->setObjectName("ServerWidget");
        ServerWidget->resize(1000, 800);
        ServerWidget->setMinimumSize(QSize(1000, 800));
        ServerWidget->setMaximumSize(QSize(1000, 800));
        QFont font;
        font.setPointSize(15);
        ServerWidget->setFont(font);
        verticalLayoutWidget = new QWidget(ServerWidget);
        verticalLayoutWidget->setObjectName("verticalLayoutWidget");
        verticalLayoutWidget->setGeometry(QRect(10, 10, 991, 791));
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
        TabGeneralChat->setFont(font);
        tabWidget->addTab(TabGeneralChat, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName("tab_2");
        tabWidget->addTab(tab_2, QString());

        horizontalLayout_2->addWidget(tabWidget, 0, Qt::AlignLeft|Qt::AlignVCenter);

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


        retranslateUi(ServerWidget);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(ServerWidget);
    } // setupUi

    void retranslateUi(QWidget *ServerWidget)
    {
        ServerWidget->setWindowTitle(QCoreApplication::translate("ServerWidget", "ServerWidget", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(TabGeneralChat), QCoreApplication::translate("ServerWidget", "General Chat", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QCoreApplication::translate("ServerWidget", "Tab 2", nullptr));
        labelUserList->setText(QCoreApplication::translate("ServerWidget", "User List", nullptr));
        label->setText(QCoreApplication::translate("ServerWidget", "Message", nullptr));
        EnterLine->setText(QCoreApplication::translate("ServerWidget", "Enter", nullptr));
        labelLog->setText(QCoreApplication::translate("ServerWidget", "Log", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ServerWidget: public Ui_ServerWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SERVERWIDGET_H
