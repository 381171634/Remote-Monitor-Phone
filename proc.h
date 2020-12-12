#ifndef PROC_H
#define PROC_H

#include <QTcpSocket>
#include <QObject>

#define PROC_MAX_LEN 512
#define SERVER_IP   "106.13.1.239"
#define SERVER_PORT 8888

typedef enum{
    PROC_FIND_1ST_HEAD = 0,
    PROC_FIND_2ND_HEAD,
    PROC_GET_LEN_L,
    PROC_GET_LEN_H,
    PROC_GET_DATA,
    PROC_CHECK_SUM
}ProcStateTypedef;//协议状态机

class PROC:public QObject
{
    Q_OBJECT
public:
    PROC();
    QTcpSocket socket;

    uint8_t contentBuf[PROC_MAX_LEN];
    uint16_t pWrite_contentBuf = 0;
    uint16_t proclen = 0;
    uint8_t state = PROC_FIND_1ST_HEAD;

    void phone_online(QString devID);
    void deal_online(uint8_t *pSrc);
    void deal_record(uint8_t *pSrc);
    void getProc(uint8_t *pSrc,uint16_t len);
public slots:
    void read_socket();
};

#endif // PROC_H
