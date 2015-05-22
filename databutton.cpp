#include "databutton.h"

DataButton::DataButton(int i , int j, int dir, QWidget *parent)
    :GridButton(parent)
{
    direction = dir;
    if (direction == 0)
        setAccessibleName(QString("GridButtonY"));
    else
        setAccessibleName(QString("GridButtonX"));

    row = i;
    col = j;
    XIcon = QIcon(":/new/prefix1/images/whiteX.png");
    textColor = QColor(Qt::black);
    this->setFixedSize(20,20);

    //Load the stylesheet file for widgets
    QFile stylesFile(":/new/prefix1/qss/dataButtonsStyle.qss");
    if(stylesFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qss = stylesFile.readAll();
        stylesFile.close();
    }
    setProperty("withIcon",false);
    initBorders();
    updateStyle();
}
void DataButton::mousePressEvent(QMouseEvent *e)
{
    if(e->button() == Qt::RightButton)
        emit rightClicked();
    if(e->button() == Qt::LeftButton)
        emit leftClicked();
}
void DataButton::disableButton()
{
    this->setEnabled(false);
    this->setIcon(QIcon());
}
void DataButton::enableButton()
{
    this->setEnabled(true);

    this->setIcon(QIcon());
}
void DataButton::addX(const bool a)
{
    this->setProperty("withIcon",true);
    this->style()->unpolish(this);
    this->style()->polish(this);
    this->update();
    if (a)
    {
        this->setProperty("withIcon",true);
        this->style()->unpolish(this);
        this->style()->polish(this);
        this->update();
    }
    else
    {
        this->setProperty("withIcon",false);
        this->style()->unpolish(this);
        this->style()->polish(this);
        this->updateStyle();
    }
}
void DataButton::setColor(const QColor clr)
{
    color = clr;
    updateStyle();
}
void DataButton::setTextColor(const QColor clr)
{
    textColor = clr;
    updateStyle();
}
void DataButton::updateStyle()
{
    setStyleSheet(qss.arg(color.name()).arg(textColor.name()).arg(borderTop.name()).arg(borderRight.name()).arg(borderBottom.name()).arg(borderLeft.name()));
}
void DataButton::initBorders()
{
    int pos;
    QColor darkGray("#3D3D3D");

    if (direction == 0)
    {
        pos = getRow();
        if ((pos + 1) % 5 == 0)
            borderBottom = darkGray;

        if (pos % 5 == 0)
            borderTop = darkGray;
    }
    else
    {
        pos = getCol();
        if ((pos + 1) % 5 == 0)
            borderRight = darkGray;

        if (pos % 5 == 0)
            borderLeft = darkGray;
    }
}
int DataButton::getDirection() const
{
    return direction;
}
