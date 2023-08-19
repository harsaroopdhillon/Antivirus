#ifndef SETTINGS_H
#define SETTINGS_H

#include <QWidget>

namespace Ui {
class settings;
}

class settings : public QWidget
{
    Q_OBJECT

public:
    explicit settings(QWidget *parent = 0);
    ~settings();

private:
    Ui::settings *ui;
    void connections();

private slots:
    void gotoScanner(bool);
    void gotoBrowser(bool);
    void gotoHome();
};

#endif // SETTINGS_H
