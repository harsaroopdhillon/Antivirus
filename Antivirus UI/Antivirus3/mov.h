#ifndef MOV_H
#define MOV_H
#include <QWidget>
#include <QMouseEvent>

class mov
        : public QWidget
{
    int x , y;
    QWidget *par;
public:
    mov(QWidget *parent){
        par = parent;
    }
    void mousePressEvent(QMouseEvent *event)
    {
        x = event->x(); y = event->y();
    }
    void mouseMoveEvent(QMouseEvent * event)
    {
        if(x >= 0 && ((y >= 0) & (par != 0)))
        par->move(event->globalX()- x , event->globalY()- y);
    }
};

#endif // MOV_H
