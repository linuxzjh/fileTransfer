#include "widget.h"
#include "ui_widget.h"

#include "kfiletranfermanager.h"
#include "Singleton.h"

#include <QFileDialog>
#include <QDebug>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    connect(ui->btn_selectFile, SIGNAL(clicked(bool)), this, SLOT(onSelectFileClicked(bool)));
    connect(ui->btn_send, SIGNAL(clicked(bool)), this, SLOT(onSendFileClicked(bool)));
    connect(ui->btn_listen, SIGNAL(clicked(bool)), this, SLOT(onStartListen(bool)));
    SINGLETON(KFileTranferManager);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::onSelectFileClicked(bool)
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("选择文件"), "", "Any files (*)");
    if (QFile::exists(fileName))
    {
        QList<QString> files;
        files.append(fileName);
        ui->lineEdit->setText(fileName);
        SINGLETON(KFileTranferManager)->setTransferFileList(files);
    }
}

void Widget::onSendFileClicked(bool)
{
    bool bOk = SINGLETON(KFileTranferManager)->startTranfer();
}

void Widget::onStartListen(bool)
{
    quint64 port = ui->lineEdit_port->text().toInt();
    if (port <= 0  || port > 65535)
    {
        ui->lineEdit_port->setText("60000");
        //qDebug() << "prot set error, reset default value.";
    }
    SINGLETON(KFileTranferManager)->setListenPort(port);
}
