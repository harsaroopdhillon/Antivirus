#include "mainwindow.h"
#include "header.h"
#include "home.h"
#include "scan.h"
#include "history.h"
#include "setting_page.h"
#include "mov.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    widget_init();
    layout_widget();
    connections();
    styles();
    emit sideBar->currentRowChanged(0);
    sideBar->setItemSelected(list_item[0] , true);
}

MainWindow::~MainWindow()
{

}

void MainWindow :: paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void MainWindow :: resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);
    this->setAutoFillBackground(true);
}

void MainWindow :: widget_init()
{
    //For title bar
    title_widget = new mov(this);
    title = new QLabel(tr("")); title->setStyleSheet("color: white; font-size: 18px; font-family: 'arial';");
    close_but = new QPushButton(QIcon(":/images/n_icons/nclose.png") , "");  mini_but = new QPushButton(QIcon(":/images/n_icons/nminimize.png") , "");
    search_but = new QPushButton(QIcon(":/images/n_icons/maxi.png") , "");
    close_but->setIconSize(QSize(18 , 18)); mini_but->setIconSize(QSize(18, 18)); search_but->setIconSize(QSize(18 , 18));
    search_but->setFixedSize(50 , 27); close_but->setFixedSize(50 , 27); mini_but->setFixedSize(50 , 27);
    search_but->setStyleSheet("QPushButton{background: #E1E1E1; border: 1px solid transparent; border-bottom-left-radius: 5px;} QPushButton::hover{background: #D4D4D4;}");
    close_but->setStyleSheet("QPushButton{background: #E1E1E1; border: 1px solid transparent; border-bottom-right-radius: 5px;} QPushButton::hover{background: red;}");
    mini_but->setStyleSheet("QPushButton{background: #E1E1E1; border: 1px solid transparent; border-bottom-left-radius: 5px; border-bottom-right-radius: 5px; } QPushButton::hover{background: #D4D4D4;}");


    //For Sidebar
    sideBar = new QListWidget(); footer = new QLabel();
    footer->setFixedHeight(55);

    QPixmap pix(":/images/footer2.gif"); footer->setPixmap(pix);
    status_widget = new QWidget();
    status = new QLabel("Protected");
    status_widget->setStyleSheet("background: #67e600;");

    status_widget->setFixedHeight(35);
    const char* links[] = {"Home", "Scan", "History" , "File Guard", "Settings", "Help and support"};
    QString icon_links[] = {":/images/home.svg" , ":/images/scan.svg" , ":/images/history.png" , ":/images/update.png" , ":/images/setup.png" , ":/images/help.png"};
    sideBar->setSizeAdjustPolicy(QListWidget::AdjustToContents);

    for(size_t i = 0; i <= 5; i++)
    {
        list_item[i] = new QListWidgetItem(links[i], sideBar);
        list_item[i]->setData(Qt::DecorationRole, QPixmap(icon_links[i]).scaled(25, 25 , Qt::KeepAspectRatio, Qt::SmoothTransformation));
        list_item[i]->setSizeHint(QSize(list_item[i]->sizeHint().width(), 80 ));
        list_item[i]->setTextAlignment(Qt::AlignCenter);

    }

    window_stack = new QStackedWidget(); hm = new home(); history_widget = new history();

    window_stack->addWidget(hm);
    sett = new setting_page();
    scan_widget = new scan();
    window_stack->addWidget(scan_widget);
    window_stack->addWidget(history_widget);
    window_stack->addWidget(sett);
}

void MainWindow :: layout_widget()
{
    //For title bar
    QHBoxLayout *ttl = new QHBoxLayout(); ttl->addWidget(mini_but); ttl->addSpacing(-9);
    ttl->addWidget(search_but); ttl->addSpacing(-9); ttl->addWidget(close_but);
    QVBoxLayout *tvl = new QVBoxLayout(); tvl->addLayout(ttl);
    QHBoxLayout *thl = new QHBoxLayout();
    thl->addWidget(title);
    thl->addStretch(); thl->addLayout(tvl);
    title_widget->setFixedHeight(25); title_widget->setLayout(thl);
    title_widget->setContentsMargins(-9, -9, -9, -9);

    QHBoxLayout *hl[5]; QVBoxLayout *vl[5];
    for(size_t i = 0; i < 5; i++) {
        hl[i] = new QHBoxLayout();
        vl[i] = new QVBoxLayout();
    }

    //For Sidebar
    hl[0]->addSpacing(-9);
    vl[1]->addWidget(sideBar);


    hl[0]->addLayout(vl[1]);

    vl[2]->addWidget(window_stack);
    vl[2]->addWidget(footer , 0 , Qt::AlignCenter);
    hl[0]->addSpacing(-6); hl[0]->addLayout(vl[2]); hl[0]->addSpacing(-9);

    QLabel *templ = new QLabel("<b>PC Status: Protected</b>");
    templ->setStyleSheet("background: transparent; color: white; font-size: 15px; font-family: 'quicksand'; ");
    status->setStyleSheet("background: transparent; color: white; font-size: 15px; font-family: 'quicksand'; ");
    QHBoxLayout *header_l = new QHBoxLayout();
    header_l->addWidget(templ);
    header_l->addSpacing(10);
    header_l->addStretch();
    status_widget->setLayout(header_l);
    hl[1]->addSpacing(-10); hl[1]->addWidget(status_widget); hl[1]->addSpacing(-10);

    vl[0]->addSpacing(-9); vl[0]->addWidget(title_widget);
    vl[0]->addLayout(hl[1]); vl[0]->addSpacing(-7); vl[0]->addLayout(hl[0]);

    vl[0]->addSpacing(-8);

    QWidget *window = new QWidget();
    window->setLayout(vl[0]);
    this->setCentralWidget(window);
}

void MainWindow :: minMax()
{
    if(this->isMaximized())
        this->showNormal();
    else{

        this->showMaximized();
    }
}

void MainWindow :: connections()
{
    connect(sideBar , SIGNAL(currentRowChanged(int)) , this , SLOT(sideBarIndexChanged(int)));
    connect(hm , SIGNAL(scanRequired(int)) , this, SLOT(sideBarIndexChanged(int)));
    connect(close_but , SIGNAL(clicked(bool)) , this , SLOT(close()));
    connect(mini_but , SIGNAL(clicked(bool)) , this , SLOT(showMinimized()));
    connect(search_but , SIGNAL(clicked(bool)) , this , SLOT(minMax()));
}

void MainWindow :: styles()
{
    footer->setObjectName("footer");
    setStyleSheet("QMainWindow{background: qlineargradient(x1:0 y1:0 x2:1 y2:0 stop:0 #595959, stop:1 #595959); color: #000000; border: 1px solid transparent; border-radius: 3px;}"
                  "QListWidget{background-color: #494949; border: none;  font-size: 16px; font-family: 'quicksand';}" //#919191;
                  "QListWidget::item{color: #E3F2F4;}"//#E3F2F4;
                  "QListWidget::item:selected{background: #EFFAF5; color: black; font-weight: bold;}"
                  "QListView{outline: none;} QListWidget::item:icon{padding-left: 20px;}"
                  "QWidget#footer{background #FFFFFF;}"
                  "QStackedWidget{background: #EFFAF5;}"
);
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    this->setWindowIcon(QIcon(":/images/shield.png"));
    setMinimumSize(QApplication::desktop()->width() * 8/10 , QApplication::desktop()->height() * 9/10);
    move(QApplication::desktop()->availableGeometry().center() - rect().center());
    sideBar->setFixedWidth(180);

}

void MainWindow :: sideBarIndexChanged(int idx)
{
    if(sideBar->currentRow() != idx){
        sideBar->setCurrentRow(idx);
        return;
    }
    if(window_stack->widget(idx) != nullptr)
            window_stack->setCurrentIndex(idx);
}
