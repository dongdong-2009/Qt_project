#ifndef WIFIPROTOCOL_H
#define WIFIPROTOCOL_H

#include <QObject>

//ID类型枚举
typedef enum id_type {
//    ID00_BASE = 0x00,	        /* 0.  基本信息ID 包含楼层，箭头、功能信息*/
//    ID01_FLOORSTATUS = 0x01,    /* 1.  楼层及功能状态 (TAG = 0x01) */
//    ID02_BUTTONSTATUS = 0x02,   /* 2.  按钮状态 (TAG = 0x02) */
//    ID03_BUTTONOUTPUT = 0x03,   /* 3.  按钮输出数据 (TAG = 0x03) */
    ID04_CONFIGCMD = 0x04,      /* 4.  配置指令 (TAG = 0x04) */
    ID05_FILETRAS = 0x05,       /* 5.  文件传输指令 (TAG = 0x05) */
//    ID06_UPDATEREQREPLY = 0x06, /* 6.  升级请求应答 (TAG = 0x06) */
    ID10_BEATHEART = 0x10,      /* 10. 心跳指令 (TAG = 0x10) */
}IDTYPE;

class WifiProtocol : public QObject
{
    Q_OBJECT
public:
    explicit WifiProtocol(QObject *parent = 0);
    quint8 frameVerify(quint8* frame, int frameLength);

signals:

public slots:

};

#endif // WIFIPROTOCOL_H
