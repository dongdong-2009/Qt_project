#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QDebug>
#define FILENAME  "/home/devin/Desktop/BVT607_LPC11C14.bin"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void CountFileSize(const char* filename, unsigned char *str, int count);
    void IntToUnsignedChar(int length, int count, unsigned char *str);
private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
