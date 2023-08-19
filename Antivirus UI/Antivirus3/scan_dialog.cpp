#include "scan_dialog.h"
#include "header.h"
#include <QDebug>
#include <checksum.h>
QByteArray fileCheckHash(const QString&);
scan_dialog::scan_dialog(QWidget *parent)
    : QWidget(parent)
{

    //Widget Initializations
    dir_label = new QLabel(); infected = 0;
    table = new QTableWidget(); close = new QPushButton("Close");
    close->setFixedSize(120 , 35); state = 1;

    //Widget Layout
    QVBoxLayout *vl = new QVBoxLayout(); vl->addSpacing(-10);
    vl->addWidget(dir_label); vl->addSpacing(10);
    vl->addWidget(table); vl->addWidget(close , 0 , Qt::AlignRight);
    this->setLayout(vl);

    //Widget setups
    QStringList headers; headers << "File " << "Hash" << "Status";
    table->setColumnCount(3); table->setHorizontalHeaderLabels(headers);
    table->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    table->setShowGrid(true); table->setWordWrap(true); table->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    table->setMinimumHeight(200);

    this->setObjectName("window");
    this->setStyleSheet(
                    "QWidget#window{background: #F0F1F5; border: 3px solid #94A1A9; border-radius: 4px;}"
                    "QTableView{border: 2px solid grey; border-radius: 4px; font-size: 11px;}"
                    "QHeaderView::section { background-color: none; border: none; height: 35px;}" //#404244
                    "QPushButton{background: #C7C8FC; font-size: 18px; color: white; border-radius: 4px; }"
                    "QPushButton::hover{background: #B1D6F3;} QPushButton::pressed{background: #C7C8FC;}"
                    "QLabel{font-size: 18px; font-family: 'segoe ui'; font-weight: 100; color: green;}"
                        );
    this->setAutoFillBackground(true);
    //Widget connection
    connect(close , SIGNAL(clicked(bool)) , this , SLOT(onBackClicked()));
}

void scan_dialog :: getDir(const QString &dir)
{
    QDir *temp = new QDir(dir);
    QStringList *files_name = new QStringList(temp->entryList(QDir::Files | QDir::Readable));

    num = files_name->length();
    fsize = 0;
    for(int i = 0; i < num; i++)
    {
        QFile myFile(dir + "/" + files_name->at(i));
        if (myFile.open(QIODevice::ReadOnly)){
            fsize += myFile.size();
            myFile.close();
        }
    }

    for(int i = 0; i < files_name->length(); i++){
        dir_label->setText("Number of files:  " + QString::number(num));

        table->insertRow(table->rowCount());
        table->setItem(table->rowCount()-1, 0 , new QTableWidgetItem(files_name->at(i)));
        future.append(QtConcurrent::run(fileCheckHash , QString(dir + "/" + files_name->at(i))));
    }
    refresh();
}

void scan_dialog :: refresh()
{
    state = 0;
    bool cont = true;
    int k, n;

    while(cont)
    {
        k = 0;
        for(int i = 0; i < num; i++)
        {
            if(future.at(i).isFinished())
                k = k + 1;
        }
        float vval = ((k * 1.0)/(num * 1.0)) * fsize;
        float prog = (vval/(fsize * 1.0)) * 100;
        emit progress(prog);
        qDebug() << "Progress  " << k << fsize << prog;
        if(k >= num)
            cont = false;
    }

    if(future.length() == num){
        for(int i = 0; i < num; i++)
        {
            if(future.at(i).result() == "0")
                while(future.at(i).result() == "0");
            QTableWidgetItem *item = new QTableWidgetItem(QString(future.at(i).result().toHex()));
            item->setTextAlignment(Qt::AlignHCenter); table->setItem(i, 1 , item);
            checksum check;
            if(check.CheckDataBase(future.at(i).result().toHex())){
                table->setItem(i , 2 , new QTableWidgetItem("FOUND"));
                infected += 1;
            }
            else
                table->setItem(i , 2 , new QTableWidgetItem("NOT FOUND"));
            table->resizeColumnToContents(1);
        }
    }
    emit done(infected);
    state = 1;
}

void scan_dialog :: onBackClicked()
{
    if(state == 0)
    {
        while(state == 0);
    }
    table->model()->removeRows(0 , table->rowCount()); table->setRowCount(0);
    num = 0; dir_label->clear(); future.clear();
    emit goBack(0);
}

QByteArray fileCheckHash(const QString &fileName)
{
    QFile f(fileName);
    if (f.open(QFile::ReadOnly)) {
        QCryptographicHash hash(QCryptographicHash::Sha256);
        if (hash.addData(&f)) {
            return hash.result();
        }
    }
    return QByteArray();
}

