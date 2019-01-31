#ifndef KFILETRANFERMANAGER_H
#define KFILETRANFERMANAGER_H

#include <QObject>

class QTcpServer;
class Sender;

class KFileTranferManager : public QObject
{
    Q_OBJECT
public:
    explicit KFileTranferManager(QObject *parent = nullptr);
    void    setTransferFileList(QList<const QString&> files);
signals:

public slots:
    void onNewConnection();
private:
    QTcpServer *_pTcpServer;
    QList<Sender *> _senderList;    //连接的客户端
    QList<const QString&> _files;    //准备传输的文件列表
};

#endif // KFILETRANFERMANAGER_H
