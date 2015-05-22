#ifndef GRIDBUTTON_H
#define GRIDBUTTON_H
#include <QPushButton>
#include <QMouseEvent>
#include <QtWidgets>

class GridButton: public QPushButton
{
    Q_OBJECT

public:
    GridButton(QWidget *parent = 0);
    GridButton(const int, const int, QWidget *parent = 0);
    ~GridButton();
    int getRow() const;
    int getCol() const;
    QColor getColor() const;
    void addX(const bool);
    void setColor(const QColor);

public slots:
    void mousePressEvent(QMouseEvent *e);
    void mousePressEventLeft(QMouseEvent *e);
    void mousePressEventRight(QMouseEvent *e);

signals:
    void rightClicked();
    void leftClicked();

protected:
    int row;
    int col;
    QIcon XIcon;
    QColor color;
    QColor borderTop;
    QColor borderRight;
    QColor borderBottom;
    QColor borderLeft;
    QString qss;

private:
    void updateStyle();
    void initBorders();
};
#endif // GridButton_H

