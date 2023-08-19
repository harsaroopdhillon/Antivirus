#include "settings.h"
#include "ui_settings.h"

settings::settings(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::settings)
{
    ui->setupUi(this);

    setStyleSheet("QLabel{color: #1B7A6B; font-family: 'segoe ui'; font-size: 16px; }"
                  "QRadioButton , QCheckBox{color: #1B7A6B; font-family: 'segoe ui'; font-size: 16px; }"
                  "QPushButton{background: #8CC53F; font-size: 18px; color: white; border-radius: 4px; }"
                  "QPushButton::hover{background: #7BB42E;} QPushButton::pressed{background: #8CC53F;}"
                  "QTableView{border: 2px solid grey; border-radius: 4px; font-size: 11px;}"
                  "QHeaderView::section { background-color: none; border: none; height: 35px;}" //#404244
                  "QHeaderView::section { background-color: none; border: none; height: 35px;}"
                  "QScrollArea{border: none;}"
                  );

    connections();
    ui->scanner_sw->setCheckable(true);
    ui->browser_sw->setCheckable(true);
    for(int i{0}; i < 3; i++) ui->browser_status_tab->horizontalHeader()->setSectionResizeMode(i, QHeaderView::Stretch);
    ui->browser_status_tab->setShowGrid(true);

}

void settings :: connections()
{
    connect(ui->scn_but , SIGNAL(clicked(bool)) , this , SLOT(gotoScanner(bool)));
    connect(ui->brow_prot , SIGNAL(clicked(bool)) , this , SLOT(gotoBrowser(bool)));
    connect(ui->back1 , SIGNAL(clicked(bool)) , this , SLOT(gotoHome()));
    connect(ui->back2 , SIGNAL(clicked(bool)) , this , SLOT(gotoHome()));
}

void settings :: gotoScanner(bool st)
{
    ui->stack->setCurrentIndex(1);
}

void settings :: gotoBrowser(bool st)
{
    ui->stack->setCurrentIndex(2);
}

void settings :: gotoHome()
{
    ui->stack->setCurrentIndex(0);
}

settings::~settings()
{
    delete ui;
}
