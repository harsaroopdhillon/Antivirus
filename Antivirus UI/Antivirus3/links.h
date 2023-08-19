#ifndef LINKS_H
#define LINKS_H
#include <QLabel>


class links : public QLabel
{
    Q_OBJECT
public:
    links(const QString& = 0 ,QWidget *parent = 0);
    void mousePressEvent(QMouseEvent *ev);
    void enterEvent(QEvent *ev);
    void leaveEvent(QEvent *ev);
    ~links();
signals:
    void clicked();
};

#endif // LINKS_H
