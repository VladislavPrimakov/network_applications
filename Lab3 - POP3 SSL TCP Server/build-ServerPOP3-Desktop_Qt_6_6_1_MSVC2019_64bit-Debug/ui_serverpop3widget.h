/********************************************************************************
** Form generated from reading UI file 'serverpop3widget.ui'
**
** Created by: Qt User Interface Compiler version 6.6.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SERVERPOP3WIDGET_H
#define UI_SERVERPOP3WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ServerPOP3Widget
{
public:

    void setupUi(QWidget *ServerPOP3Widget)
    {
        if (ServerPOP3Widget->objectName().isEmpty())
            ServerPOP3Widget->setObjectName("ServerPOP3Widget");
        ServerPOP3Widget->resize(800, 600);

        retranslateUi(ServerPOP3Widget);

        QMetaObject::connectSlotsByName(ServerPOP3Widget);
    } // setupUi

    void retranslateUi(QWidget *ServerPOP3Widget)
    {
        ServerPOP3Widget->setWindowTitle(QCoreApplication::translate("ServerPOP3Widget", "ServerPOP3Widget", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ServerPOP3Widget: public Ui_ServerPOP3Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SERVERPOP3WIDGET_H
