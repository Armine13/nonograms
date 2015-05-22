#include "picturedata.h"
#include "solver.h"
#include "nonogram.h"

PictureData::PictureData(QString path)
{

    cols_x = 0;
    rows_x = 0;
    cols_y = 0;
    rows_y = 0;

    grid_rows = 0;
    grid_cols = 0;
    filePath = path;
    colorsUsed = QVector<QColor>();
    loadPictureData();
    scanColors();
}
void PictureData::loadPictureData()
{
    QImage image;
    image.load(filePath);


    width = image.size().width();
    height = image.size().height();

    correctColors.resize(height);
    for(int i = 0; i < height; i++)
    {
        correctColors[i].resize(width);
        for(int j = 0; j < width; j++)
            correctColors[i][j] = QColor(image.pixel(j,i));
    }

    dataY.resize(height);
    colorDataY.resize(height);
    dataX.resize(width);
    colorDataX.resize(width);
    int maxLength = 0;
    int count = 0;
    QVector<QColor> vector;
    QColor c;
    // Process rows

    for(int i = 0; i < height; i++)
    {
        vector = correctColors[i];
        c = vector[0];
        count = 0;

        while(!vector.isEmpty())
        {
            if(vector[0] == c)
            {
                count++;
                if(vector.size() == 1 && c != QColor(Qt::white))
                {
                    dataY[i].append(count);
                    colorDataY[i].append(c);
                }

            }
            else
            {
                if(c != QColor(Qt::white))
                {
                    dataY[i].append(count);
                    colorDataY[i].append(c);
                }
                c = vector[0];
                count = 1;
                if(vector.size() == 1 && c != QColor(Qt::white))
                {
                    dataY[i].append(count);
                    colorDataY[i].append(c);
                }
            }
            vector.removeFirst();

        }
      if (dataY[i].length() > maxLength)
            maxLength = dataY[i].size();

    }

    for(int i = 0; i < height; i++)
        while(dataY[i].length() < maxLength)
        {
            dataY[i].prepend(0);
            colorDataY[i].prepend(QColor(Qt::white));
        }

    maxLength = 0;
    for(int j = 0; j < width; j++)
    {
        vector.clear();
        count = 0;
        for(int i = 0; i < height; i++)
            vector.append(correctColors[i][j]);

        c = vector[0];

        while(!vector.isEmpty())
        {
            if(vector[0] == c)
            {
                count++;
                if(vector.size() == 1 && c != QColor(Qt::white))
                {
                    dataX[j].append(count);
                    colorDataX[j].append(c);
                }

            }
            else
            {
                if(c != QColor(Qt::white))
                {
                    dataX[j].append(count);
                    colorDataX[j].append(c);
                }
                c = vector[0];
                count = 1;
                if(vector.size() == 1 && c != QColor(Qt::white))
                {
                    dataX[j].append(count);
                    colorDataX[j].append(c);
                }
            }
            vector.removeFirst();

        }
        if (dataX[j].size() > maxLength)
            maxLength = dataX[j].size();
    }
    for(int j = 0; j < width; j++)
        while(dataX[j].length() < maxLength)
        {
            dataX[j].prepend(0);
            colorDataX[j].prepend(QColor(Qt::white));
        }

    cols_x = width;
    rows_x = dataX[0].size();
    cols_y = dataY[0].size();
    rows_y = height;
    grid_rows = height;
    grid_cols = width;

}

int PictureData::getGridHeight() const
{
    return height;
}
int PictureData::getGridLength() const
{
    return width;
}
int PictureData::getDataXRows() const
{
    return rows_x;
}
int PictureData::getDataXColumns() const
{
    return getGridLength();
}
int PictureData::getDataYRows() const
{
    return getGridHeight();
}
int PictureData::getDataYColumns() const
{
    return cols_y;
}
QColor PictureData::getColorX(const int row, const int col) const
{
    return colorDataX[col][row];
}
QColor PictureData::getColorY(const int row, const int col) const
{
    return colorDataY[row][col];
}
int PictureData::getDataX(int row, int col) const
{
    return dataX[col][row];
}
int PictureData::getDataY(const int row, const int col) const
{
    return dataY[row][col];
}
void PictureData::scanColors()
{
    for(int i = 0; i < rows_y; i++)
        for(int j = 0; j < cols_y; j++)
            if (!colorsUsed.contains(getColorY(i,j)))
            {
                colorsUsed.append(getColorY(i,j));
            }
    for(int i = 0; i < rows_x; i++)
        for(int j = 0; j < cols_x; j++)
            if (!colorsUsed.contains(getColorX(i,j)))
            {
                colorsUsed.append(getColorX(i,j));
            }
    if (!colorsUsed.contains(QColor(Qt::white)))
        colorsUsed.append(QColor(Qt::white));
}
QVector<QColor> PictureData::getColorsVector() const
{
    QVector<QColor> colorsList;
    colorsList = colorsUsed;
    return colorsList;
}

bool PictureData::verifyPuzzle(const QVector<QVector<QColor> > &markedColors) const
{
    int rows = getGridHeight();
    int cols = getGridLength();
    for(int i = 0; i < rows; i++)
        for(int j = 0; j < cols; j++)
            if (markedColors[i][j] != correctColors[i][j])
                return false;
    return true;

}
const QVector<QVector<QColor> > &PictureData::getCorrectColors() const
{
    return correctColors;
}
