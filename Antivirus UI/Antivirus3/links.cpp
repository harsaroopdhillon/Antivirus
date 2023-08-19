#include "links.h"
#include <QFont>

links::links(const QString& text, QWidget *parent)
    : QLabel(text , parent){
}
void links :: mousePressEvent(QMouseEvent *ev)
{
    emit clicked();
}

void links :: enterEvent(QEvent *ev)
{
    QFont f = font();
    f.setUnderline(true);
    setFont(f);
}

void links :: leaveEvent(QEvent *ev)
{
    QFont f = font();
    f.setUnderline(false);
    setFont(f);
}

links::~links()
{

}

