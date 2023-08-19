#ifndef SCAN_SETTING_H
#define SCAN_SETTING_H
#include <QWidget>

class QPushButton;
class QSlider;
class QComboBox;

class scan_setting
        : public QWidget
{
    Q_OBJECT
    QPushButton *back_but;
    QSlider *sec_lev;
    QComboBox *action_list, *scan_rem;

public:
    scan_setting(QWidget *parent = 0);
    void init_wid();
    void layout_wid();
    void connections();

private slots:
    void back_clicked();

signals:
    void gotoHome();
};

#endif // SCAN_SETTING_H
