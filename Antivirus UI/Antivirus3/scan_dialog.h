#ifndef SCAN_DIALOG_H
#define SCAN_DIALOG_H
#include <QWidget>
#include <QCryptographicHash>
#include <QFuture>
#include <QFutureWatcher>
#include <QThread>

class QTableWidget; class QLabel;
class QPushButton;
class scan_dialog
        : public QWidget
{
    Q_OBJECT
    QLabel *dir_label;
    int num, state, infected;
    unsigned long long fsize;
    QTableWidget *table;
    QPushButton *close;
    QThread thrd;
    QList<QFuture<QByteArray>> future;

public:
    scan_dialog(QWidget *parent = 0);
    bool checkDatabase(QByteArray);
    void refresh();

public slots:
    void getDir(const QString& dir);
    void onBackClicked();
signals:
    void goBack(int);
    void progress(int);
    void done(int infected);
};

#endif // SCAN_DIALOG_H
