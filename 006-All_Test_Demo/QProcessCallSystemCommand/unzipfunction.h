#ifndef UNZIPFUNCTION_H
#define UNZIPFUNCTION_H

#include <QObject>
#include <QProcess>
#include <QElapsedTimer>

class UnZipFunction : public QObject
{
    Q_OBJECT
public:
    explicit UnZipFunction(QObject *parent = 0);
    void startUnZip();
signals:

public slots:
    void sltStarted();
    void sltUnZipFinished(int code);
    void sltOcurrErrors(QProcess::ProcessError error);
private:
    QProcess m_ZipProcess;
    QElapsedTimer mTimer;
};

#endif // UNZIPFUNCTION_H
