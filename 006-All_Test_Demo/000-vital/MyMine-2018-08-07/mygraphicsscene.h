#ifndef MYGRAPHICSSCENE_H
#define MYGRAPHICSSCENE_H

#include <QGraphicsScene>
#include <QList>
#include "mygraphicspixmapitem.h"

class MyGraphicsPixmapItem;

class MyGraphicsScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit MyGraphicsScene(QGraphicsScene *parent = 0);

    int getGameOverFlag() const;
    void setGameOverFlag(int gameOverFlag);
    void setMineScene(int pWidth, int pHeight, int pMineCount);
    void initMineScenery();

signals:

public slots:

private:
    int mWidth;
    int mHeight;
    int mMineCounts;
    int mGameOver;
    QList<MyGraphicsPixmapItem *> mListPixmapItem;
};

#endif // MYGRAPHICSSCENE_H
