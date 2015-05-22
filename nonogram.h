#ifndef Nonogram_H
#define Nonogram_H

#include <QMainWindow>
#include <QColor>
#include "GridButton.h"
#include "picturedata.h"
#include "ColorSelectionButton.h"
#include "databutton.h"
#include <QGridLayout>
#include <QLabel>
#include <QVector>

class Nonogram : public QWidget
{
    Q_OBJECT

public:
    Nonogram(QWidget *parent = 0);
    Nonogram(const int rows,const int cols,const int clrs, QWidget *parent = 0);//New nonogram constructor
    Nonogram(const QString str, QWidget *parent = 0);//Open existing nonogram
    ~Nonogram();
    void savePicture(const QString str) const;//save created nonogram
    void saveProgress(const QString str) const;
    void loadNonogram(const QString str);

private slots:
    void rightButtonClicked();
    void leftButtonClicked();
    void colorButtonClicked();
    void rightDataButtonClicked();
    void leftDataButtonClicked();
    bool eventFilter(QObject *object, QEvent *e);
    void resetPuzzle();
    void verify();
    void answer();
    void pickColor();
    void solve();

private:
    void setMarked(const int ind1,const int ind2,const int value);
    void setMarkedData(const int direction, const int index1, const int index2,const int value);
    int getMarked(const int index1,const int index2)const;
    int getMarkedData(const int direction,const int index1, const int index2);

    void createColorSelectionBar();
    void createColorSelectionBarBlank();
    void createToolbar();
    void drawGrid();
    void drawBlankGrid();
    void updateColorIndicator();
    void createToolbarBlank();

    static int mouseCol;
    static int mouseRow;

    bool leftButtonPressedFlag;
    bool rightButtonPressedFlag;
    bool blankNonogram;
    int dataRowsY;
    int dataColsY;
    int dataRowsX;
    int dataColsX;
    int gridLength;
    int gridHeight;
    QVector<QVector<int> > marked;
    QVector<QVector<int> > markedX;
    QVector<QVector<int> > markedY;
    QVector<QVector<QColor> > markedColors;
    QVector<QVector<GridButton*> > button;
    QVector<QVector<DataButton*> > buttonY;
    QVector<QVector<DataButton*> > buttonX;
    QVector<QColor> colorList;
    QColor* selectedColor;
    QGridLayout* gridLayout;
    PictureData* picture;
    QVector<ColorSelectionButton*> colorButton;
    QVector<ColorSelectionButton*> selectColorButton;
    QWidget* mainWidget;
    QWidget* gridWidget;
    QWidget* colorButtonsWidget;
    QWidget* toolbarWidget;
    QVBoxLayout* mainLayout;
    QColorDialog* colorDialog;
    QPushButton* selectedColorIndicator;
    QPushButton* verifyButton;
    QPushButton* solvableButton;



};

#endif // Nonogram_H

