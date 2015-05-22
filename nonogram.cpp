#include "nonogram.h"
#include "picturedata.h"
#include "GridButton.h"
#include "databutton.h"
#include "solver.h"


int Nonogram::mouseCol = 0;
int Nonogram::mouseRow = 0;
Nonogram::Nonogram(QWidget *parent)//empty nonogram constructor
{
    blankNonogram = true;
    leftButtonPressedFlag = false;
    rightButtonPressedFlag = false;
    selectedColor = new QColor(Qt::white);
}

Nonogram::Nonogram(const int rows, const int cols, const int clrs, QWidget *parent)//new nonogram
{
    installEventFilter(this);
    blankNonogram = true;
    leftButtonPressedFlag = false;
    rightButtonPressedFlag = false;

    gridWidget = new QWidget(this);
    selectedColor = new QColor(Qt::white);

    gridLength = cols;
    gridHeight = rows;
    dataRowsY = rows;
    dataColsY = 5;
    dataRowsX = 5;
    dataColsX = cols;

    marked.resize(gridHeight);
    markedColors.resize(gridHeight);
    button.resize(gridHeight);
    for(int i = 0; i < gridHeight; i++)
    {
        marked[i].resize(gridLength);
        markedColors[i].resize(gridLength);
        button[i].resize(gridLength);
        marked[i].fill(0);
        markedColors[i].fill(QColor(Qt::white));
    }
    markedX.resize(dataRowsX);
    buttonX.resize(dataRowsX);
    for(int i = 0; i < dataRowsX; i++)
    {
        markedX[i].resize(dataColsX);
        markedX[i].fill(0);
        buttonX[i].resize(dataColsX);
    }
    markedY.resize(dataRowsY);
    buttonY.resize(dataRowsY);
    for(int i = 0; i < dataRowsY; i++)
    {
        markedY[i].resize(dataColsY);
        markedY[i].fill(0);
        buttonY[i].resize(dataColsY);
    }

    drawBlankGrid();
    colorList.resize(clrs);
    colorList.fill(Qt::white);
    createColorSelectionBarBlank();

    createToolbarBlank();
    mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(toolbarWidget);
    mainLayout->addWidget(gridWidget);
    mainLayout->addWidget(colorButtonsWidget);
    setLayout(mainLayout);

    mainLayout->setSizeConstraint(QLayout::SetFixedSize);

}

Nonogram::Nonogram(const QString str, QWidget *parent)//existing nonogram constructor
    : QWidget(parent)
{
    blankNonogram = false;
    installEventFilter(this);
    leftButtonPressedFlag = false;
    rightButtonPressedFlag = false;

    gridWidget = new QWidget(this);
    selectedColor = new QColor(Qt::white);

    //read nonogram data from file
    picture = new PictureData(str);

    gridLength = picture->getGridLength();
    gridHeight = picture->getGridHeight();
    dataRowsY = picture->getDataYRows();
    dataColsY = picture->getDataYColumns();
    dataRowsX = picture->getDataXRows();
    dataColsX = picture->getDataXColumns();

    marked.resize(gridHeight);
    markedColors.resize(gridHeight);
    button.resize(gridHeight);
    for(int i = 0; i < gridHeight; i++)
    {
        marked[i].resize(gridLength);
        markedColors[i].resize(gridLength);
        button[i].resize(gridLength);
        marked[i].fill(0);
        markedColors[i].fill(QColor(Qt::white));
    }
    markedX.resize(dataRowsX);
    buttonX.resize(dataRowsX);
    for(int i = 0; i < dataRowsX; i++)
    {
        markedX[i].resize(dataColsX);
        markedX[i].fill(0);
        buttonX[i].resize(dataColsX);
    }
    markedY.resize(dataRowsY);
    buttonY.resize(dataRowsY);
    for(int i = 0; i < dataRowsY; i++)
    {
        markedY[i].resize(dataColsY);
        markedY[i].fill(0);
        buttonY[i].resize(dataColsY);
    }

    drawGrid();
    createColorSelectionBar();
    createToolbar();
    mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(toolbarWidget);
    mainLayout->addWidget(gridWidget);
    mainLayout->addWidget(colorButtonsWidget);

    setLayout(mainLayout);
    mainLayout->setSizeConstraint(QLayout::SetFixedSize);


}
//function to clear all marks from the grid
void Nonogram::resetPuzzle()
{
    selectedColor = new QColor(Qt::white);
    updateColorIndicator();
    if (blankNonogram) solvableButton->setStyleSheet(QString("background-color: #BEBEBE;"));
    else verifyButton->setStyleSheet(QString("background-color: #BEBEBE;"));

    QVector< QVector<int> >        ::iterator it;
    QVector< QVector<QColor> >     ::iterator itc;
    QVector<QVector<GridButton*> > ::iterator itb;
    QVector<GridButton*>           ::iterator itb2;
    QVector<QVector<DataButton*> >::iterator itbY;
    QVector<DataButton*>          ::iterator itbY2;
    QVector<QVector<DataButton*> >::iterator itbX;
    QVector<DataButton*>          ::iterator itbX2;
    //resetting all data
    for ( itc = markedColors.begin(); itc != markedColors.end(); itc++)
        itc->fill(QColor(Qt::white));

    for ( it = marked.begin(); it != marked.end(); it++)
        it->fill(0);

    for ( it = markedX.begin(); it != markedX.end(); it++)
        it->fill(0);

    for ( it = markedY.begin(); it != markedY.end(); it++)
        it->fill(0);

    for(itb = button.begin(); itb != button.end(); itb++)
        for(itb2 = (*itb).begin(); itb2 != (*itb).end(); itb2++)
            (*itb2)->addX(false);

    for(itbX = buttonX.begin(); itbX != buttonX.end(); itbX++)
        for(itbX2 = (*itbX).begin(); itbX2 != (*itbX).end(); itbX2++)
            (*itbX2)->addX(false);


    for(itbY = buttonY.begin(); itbY != buttonY.end(); itbY++)
        for(itbY2 = (*itbY).begin(); itbY2 != (*itbY).end(); itbY2++)
            (*itbY2)->addX(false);
}

Nonogram::~Nonogram()
{
    delete selectedColor;
    selectedColor = NULL;
}
void Nonogram::drawGrid()
{
    int h, s, v;
    gridLayout = new QGridLayout(gridWidget);
    gridLayout->setHorizontalSpacing(0);
    gridLayout->setVerticalSpacing(0);

    //creating the central buttons
    for(int i = 0; i < gridHeight; i++)
    {
        for(int j = 0; j < gridLength; j++)
        {
            button[i][j] = new GridButton(i, j,gridWidget);
            button[i][j]->setEnabled(true);
            gridLayout->addWidget(button[i][j], i + dataRowsX, j + dataColsY);
            connect(button[i][j], SIGNAL(rightClicked()), this, SLOT(rightButtonClicked()));
            connect(button[i][j], SIGNAL(leftClicked()), this, SLOT(leftButtonClicked()));
        }
    }
    //creating vertical data buttons
    for(int i = 0; i < gridHeight; i++)
        for(int j = 0; j < dataColsY; j++)
        {
            buttonY[i][j] = new DataButton(i,j,0,gridWidget);
            buttonY[i][j]->setColor(picture->getColorY(i,j));//read color of button from data
            picture->getColorY(i,j).getHsv(&h,&s,&v);// find HSV values of the button color
            if (picture->getColorY(i,j) == QColor(Qt::white) || v < 150)//if color of button is darker
                //than threshold value of 150/255, set button text color to white, otherwise leave it black
            {
                buttonY[i][j]->setTextColor(QColor(Qt::white));
            }
            buttonY[i][j]->setText(QString::number(picture->getDataY(i,j)));
            if (picture->getDataY(i,j) == 0)
                buttonY[i][j]->disableButton();//disable all data buttons
            connect(buttonY[i][j], SIGNAL(rightClicked()), this, SLOT(rightDataButtonClicked()));
            connect(buttonY[i][j], SIGNAL(leftClicked()), this, SLOT(leftDataButtonClicked()));
            gridLayout->addWidget(buttonY[i][j],i + dataRowsX, j);
        }
    //creating horizontal data buttons
    for(int i = 0; i < dataRowsX; i++)
        for(int j = 0; j < dataColsX; j++)
        {
            buttonX[i][j] = new DataButton(i,j,1,gridWidget);
            buttonX[i][j]->setColor(picture->getColorX(i,j));//read and apply color of button from data
            picture->getColorX(i,j).getHsv(&h,&s,&v);// find HSV values of the button color
            if (picture->getColorX(i,j) == QColor(Qt::white) || v < 150)//if color of button is darker
                //than threshold value of 150/255, set button text color to white, otherwise leave it black
            {
                buttonX[i][j]->setTextColor(QColor(Qt::white));
            }
            buttonX[i][j]->setText(QString::number(picture->getDataX(i,j)));
            if (picture->getDataX(i,j) == 0)
                buttonX[i][j]->disableButton();
            connect(buttonX[i][j], SIGNAL(rightClicked()), this, SLOT(rightDataButtonClicked()));
            connect(buttonX[i][j], SIGNAL(leftClicked()), this, SLOT(leftDataButtonClicked()));
            gridLayout->addWidget(buttonX[i][j],i, j + dataColsY);
        }

    selectedColorIndicator = new QPushButton(gridWidget);//the indicator of selected color
    selectedColorIndicator->setEnabled(false);
    QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(selectedColorIndicator->sizePolicy().hasHeightForWidth());
    selectedColorIndicator->setSizePolicy(sizePolicy);

    selectedColorIndicator->setStyleSheet(QString("background-color: %1;").arg(selectedColor->name()));
    if (dataRowsX > 1 && dataColsY > 1)
        gridLayout->addWidget(selectedColorIndicator, 0, 0, dataRowsX - 1, dataColsY - 1);
    else
        gridLayout->addWidget(selectedColorIndicator, 0, 0, dataRowsX , dataColsY);
}
void Nonogram::drawBlankGrid()//draw the grid for a new nonogram(data buttons disabled)
{
    gridLayout = new QGridLayout(gridWidget);
    gridLayout->setHorizontalSpacing(0);
    gridLayout->setVerticalSpacing(0);

    //create central buttons
    for(int i = 0; i < gridHeight; i++)
    {
        for(int j = 0; j < gridLength; j++)
        {
            button[i][j] = new GridButton(i, j,gridWidget);
            button[i][j]->setEnabled(true);
            gridLayout->addWidget(button[i][j], i + dataRowsX, j + dataColsY);
            connect(button[i][j], SIGNAL(rightClicked()), this, SLOT(rightButtonClicked()));
            connect(button[i][j], SIGNAL(leftClicked()), this, SLOT(leftButtonClicked()));
        }
    }
    //create vertical buttons
    for(int i = 0; i < gridHeight; i++)
        for(int j = 0; j < dataColsY; j++)
        {
            buttonY[i][j] = new DataButton(i,j,0,gridWidget);
            buttonY[i][j]->setColor(QColor("#BEBEBE"));
            buttonY[i][j]->disableButton();
            connect(buttonY[i][j], SIGNAL(rightClicked()), this, SLOT(rightDataButtonClicked()));
            connect(buttonY[i][j], SIGNAL(leftClicked()), this, SLOT(leftDataButtonClicked()));
            gridLayout->addWidget(buttonY[i][j],i + dataRowsX, j);
        }
    //create horizontal buttons
    for(int i = 0; i < dataRowsX; i++)
        for(int j = 0; j < dataColsX; j++)
        {
            buttonX[i][j] = new DataButton(i,j,1,gridWidget);
            buttonX[i][j]->setColor(QColor("#BEBEBE"));
            buttonX[i][j]->disableButton();
            connect(buttonX[i][j], SIGNAL(rightClicked()), this, SLOT(rightDataButtonClicked()));
            connect(buttonX[i][j], SIGNAL(leftClicked()), this, SLOT(leftDataButtonClicked()));
            gridLayout->addWidget(buttonX[i][j],i, j + dataColsY);
        }

    selectedColorIndicator = new QPushButton(gridWidget);
    selectedColorIndicator->setEnabled(false);
    QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(selectedColorIndicator->sizePolicy().hasHeightForWidth());
    selectedColorIndicator->setSizePolicy(sizePolicy);

    selectedColorIndicator->setStyleSheet(QString("background-color: %1;").arg(selectedColor->name()));
    gridLayout->addWidget(selectedColorIndicator, 0, 0, dataRowsX - 1, dataColsY - 1);

}

void Nonogram::rightButtonClicked()
{//if button is not marked with X, mark with X.
    QObject* clickedObject = sender();
    GridButton* clickedButton = qobject_cast<GridButton*>(clickedObject);
    int row = clickedButton->getRow();
    int col = clickedButton->getCol();
    markedColors[row][col] = QColor(Qt::white);
    if(getMarked(row,col) == 0)
    {
        clickedButton->addX(true);
        setMarked(row, col, 1);
    }
    else {
        clickedButton->addX(false);
        setMarked(row, col, 0);
    }
}
void Nonogram::leftButtonClicked()
{//color button with current selected color
    QObject* clickedObject = sender();
    GridButton* clickedButton = qobject_cast<GridButton*>(clickedObject);
    int row = clickedButton->getRow();
    int col = clickedButton->getCol();
    if (getMarked(row,col) == 1)//if marked with X unmark
    {
        clickedButton->addX(false);
        setMarked(row, col, 0);
    }
    clickedButton->setColor(*selectedColor);//apply selected color
    markedColors[row][col] = *selectedColor;
}
void Nonogram::rightDataButtonClicked()//marks with X data buttons
{
    QObject* clickedObject = sender();
    DataButton* clickedButton = qobject_cast<DataButton*>(clickedObject);
    int row = clickedButton->getRow();
    int col = clickedButton->getCol();
    if (clickedButton->getDirection() == 1)// horizontal data buttons
    {
        if(getMarkedData(1, row, col) == 0)
        {
            clickedButton->addX(true);
            setMarkedData(1, row, col, 1);
        }
        else {//if marked, unmark
            clickedButton->addX(false);
            setMarkedData(1, row, col, 0);
        }
    }
    else//vertical data buttons
    {
        if(getMarkedData(0, row, col) == 0)
        {
            clickedButton->addX(true);
            setMarkedData(0, row, col, 1);
        }
        else {
            clickedButton->addX(false);
            setMarkedData(0, row, col, 0);
        }
    }

}
void Nonogram::leftDataButtonClicked()
{
    QObject* clickedObject = sender();
    DataButton* clickedButton = qobject_cast<DataButton*>(clickedObject);
    *selectedColor = clickedButton->getColor();
    updateColorIndicator();
}

void Nonogram::colorButtonClicked()
{
    QObject* clickedObject = sender();
    ColorSelectionButton* clickedButton = qobject_cast<ColorSelectionButton*>(clickedObject);
    *selectedColor = clickedButton->getButtonColor();
    updateColorIndicator();
}
void Nonogram::updateColorIndicator()
{
    selectedColorIndicator->setStyleSheet(QString("background-color: %1;").arg(selectedColor->name()));
}

void Nonogram::createColorSelectionBar()
{
    colorButtonsWidget = new QWidget(this);
    QHBoxLayout* colorButtonsLayout = new QHBoxLayout(colorButtonsWidget);
    colorList = picture->getColorsVector();//read all used colors from nonogram data
    int numOfColors = colorList.size();
    colorButton.resize(numOfColors);
    for(int i = 0; i < numOfColors;  i++)//for each color in list create button
    {
        colorButton[i] = new ColorSelectionButton(i, colorButtonsWidget);
        colorButtonsLayout->addWidget(colorButton[i]);
        colorButton[i]->setButtonColor(colorList[i]);
        connect(colorButton[i], SIGNAL(clicked()),this, SLOT(colorButtonClicked()));
    }
}
void Nonogram::createColorSelectionBarBlank()
{//creates blank color selection buttons, color-picker buttons for new nonograms
    colorButtonsWidget = new QWidget(this);
    QGridLayout* colorButtonsLayout = new QGridLayout(colorButtonsWidget);

    QIcon colorIcon(":/new/prefix1/images/color-picker-icon.jpg");
    int numOfColors = colorList.size();
    colorButton.resize(numOfColors);
    selectColorButton.resize(numOfColors);//color-picker buttons
    for(int i = 0; i < numOfColors;  i++)
    {
        selectColorButton[i] = new ColorSelectionButton(i,colorButtonsWidget);
        selectColorButton[i]->setStyleSheet(QString(
                                                "background-color: white;border-width: 0;height:30px;line-height:30px;width:30px;"));
        selectColorButton[i]->setIcon(colorIcon);
        selectColorButton[i]->setIconSize(selectColorButton[i]->size());
        connect(selectColorButton[i], SIGNAL(clicked()), this, SLOT(pickColor()));//connect to color-picker dialog
        colorButtonsLayout->addWidget(selectColorButton[i],2,i);

        colorButton[i] = new ColorSelectionButton(i, colorButtonsWidget);
        colorButton[i]->setButtonColor(colorList[i]);
        connect(colorButton[i], SIGNAL(clicked()),this, SLOT(colorButtonClicked()));
        colorButtonsLayout->addWidget(colorButton[i],1,i);
    }
    colorDialog = new QColorDialog(colorButtonsWidget);

}
void Nonogram::createToolbar()
{//create the toolbar at the top, consisting of "clear", "verify" and "show answer" buttons
    toolbarWidget = new QWidget(this);
    QHBoxLayout* toolbarLayout = new QHBoxLayout(toolbarWidget);
    QPushButton* restartButton = new QPushButton(QString("Clear"),toolbarWidget);
    restartButton->setStyleSheet(QString("background-color: #BEBEBE;"));
    connect(restartButton, SIGNAL(clicked()),this, SLOT(resetPuzzle()));
    verifyButton = new QPushButton(QString("Verify"),toolbarWidget);
    verifyButton->setStyleSheet(QString("background-color: #BEBEBE;"));
    connect(verifyButton, SIGNAL(clicked()),this,SLOT(verify()));
    QPushButton* answerButton = new QPushButton(QString("Show Solution"),toolbarWidget);
    answerButton->setStyleSheet(QString("background-color: #BEBEBE;"));
    connect(answerButton, SIGNAL(clicked()), this, SLOT(answer()));

    toolbarLayout->addWidget(restartButton);
    toolbarLayout->addWidget(verifyButton);
    toolbarLayout->addWidget(answerButton);
}
void Nonogram::createToolbarBlank()
{//toolbar for blank nonograms. Consists of "clear" and "check solvability" buttons
    toolbarWidget = new QWidget(this);
    QHBoxLayout* toolbarLayout = new QHBoxLayout(toolbarWidget);
    QPushButton* restartButton = new QPushButton(QString("Clear"),toolbarWidget);
    restartButton->setStyleSheet(QString("background-color: #BEBEBE;"));
    restartButton->setFixedSize(90, 26);

    connect(restartButton, SIGNAL(clicked()),this, SLOT(resetPuzzle()));

    solvableButton = new QPushButton(QString("Check Solvability"),toolbarWidget);
    solvableButton->setStyleSheet(QString("background-color: #BEBEBE;"));
    solvableButton->setFixedSize(90, 26);
    connect(solvableButton, SIGNAL(clicked()),this, SLOT(solve()));

    toolbarLayout->addWidget(restartButton);
    toolbarLayout->addWidget(solvableButton);
}
bool Nonogram::eventFilter(QObject *object, QEvent *e)
{
    QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(e);

    GridButton *button = (GridButton*) qApp->widgetAt(QCursor::pos());

    if (e->type() == QEvent::MouseButtonPress && QApplication::mouseButtons()==Qt::LeftButton && !leftButtonPressedFlag)
    {
        leftButtonPressedFlag=true;
        button->mousePressEventLeft(mouseEvent);
    }
    if (e->type() == QEvent::MouseButtonPress && QApplication::mouseButtons()==Qt::RightButton && !rightButtonPressedFlag)
    {
        rightButtonPressedFlag=true;
        button->mousePressEventRight(mouseEvent);
        mouseRow = button->getRow();
        mouseCol = button->getCol();
    }
    if (e->type() == QEvent::MouseButtonRelease)
    {
        leftButtonPressedFlag=false;
        rightButtonPressedFlag=false;
    }
    if (e->type() == QEvent::MouseMove && button->accessibleName() == QString("GridButton"))
    {
        if(rightButtonPressedFlag)
        {
            if (!(mouseRow == button->getRow() && mouseCol == button->getCol()))
            {
                button->mousePressEventRight(mouseEvent);
                mouseRow = button->getRow();
                mouseCol = button->getCol();
            }
        }
        if(leftButtonPressedFlag)
        {
            button->mousePressEventLeft(mouseEvent);
        }
    }
    return false;
}

void Nonogram::verify()//verify solution. If solved correctly turn the button green, otherwise - red.
{
    if(picture->verifyPuzzle(markedColors))
        verifyButton->setStyleSheet(QString("background-color: green;"));
    else
        verifyButton->setStyleSheet(QString("background-color: red;"));

}
void Nonogram::answer()//shows correct solution
{
    markedColors = picture->getCorrectColors();

    for (int i = 0; i < gridHeight; i++)
    {
        marked[i].fill(0);
        for(int j = 0; j < gridLength; j++)
        {
            button[i][j]->addX(false);
            button[i][j]->setColor(markedColors[i][j]);
        }
    }
}
void Nonogram::pickColor()//opens color-picker dialog
{
    QObject* clickedObject = sender();
    ColorSelectionButton* clickedButton = qobject_cast<ColorSelectionButton*>(clickedObject);
    QColor clr = clickedButton->getButtonColor();

    int index = clickedButton->getIndex();
    QColor myColor = QColorDialog::getColor(colorButton[index]->getButtonColor(), colorButtonsWidget);
    if(myColor.isValid())
    {
        *selectedColor = myColor;
        updateColorIndicator();
        colorButton[index]->setButtonColor(*selectedColor);
    }
}

void Nonogram::savePicture(const QString str) const// save created nonogram
{
    QFile file(str);
    if (!file.open(QIODevice::WriteOnly)) {
        return;
    }

    QImage im(QSize(gridLength, gridHeight),QImage::Format_RGB32);
    QRgb colorValue;
    for(int i = 0; i < gridHeight; i++)
        for(int j = 0; j < gridLength; j++)
        {
            colorValue = markedColors[i][j].rgb();
            im.setPixel(j,i,colorValue);
        }

    im.save(&file, "XPM");
    file.close();
}

void Nonogram::saveProgress(const QString str) const //save progress of nonogram
{
    QFile file(QString(str.left(str.length()-4) + ".ppm"));
    if (!file.open(QIODevice::WriteOnly)) {
        return;
    }

    QImage im(QSize(gridLength,gridHeight),QImage::Format_RGB32);
    QRgb colorValue;
    for(int i = 0; i < gridHeight; i++)
        for(int j = 0; j < gridLength; j++)
        {

            if(getMarked(i,j) == 1)
            {
                colorValue = QColor(1,1,1).rgb();
            }
            else
            {
                colorValue = markedColors[i][j].rgb();
            }
            im.setPixel(j,i,colorValue);
        }

    im.save(&file, "PPM");
    file.close();
}
void Nonogram::loadNonogram(const QString str)//load saved progress
{
    QImage im;
    QColor clr;
    im.load(str,"PPM");

    for(int i = 0; i < gridHeight; i++)
        for(int j = 0; j < gridLength; j++)
        {

            clr = QColor(im.pixel(j,i));

            if(clr == QColor(1,1,1))//this color stands for marked with X
            {
                setMarked(i,j,1);
                button[i][j]->addX(true);
                markedColors[i][j] = QColor(Qt::white);
            }
            else
            {
                markedColors[i][j] = clr;
                button[i][j]->setColor(clr);
            }
        }
}
void Nonogram::solve()// check solvability of created nonogram
{
    QImage im(gridLength, gridHeight, QImage::Format_RGB32);
    for(int i = 0; i < gridHeight; i++)
        for(int j = 0; j < gridLength; j++)
            im.setPixel(j,i,markedColors[i][j].rgb());

    Solver mySolver(im);

    if(mySolver.isSolvable())
        solvableButton->setStyleSheet(QString("background-color: green;"));
    else
        solvableButton->setStyleSheet(QString("background-color: red;"));
}
void Nonogram::setMarked(const int ind1, const int ind2, const int value)
{
    if (ind1 < gridHeight && ind2 < gridLength)
        marked[ind1][ind2] = value;
}

void Nonogram::setMarkedData(const int dir, const int ind1, const int ind2, const int value)
{
    if (dir == 1)// horizontal data buttons
    {
        if (ind2 < dataColsX && ind1 < dataRowsX)
            markedX[ind1][ind2] = value;

    }
    else//vertical data buttons
    {
        if (ind1 < dataRowsY && ind2 < dataColsY)
            markedY[ind1][ind2] = value;
    }
}
int Nonogram::getMarkedData(const int direction, const int index1, const int index2)
{
    if (direction == 1)// horizontal data buttons
    {
       if (index2 < dataColsX && index1 < dataRowsX)
            return markedX[index1][index2];
    }
    else//vertical data buttons
    {
        if (index1 < dataRowsY && index2 < dataColsY)
            return markedY[index1][index2];
    }
    return -1;
}
int Nonogram::getMarked(const int index1, const int index2)const
{
    if (index1 < gridHeight && index2 < gridLength)
        return marked[index1][index2];
    return -1;
}

