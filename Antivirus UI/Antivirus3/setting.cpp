#include "setting.h"
#include <QPushButton>
#include <QScrollArea>
#include <QIcon>
#include "links.h"
#include "switch.h"

#include <QVBoxLayout>
#include <QHBoxLayout>

setting::setting(QWidget *parent)
    : QWidget(parent)
{
    wid_init();
    layout_wid();
    connections();

    setStyleSheet("QLabel{color: #1B7A6B; font-family: 'quicksand'; font-size: 16px; }"
                  "QPushButton{background: #8CC53F; font-size: 18px; color: white; border-radius: 4px; }"
                  "QPushButton::hover{background: #7BB42E;} QPushButton::pressed{background: #8CC53F;}"
                  "QScrollArea{border: none;}");

}

void setting :: wid_init()
{
    QHBoxLayout *container_layout[5];

    QStringList but_icon = QStringList() << ":/images/scan.svg" << ":/images/www.png" << ":/images/update.png" << ":/images/scan.svg" << ":/images/scan.svg";
    QStringList link_text = QStringList() << "Scan Settings" << "Browser Protection Settings" << "Update Settings" << "To Fill" << "To Fill";

    for(int i = 0; i < 5; i++)
    {
        QIcon icn(but_icon.at(i));
        but_list[i] = new QPushButton(icn , "");
        but_list[i]->setFixedSize(40 , 40);
        but_list[i]->setIconSize(QSize(30 , 30));
        link_list[i] = new links(link_text.at(i));
        switch_list[i] = new Switch();
        switch_list[i]->setCheckable(true);
        container_list[i] = new QWidget();

        link_list[i]->setStyleSheet("QLabel{background: transparent; border: none; color: #1B7A6B; font-family: 'quicksand'; font-size: 16px;}");
        container_list[i]->setStyleSheet("QWidget{background: #E4E4E4; border: 1px solid transparent; border-radius: 3px;}");
        but_list[i]->setStyleSheet(
                    "QPushButton{background: #8CC53F; font-size: 18px; color: white; border-radius: 4px; }"
                    "QPushButton::hover{background: #7BB42E;} QPushButton::pressed{background: #8CC53F;}");

        container_layout[i] = new QHBoxLayout();
        container_layout[i]->addWidget(but_list[i]); container_layout[i]->addSpacing(10); container_layout[i]->addWidget(link_list[i]);
        container_layout[i]->addStretch(); container_layout[i]->addWidget(switch_list[i]); container_layout[i]->addSpacing(10);
        container_list[i]->setLayout(container_layout[i]);
    }
}

void setting :: layout_wid()
{
   QVBoxLayout *vl = new QVBoxLayout();

    QVBoxLayout *vl2 = new QVBoxLayout();
    for(int i = 0; i < 5; i++)
    {
        vl2->addWidget(container_list[i]);
        vl2->addSpacing(-4);
    }
    QWidget *swid = new QWidget();
    swid->setLayout(vl2);

    QScrollArea *scrollArea = new QScrollArea(); scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(swid); scrollArea->setAlignment(Qt::AlignCenter);

    vl->addWidget(scrollArea);

    this->setLayout(vl);

}

void setting :: connections()
{
    connect(but_list[0] , SIGNAL(clicked(bool)) , this , SLOT(gotoScanner(bool)));
    connect(link_list[0], SIGNAL(clicked()) , but_list[0], SLOT(click()));

    connect(but_list[1] , SIGNAL(clicked(bool)) , this , SLOT(gotoBrowser(bool)));
    connect(link_list[1], SIGNAL(clicked()) , but_list[1], SLOT(click()));
}

void setting :: gotoScanner(bool st)
{
    emit scanner_req();
}

void setting :: gotoBrowser(bool st)
{
    emit browser_req();
}
