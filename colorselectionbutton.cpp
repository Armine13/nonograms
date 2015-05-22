#include "colorselectionbutton.h"

ColorSelectionButton::ColorSelectionButton(int ind, QWidget *parent) :
    QPushButton(parent)
{
    index = ind;
    color = QColor(Qt::white);
}

int ColorSelectionButton::getIndex() const
{
    return index;
}

void ColorSelectionButton::setButtonColor(const QColor clr)
{
    color = clr;
    QString qss = QString("background-color: %1;").arg(color.name());
    setStyleSheet(qss);
}
QColor ColorSelectionButton::getButtonColor() const
{
    return color;
}
