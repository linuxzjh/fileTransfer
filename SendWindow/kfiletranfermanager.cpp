#include "kfiletranfermanager.h"
#include "sender.h"

#include <QTcpServer>

KFileTranferManager::KFileTranferManager(QObject *parent) : QObject(parent)
{
    _pTcpServer = new QTcpServer(this);
    connect(_pTcpServer, SIGNAL(newConnection()), this, SLOT(onNewConnection()));
}

void KFileTranferManager::setTransferFileList(QList<const QString &> files)
{
    _files = files;
}

void KFileTranferManager::onNewConnection()
{
    QTcpSocket *socket = _pTcpServer->nextPendingConnection();
    Sender *sender = new Sender(socket->socketDescriptor(), this);
    _senderList.append(sender);
}
