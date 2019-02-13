#include "kfiletranfermanager.h"
#include "sender.h"

#include <QTcpServer>

KFileTranferManager::KFileTranferManager(QObject *parent)
    : QObject(parent)
    , _port(60000)
{
    _pTcpServer = new QTcpServer(this);
    connect(_pTcpServer, SIGNAL(newConnection()), this, SLOT(onNewConnection()));
}

void KFileTranferManager::setTransferFileList(QList<QString> files)
{
    _files = files;
}

bool KFileTranferManager::startTranfer()
{
    if (_files.count() == 0) return false;

    for (Sender *sender : _senderList)
    {
        sender->SendFile(_files.at(0));
        sender->startTransfer();
    }
    return true;
}

bool KFileTranferManager::setListenPort(quint64 port)
{
    if (port > 0 && port < 65535)
    {
        qDebug() << QString("prot changed, port = %1").arg(QString::number(port));
        _port = port;
        _pTcpServer->close();
        return _pTcpServer->listen(QHostAddress::Any, _port);
    }
    return false;
}

void KFileTranferManager::onNewConnection()
{
    QTcpSocket *socket = _pTcpServer->nextPendingConnection();
    Sender *sender = new Sender(socket->socketDescriptor(), this);
    QHostAddress address = socket->peerAddress();
    quint16 port = socket->peerPort();
    _senderList.append(sender);
    qDebug() << QString("new client connected ip = %1, port = %2.").arg(address.toString()).arg(QString::number(port));
}
