#include "sender.h"

#include <QFile>
#include <QDataStream>

#define READ_BLOCK_SIZE (qint64)(4 * 1024 * 1024)

Sender::Sender(qintptr socketDesc, QObject *parent)
    : QObject(parent)
    , _pFile(nullptr)
    , _totalSize(0)
    , _bytesToWrite(0)
    , _bytesWritten(0)
{
    _pFile = new QFile(this);
    _pTcpSocket = new QTcpSocket(this);
    connect(_pTcpSocket, SIGNAL(bytesWritten(qint64)), this, SLOT(updateProgress(qint64)));
    connect(_pTcpSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(displayError(QAbstractSocket::SocketError)));
    _pTcpSocket->setSocketDescriptor(socketDesc);
}

void Sender::startTransfer()
{
    if(!_pFile->open(QFile::ReadOnly))
    {
        qDebug() << "open file error!!!";
        return;
    }

    //发送文件信息
    _bytesWritten = 0;
    _totalSize = _pFile->size();
    QByteArray data;
    QDataStream out(&data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_9);

    QString fileName = _pFile->fileName();
    fileName = fileName.right(fileName.size()-fileName.lastIndexOf('/') - 1);
    out << qint64(0) << qint64(0) << fileName;
    _totalSize += data.size();
    out.device()->seek(0);
    out << qint64(_totalSize) << qint64((data.size() - sizeof(qint64) * 2));
    _bytesToWrite = _totalSize - _pTcpSocket->write(data);
}

void Sender::updateProgress(qint64 numBytes)
{
    _bytesWritten += numBytes;

    //如果还有数据要发，就继续发送
    if (_bytesToWrite > 0)
    {
        QByteArray data;
        data = _pFile->read(qMin(_bytesToWrite, READ_BLOCK_SIZE));
        _bytesToWrite -= _pTcpSocket->write(data);
    }
    else
    {
        _pTcpSocket->close();
    }

    if (_bytesWritten == _totalSize)
    {
        qDebug() << tr("send file %1 success!!!").arg(_pFile->fileName());
        if (_pFile->isOpen())
        {
            _pFile->close();
        }
        _pTcpSocket->close();
    }
    qDebug() << "已发送大小: " << _bytesWritten / 1024 << "KB\n"
             << "剩余大小: " << _bytesToWrite / 1024 << "KB\n";
}

void Sender::displayError(QAbstractSocket::SocketError)
{
    qDebug() << "tcp error:" << _pTcpSocket->errorString();
    _pTcpSocket->close();
}

void Sender::SendFile(const QString & file)
{
   _pFile->setFileName(file);
}
