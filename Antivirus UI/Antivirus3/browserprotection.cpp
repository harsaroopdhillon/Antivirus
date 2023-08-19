#include "browserprotection.h"
#include <QPushButton>
#include <QSlider>
#include <QLabel>
#include <QCheckBox>
#include <QTableWidget>
#include "switch.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDebug>

browserProtection::browserProtection(QWidget *parent)
    : QWidget(parent)
{
    init_wid();
    layout_wid();
    connections();

    setStyleSheet("QLabel{color: #1B7A6B; font-family: 'quicksand'; font-size: 16px; }"
                  "QRadioButton , QCheckBox{color: #1B7A6B; font-family: 'quicksand'; font-size: 16px; }"
                  "QPushButton{background: #8CC53F; font-size: 18px; color: white; border-radius: 4px; }"
                  "QPushButton::hover{background: #7BB42E;} QPushButton::pressed{background: #8CC53F;}"
                  "QTableView{border: 2px solid grey; border-radius: 4px; font-size: 11px;}"
                  "QHeaderView::section { background-color: none; border: none; height: 35px;}"
                  "QHeaderView::section { background-color: none; border: none; height: 35px;}"
                  );
}

void browserProtection :: init_wid()
{
    back_but = new QPushButton(QIcon(":/images/left.png") , "");
    back_but->setFixedSize(30 , 30); back_but->setIconSize(QSize(16 , 16));

    browser_sw = new Switch(); browser_sw->setCheckable(true);

    sec_lev = new QSlider(Qt::Vertical);
    sec_lev->setMinimum(1);
    sec_lev->setMaximum(3);
    sec_lev->setTickPosition(QSlider::TicksBelow);
    sec_lev->setTickInterval(1);
    sec_lev->setSingleStep(1);

    enable_bp = new QCheckBox("Enable protection for all browsers");
    enable_bp->setLayoutDirection(Qt::LeftToRight);

    table = new QTableWidget();
    QStringList h_labels = QStringList() << "Browser" << "Status" << "Options";
    table->setColumnCount(3); table->setHorizontalHeaderLabels(h_labels);
    for(int i = 0; i < 3; i++){
        table->horizontalHeader()->setSectionResizeMode(i, QHeaderView::Stretch);
    }
    table->setShowGrid(true);
}

void browserProtection :: layout_wid()
{
    QHBoxLayout *h1 = new QHBoxLayout();
    QLabel *title = new QLabel("Browser Protection");
    title->setStyleSheet("font-size: 24px;");
    h1->addWidget(back_but);
    h1->addStretch();
    h1->addWidget(title);
    h1->addStretch();

    QHBoxLayout *enab_l = new QHBoxLayout();
    enab_l->addSpacing(40);
    enab_l->addWidget(new QLabel("Enable Browser Protection"));
    enab_l->addStretch();
    enab_l->addWidget(browser_sw);
    enab_l->addSpacing(40);
    QWidget *sw_wid = new QWidget();
    sw_wid->setLayout(enab_l);
    sw_wid->setStyleSheet("QWidget{background: #E4E4E4; border: 1px solid transparent; border-radius: 3px;}");

    //For Slider sidelabel
    QVBoxLayout *sv = new QVBoxLayout();
    sv->addWidget(new QLabel("High")); sv->addStretch(); sv->addWidget(new QLabel("Recommended"));
    sv->addStretch(); sv->addWidget(new QLabel("Low"));
    QVBoxLayout *sv1 = new QVBoxLayout();
    QLabel *lb1 = new QLabel("Maximum Protection");
    QLabel *lb2 = new QLabel("Optimal Protection");
    QLabel *lb3 = new QLabel("Minimum Protection");
    lb1->setStyleSheet("color: grey;");
    lb2->setStyleSheet("color: grey;");
    lb3->setStyleSheet("color: grey;");
    sv1->addWidget(lb1);
    sv1->addStretch();
    sv1->addWidget(lb2);
    sv1->addStretch();
    sv1->addWidget(lb3);
    QHBoxLayout *sh = new QHBoxLayout();
    sh->addSpacing(80);
    sh->addWidget(sec_lev);
    sh->addLayout(sv);
    sh->addSpacing(40);
    sh->addLayout(sv1);
    sh->addStretch();
    QWidget *slider_cont = new QWidget();
    slider_cont->setLayout(sh);
    slider_cont->setStyleSheet("QWidget{background: #E4E4E4; border: 1px solid transparent; border-radius: 3px;}");

    QVBoxLayout *mvl = new QVBoxLayout();
    QLabel *lab1 = new QLabel("Browser Security Level");
    lab1->setStyleSheet("font-size: 18px;");

    mvl->addLayout(h1);
    mvl->addSpacing(20);
    mvl->addWidget(sw_wid);
    mvl->addSpacing(20);
    mvl->addWidget(lab1 , 0 , Qt::AlignCenter);
    mvl->addWidget(new QLabel("<hr>"));
    mvl->addSpacing(20);
    mvl->addWidget(slider_cont);
    mvl->addSpacing(10);
    mvl->addWidget(enable_bp);
    mvl->addWidget(table);


    QWidget *top_container = new QWidget();
    top_container->setObjectName("tp");
    top_container->setLayout(mvl);
    top_container->setStyleSheet("QWidget#tp{background: #E6E6E6; border: 1px solid transparent; border-radius: 3px;}");
    QHBoxLayout *main_layout = new QHBoxLayout(); main_layout->addWidget(top_container);

    this->setLayout(main_layout);

}

void browserProtection :: back_clicked()
{
    emit gotoHome();
}

void browserProtection :: connections()
{
    connect(back_but , SIGNAL(clicked(bool)) , this , SLOT(back_clicked()));
}
