#ifndef SCAN_H
#define SCAN_H
#include <QStackedWidget>
#include <QThread>

/*
 * Scan your computer window interface
 * declared as a subclass of QWidget
 * */

class QPushButton; class QLabel; class QTableWidget;
class scan_dialog; class QProgressBar; class checksum;
class QRadioButton; class QGroupBox; class QDirIterator;

class scan
        : public QWidget
{
    Q_OBJECT
  QPushButton *open_dir, *clear_table , *stop_but;
  QLabel *status1;
  QLabel *curr_Dir, *curr_file, *n_infected, *scanned;
  scan_dialog *sd;  checksum *cs;
  QProgressBar *bar;
  QTableWidget *table;
  QStackedWidget *stack;
  QRadioButton *b1 , *b2, *b3;
  QGroupBox *bx;
  QThread thread;
  QWidget *al_scan;
  unsigned long long number_of_files, n_done, infected;

public:
    scan(QWidget *parent = 0);
    ~scan();

private slots:
    void scanDir(); //To scan directories for files
    void updateProgress(int);
    void hidebar();
    void scanDone(int infected);

    void ScanStarting(const QString& dir , int num);
    void ScanFileDone(const QString& filen, QByteArray hash , bool found , int num);
    void finally();
    void clear_table_s();

signals:
    void scanDirNow(const QString& , int type);
};

#endif // SCAN_H
