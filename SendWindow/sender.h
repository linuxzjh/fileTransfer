#ifndef SENDER_H
#define SENDER_H

#include <QObject>
#include <QTcpSocket>

class QFile;

class Sender : public QObject
{
    Q_OBJECT
public:
    explicit Sender(qintptr socketDesc, QObject *parent = nullptr);
    void SendFile(const QString & file);
    void startTransfer();
    void SendByteArray(const QByteArray& data);
signals:

public slots:
    void updateProgress(qint64);
    void displayError(QAbstractSocket::SocketError);
private:

    qint64 _totalSize;     //总大小包括: 传输文件的大小; 2*sizeof（qint64）的大小; 用于记录总大小及文件名大小，文件名的大小;
    qint64 _bytesToWrite;  //剩余数据大小
    qint64 _bytesWritten;  //已发送数据的大小
    QFile *_pFile;
    QTcpSocket *_pTcpSocket;
};

#endif // SENDER_H
