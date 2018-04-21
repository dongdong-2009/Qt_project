#ifndef GLOBALFUN_H
#define GLOBALFUN_H

#include "define.h"

QIcon GetIcon(QPixmap pPixmap, QRect pRect);
QPixmap GetPixmap(QPixmap pPixmap, QRect pRect);

bool isDigitStr(QString src);

void QtSleep(unsigned int msec, QEventLoop::ProcessEventsFlags flags = QEventLoop::AllEvents);
bool QtWait(bool &pFlag, bool pEqlFlag, unsigned int msec);
bool QtWait(int &pFlag, int pEqlFlag, unsigned int msec);

//>@ע��key����Ϊ��/�������������
class iniParse : public QSettings
{
    Q_OBJECT
public:
    iniParse(QString pfileName);
    void setProperty(QString key,QString value) { setValue(key, value); }
    void setProperty(QString key,int value)     { setValue(key, value); }

    bool getProperty(QString pkey, QString &pValue);
    bool getProperty(QString pkey, int &pValue);

    /*
    ����ֱ�Ӷ�ȡ���ĵ�QSTRING�ģ������������������ڰ�QSETTING�Ķ�ȡ����ֵ���� ��QByteArray�в��ǹؼ���
    QTextCodec *codec=QTextCodec ::codecForName("GBK"); //�趨�ַ���ΪGBK����Сд���ɣ�������GB18030���棩
    QString outStr = codec->toUnicode(myArray); //����ͺý����ˣ�ͨ�����������ֽ������е�ֵת��ΪUNICODE��QSTRING��O��
    */
};

#endif // GLOBALFUN_H
