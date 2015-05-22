#ifndef PICTUREDATA_H
#define PICTUREDATA_H
#include <QColor>
#include <QVector>
#include <QMap>
#include <QUrl>
#include <QPixmap>
#include "gridbutton.h"

class PictureData
{
public:
    PictureData(QString path);

    int getGridHeight() const;
    int getGridLength() const;
    int getDataXRows() const;
    int getDataXColumns() const;
    int getDataYRows() const;
    int getDataYColumns() const;
    int getDataX(const int, const int) const;
    int getDataY(const int,const  int) const;
    QColor getColorX(const int,const int) const;
    QColor getColorY(const int, const int) const;
    QVector<QColor> getColorsVector() const;
    bool verifyPuzzle(const QVector<QVector<QColor> > &) const;
    const QVector<QVector<QColor> >& getCorrectColors() const;


private:
    void loadPictureData();
    void scanColors();

    int cols_x;
    int rows_x;
    int cols_y;
    int rows_y;
    int height;
    int width;
    int grid_rows;
    int grid_cols;
    QVector<QVector<int> > dataX;
    QVector<QVector<int> > dataY;
    QVector<QVector<QColor> > colorDataX;
    QVector<QVector<QColor> > colorDataY;
    QVector<QVector<QColor> > correctColors;
    QVector<QColor> colorsUsed;
    QMap<QChar, QColor> colorSymbols;
    QVector<QString> rawImageData;
    QString filePath;
};

#endif // PICTUREDATA_H

