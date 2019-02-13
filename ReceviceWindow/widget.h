#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

#include "recevicer.h"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
public slots:
    void onConnectBtnClk();
    void onSendLog(QString);
    void onSendTotalSize(qint64);
    void onSendCurReceivedDataSize(qint64);
private:
    Ui::Widget *ui;
    Recevicer *recevicer;
};

#endif // WIDGET_H
