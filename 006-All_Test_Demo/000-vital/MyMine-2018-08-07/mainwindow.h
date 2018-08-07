#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void initWindow();

private slots:
    void sltNewGame();                    // 新游戏
    void sltExitGame();                   // 退出游戏
    void sltGameEnded();                  // 游戏结束，判断是胜利还是失败
    void sltGameRulesIntroduce();         // 游戏历史介绍
    void sltGamePrompt();                 // 游戏提示，自己标记雷

    void sltGame9And9And10();             // 9*9 10个雷游戏
    void sltGame16And16And40();           // 16*16 40个雷游戏
    void sltSelfDefineGame();             // 自定义游戏
    void sltWinnerHistory();              // 胜利者的历史游戏记录

private:
    Ui::MainWindow *ui;

};

#endif // MAINWINDOW_H
