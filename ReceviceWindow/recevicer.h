#ifndef RECEVICER_H
#define RECEVICER_H

#include <QObject>
#include <QTcpSocket>

class QFile;

class Recevicer : public QObject
{
    Q_OBJECT
public:
    explicit Recevicer(QObject *parent = nullptr);
    void connectToHost(const QString &hostName, quint16 port);
    void connectToHost(const QHostAddress &address, quint16 port);
signals:
    void sendLog(QString log);
    void sendTotalSize(qint64 bytes);
    void sendCurReceivedDataSize(qint64 bytes);
public slots:
    void receviceData();
    void displayError(QAbstractSocket::SocketError);
private:
    void resetData();
    QTcpSocket *tcpSocket;

    qint64 bytesReceived;
    qint64 totalBytes;
    qint64 fileNameSize;
    QString fileName;
    QFile *localFile;
};

#endif // RECEVICER_H
