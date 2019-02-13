#ifndef KFILETRANFERMANAGER_H
#define KFILETRANFERMANAGER_H

#include <QObject>

#include "Singleton.h"

#define LISTEN_PORT     60000

class QTcpServer;
class Sender;

class KFileTranferManager : public QObject
{
    Q_OBJECT
    DECLARESINGLETON(KFileTranferManager)
public:
    static KFileTranferManager* GetInstance()
    {
        return SINGLETON(KFileTranferManager);
    }
    void    setTransferFileList(QList<QString> files);
    bool    startTranfer();
    bool    setListenPort(quint64 port);
signals:

public slots:
    void onNewConnection();
private:
    explicit KFileTranferManager(QObject *parent = nullptr);
    QTcpServer *_pTcpServer;
    QList<Sender *> _senderList;    //连接的客户端
    QList<QString> _files;          //准备传输的文件列表
    quint64 _port;
};

#endif // KFILETRANFERMANAGER_H
