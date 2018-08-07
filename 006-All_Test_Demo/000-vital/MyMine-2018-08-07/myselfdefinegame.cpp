#include "myselfdefinegame.h"
#include "ui_myselfdefinegame.h"

MySelfDefineGame::MySelfDefineGame(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MySelfDefineGame)
{
    ui->setupUi(this);
}

MySelfDefineGame::~MySelfDefineGame()
{
    delete ui;
}
