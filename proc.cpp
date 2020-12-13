#include "proc.h"
#include "string.h"
#include <QtDebug>

PROC::PROC()
{
    connect(&timer,SIGNAL(timeout()),this,SLOT(tickInc()));
    timer.start(1000);

}

void PROC::tickInc()
{
    static int heartBeatCnt = 0;
    uwTick++;

    if(++heartBeatCnt >= 30 && phoneState == PHONE_ONLINE)
    {
        heartBeatCnt = 0;
        heartBeat();
    }
}

void PROC::heartBeat()
{
    uint8_t buf[6] = {0xA5,0x5A,0x01,0x00,0x01,0x01};
    this->socket.write((const char *)buf,6);
    qDebug()<<"heart beat";
}

void PROC::phone_disconn()
{
    this->phoneState = PHONE_DISCONNECT;
}

static int proc_makeAproc(uint8_t *pSrc,unsigned short len)
{
    int i;
    int p = 0;
    uint8_t *pContent = pSrc + 4;
    uint8_t sum = 0;

    pSrc[p++] = 0xa5;
    pSrc[p++] = 0x5a;
    memcpy((void *)(pSrc + p),&len,2);
    p += 2;
    p += len;
    for(i = 0; i< len; i++)
    {
        sum += pContent[i];
    }
    pSrc[p] = sum;

    return (len + 2 + 2 + 1);
}

int PROC::phone_online(QString devID)
{
    int res = PROC_OK;
    uint32_t endtick = 0;
    uint8_t buf[512] = {0};
    char *p_id = nullptr;
    uint16_t len_t = 4;
    QByteArray data = devID.toUtf8();
    p_id = data.data();

    buf[len_t++] = 0x03;
    memcpy(buf+len_t,p_id,16);
    len_t += 16;
    len_t = proc_makeAproc(buf,len_t - 4);

    data.resize(len_t);
    memcpy((void *)data.data(),buf,len_t);
    qDebug()<<"send"<<data.toHex();

    flag_online = 0;
    this->socket.write((const char *)buf,len_t);

    endtick = uwTick + 10;
    while(1)
    {
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
        if(flag_online)
        {
            flag_online = 0;
            res = PROC_OK;
            break;
        }
        if(endtick - uwTick > 0xffffffff/2)
        {
            res = PROC_TIMEOUT;
            break;
        }
    }

    if(res == PROC_OK)
    {
        if(contentBuf[1] == 0x00)
        {
            res = PROC_ERR;
        }
        else
        {
            phoneState = PHONE_ONLINE;
        }
    }

    return res;
}


int PROC::deal_record(uint32_t startTick,uint32_t endTick,QString devID)
{
    int res = PROC_OK;
    uint32_t endtick = 0;
    uint8_t buf[512] = {0};
    uint8_t ack[6] = {0xA5,0x5A,0x01,0x00,0x14,0x14};    char *p_id = nullptr;
    uint16_t len_t = 4;
    QByteArray data = devID.toUtf8();



    p_id = data.data();

    buf[len_t++] = 0x04;
    memcpy(buf+len_t,p_id,16);
    len_t += 16;
    memcpy(buf+len_t,&startTick,4);
    len_t += 4;
    memcpy(buf+len_t,&endTick,4);
    len_t += 4;
    len_t = proc_makeAproc(buf,len_t - 4);

    data.resize(len_t);
    memcpy((void *)data.data(),buf,len_t);
    qDebug()<<"send"<<data.toHex();

    flag_record = 0;
    flag_record_ui_ok = 0;
    this->socket.write((const char *)buf,len_t);

    endtick = uwTick + 5;
    while(1)
    {
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
        if(flag_record)
        {
            flag_record = 0;

            if(contentBuf[1] == 0 && contentBuf[2] == 0)
            {
                res = PROC_ERR;
                break;
            }
            else
            {
                emit update_record();
                while(!flag_record_ui_ok);
                flag_record_ui_ok = 0;
                if(contentBuf[0] == 0x04)
                {
                    res = PROC_OK;
                    break;
                }
                else if(contentBuf[0] == 0x14)
                {
                    this->socket.write((const char *)ack,6);
                    endtick = uwTick + 5;
                }
            }
        }
        if(endtick - uwTick > 0xffffffff/2)
        {
            res = PROC_TIMEOUT;
            break;
        }
    }

    qDebug()<<"res = "<<res;

    return res;
}


void PROC::getProc(uint8_t *pSrc,uint16_t len)
{
    int i,j;
    uint8_t byte;
    uint8_t sum = 0;

    for(j = 0;j < len; j++)
    {
        byte = pSrc[j];
        switch(this->state)
        {
            case PROC_FIND_1ST_HEAD:
                if(byte == 0xa5)
                {
                    this->state++;
                }
                break;
            case PROC_FIND_2ND_HEAD:
                if(byte == 0x5a)
                {
                    this->state++;
                    this->proclen  = 0;
                }
                else
                {
                    this->state = PROC_FIND_1ST_HEAD;
                }
                break;
            case PROC_GET_LEN_L:
                this->proclen &= ~(0xff);
                this->proclen |= byte;
                this->state++;
                break;
            case PROC_GET_LEN_H:
                this->proclen &= ~(0xff00);
                this->proclen |= (byte << 8);
                if(this->proclen > PROC_MAX_LEN)
                {
                    this->state = PROC_FIND_1ST_HEAD;
                }
                else
                {
                    this->pWrite_contentBuf = 0;
                    this->state++;
                }
                break;
            case PROC_GET_DATA:
                if(this->pWrite_contentBuf < this->proclen)
                {
                    this->contentBuf[(this->pWrite_contentBuf) % PROC_MAX_LEN] = byte;
                }

                if(++this->pWrite_contentBuf >= this->proclen)
                {
                    this->state++;
                }
                break;
            case PROC_CHECK_SUM:
                for(i = 0;i < this->proclen; i++)
                {
                    sum += this->contentBuf[i];
                }

                if(sum == byte)
                {
                    qDebug()<<"proc get!";

                    if(contentBuf[0] == 0x03)
                    {
                        flag_online = 1;
                    }
                    else if(contentBuf[0] == 0x04 || contentBuf[0] == 0x14)
                    {
                        flag_record = 1;
                    }
                }

                this->state = PROC_FIND_1ST_HEAD;
                break;
            default:
                this->state = PROC_FIND_1ST_HEAD;
                break;
        }

    }
}

void PROC::read_socket()
{
    QByteArray data;
    data = this->socket.readAll();
    qDebug()<<"recv:"<<data.toHex();
    getProc((uint8_t *)data.data(),data.size());
}
