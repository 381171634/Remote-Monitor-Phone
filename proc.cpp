#include "proc.h"
#include "string.h"
#include <QtDebug>

PROC::PROC()
{

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

void PROC::phone_online(QString devID)
{
    uint8_t buf[512] = {0xa5,0x5a,0x11,0x00};
    uint8_t i;
    char *p_id = 0;
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

    this->socket.write((const char *)buf,len_t);
}
void PROC::deal_online(uint8_t *pSrc)
{

}
void PROC::deal_record(uint8_t *pSrc)
{

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
                    qDebug()<<"proc right";
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
    getProc((uint8_t *)data.data(),data.length());
}
