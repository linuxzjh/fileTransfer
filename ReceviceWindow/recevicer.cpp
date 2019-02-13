#include "recevicer.h"

#include <QDataStream>
#include <QHostAddress>
#include <QFile>
#include <QDebug>

Recevicer::Recevicer(QObject *parent)
    : QObject(parent)
    , bytesReceived(0)
    , totalBytes(0)
    , fileNameSize(0)
{
    tcpSocket = new QTcpSocket(this);
    connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(receviceData()));
    connect(tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(displayError(QAbstractSocket::SocketError)));
}

void Recevicer::connectToHost(const QString &hostName, quint16 port)
{
    resetData();
    tcpSocket->connectToHost(hostName, port);
    emit sendLog(QString("connect host %1, port %2.").arg(hostName).arg(QString::number(port)));
}

void Recevicer::connectToHost(const QHostAddress &address, quint16 port)
{
    resetData();
    tcpSocket->close();
    tcpSocket->connectToHost(address, port);
    emit sendLog(QString("connect host %1, port %2.").arg(address.toString()).arg(QString::number(port)));
}

void Recevicer::receviceData()
{
    QDataStream in(tcpSocket);
    in.setVersion(QDataStream::Qt_5_9);

    if (bytesReceived <= sizeof(qint64) * 2)
    {
        if ((tcpSocket->bytesAvailable() >= sizeof(qint64) * 2) && (fileNameSize == 0))
        {
            in >> totalBytes >> fileNameSize;
            bytesReceived += sizeof(qint64) * 2;
            emit sendTotalSize(totalBytes);
        }

        if ((tcpSocket->bytesAvailable() >= fileNameSize) && (fileNameSize != 0))
        {
            in >> fileName;
            emit sendLog(QString("start recevice file :%1").arg(fileName));
            bytesReceived += fileNameSize;
            localFile = new QFile(fileName);
            if(!localFile->open(QFile::WriteOnly))
            {
                emit sendLog("open file error!");
                return;
            }
            emit sendCurReceivedDataSize(bytesReceived);
        }
        else
        {
            return;
        }
    }

    if (bytesReceived < totalBytes)
    {
        bytesReceived += tcpSocket->bytesAvailable();
        localFile->write(tcpSocket->readAll());
    }

    if (bytesReceived == totalBytes)
    {
        emit sendLog("file recevice finish");
        localFile->close();
    }
    emit sendCurReceivedDataSize(bytesReceived);
}

void Recevicer::displayError(QAbstractSocket::SocketError)
{
    emit sendLog("socket error:" + tcpSocket->errorString());
}

void Recevicer::resetData()
{
    bytesReceived = 0;
    totalBytes = 0;
    fileNameSize = 0;
    emit sendTotalSize(100);
    emit sendCurReceivedDataSize(0);
}
