#include "widget.h"
#include "ui_widget.h"

#include <QHostAddress>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    recevicer = new Recevicer(this);
    connect(ui->btn_connect, SIGNAL(clicked(bool)), this, SLOT(onConnectBtnClk()));
    connect(recevicer, SIGNAL(sendLog(QString)), this, SLOT(onSendLog(QString)));
    connect(recevicer, SIGNAL(sendTotalSize(qint64)), this, SLOT(onSendTotalSize(qint64)));
    connect(recevicer, SIGNAL(sendCurReceivedDataSize(qint64)), this, SLOT(onSendCurReceivedDataSize(qint64)));
}

Widget::~Widget()
{
    delete ui;
}

void Widget::onConnectBtnClk()
{
    recevicer->connectToHost(QHostAddress::LocalHost, PORT);
}

void Widget::onSendLog(QString text)
{
    ui->plainTextEdit->appendPlainText(text);
}

void Widget::onSendTotalSize(qint64 totalSize)
{
    ui->progressBar->setMaximum(totalSize);
}

void Widget::onSendCurReceivedDataSize(qint64 receivedSize)
{
    ui->progressBar->setValue(receivedSize);
}
