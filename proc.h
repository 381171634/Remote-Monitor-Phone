#ifndef PROC_H
#define PROC_H

#include <QTcpSocket>
#include <QObject>
#include <QTimer>
#include <QCoreApplication>

#define PROC_MAX_LEN 512
#define SERVER_IP   "106.13.1.239"
#define SERVER_PORT 8888

typedef enum{
    PHONE_ONLINE,
    PHONE_DISCONNECT
}phoneStateTypedef;

typedef enum{
    PROC_OK = 0,
    PROC_TIMEOUT,
    PROC_ERR
}procResTypedef;

typedef enum{
    PROC_FIND_1ST_HEAD = 0,
    PROC_FIND_2ND_HEAD,
    PROC_GET_LEN_L,
    PROC_GET_LEN_H,
    PROC_GET_DATA,
    PROC_CHECK_SUM
}ProcStateTypedef;//协议状态机

typedef struct{
    uint32_t        timeTick;   //时间戳
    int             tempture;   //温度
    int             humidity;   //湿度
    int             HCHO;       //甲醛
    int             CO2;        //二氧化碳
    int             cellVoltage;//锂电池电压
}SampleDataTypedef;//整数，除以1000保留到小数点后三位

class PROC:public QObject
{
    Q_OBJECT
public:
    PROC();
    QTcpSocket socket;
    QTimer timer;
    volatile uint32_t uwTick = 0;

    uint8_t phoneState = PHONE_DISCONNECT;

    uint8_t contentBuf[PROC_MAX_LEN];
    uint16_t pWrite_contentBuf = 0;
    uint16_t proclen = 0;
    uint8_t state = PROC_FIND_1ST_HEAD;

    volatile int flag_online = 0;
    volatile int flag_record = 0;
             int flag_record_ui_ok = 0;

    int phone_online(QString devID);
    void heartBeat();

    int deal_record(uint32_t startTick,uint32_t endTick,QString devID);
    void getProc(uint8_t *pSrc,uint16_t len);
public slots:
    void read_socket();
    void phone_disconn();
    void tickInc();

signals:
    void update_record();
};

#endif // PROC_H
