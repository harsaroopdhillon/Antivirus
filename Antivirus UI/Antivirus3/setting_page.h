#ifndef SETTING_PAGE_H
#define SETTING_PAGE_H
#include <QStackedWidget>

class setting;
class scan_setting;
class browserProtection;

class setting_page
        : public QStackedWidget
{
    Q_OBJECT

    setting *st;
    scan_setting *ss;
    browserProtection *bp;

    void wid_init();

    void connections();

public:
    setting_page();

private slots:
    void gotoHome();
    void gotoSS();
    void gotoBP();
};

#endif // SETTING_PAGE_H
