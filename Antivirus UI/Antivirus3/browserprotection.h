#ifndef BROWSERPROTECTION_H
#define BROWSERPROTECTION_H
#include <QWidget>

class QPushButton;
class QSlider;
class Switch;
class QCheckBox;
class QTableWidget;


class browserProtection
        : public QWidget
{
    Q_OBJECT
    QPushButton *back_but;
    QSlider *sec_lev;
    Switch *browser_sw;
    QCheckBox *enable_bp;
    QTableWidget *table;

public:
    browserProtection(QWidget *parent = 0);
    void init_wid();
    void layout_wid();
    void connections();

private slots:
    void back_clicked();

signals:
    void gotoHome();
};

#endif // BROWSERPROTECTION_H
