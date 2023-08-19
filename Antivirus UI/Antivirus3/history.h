#ifndef HISTORY_H
#define HISTORY_H
#include <QWidget>

class QRadioButton; class QLabel; class QPushButton;
class QTableWidget;

class history
        : public QWidget
{
    Q_OBJECT

public:
    history(QWidget *parent = 0);

private:
    QRadioButton *q_items, *a_items, *ad_items;
    QLabel *q_label, *a_label, *ad_label, *v_label;
    QPushButton *view_b;

    QTableWidget *table;
    QLabel *cat , *desc , * t1 , *t2 , *t3;
    QPushButton *remov_but, *restor_but;

    void init_widgets();
    void layout_widgets();
    void connections();

private slots:
    void showHistoryTable(bool);
};

#endif // HISTORY_H
