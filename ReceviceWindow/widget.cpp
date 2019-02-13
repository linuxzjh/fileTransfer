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

    QValidator *validator = new QIntValidator(0, 65535, this);
    ui->lineEdit_port->setValidator(validator);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::onConnectBtnClk()
{
    QString ip = ui->lineEdit_ip->text();
    quint64 port = ui->lineEdit_port->text().toInt();
    QHostAddress ipAddress(ip);
    if (! ipAddress.isNull() && (0 != port))
    recevicer->connectToHost(ipAddress, port);
}

void Widget::onSendLog(QString text)
{
    ui->plainTextEdit->appendPlainText(text);
}

void Widget::onSendTotalSize(qint64 totalSize)
{
    ui->plainTextEdit->appendPlainText(QString("total size: %1 KB").arg(totalSize / 1024));
    ui->progressBar->setMaximum(totalSize / 1024);
}

void Widget::onSendCurReceivedDataSize(qint64 receivedSize)
{
    qint64 recvData = receivedSize / 1024;
    ui->plainTextEdit->appendPlainText(QString("%1 KB").arg(QString::number(recvData)));
    ui->progressBar->setValue(recvData);
}
