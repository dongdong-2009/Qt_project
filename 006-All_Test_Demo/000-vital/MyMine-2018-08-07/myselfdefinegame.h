#ifndef MYSELFDEFINEGAME_H
#define MYSELFDEFINEGAME_H

#include <QDialog>

namespace Ui {
class MySelfDefineGame;
}

class MySelfDefineGame : public QDialog
{
    Q_OBJECT

public:
    explicit MySelfDefineGame(QWidget *parent = 0);
    ~MySelfDefineGame();

private:
    Ui::MySelfDefineGame *ui;
};

#endif // MYSELFDEFINEGAME_H
