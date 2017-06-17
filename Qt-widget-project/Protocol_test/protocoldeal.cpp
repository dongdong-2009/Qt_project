#include "protocoldeal.h"
#include <QThread>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string.h>
using namespace std;

#define BVT_ESC 0x1B	/* 转换字符 */
#define BVT_STX 0x80	/* 帧起始字符 */
#define BVT_ETX 0x81	/* 帧结束字符 */
#define BVT_STX_AF 0xE7	/* 帧起始转换后增加字符 */
#define BVT_ETX_AF 0xE8	/* 帧结束转换后增加字符 */
#define BVT_ESC_AF 0x00	/* 转换字符转换后增加字符 */

#define BVT_MAX_FRAME_LENTH 44	/* 最大帧字节长度 */
#define BVT_SEND_INTERVAL 50	/* 发送数据间隔,单位ms */
#define BVT_RECV_CHANNEL_SIZE 1	/* 接收通道数据类型大小 */
#define BVT_PTL_FRAMEID_POS 1	/* 协议帧ID地址 */
#define BVT_PTL_DATASTART_POS 2	/* 协议帧数据地址 */
#define MAX_LENGTH 4096         /*缓冲区大小*/
//static unsigned char gBvtRecStatus ;
//static unsigned char gBvtRecCnt ;
//static unsigned char gBvtFrameBuf[BVT_MAX_FRAME_LENTH] ;
static char WriteDataBuf[1024] ;
static char WriteSrc[1000];
//static char curstr[BVT_MAX_FRAME_LENTH] ;

static unsigned char totalBuf[MAX_LENGTH]; // 串口读到的数据存储的数据缓冲区
static unsigned char tempBuf[1024];        // 去掉帧头和帧尾，并且解析好的数据的缓冲区
static unsigned long StringSize;
static unsigned long position;
static unsigned long UiWriteSize;
messagetable mestable;
Protocoldeal* Protocoldeal::instance = NULL;
//QTimer *timer;
QSerialPort *my_serialport = NULL;

Protocoldeal::Protocoldeal()
{
    SetSerialArgument();     //配置串口参数，连接信号和槽
    ReadDataPthread = new ProducerFromBottom;
    ReadDataPthread->StartThread(ReadDataPthread);

    WriteDataPthread = new WriteDataToBottom;
    WriteDataPthread->StartThread(WriteDataPthread);

    connect(my_serialport, SIGNAL(readyRead()), ReadDataPthread, SLOT(ReadyreadSlots()), Qt::QueuedConnection);
    position = 0;
    cout << __PRETTY_FUNCTION__<<"启动协议的的构造函数"<<endl;
}

Protocoldeal::~Protocoldeal()
{
    ReadDataPthread->requestInterruption();
    ReadDataPthread->quit();
    ReadDataPthread->wait();
    delete ReadDataPthread;

    WriteDataPthread->requestInterruption();
    WriteDataPthread->quit();
    WriteDataPthread->wait();
    delete WriteDataPthread;

    CloseSerial();
    delete my_serialport;
    cout << __PRETTY_FUNCTION__<<"启动协议的的析构函数"<<endl;
}

// 单例模式
Protocoldeal* Protocoldeal::GetInstance()
{
    if (NULL == instance)
    {
        instance = new Protocoldeal();
    }
    return instance;
}

void Protocoldeal::BstBvtPtlInit()
{

}

/*!
 * BstBvtSetFrameData
 * 设置帧数据
 * @param[in]   id：帧ID	dat：帧数据地址
 * @param[out]  None
 * @retval  	None
 */
void Protocoldeal::BstBvtSetFrameData(e_IDTYPE_T id, void *dat)
{
//    unsigned char TranBuf[BVT_MAX_FRAME_LENTH] = {0};
    unsigned char FrameBuf[BVT_MAX_FRAME_LENTH] = {0};
    unsigned char *pFrameBuf = FrameBuf;
    unsigned char *pStarVer;
    unsigned char DatLen = 0;
//    unsigned long TranLen = 0;/*转换后帧长度*/

    if(id >= ID_UNKNOW)/*未知ID*/
        return;
//    DatLen = BstBvtGetFrameDatLen(id);
    *pFrameBuf++ = BVT_STX;	/*加入帧头*/
    pStarVer = pFrameBuf;
    *pFrameBuf++ = id;	/*加入ID*/
    BstFifoMemCpy(pFrameBuf,dat, DatLen);/*加入数据*/
    pFrameBuf += DatLen;
    *pFrameBuf++ = BstBvtVerify(pStarVer, DatLen+1);/*加入校验值*/
    *pFrameBuf++ = BVT_ETX;/*加入帧尾*/
//    TranLen = BstBvtTransformFrame(FrameBuf,pFrameBuf-FrameBuf,TranBuf);/*帧转换*/
//    SendFifoChannelPush(BST_BVTSEND_USE,TranBuf,TranLen);/*压入发送FIFO*/
}

//CRC 数据校验
/*!
 * BstBvtVerify
 * 协议校验算法
 * @param[in]   data：待校验的数据指针	length：待校验的数据长度
 * @param[out]  None
 * @retval  	None
 */
unsigned char Protocoldeal::BstBvtVerify(unsigned char *data, unsigned long length)
{
    qDebug()<< __PRETTY_FUNCTION__<<"length = "<< length;
    unsigned char result = 0;
    unsigned int i;
    if (length <= 0)
    {
        qDebug()<< "校验失败,长度有误!";
        return result;
    }
    for (i = 0; i < length; i++)
    {
        result = result ^ data[i];
    }
    return result & 0x7f;
}

/*!
 * BstBvtRecoverFrame
 * 协议去除字符转换，恢复数据
 * @param[in]   src：原数据地址 len：原数据长度
 * @param[out]  None
 * @retval  	恢复后的帧长度
 */
unsigned long Protocoldeal::BstBvtRecoverFrame(void *des, void *src, unsigned long srclen)
{
    unsigned long Len = srclen - 2; /*去掉帧头和帧尾*/
    unsigned long TranLen = 0;      /*数据域的数据长度*/
    unsigned long Cnt = 0;
    unsigned char *lDst = (unsigned char*)des;
    unsigned char *lSrc = (unsigned char*)src;
    qDebug()<< "srclen = "<< srclen;
    if (2 >= srclen)
    {
        qDebug()<< "buf is empty ";
        return TranLen;
    }
    lSrc++; /*取出跳过帧头数据*/
    while(Cnt < Len)
    {
        qDebug()<< "enter while";
        if(BVT_ESC == *lSrc)                   //如果是转换字符
        {
            if(*(lSrc + 1) == BVT_STX_AF)      //转换后的帧头
                *lDst++ = BVT_STX;
            else if(*(lSrc + 1) == BVT_ETX_AF) //转换后的 帧尾
                *lDst++ = BVT_ETX;
            else if(*(lSrc + 1) == BVT_ESC_AF) //转换后的 转换符
                *lDst++ = BVT_ESC;
            lSrc += 2;
            Cnt += 2;
        }else{
            *lDst++ = *lSrc++;
            Cnt++;
        }
        TranLen++;
    }
    qDebug()<< "TranLen = "<< TranLen;
    //*lDst = *lSrc; /*取出帧尾数据*/
    return TranLen;
}

//内存拷贝
void Protocoldeal::BstFifoMemCpy(unsigned char *pFrameBuf,void* dat, unsigned char DatLen)
{
    memcpy(pFrameBuf, dat, DatLen);
}

/*!
 * BstBvtGetFrameDatLen
 * 获取帧数据长度
 * @param[in]   id：帧ID
 * @param[out]  None
 * @retval  	帧ID对应的数据长度
 */
unsigned long Protocoldeal::BstBvtGetFrameDatLen(unsigned char id)
{
    qDebug()<< __PRETTY_FUNCTION__;
    unsigned long len = 0;
    switch(id)
    {
    case 0x00: len = sizeof(s_BVTID0_T); break;
    case 0x01: len = sizeof(s_BVTID1_T); break;
    case 0x02: len = sizeof(s_BVTID2_T); break;
    case 0x03: len = sizeof(s_BVTID3_T); break;
    case 0x04: len = sizeof(s_BVTID4_T); break;
    case 0x05: len = sizeof(s_BVTID5_T); break;
    case 0x06: len = sizeof(s_BVTID6_T); break;
    case 0x07: len = sizeof(s_BVTID7_T); break;
    default: qDebug()<<"Id error"; break;
    }
    qDebug("id = %x, len = %ld",id, len);
    return len;
}

//比较unsigned char 类型的字符串是否相同
bool Protocoldeal::StringCompare(unsigned char *temp, unsigned char *str, unsigned long len)
{
    unsigned long i = 0;
    for (i = 0; i < len; i++)
    {
        if(temp[i] != str[i])
        {
            cout << "the different index i = " << i << endl;
            return true;
        }
    }
    cout << "the string is same as before i = " << i << endl;
    return false;
}

// 分配内存是否成功
bool Protocoldeal::AllocteMemory(void *p)
{
    if (NULL == p)
    {
        qDebug("allcote memory is failed");
        return true;
    }
    return false;
}

// 检测发送的新的数据帧的内容是否有变更
bool Protocoldeal::JudgeChange(unsigned char ID, unsigned char str[])
{
    qDebug()<<__PRETTY_FUNCTION__;
    unsigned long len;
    unsigned char *temp;
    bool AllocteFlag = false;
    len = BstBvtGetFrameDatLen(ID);      // 需要拷贝的ID的数据长度
    temp = (unsigned char *)malloc(len); // 分配内存
    if (!AllocteMemory(temp))
    {
        qDebug()<<__PRETTY_FUNCTION__<<"allocate success!"<< "ID = " << ID;
        memset(temp, 0, len);
        switch(ID)
        {
        case 0x00:
            memcpy(temp, &mestable.ID0_Message, len);// 拷贝信息表中的数据到temp中，用来和新收到的数据进行比较
            AllocteFlag = true;
            break;
        case 0x01:
            AllocteFlag = true;
            break;
        case 0x02:
            AllocteFlag = true;
            break;
        case 0x03:
            AllocteFlag = true;
            break;
        case 0x04:
            AllocteFlag = true;
            break;
        case 0x05:
            AllocteFlag = true;
            break;
        case 0x06:
            AllocteFlag = true;
            break;
        case 0x07:
            AllocteFlag = true;
            break;
        default:
            AllocteFlag = true;
            break;
        }
        if (AllocteFlag)
        {
            cout << "AllocteFlag = " << AllocteFlag << endl;
            PrintString(temp, len);
            AllocteFlag = false;
            if (StringCompare(temp, str, len))
            {
                free(temp);
                temp = NULL;
                return true;  // true 表示变更了
            }
            else
            {
                free(temp);
                temp = NULL;
                return false;
            }
        }
    }
    qDebug()<<__PRETTY_FUNCTION__<<"allocate failure!";
    return false;
}

/*应用层调用从协议层拷贝数据到应用层的接口*/
void Protocoldeal::CopyStringFromProtocol(unsigned char Id, void *str)
{
    if (0x00 == Id)
    {
        memcpy(str, tempBuf, StringSize);
        cout << "Id == " << "0x00 in if()"<< endl;
    }
    else if (0x01 == Id)
    {
        memcpy(str, tempBuf, StringSize);
        cout << "Id == " << "0x01 in else if()"<< endl;
    }
    memset(tempBuf, 0, sizeof(tempBuf));
    cout << "StringSize = 拷贝string的长度"<< StringSize << endl;
}

// 从UI层拷贝数据到协议层,根据Id指定拷贝的数据长度
void Protocoldeal::CopyStringFromUi(unsigned char Id, void *str)
{
    cout << __PRETTY_FUNCTION__<<" ";
    printf("Id = %x\n",Id);
    unsigned long strlength = BstBvtGetFrameDatLen(Id);
    memset(WriteSrc, 0, sizeof(WriteSrc));
    memcpy(WriteSrc, str, strlength);
    emit WriteDataPthread->FillDataSignal(WriteDataBuf, WriteSrc);
}

void Protocoldeal::PrintString(unsigned char *src, unsigned long length)
{
    cout << __PRETTY_FUNCTION__<<endl;
    unsigned long i = 0;
    for (i = 0; i < length; i++)
    {
        printf("%X ", src[i]);
    }
    printf("\n");
}

//配置串口参数，连接信号和槽
void Protocoldeal::SetSerialArgument()
{
    if (NULL == my_serialport)
    {
        my_serialport = new QSerialPort;
    }
    my_serialport->setPortName(SERIAL_DEVICE);
    qDebug() << "Name : " << my_serialport->portName();
    bool OpenSerialFlag = false;
    if (my_serialport->open(QIODevice::ReadWrite))  //使用读写的方式打开串口
    {
        cout << __PRETTY_FUNCTION__<< "enter funtion"<<endl;
        //设置波特率
        my_serialport->setBaudRate(QSerialPort::Baud9600);
        //设置数据位
        my_serialport->setDataBits(QSerialPort::Data8);
        //设置校验位
        my_serialport->setParity(QSerialPort::NoParity);
        //设置流控制
        my_serialport->setFlowControl(QSerialPort::NoFlowControl);
        //设置停止位
        my_serialport->setStopBits(QSerialPort::OneStop);
        my_serialport->clearError();
        OpenSerialFlag = true;
    }
    if (!OpenSerialFlag)
        qDebug()<<__PRETTY_FUNCTION__<< "OpenSerialFlag = "<< OpenSerialFlag << "打开串口失败!";
    else
        qDebug()<<__PRETTY_FUNCTION__<< "OpenSerialFlag = "<< OpenSerialFlag << "打开串口成功!";
}

// 当线程结束时，将打开的串口关闭掉
void Protocoldeal::CloseSerial()
{
    if (NULL != my_serialport)
    {
        qDebug() <<__PRETTY_FUNCTION__ <<"close readserialport success!";
        my_serialport->close(); // 关闭串口
    }
}

ProducerFromBottom::ProducerFromBottom()
{
    cout << __PRETTY_FUNCTION__<<"启动读串口的线程的构造函数"<<endl;
}

ProducerFromBottom::~ProducerFromBottom()
{
    cout << __PRETTY_FUNCTION__<<"启动读串口的线程的析构函数"<<endl;
}

// 读串口数据函数

void ProducerFromBottom::ReadyreadSlots()
{
    cout << __PRETTY_FUNCTION__ << "读数据的槽函数"<<endl;
    static bool Isstart = false;
    char str;
    unsigned char s;
    unsigned long i = 0;
    unsigned long j = 0;
    unsigned long count = 0;
    while(1)
    {
        int flag = my_serialport->read(&str, 1); // 每次读取一个字节到str中存储
        if (0 == flag)
        {
            qDebug()<< "读不到数据退出";
            break;
        }
        s = (unsigned char)str;
        count ++;
        printf("%X ", s);
        if (BVT_STX == s)
        {
            Isstart = true;
            i = 0;
            totalBuf[i++] = s /*(unsigned char)str*/;
        }
        else if(Isstart)      // 如果遇到帧头，则将遇到帧尾之前的所有数据保存下来
        {
            totalBuf[i++] = s /*(unsigned char)str*/;

            if (BVT_ETX == s)
            {
                Isstart = false;
                j = i;
                i = 0;
                qDebug()<< "遇到帧尾退出";
                break;
            }
        }
    }

    printf("count = %ld\n", count);
    Protocoldeal *Protocol = Protocoldeal::GetInstance();
    StringSize = Protocol->BstBvtRecoverFrame(tempBuf, totalBuf, j);
    printf("接收到的数据为 totalBuf = ");
    Protocol->PrintString(totalBuf, j);
    printf("解析后的数据为 tempBuf = ");
    Protocol->PrintString(tempBuf, StringSize);
    Protocol->SetContinueFlag(tempBuf);
    memcpy(&mestable.ID0_Message, tempBuf, sizeof(mestable));
    if ( tempBuf[j-1] == Protocol->BstBvtVerify(tempBuf, j)) // 数据校验
    {
        printf("tempBuf[0] = %X \n", tempBuf[0]);
        cout <<" verify is true "<< endl;
        if (Protocol->JudgeChange(tempBuf[0], tempBuf))
        {
            printf("emit message\n");
            emit Protocol->AcceptDataFormBottom(tempBuf[0]);
        }
    }


//    printf("ID0_Message.ID = %X \n", mestable.ID0_Message.ID);
//    printf("Data1 = %X \n", mestable.ID0_Message.Data1);
//    printf("Data2 = %X \n", mestable.ID0_Message.Data2);
//    printf("Data3 = %X \n", mestable.ID0_Message.Data3);
//    printf("Data4 = %X \n", mestable.ID0_Message.Data4);
//    printf("ArrowStatus = %X \n", mestable.ID0_Message.ArrowStatus);
//    printf("LiftSpecialStatus = %X \n", mestable.ID0_Message.LiftSpecialStatus);
//    printf("StationClockStatus = %X \n", mestable.ID0_Message.StationClockStatus);
//    printf("StationLightStatus = %X \n", mestable.ID0_Message.StationLightStatus);
}

unsigned long Protocoldeal::GetDataLength()
{
    return StringSize;
}

void Protocoldeal::SetContinueFlag(unsigned char buf[])
{
    int ContinueFlag = -1;
    qDebug()<< __PRETTY_FUNCTION__;
    if (0x06 == buf[0] && 0x02 == buf[1])     // 收到升级数据包为真
    {
        ContinueFlag = 1;
    }
    else if (0x06 == buf[0] && 0x03 == buf[1])// 收到数据包有误
    {
        ContinueFlag = 2;
    }
    else                                      // 没有收到回应
    {
        ContinueFlag = 0;
    }
    qDebug()<<"ContinueFlag = "<< ContinueFlag;
}

void Protocoldeal::SetContinueFlag(int num)
{
    qDebug()<< __PRETTY_FUNCTION__<< "before flag = "<< ContinueFlag;
    ContinueFlag = num;
    qDebug()<< __PRETTY_FUNCTION__<< "after flag = "<< ContinueFlag;
}

int Protocoldeal::GetContinueFlag()
{
    qDebug()<< __PRETTY_FUNCTION__<<"ContinueFlag = "<< ContinueFlag;
    return ContinueFlag;
}

void ProducerFromBottom::run()
{
    cout << __PRETTY_FUNCTION__ << "配置串口"<<endl;
//    SetSerialArgument();  // 配置串口，连接信号，传输数据
}

// 开启线程
void ProducerFromBottom::StartThread(ProducerFromBottom *p)
{
    cout << __PRETTY_FUNCTION__<<endl;
    if (NULL != p)
    {
        if (!(p->isRunning()))  // 当线程不在运行时，启动线程
        {
            qDebug() << "启动读取串口的线程";
            p->start();
        }
    }
}

WriteDataToBottom::WriteDataToBottom()
{
    cout << __PRETTY_FUNCTION__<<"启动写线程的构造函数"<<endl;
}

WriteDataToBottom::~WriteDataToBottom()
{
    cout <<__PRETTY_FUNCTION__<<"启动写线程的析构函数"<< endl;
}

void WriteDataToBottom::run()
{
    qDebug() <<__PRETTY_FUNCTION__ <<"Will setArgument";
    connect(this, SIGNAL(FillDataSignal(char*,char*)), this, SLOT(ConstructWriteData(char*,char*)),
            Qt::QueuedConnection);
    connect(this, SIGNAL(WriteDataSignal()), this, SLOT(WriteDataSerial()), Qt::QueuedConnection);
    this->exec();
}

// 组成一帧完整的包含帧头帧尾和校验值的数据帧
void WriteDataToBottom::ConstructWriteData(char *wstr, char *src)
{
    qDebug() << __PRETTY_FUNCTION__;
    unsigned long i = 0;
    unsigned long counts = 0;
    memset(WriteDataBuf, 0, sizeof(WriteDataBuf));  // 先清空上一次的数据
    unsigned long len = Protocoldeal::GetInstance()->BstBvtGetFrameDatLen((unsigned char)src[0]);
    src[len] = GenerateDataVerifyForChar(src, len); // 先在用户拷贝的数据后面加入校验值
    wstr[counts++] = BVT_STX;  // 在需要写的数据开头加上帧头
    for (i = 0; i < len + 1; i++) //遍历用户数据进行数据帧转换，加上帧头帧尾以及对数据中出现的帧头帧尾和转义字符替换
    {
        if (BVT_STX == (unsigned char)src[i])
        {
            wstr[counts++] = BVT_ESC;
            wstr[counts++] = BVT_STX_AF;
        }
        else if (BVT_ETX == (unsigned char)src[i])
        {
            wstr[counts++] = BVT_ESC;
            wstr[counts++] = BVT_ETX_AF;
        }
        else if (BVT_ESC == (unsigned char)src[i])
        {
            wstr[counts++] = BVT_ESC;
            wstr[counts++] = BVT_ESC_AF;
        }
        else
        {
            wstr[counts++] = src[i];
        }
    }
    wstr[counts++] = BVT_ETX;
    UiWriteSize = counts;
    printf("即将要write的数据为 ");
    for(i = 0;i < counts; i++)
        printf("%X ", wstr[i]);
    qDebug()<<" 转换后的数据长度为 UiWriteSize =" << UiWriteSize;
    emit WriteDataSignal();
}

// 向底层写数据
void WriteDataToBottom::WriteDataSerial()
{
    cout<< __PRETTY_FUNCTION__;
    if (NULL != my_serialport)
    {
        qDebug()<<"write data";
        qint64 len = my_serialport->write(WriteDataBuf, UiWriteSize);
        qDebug() << "write length = " << len;
    }
}

// 启动线程
void WriteDataToBottom::StartThread(WriteDataToBottom *w)
{
    cout << __PRETTY_FUNCTION__ <<endl;
    // 开启线程
    if (NULL != w)
    {
        if (!(w->isRunning())) // 当线程不在运行时，启动线程
        {
            cout <<" 启动写数据串口的线程 "<< endl;
            w->start();
        }
    }
}

// 生成校验值
char WriteDataToBottom::GenerateDataVerifyForChar(char *str, unsigned long len)
{
    qDebug()<< __PRETTY_FUNCTION__<<"len = "<< len;
    char result = 0;
    unsigned long i;
    if (len <= 0)
    {
        qDebug()<< "校验失败,长度有误!";
        return result;
    }

    for (i = 0; i < len; i++)
    {
        result = result ^ str[i];
    }
    qDebug()<< __PRETTY_FUNCTION__<<"result = "<< result;
    qDebug("result = %x\n", result);
    return result & 0x7f;
}

// 系统升级
UpdateData::UpdateData()
{

}

UpdateData::~UpdateData()
{

}

// 从文件中指定的位置获取升级的版本信息
void UpdateData::GetUpdateVersion(const char *filename, UpdateVersion *Uver)
{
    QFile file(filename);
    char buf[5];
    if (!file.open(QIODevice::ReadOnly))
    {
        qDebug("打开文件%s失败!", filename);
        return;
    }
    file.seek(OffsetHead);
    memset(Uver->ver, 0, sizeof(Uver->ver)); // 使用前先清为0
    file.read(buf, 5);
    memcpy(Uver->ver, buf, 5);
    qDebug("verinfo = %s", Uver->ver);
    file.close();
}

// 版本位数为定长五位数
bool UpdateData::CompareVersion(unsigned char Revversion[], unsigned char Readversion[])
{
    unsigned long i;
    for (i = 0; i < 5; i++)
    {
        if (Revversion[i] != Readversion[i])
        {
            if (Revversion[i] < Readversion[i])
            {
                qDebug()<< "获取到的版本比当前版本新，需升级，返回true";
                return true;    // 需要升级，返回true
            }
            else
            {
                qDebug()<< "获取到的版本比当前版本旧，无需升级，返回false";
                return false;   // 获取到的版本比当前版本旧，不用升级，返回false
            }
        }
    }
    qDebug()<< "版本号相同，无需升级，返回false";
    return false;    // 版本号相同，无需升级，返回false
}

// 当读取的字符长度小于64时，对buf用0xff填充至64个字节
void UpdateData::AppendByte(char *buf, int len)
{
    int i;
    for (i = len; i < 64; i++)
    {
        buf[i] = 0xff;
    }
}

void UpdateData::ReadUpdateFile(const char *filename)
{
    QFile file(filename);
    char buffer[64];
    int readlen = 0;
    memset(buffer, 0, sizeof(buffer));
    int ret;
    Protocoldeal *pro = Protocoldeal::GetInstance();
    if (!file.open(QIODevice::ReadOnly))
    {
        qDebug("打开文件%s失败!", filename);
        return ;
    }
    while(!file.atEnd())  // 未到达文件末尾时
    {
        readlen = file.read(buffer, 64);
        if (readlen < 64)
        {
            AppendByte(buffer, readlen);// 字符长度小于64时，对buf用0xff填充至64个字节
        }
        pro->CopyStringFromUi(0x07, buffer);

        while( !(ret = pro->GetContinueFlag())) // 未收到校验数据时
            ;  // 返回值为0时，一直在此处阻塞,一直检测返回值，直到不为0时，继续往下执行
        if (1 == ret)   // 数据校验正确
        {
            readlen = file.read(buffer, 64);
            if (64 == readlen)  // 读取到了指定的字节数的，表明目前文件还没有结束
            {

            }
            else if (64 > readlen) // 未读取到指定的字节数，说明已经到达文件尾部
            {
                break;
            }
        }
        else if ( 2 == ret) // 数据校验失败,重发
        {
            pro->CopyStringFromUi(0x07, buffer); // 重新发送buffer的内容
        }
        pro->SetContinueFlag(0);  // 重新将标志位赋值为0
    }
    file.close();
}
