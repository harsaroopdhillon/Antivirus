#include "home.h"
#include "header.h"
home::home(QWidget *parent)
    : QWidget(parent)
{
    init_widget();
    layout_widget();
    connections();
}

void home :: init_widget()
{
    status = new QLabel("You are protected !"); status->setObjectName("status"); status->setFixedSize(580 , 85);
    status1 = new QLabel();
    QImage image(":/images/protected.png");

    comp_stat = new QPushButton(QIcon(QPixmap(":/images/n_icons/nprotected.png")) , "");
    comp_stat->setIconSize(QSize(120 , 120)); comp_stat->setFlat(true);
    comp_stat->setStyleSheet("background: transparent;");
    for(int i = 0; i < 4; i++)
    {
        b_ind[i] = new QPushButton(QIcon(":/images/n_icons/check.png") , "");
        b_ind[i]->setIconSize(QSize(24 , 24)); b_ind[i]->setFixedSize(30 , 30);
        b_ind[i]->setStyleSheet("background: transparent;");
    }

    status1->setPixmap(QPixmap::fromImage(image));

    const QString icn[] = {":/images/scan.svg" , ":/images/card.png" , ":/images/www.png"};
    const QString txt[] = {"<h4>Scan Your Computer</h4>A full in-depth malware scan" ,
                           "<h4>Banking Protection</h4>Manage Banking Protection" ,
                           "<h4>my.website.com</h4>Get access to other services"};
    QVBoxLayout *pLayout[3]; QLabel *pIconLabel[3] , *pTextLabel[3];
    QHBoxLayout *hLayout[3];

    for(int i = 0; i < 3; i++){
        pLayout[i] = new QVBoxLayout();
        hLayout[i] = new QHBoxLayout();
        pIconLabel[i] = new QLabel();
        pTextLabel[i] = new QLabel();

        pIconLabel[i]->setPixmap(QPixmap(icn[i]).scaled(50 , 50 , Qt::KeepAspectRatio, Qt::SmoothTransformation)); pIconLabel[i]->setAlignment(Qt::AlignCenter);
        pIconLabel[i]->setMouseTracking(false); pIconLabel[i]->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

        pTextLabel[i]->setText(txt[i]);  pTextLabel[i]->setAlignment(Qt::AlignCenter); pTextLabel[i]->setWordWrap(true);
        pTextLabel[i]->setTextInteractionFlags(Qt::NoTextInteraction); pTextLabel[i]->setMouseTracking(false);
        pTextLabel[i]->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        pTextLabel[i]->setStyleSheet("color: white; font-family: 'quicksand'; font-size: 18px;");

        pLayout[i]->addSpacing(15); pLayout[i]->addWidget(pIconLabel[i]); pLayout[i]->addWidget(pTextLabel[i]);
        pLayout[i]->setSpacing(5); pLayout[i]->setMargin(0);
        pLayout[i]->setContentsMargins(5, 5, 5, 5);


        links[i] = new QPushButton();
        links[i]->setFixedHeight(180);
        hLayout[i]->addStretch(); hLayout[i]->addLayout(pLayout[i]); hLayout[i]->addStretch();
        links[i]->setLayout(hLayout[i]);
    }

}

void home :: layout_widget()
{
    QHBoxLayout *hl = new QHBoxLayout(); QVBoxLayout *vl = new QVBoxLayout();
    this->setContentsMargins(-7, -7 , -7 , -7); hl->addSpacing(-9);
    for(int i = 0; i < 3; i++)
    {
        hl->addWidget(links[i]);
        hl->addSpacing(-8);
    }

    QHBoxLayout *indl[4];
    QStringList ind_lab = QStringList() << "<b>Threats:</b> None" << "<b>Protection components:</b> Enabled" <<  "<b>Databases:</b> Up to date" <<  "<b>License:</b> 365 days";
    QVBoxLayout *v_ind = new QVBoxLayout();
    QLabel *prot_l = new QLabel("Computer is protected");
    prot_l->setObjectName("protectedlabel");

    prot_l->setFixedSize(480 , 65);
    v_ind->addWidget(prot_l);
    v_ind->addSpacing(10);
    QLabel *ind_ll[4];
    for(int i  = 0; i < 4; i++)
    {
        indl[i] = new QHBoxLayout();
        ind_ll[i] = new QLabel(ind_lab.at(i)); ind_ll[i]->setStyleSheet("font-size: 15px;");
        indl[i]->addWidget(b_ind[i]); indl[i]->addSpacing(10); indl[i]->addWidget(ind_ll[i]);
        v_ind->addLayout(indl[i]);
    }

    QHBoxLayout *hl1 = new QHBoxLayout();
    hl1->addStretch(); hl1->addWidget(comp_stat);
    hl1->addSpacing(140); hl1->addLayout(v_ind);
    vl->addSpacing(80); vl->addLayout(hl1);  vl->addStretch(); vl->addLayout(hl); vl->addSpacing(-9);
    this->setLayout(vl);


    //Stylesheets
    this->setStyleSheet(
                "QPushButton{background: #737373; color: white; border: none;}"
                 "QPushButton::hover{background: #808080;}"
                  "QLabel#protectedlabel{background: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 0, stop: 0 #67e600, stop: 0.4 #67e600, stop: 0.8 #76ff03, stop: 1.0 #76ff03); color: white; font-family: 'quicksand'; font-size: 30px;"
                  "padding-left: 20px;}"

                );


}

void home :: connections()
{
    connect(links[0] , SIGNAL(clicked(bool)) , this , SLOT(gotoScan()));
}

void home :: gotoScan()
{
    emit scanRequired(1);
}
