#ifndef WIDGET_H
#define WIDGET_H
#define LCD_DEVICE ("/dev/ttyUSB0")

#include <QWidget>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT
public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

public slots:
    void sett();

private:
    Ui::Widget *ui;

};

#endif // WIDGET_H
