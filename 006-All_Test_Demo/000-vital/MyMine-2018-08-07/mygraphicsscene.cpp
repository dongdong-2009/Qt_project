#include "mygraphicsscene.h"

MyGraphicsScene::MyGraphicsScene(QGraphicsScene *parent) : QGraphicsScene(parent)
{
    mGameOver = 0;
}

int MyGraphicsScene::getGameOverFlag() const
{
    return mGameOver;
}

void MyGraphicsScene::setGameOverFlag(int gameOver)
{
    mGameOver = gameOver;
}

void MyGraphicsScene::setMineScene(int pWidth, int pHeight, int pMineCount)
{
    mWidth = pWidth;
    mHeight = pHeight;
    mMineCounts = pMineCount;
}
