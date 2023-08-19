#ifndef MAINWINDOW_H
#define MAINWINDOW_H

/*
 * Main window class interface
 * declared as a subclass of QMainWindow
 *
*/


#include <QMainWindow>

class QPushButton; class QListWidget; class QListWidgetItem;
class QStackedWidget; class QLabel; class home;
class scan; class tools; class history;
class settings; class setting_page;  class mov;


class MainWindow : public QMainWindow
{
    Q_OBJECT

    //For sidebar
    QListWidget *sideBar;
    QListWidgetItem *list_item[7];
    setting_page *sett;

    //Other display widgets
    QStackedWidget *window_stack;
    QLabel *footer; home *hm;
    scan *scan_widget; tools *toolsWidget;
    history *history_widget;
    QLabel *status; QWidget *status_widget;

    //For title bar
    mov *title_widget; QLabel *title;
    QPushButton *close_but, *mini_but, *search_but;

    void widget_init(); //For all widget initialization
    void layout_widget(); //Takes care of page layout
    void connections(); //For all connections
    void styles();      //styles implementation

public:
    MainWindow(QWidget *parent = 0);
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent* event);
    ~MainWindow();

private slots:
    void sideBarIndexChanged(int);
    void minMax();
};

#endif // MAINWINDOW_H
