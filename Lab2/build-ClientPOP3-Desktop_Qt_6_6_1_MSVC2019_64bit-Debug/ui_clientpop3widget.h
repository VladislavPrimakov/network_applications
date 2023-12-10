/********************************************************************************
** Form generated from reading UI file 'clientpop3widget.ui'
**
** Created by: Qt User Interface Compiler version 6.6.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CLIENTPOP3WIDGET_H
#define UI_CLIENTPOP3WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ClientPOP3Widget
{
public:

    void setupUi(QWidget *ClientPOP3Widget)
    {
        if (ClientPOP3Widget->objectName().isEmpty())
            ClientPOP3Widget->setObjectName("ClientPOP3Widget");
        ClientPOP3Widget->resize(16, 16);

        retranslateUi(ClientPOP3Widget);

        QMetaObject::connectSlotsByName(ClientPOP3Widget);
    } // setupUi

    void retranslateUi(QWidget *ClientPOP3Widget)
    {
        ClientPOP3Widget->setWindowTitle(QCoreApplication::translate("ClientPOP3Widget", "ClientPOP3Widget", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ClientPOP3Widget: public Ui_ClientPOP3Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CLIENTPOP3WIDGET_H
