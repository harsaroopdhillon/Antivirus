#ifndef HOME_H
#define HOME_H
#include <QWidget>

/*
 * Home page widget interface
 * declared as a subclass of QWidget
 * */
class QPushButton; class QLabel;

class home
        : public QWidget
{
    Q_OBJECT

  QPushButton *links[3];
  QLabel *status, *status1, *status2;
  QPixmap pix1 , pix2;
  QPushButton *comp_stat;

  QPushButton *b_ind[4];

  void init_widget();
  void layout_widget();
  void connections();

public:
    home(QWidget *parent  = 0);

private slots:
    void gotoScan();

signals:
    void scanRequired(int);
};

#endif // HOME_H
