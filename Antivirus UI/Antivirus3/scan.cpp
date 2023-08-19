#include "scan.h"
#include "header.h"
#include "scan_dialog.h"
#include <QButtonGroup>
#include <QGroupBox>
#include <QPixmap>
#include <QIcon>
#include <QDirIterator>
#include "checksum.h"

scan::scan(QWidget *parent)
    : QWidget(parent)
{
    this->setStyleSheet(
                "QPushButton{background: #8CC53F; font-size: 18px; color: white; border-radius: 4px; }"
                "QPushButton::hover{background: #7BB42E;} QPushButton::pressed{background: #8CC53F;}"
                "QRadioButton{color: #1B7A6B; font-size: 14px;}"
                "QTableView{border: 2px solid grey; border-radius: 4px; font-size: 11px;}"
                "QHeaderView::section { background-color: none; border: none; height: 35px;}" //#404244
                "QGroupBox{font-size: 18px; font-family: 'segoe ui'; font-weight: 100; color: #114A42; }"
                "QProgressBar{border: 2px solid grey; border-radius: 3px; text-align: center;}"
                " QProgressBar::chunk{background: #8CC53F; border-right: 4px solid transparent; border-radius: 2px;}"
                "QLabel{color: #114A42; font-size: 16px; font-family: 'segoe ui';}"
                );

    open_dir = new QPushButton(QIcon(":/images/run.png") , "Run"); open_dir->setFixedSize(120 , 45); clear_table = new QPushButton("Clear"); clear_table->setFixedSize(120 , 35);
    sd = new scan_dialog(); sd->setVisible(false); bar = new QProgressBar(); bar->setMinimum(0); bar->setMaximum(100); bar->setVisible(false);
    bar->setFixedHeight(30);
    stack = new QStackedWidget();
    b1 = new QRadioButton("Scan File"); b2 = new QRadioButton("Scan directory"); b3 = new QRadioButton("Full Scan");
    QButtonGroup *bbg  = new QButtonGroup();
    bbg->addButton(b1); bbg->addButton(b2); bbg->addButton(b3); b1->setChecked(true);
    status1 = new QLabel();
    QImage image(":/images/protected.png");
    status1->setPixmap(QPixmap::fromImage(image).scaled(250 , 250));
    bx = new QGroupBox("Scan option");
    curr_Dir = new QLabel(); curr_file = new QLabel(); n_infected = new QLabel(); scanned = new QLabel();
    table = new QTableWidget();
    QStringList headers; headers << "File " << "Hash" << "Status";
    table->setColumnCount(3); table->setHorizontalHeaderLabels(headers);
    table->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    table->setShowGrid(true); table->setWordWrap(true); table->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    stop_but = new QPushButton(QIcon(":/images/stop.png") , tr("Stop")); stop_but->setFixedSize(120 , 45);
    al_scan = new QWidget(); al_scan->setVisible(false);

    number_of_files = 0; n_done = 0; infected = 0;
    cs = new checksum();
    connect(cs , SIGNAL(done(QString,QByteArray,bool,int)) , this , SLOT(ScanFileDone(QString,QByteArray,bool,int)));
    connect(cs , SIGNAL(finally()) , this , SLOT(finally()));
    connect(cs , SIGNAL(starting(QString,int)) , this , SLOT(ScanStarting(QString,int)));
    connect(this, SIGNAL(scanDirNow(QString,int)) , cs , SLOT(fileCheckHash(QString,int)));
    connect(clear_table , SIGNAL(clicked(bool)) , this , SLOT(clear_table_s()));
    cs->moveToThread(&thread);
    thread.start();


    QVBoxLayout *vl = new QVBoxLayout();
    QVBoxLayout *vl1 = new QVBoxLayout();
    vl->addStretch();
    vl->addWidget(b1); vl->addSpacing(10); vl->addWidget(b2); vl->addSpacing(10); vl->addWidget(b3); vl->addSpacing(20);
    bx->setLayout(vl);
    vl1->addWidget(bx);
    QHBoxLayout *bll = new QHBoxLayout(); bll->addWidget(open_dir); bll->addSpacing(10); bll->addWidget(stop_but);
    vl1->addLayout(bll); /* vl1->addSpacing(10); vl1->addWidget(wait_label);*/ vl1->addStretch();

    QVBoxLayout *vv = new QVBoxLayout(); vv->addStretch(); vv->addWidget(status1); vv->addStretch();
    QHBoxLayout *hl = new QHBoxLayout(); hl->addStretch(); hl->addLayout(vv); hl->addStretch(); hl->addLayout(vl1); hl->addSpacing(40);
    QWidget *mWid = new QWidget(); mWid->setLayout(hl);

    stack->addWidget(mWid);

    QHBoxLayout *bl1 = new QHBoxLayout();
    bl1->addWidget(new QLabel("Current directory")); bl1->addSpacing(10); bl1->addWidget(curr_Dir); bl1->addStretch();

    QHBoxLayout *bl2 = new QHBoxLayout();
    bl2->addWidget(new QLabel("Scanning now")); bl2->addSpacing(10); bl2->addWidget(curr_file); bl2->addStretch();

    QHBoxLayout *bl3 = new QHBoxLayout();
    bl3->addWidget(new QLabel("Files scanned")); bl3->addSpacing(10); bl3->addWidget(scanned); bl3->addStretch();

    QVBoxLayout *tvb = new QVBoxLayout();
    tvb->addWidget(bar); tvb->addLayout(bl1); tvb->addLayout(bl2); tvb->addLayout(bl3);
    al_scan->setLayout(tvb);



    QVBoxLayout *vb = new QVBoxLayout();
    vb->addWidget(stack); vb->addStretch(); vb->addWidget(al_scan);
    stack->setObjectName("stk"); al_scan->setObjectName("al");
    stack->setStyleSheet("QWidget#stk{background: transparent;}"); al_scan->setStyleSheet("QWidget#al{background: transparent;}");

    QWidget *main_container = new QWidget(); main_container->setObjectName("cont");
    main_container->setLayout(vb); main_container->setStyleSheet("QWidget#cont{background: #E6E6E6; border: 1px solid transparent; border-radius: 3px;}");
    QHBoxLayout *m_lay = new QHBoxLayout(); m_lay->addWidget(main_container);
    this->setLayout(m_lay);

    connect(open_dir , SIGNAL(clicked(bool)) , this , SLOT(scanDir()));
    connect(sd , SIGNAL(goBack(int)) , stack , SLOT(setCurrentIndex(int)));
    connect(sd , SIGNAL(goBack(int)) , this , SLOT(hidebar()));
    connect(sd , SIGNAL(progress(int)) , this , SLOT(updateProgress(int)));
}

void scan :: hidebar()
{
    bar->setVisible(false); bar->setValue(0);
    sd->setVisible(false);
}
void scan :: scanDir()
{
    stack->setCurrentIndex(1);

    if(b1->isChecked())
    {
        //Scan file
        QString dir = QFileDialog::getOpenFileName(this , "Select file to scan" , QDir::homePath());
        emit scanDirNow(dir , 2);
    }else if(b2->isChecked())
    {
        //Scan directory
        QString dir = QFileDialog::getExistingDirectory(this , "Select a folder to generate and compare hash" , QDir::homePath());
        emit scanDirNow(dir , 0);
    }else if(b3->isChecked()){
        //Scan All
        emit scanDirNow(QDir::homePath() , 1);
    }
}

void scan :: updateProgress(int a)
{
    this->bar->setValue(a);
}

void scan :: scanDone(int infected)
{
//
}

void scan :: ScanStarting(const QString &dir, int num)
{
    open_dir->setDisabled(true);
    clear_table->setDisabled(true);
    curr_Dir->setText(dir);
    bar->setRange(0 , num);
    bar->setVisible(true);
    al_scan->setVisible(true);
}

void scan :: ScanFileDone(const QString &filen, QByteArray hash, bool found, int num)
{
    curr_file->setText(filen);
    bar->setValue(num);
    if(found){
        infected += 1;
        table->insertRow(table->rowCount());
        table->setItem(table->rowCount() - 1 , 0 , new QTableWidgetItem(filen));
        table->setItem(table->rowCount() - 1 , 1 , new QTableWidgetItem(QString(hash.toHex())));
        table->setItem(table->rowCount() - 1 , 2 , new QTableWidgetItem("FOUND"));
        table->scrollToBottom();
    }
    else{
        //
    }
    n_infected->setText(QString::number(infected));
    number_of_files += 1;
    scanned->setText(QString::number(number_of_files));
}

void scan :: finally()
{
    bar->setVisible(false);
    bar->setValue(0);
    open_dir->setDisabled(false);
    clear_table->setDisabled(false);
}

void scan :: clear_table_s()
{
    table->model()->removeRows(0 , table->rowCount()); table->setRowCount(0);
    curr_Dir->setText(""); curr_file->setText(""); n_infected->setText(""); scanned->setText("");
    number_of_files = 0; n_done = 0; infected = 0;
    al_scan->setVisible(false);
}

scan :: ~scan()
{
    if(thread.isRunning())
        thread.terminate();

}


