#ifndef ColorSelectionButton_H
#define ColorSelectionButton_H

#include <QWidget>
#include <QPushButton>

class ColorSelectionButton : public QPushButton
{
    Q_OBJECT
public:
    ColorSelectionButton(int ind = 0, QWidget *parent = 0);
    int getIndex() const;
    QColor getButtonColor() const;
    void setButtonColor(const QColor);

private:
    int index;
    QColor color;

private slots:

signals:
};

#endif // ColorSelectionButton_H
