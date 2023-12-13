#ifndef POP3PROXYWIDGET_H
#define POP3PROXYWIDGET_H

#include <QWidget>
#include "pop3proxy.h"


QT_BEGIN_NAMESPACE
namespace Ui {
class POP3ProxyWidget;
}
QT_END_NAMESPACE

class POP3ProxyWidget : public QWidget
{
    Q_OBJECT

public:
    POP3ProxyWidget(QWidget *parent = nullptr);
    ~POP3ProxyWidget();

private:
    Ui::POP3ProxyWidget *ui;
};
#endif // POP3PROXYWIDGET_H
