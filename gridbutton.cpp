#include "GridButton.h"

GridButton::GridButton(QWidget *parent)
{
    row = 0;
    col = 0;
    color = QColor(Qt::white);
    borderTop = QColor();
    borderRight = QColor();
    borderBottom = QColor();
    borderLeft = QColor();
    borderTop.setNamedColor("#A0A0A0");
    borderRight.setNamedColor("#A0A0A0");
    borderBottom.setNamedColor("#A0A0A0");
    borderLeft.setNamedColor("#A0A0A0");
}

GridButton::GridButton(const int i , const int j , QWidget *parent)
    :QPushButton(parent)
{
    setAccessibleName(QString("GridButton"));
    row = i;
    col = j;
    XIcon = QIcon(":/new/prefix1/images/x3.png");
    color = QColor(Qt::white);
    borderTop = QColor();
    borderRight = QColor();
    borderBottom = QColor();
    borderLeft = QColor();
    borderTop.setNamedColor("#A0A0A0");
    borderRight.setNamedColor("#A0A0A0");
    borderBottom.setNamedColor("#A0A0A0");
    borderLeft.setNamedColor("#A0A0A0");
    this->setFixedSize(20,20);

    //Load the stylesheet file for widgets
    QFile stylesFile(":/new/prefix1/qss/styles.qss");
    if(stylesFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qss = stylesFile.readAll();
        stylesFile.close();
    }
    initBorders();
    updateStyle();
}
GridButton::~GridButton()
{}
int GridButton::getCol() const
{
    return this->col;
}
int GridButton::getRow() const
{
    return this->row;
}
void GridButton::addX(const bool a)
{
    color = QColor(Qt::white);
    setStyleSheet(qss.arg(color.name()).arg(borderTop.name()).arg(borderRight.name()).arg(borderBottom.name()).arg(borderLeft.name()));
    if (a)
    {
        this->setIcon(XIcon);
        setIconSize(QSize(size().width()*0.8, size().height()*0.8));
    }
    else
    {
        this->setIcon(QIcon());
    }
}
void GridButton::setColor(const QColor clr)
{
    color = clr;
    updateStyle();
}
QColor GridButton::getColor() const
{
    return color;
}
void GridButton::updateStyle()
{
    setStyleSheet(qss.arg(color.name()).arg(borderTop.name()).arg(borderRight.name()).arg(borderBottom.name()).arg(borderLeft.name()));
}
void GridButton::initBorders()
{
    int r = getRow();
    int c = getCol();
    QColor darkGray("#3D3D3D");

    if ((c + 1) % 5 == 0)
        borderRight = darkGray;

    if (c % 5 == 0)
        borderLeft = darkGray;

    if ((r + 1) % 5 == 0)
        borderBottom = darkGray;

    if (r % 5 == 0)
        borderTop = darkGray;
}
void GridButton::mousePressEventLeft(QMouseEvent *e)
{
     emit leftClicked();
}
void GridButton::mousePressEventRight(QMouseEvent *e)
{
     emit rightClicked();
}
void GridButton::mousePressEvent(QMouseEvent *e)
{
     QWidget::mousePressEvent(e);
}
