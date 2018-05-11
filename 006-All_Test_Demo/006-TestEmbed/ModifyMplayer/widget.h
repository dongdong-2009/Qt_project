#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "mplayer.h"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private:
    Ui::Widget *ui;
    Mplayer* m_player;
};

#endif // WIDGET_H
