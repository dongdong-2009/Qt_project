#include "floor.h"

Floor::Floor(QObject *parent) : QObject(parent)
{
    mCurrentFloor = -1;
    mArrowUp = true;
}

void Floor::autoTestFloorChanage()
{
    if (mArrowUp)
    {
        if (mCurrentFloor <= 6)
        {
            ++mCurrentFloor;
        }
        else
        {
            --mCurrentFloor;
            mArrowUp = false;
        }
    }
    else
    {
        if (mCurrentFloor >= -1)
        {
            --mCurrentFloor;
        }
        else
        {
            ++mCurrentFloor;
            mArrowUp = true;
        }
    }
    emit floorChanged(mCurrentFloor, mArrowUp);
}
