#include "scan_setting.h"
#include <QPushButton>
#include <QSlider>
#include <QLabel>
#include <QComboBox>

#include <QHBoxLayout>
#include <QVBoxLayout>

scan_setting::scan_setting(QWidget *parent)
    : QWidget(parent)
{
    init_wid();
    layout_wid();
    connections();

    setStyleSheet("QLabel{color: #1B7A6B; font-family: 'segoe ui'; font-size: 16px; }"
                  "QPushButton{background: #8CC53F; font-size: 18px; color: white; border-radius: 4px; }"
                  "QPushButton::hover{background: #7BB42E;} QPushButton::pressed{background: #8CC53F;}"
                  "QScrollArea{border: none;}");
}

void scan_setting :: init_wid()
{
    back_but = new QPushButton(QIcon(":/images/left.png") , "");
    back_but->setFixedSize(30 , 30); back_but->setIconSize(QSize(16 , 16));

    sec_lev = new QSlider(Qt::Vertical); sec_lev->setMinimum(1); sec_lev->setMaximum(3);
    sec_lev->setTickPosition(QSlider::TicksBelow); sec_lev->setTickInterval(1);
    sec_lev->setSingleStep(1);

    action_list = new QComboBox(); action_list->addItem("Select Actions"); action_list->setFixedHeight(30);
    scan_rem = new QComboBox(); scan_rem->addItem("Yes"); scan_rem->addItem("No"); scan_rem->setFixedHeight(30);
}

void scan_setting :: layout_wid()
{
    QHBoxLayout *h1 = new QHBoxLayout();
    QLabel *title = new QLabel("Scan"); title->setStyleSheet("font-size: 24px;");
    h1->addWidget(back_but); h1->addStretch(); h1->addWidget(title); h1->addStretch();

    //For Slider sidelabel
    QVBoxLayout *sv = new QVBoxLayout();
    sv->addWidget(new QLabel("High")); sv->addStretch(); sv->addWidget(new QLabel("Recommended"));
    sv->addStretch(); sv->addWidget(new QLabel("Low"));
    QHBoxLayout *sh = new QHBoxLayout();
    sh->addSpacing(80); sh->addWidget(sec_lev); sh->addLayout(sv); sh->addStretch();
    QWidget *slider_cont = new QWidget(); slider_cont->setLayout(sh);
    slider_cont->setStyleSheet("QWidget{background: #E4E4E4; border: 1px solid transparent; border-radius: 3px;}");

    QVBoxLayout *mvl = new QVBoxLayout();
    QLabel *lab1 = new QLabel("Security Level"); lab1->setStyleSheet("font-size: 18px;");
    QLabel *lab2 = new QLabel("Action"); lab2->setStyleSheet("font-size: 18px;");
    QLabel *lab3 = new QLabel("Scan Removable Drives"); lab3->setStyleSheet("font-size: 18px;");
    mvl->addLayout(h1); mvl->addSpacing(10); mvl->addWidget(lab1);
    mvl->addSpacing(20); mvl->addWidget(slider_cont); mvl->addSpacing(40);
    mvl->addWidget(lab2); mvl->addWidget(action_list); mvl->addSpacing(40);
    mvl->addWidget(lab3); mvl->addWidget(scan_rem); mvl->addStretch();


    QWidget *top_container = new QWidget(); top_container->setObjectName("tp");
    top_container->setLayout(mvl); top_container->setStyleSheet("QWidget#tp{background: #E6E6E6; border: 1px solid transparent; border-radius: 3px;}");
    QHBoxLayout *main_layout = new QHBoxLayout(); main_layout->addWidget(top_container);

    this->setLayout(main_layout);

}

void scan_setting :: back_clicked()
{
    emit gotoHome();
}

void scan_setting :: connections()
{
    connect(back_but , SIGNAL(clicked(bool)) , this , SLOT(back_clicked()));
}


