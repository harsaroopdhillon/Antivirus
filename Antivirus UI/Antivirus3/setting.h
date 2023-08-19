#ifndef SETTING_H
#define SETTING_H
#include <QWidget>


class QPushButton;
class links;
class Switch;
class QHBoxLayout;

class setting
        : public QWidget
{
    Q_OBJECT

    QPushButton *but_list[10];
    links *link_list[10];
    Switch *switch_list[10];
    QWidget *container_list[10];
    void connections();

private slots:
    void gotoScanner(bool);
    void gotoBrowser(bool);

signals:
    void scanner_req();
    void browser_req();

public:
    setting(QWidget *parent = 0);
    void wid_init();
    void layout_wid();
};

#endif // SETTING_H
