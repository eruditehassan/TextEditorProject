#ifndef SECONDARY_H
#define SECONDARY_H

#include <QMainWindow>

namespace Ui {
class secondary;
}

class secondary : public QMainWindow
{
    Q_OBJECT

public:
    explicit secondary(QWidget *parent = nullptr);
    ~secondary();
    char plus[500];
    char sub[500];
private:
    Ui::secondary *ui;

};

#endif // SECONDARY_H
