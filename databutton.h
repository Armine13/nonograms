#ifndef DATABUTTON_H
#define DATABUTTON_H

#include <QPushButton>
#include <QMouseEvent>
#include <QtWidgets>
#include "gridbutton.h"

class DataButton: public GridButton
{
    Q_OBJECT

public:
    DataButton(int, int,int, QWidget *parent = 0);
    void addX(const bool);
    void setColor(const QColor);
    void setTextColor(const QColor);
    int getDirection() const;
    void disableButton();
    void enableButton();

private:
    void initBorders();
    void updateStyle();

    QIcon XIcon;
    QColor textColor;
    QString qss;
    int direction;

private slots:
    void mousePressEvent(QMouseEvent *e);

};

#endif // DATABUTTON_H
