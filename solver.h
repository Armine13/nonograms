#ifndef SOLVER_H
#define SOLVER_H
#include <QVector>
#include <QColor>
#include <QString>
#include "nonogram.h"
#include "picturedata.h"
#include <QSet>
class Solver
{
public:
    Solver(const QImage &image);
    bool isSolvable() const;

private:
    void findPositionsRow(const int row);
    void findPositionsColumn(const int col);
    void solve();
    void processBlock(const int direction, const int pos, const int index);
    bool isSolvedLine(const int direction,const int pos);

    QVector<QVector<QColor> > markedC;// marked colors
    QVector<QVector<bool> > marked;
    QVector<QVector<int> > dataX;
    QVector<QVector<int> > dataY;
    QVector<QVector<QColor> > colorDataX;
    QVector<QVector<QColor> > colorDataY;
    int h;
    int w;
    int w0;
    int h0;
    QVector< QVector<QSet<QString> > >  possibleColors;
    QVector<QVector<QVector<int> > > blockPositionsY;
    QVector<QVector<QVector<int> > > blockPositionsX;
    QVector<bool> rowFound;
    QVector<bool> colFound;
    QStack<QPair<int,int> > tasks;//direction, position
};

#endif // SOLVER_H
