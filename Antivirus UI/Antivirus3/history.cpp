#include "history.h"
#include "header.h"

history::history(QWidget *parent)
    : QWidget(parent)
{
    init_widgets();
    layout_widgets();
}

void history :: init_widgets()
{
    q_items = new QRadioButton(tr("Quarantined Items"));
    a_items = new QRadioButton(tr("Allowed Items"));
    ad_items = new QRadioButton(tr("All Detected Items"));
    q_label = new QLabel("Some text goes here ");
    a_label = new QLabel("Some text goes here ");
    ad_label = new QLabel("Some text goes here ");

    v_label = new QLabel("These items are hidden");
    view_b = new QPushButton(tr("View")); view_b->setFixedSize(180 , 30); view_b->setCheckable(true);

    connect(view_b , SIGNAL(toggled(bool)) , this , SLOT(showHistoryTable(bool)));

    table = new QTableWidget(); table->setVisible(false);
    QStringList headers; headers << "Detected Items" << "Alert level" << "Date" << "Action taken" << "Detection method";
    table->setColumnCount(5); table->setHorizontalHeaderLabels(headers);
    //table->setColumnWidth(0 , 20); table->setColumnWidth(2 , 120);
    table->setFixedHeight(150);
    for(int i = 0 ; i < 5; i++) table->horizontalHeader()->setSectionResizeMode(i, QHeaderView::Stretch);
    table->setShowGrid(true); table->setWordWrap(true); table->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    cat = new QLabel(); desc = new QLabel(); cat->setVisible(false); desc->setVisible(false);
    remov_but = new QPushButton(tr("Remove")); restor_but = new QPushButton(tr("Restore"));
    remov_but->setVisible(false); restor_but->setVisible(false);
    remov_but->setFixedSize(110 , 30); restor_but->setFixedSize(110 , 30);
    remov_but->setStyleSheet("font-size: 14px; font-family: 'quicksand'"); restor_but->setStyleSheet("font-size: 14px;font-family: 'quicksand'");

    QButtonGroup bgrp;
    bgrp.addButton(q_items); bgrp.addButton(a_items); bgrp.addButton(ad_items);
    q_items->setChecked(true);

    setStyleSheet("QLabel{color: #1B7A6B; font-family: 'quicksand'; font-size: 14px; }"
                  "QRadioButton{color: #1B7A6B; font-family: 'quicksand'; font-size: 18px; }"
                  "QPushButton{background: #8CC53F; font-size: 18px; color: white; border-radius: 4px; }"
                  "QPushButton::hover{background: #7BB42E;} QPushButton::pressed{background: #8CC53F;}"
                  "QTableView{border: 2px solid grey; border-radius: 4px; font-size: 11px;}"
                  "QHeaderView::section { background-color: none; border: none; height: 35px;}" //#404244
                  );
}


void history :: layout_widgets()
{
    QVBoxLayout* vl1 = new QVBoxLayout();
    QHBoxLayout *hl[7]; for(int i =0 ; i < 7; i++) hl[i] = new QHBoxLayout();
    vl1->addWidget(q_items); vl1->addSpacing(5);
    hl[0]->addSpacing(20); hl[0]->addWidget(q_label);
    vl1->addLayout(hl[0]); vl1->addSpacing(5);
    vl1->addWidget(a_items); vl1->addSpacing(5);
    hl[1]->addSpacing(20); hl[1]->addWidget(a_label);
    vl1->addLayout(hl[1]); vl1->addSpacing(5);
    vl1->addWidget(ad_items); vl1->addSpacing(5);
    hl[2]->addSpacing(20); hl[2]->addWidget(ad_label);
    vl1->addLayout(hl[2]); vl1->addSpacing(10);
    QHBoxLayout *hl1 = new QHBoxLayout();
    hl1->addSpacing(60); hl1->addLayout(vl1);
    QVBoxLayout *vl2 = new QVBoxLayout();

    vl2->addLayout(hl1);

    hl[3]->addStretch(); hl[3]->addWidget(v_label); hl[3]->addStretch();
    vl2->addLayout(hl[3]); vl2->addWidget(view_b , 0 , Qt::AlignCenter);

    t1 = new QLabel("Category: "); t2 = new QLabel("Description: "); t3 = new QLabel("<hr>");
    t1->setVisible(false); t2->setVisible(false); t3->setVisible(false);
    hl[4]->addWidget(t1); hl[4]->addWidget(cat);
    hl[5]->addWidget(t2); hl[5]->addWidget(desc);
    hl[6]->addStretch(); hl[6]->addWidget(remov_but); hl[6]->addWidget(restor_but);

    vl2->addSpacing(10); vl2->addWidget(table); vl2->addLayout(hl[4]); vl2->addLayout(hl[5]);
    vl2->addLayout(hl[6]);

    vl2->addStretch();


    QWidget *top_container = new QWidget(); top_container->setObjectName("tp");
    top_container->setLayout(vl2); top_container->setStyleSheet("QWidget#tp{background: #E6E6E6; border: 1px solid transparent; border-radius: 3px;}");
    QHBoxLayout *mlay = new QHBoxLayout(); mlay->addWidget(top_container);

    this->setLayout(mlay);
}

void history :: showHistoryTable(bool b)
{
    if(b) view_b->setText("^"); else view_b->setText("View");
    table->setVisible(b); cat->setVisible(b); desc->setVisible(b);
    remov_but->setVisible(b); restor_but->setVisible(b);
    t1->setVisible(b); t2->setVisible(b); t3->setVisible(b);
}
