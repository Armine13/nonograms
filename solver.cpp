#include "solver.h"

Solver::Solver(const QImage& image)
{

    w = image.size().width();
    h = image.size().height();

    dataY.resize(h);
    colorDataY.resize(h);
    dataX.resize(w);
    colorDataX.resize(w);
    int maxLength = 0;
    int count = 0;
    QVector<QColor> vector;
    QColor c;
    QVector<QVector<QColor> > correctColors;

    correctColors.resize(h);
    for(int i = 0; i < h; i++)
    {
        correctColors[i].resize(w);
        for(int j = 0; j < w; j++)
            correctColors[i][j] = QColor(image.pixel(j,i));
    }

    for(int i = 0; i < h; i++)
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

    for(int i = 0; i < h; i++)
        while(dataY[i].length() < maxLength)
        {
            dataY[i].prepend(0);
            colorDataY[i].prepend(QColor(Qt::white));
        }

    maxLength = 0;
    for(int j = 0; j < w; j++)
    {
        vector.clear();
        count = 0;
        for(int i = 0; i < h; i++)
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
    for(int j = 0; j < w; j++)
        while(dataX[j].length() < maxLength)
        {
            dataX[j].prepend(0);
            colorDataX[j].prepend(QColor(Qt::white));
        }


    h0 = dataX[0].size();
    w0 = dataY[0].size();

    QSet<QString> set;

    possibleColors.resize(h);
    for(QVector<QVector<QSet<QString> > >::iterator it = possibleColors.begin(); it != possibleColors.end(); it++)
        it->resize(w);

    markedC.resize(h);
    marked.resize(h);

    for(int r = 0; r < h; r++)
    {
        markedC[r].resize(w);
        markedC[r].fill(QColor(Qt::white));
        marked[r].resize(w);
        marked[r].fill(false);
    }

    for(int r = 0; r < h; r++)
    {
        set.clear();
        for(int c = 0; c < w0; c++)
        {
            set.insert(colorDataY[r][c].name());
        }
        possibleColors[r].fill(set);
    }

    for(int r = 0; r < h; r++)
        for(int c = 0; c < w; c++)
        {
            set.clear();
            for(int k = 0; k < h0; k++)
                set.insert(colorDataX[c][k].name());

            possibleColors[r][c] = possibleColors[r][c].intersect(set);
            possibleColors[r][c].insert(QColor(Qt::white).name());
        }
    blockPositionsY.resize(h);
    for(int i = 0; i < h; i++)
    {
        blockPositionsY[i].resize(w0);
    }

    blockPositionsX.resize(w);
    for(int j = 0; j < w; j++)
    {
        blockPositionsX[j].resize(h0);
    }
    rowFound.resize(h);
    rowFound.fill(false);
    colFound.resize(w);
    colFound.fill(false);



    for(int i = 0; i < h; i++)
    {
        findPositionsRow(i);

    }
    for(int j = 0; j < w; j++)
    {
        findPositionsColumn(j);

    }

    solve();
}
void Solver::findPositionsRow(const int row)
{
    QColor color;
    int max = w-1;
    int pos = 0;
    bool empty = true;
    for(int c = 0; c < w0; c++)
        if ( dataY[row][c] > 0)
        {
            empty = false;
            if (colorDataY[row][c] != color)
                blockPositionsY[row][c].append(pos);
            else
            {
                pos++;
                blockPositionsY[row][c].append(pos);
            }
            color = colorDataY[row][c];
            pos += dataY[row][c];
        }

    if (empty) rowFound[row] = true;

    color = QColor();
    pos = max;
    for(int c = w0 - 1; c >= 0; c--)
        if ( dataY[row][c] > 0)
        {
            if (colorDataY[row][c] != color)
                blockPositionsY[row][c].append(pos);
            else
            {
                pos--;
                blockPositionsY[row][c].append(pos);
            }
            color = colorDataY[row][c];
            pos -= dataY[row][c];
        }

}
void Solver::findPositionsColumn(int col)
{
    QColor color;
    int max = h-1;
    int pos = 0;
    bool empty = true;
    for(int r = 0; r < h0; r++)
        if ( dataX[col][r] > 0)
        {
            empty = false;
            if (colorDataX[col][r] != color)
                blockPositionsX[col][r].append(pos);
            else
            {
                pos++;
                blockPositionsX[col][r].append(pos);
            }
            color = colorDataX[col][r];
            pos += dataX[col][r];
        }
    if (empty) colFound[col] = true;
    color = QColor();
    pos = max;
    for(int r = h0 - 1; r >= 0; r--)
        if ( dataX[col][r] > 0)
        {
            if (colorDataX[col][r] != color)
                blockPositionsX[col][r].append(pos);
            else
            {
                pos--;
                blockPositionsX[col][r].append(pos);
            }
            color = colorDataX[col][r];
            pos -= dataX[col][r];
        }



}
void Solver::solve()
{
    QPair<int, int> pair;
    QVector<bool> vectorH;
    vectorH.resize(w);
    QVector<bool> vectorV;
    vectorV.resize(h);
    int pos;
    int dir;

    for(int i = 0; i < h; i++)
        tasks.push(QPair<int,int>(0,i));
    for(int j = 0; j < w; j++)
        tasks.push(QPair<int,int>(1,j));


    while(!tasks.isEmpty())
    {
        pair = tasks.pop();
        dir = pair.first;
        pos = pair.second;

        //if line not solved check if solved
        if (dir == 0 && !rowFound[pos] && isSolvedLine(dir, pos))
        {
            rowFound[pos] = true;
            for(int i = 0; i < w; i++)
                if (!marked[pos][i])
                {

                    marked[pos][i] = true;
                    markedC[pos][i] = QColor(Qt::white);
                    tasks.push(QPair<int,int>(1, i));
                }
            continue;
        }
        if (dir == 1 && !colFound[pos] && isSolvedLine(dir, pos))
        {
            colFound[pos] = true;
            for(int i = 0; i < h; i++)
                if (!marked[i][pos])
                {

                    marked[i][pos] = true;
                    markedC[i][pos] = QColor(Qt::white);
                    tasks.push(QPair<int,int>(0, i));
                }
            continue;
        }

        if (dir == 0)
        {
            vectorH.fill(false);
            for(int j = 0; j < w0; j++)
                if (dataY[pos][j] > 0)
                {
                    processBlock(dir, pos, j);
                    for(int k = blockPositionsY[pos][j][0]; k <= blockPositionsY[pos][j][1]; k++)
                    {
                        vectorH[k] = true;
                    }
                }

            for(int k = 0; k < w; k++)
               if (!vectorH[k] && !(marked[pos][k]))
                {

                    marked[pos][k] = true;
                    markedC[pos][k] = QColor(Qt::white);
                    tasks.push(QPair<int,int>(1, k));
                }
        }
        else

        {
            vectorV.fill(false);
            for(int j = 0; j < h0; j++)
                if (dataX[pos][j] > 0)
                {
                    processBlock(dir, pos, j);
                    for(int k = blockPositionsX[pos][j][0]; k <= blockPositionsX[pos][j][1]; k++)
                    {
                        vectorV[k] = true;
                    }
                }

            for(int k = 0; k < h; k++)
                if (!vectorV[k] && !(marked[k][pos]))
                {
                    marked[k][pos] = true;
                    markedC[k][pos] = QColor(Qt::white);
                    tasks.push(QPair<int,int>(0, k));
                }
        }

    }
}
void Solver::processBlock(const int direction, const int pos, const int block_index)// solves a block in position pos
{

    int counter = 0;
    int len = 0;
    int p;
    if (direction == 0)
    {
        int row = pos;

        p = blockPositionsY[row][block_index][0];
        for(int i = blockPositionsY[row][block_index][0]; i <= blockPositionsY[row][block_index][1]; i++)
        {
            if (possibleColors[row][i].contains(colorDataY[row][block_index].name())
                    && (!marked[row][i] || markedC[row][i] == colorDataY[row][block_index]))
            {
                counter++;
            }
            else
            {
                if (counter < dataY[row][block_index])
                {
                    blockPositionsY[row][block_index][0] = i+1;

                    tasks.push(QPair<int,int>(0, row));
                    tasks.push(QPair<int,int>(1, i));


                    counter = 0;
                }
                else
                    break;
            }

        }

        if (block_index < w0-1)//block is not the last one in row
            if (blockPositionsY[row][block_index + 1][0] < blockPositionsY[row][block_index][0]+dataY[row][block_index])
            {
                for(int t = blockPositionsY[row][block_index + 1][0]; t > blockPositionsY[row][block_index][0]+dataY[row][block_index];t++)
                    tasks.push(QPair<int,int>(1, t));
                blockPositionsY[row][block_index + 1][0] = blockPositionsY[row][block_index][0]+dataY[row][block_index];
            }


        counter = 0;
        for(int i = blockPositionsY[row][block_index][1]; i >= blockPositionsY[row][block_index][0]; i--)
        {
            if (possibleColors[row][i].contains(colorDataY[row][block_index].name())
                    && (!marked[row][i] || markedC[row][i] == colorDataY[row][block_index]))
            {
                counter++;
            }
            else
            {
                if (counter < dataY[row][block_index])
                {
                    blockPositionsY[row][block_index][1] = i-1;

                    tasks.push(QPair<int,int>(1, i));
                    tasks.push(QPair<int,int>(0, row));

                    counter = 0;
                }
                else
                    break;
            }

        }

        if (block_index > 0 && dataY[row][block_index - 1] > 0)
            if (blockPositionsY[row][block_index - 1][1] > blockPositionsY[row][block_index][1]-dataY[row][block_index])
            {
                for(int t = blockPositionsY[row][block_index - 1][1]; t > blockPositionsY[row][block_index][1]-dataY[row][block_index];t--)
                {
                    tasks.push(QPair<int,int>(1, t));
                    tasks.push(QPair<int,int>(0, row));
                }
                blockPositionsY[row][block_index - 1][1] = blockPositionsY[row][block_index][1]-dataY[row][block_index];
            }


        len = blockPositionsY[row][block_index][1] - blockPositionsY[row][block_index][0] + 1;


        for(int i = blockPositionsY[row][block_index][0] + len - dataY[row][block_index];
            i < blockPositionsY[row][block_index][0] + dataY[row][block_index]; i++)
        {
            if (!marked[row][i])
            {
                marked[row][i] = true;
                markedC[row][i] = colorDataY[row][block_index];

                tasks.push(QPair<int,int>(0, row));
                tasks.push(QPair<int,int>(1, i));
            }
        }

        len = blockPositionsY[row][block_index][1] - blockPositionsY[row][block_index][0] + 1;

        int i = blockPositionsY[row][block_index][0] + len - dataY[row][block_index]-1;
        while (i >= 0 && marked[row][i] && markedC[row][i] == colorDataY[row][block_index])
        {
            i--;
            blockPositionsY[row][block_index][1] = i + dataY[row][block_index];
        }

        i = blockPositionsY[row][block_index][0] + dataY[row][block_index];
        while (i < w && marked[row][i] && markedC[row][i] == colorDataY[row][block_index])
        {
            i++;
            blockPositionsY[row][block_index][0] = i - dataY[row][block_index];
        }

        bool flag = true;
        for(int i = 0; i < w0; i++)
            if(colorDataY[row][i] == colorDataY[row][block_index] && i!= block_index)
                flag = false;
        if (flag)
        {
            int pos1 = -1, pos2 = -1;
            for(int i = blockPositionsY[row][block_index][0];
                i <= blockPositionsY[row][block_index][1]; i++)
            {
                if (marked[row][i] && markedC[row][i] == colorDataY[row][block_index])
                {
                    pos1 = i;
                    break;
                }

            }
            for(int i = blockPositionsY[row][block_index][1];
                i >= blockPositionsY[row][block_index][0]; i--)
            {
                if (marked[row][i] && markedC[row][i] == colorDataY[row][block_index])
                {
                    pos2 = i;
                    break;
                }
            }

            if(pos1 > -1)
            {
                for(int i = pos1; i <= pos2; i++)
                {
                    marked[row][i] = true;
                    markedC[row][i] = colorDataY[row][block_index];
                }
                if (blockPositionsY[row][block_index][0] < pos2 - dataY[row][block_index]+1
                        && pos2 - dataY[row][block_index]+1 > 0
                        && pos2 - dataY[row][block_index]+1 < w)
                    blockPositionsY[row][block_index][0] = pos2 - dataY[row][block_index]+1;

                if (blockPositionsY[row][block_index][1] > pos1 + dataY[row][block_index]-1
                        && pos1 + dataY[row][block_index]-1 > 0
                        && pos1 + dataY[row][block_index]-1 < h)
                    blockPositionsY[row][block_index][1] = pos1 + dataY[row][block_index]-1;

            }

        }

        len = blockPositionsY[row][block_index][1] - blockPositionsY[row][block_index][0] + 1;
        if (len == dataY[row][block_index])
        {
            if (block_index > 0 && colorDataY[row][block_index - 1] == colorDataY[row][block_index])
            {

                marked[row][blockPositionsY[row][block_index][0]-1] = true;
                markedC[row][blockPositionsY[row][block_index][0]-1] = QColor(Qt::white);

            }
            if (block_index < w0-1 && colorDataY[row][block_index + 1] == colorDataY[row][block_index])
            {
                marked[row][blockPositionsY[row][block_index][1]+1] = true;
                markedC[row][blockPositionsY[row][block_index][1]+1] = QColor(Qt::white);

            }
        }
    }
    else//horizontal direction

    {
        counter = 0;
        int col = pos;
        //limit block to cells that have color of block in the list of possible colors
        p = blockPositionsX[col][block_index][0];
        for(int i = blockPositionsX[col][block_index][0]; i <= blockPositionsX[col][block_index][1]; i++)
        {
            if (possibleColors[i][col].contains(colorDataX[col][block_index].name())
                    && (!marked[i][col] || markedC[i][col] == colorDataX[col][block_index]))
            {

                    counter++;

            }
            else
            {
                if (counter < dataX[col][block_index])
                {

                    blockPositionsX[col][block_index][0] = i+1;

                    tasks.push(QPair<int,int>(1, col));//add column to tasks, to be processed later
                    tasks.push(QPair<int,int>(0, i)); //add row to tasks, to be processed later


                    counter = 0;
                }
                else
                    break;
            }

        }
        //adjust edges of a block
        if (block_index < h0-1)
            if (blockPositionsX[col][block_index + 1][0] < blockPositionsX[col][block_index][0]+dataX[col][block_index])
            {
                for(int t = blockPositionsX[col][block_index + 1][0]; t > blockPositionsX[col][block_index][0]+dataX[col][block_index];t++)
                    tasks.push(QPair<int,int>(0, t));
                blockPositionsX[col][block_index + 1][0] = blockPositionsX[col][block_index][0]+dataX[col][block_index];
            }



        counter = 0;
        for(int i = blockPositionsX[col][block_index][1]; i >= blockPositionsX[col][block_index][0]; i--)
        {
            if (possibleColors[i][col].contains(colorDataX[col][block_index].name())
                    && (!marked[i][col] || markedC[i][col] == colorDataX[col][block_index]))
            {
                counter++;
            }
            else
            {
                if (counter < dataX[col][block_index])
                {
                    blockPositionsX[col][block_index][1] = i-1;
                    tasks.push(QPair<int,int>(0, i));
                    tasks.push(QPair<int,int>(1, col));

                    counter = 0;
                }
                else
                    break;
            }

        }
        if (block_index > 0 && dataX[col][block_index - 1] > 0)
            if (blockPositionsX[col][block_index - 1][1] > blockPositionsX[col][block_index][1]-dataX[col][block_index])
            {
                for(int t = blockPositionsX[col][block_index - 1][1]; t > blockPositionsX[col][block_index][1]-dataX[col][block_index];t--)
                {
                    tasks.push(QPair<int,int>(0, t));
                    tasks.push(QPair<int,int>(1, col));
                }
                blockPositionsX[col][block_index - 1][1] = blockPositionsX[col][block_index][1]-dataX[col][block_index];
            }



        // solve cells within block
        len = blockPositionsX[col][block_index][1] - blockPositionsX[col][block_index][0] + 1;

        for(int i = blockPositionsX[col][block_index][0] + len - dataX[col][block_index];
            i < blockPositionsX[col][block_index][0] + dataX[col][block_index]; i++)
        {
            if (!marked[i][col])
            {
                marked[i][col] = true;
                markedC[i][col] = colorDataX[col][block_index];
                tasks.push(QPair<int,int>(1, col));
                tasks.push(QPair<int,int>(0, i));
            }
        }



        bool flag = true;
        for(int i = 0; i < h0; i++)
            if(colorDataX[col][i] == colorDataX[col][block_index] && i!= block_index)
                flag = false;
        //adjust edges of block
        if (flag)
        {
            int pos1 = -1, pos2 = -1;
            for(int i = blockPositionsX[col][block_index][0];
                i <= blockPositionsX[col][block_index][1]; i++)
            {
                if (marked[i][col] && markedC[i][col] == colorDataX[col][block_index])
                {
                    pos1 = i;
                    break;
                }
            }
            for(int i = blockPositionsX[col][block_index][1];
                i >= blockPositionsX[col][block_index][0]; i--)
            {
                if (marked[i][col] && markedC[i][col] == colorDataX[col][block_index])
                {
                    pos2 = i;
                    break;
                }
            }
            if(pos1 > -1)
            {

                for(int i = pos1; i <= pos2; i++)
                {
                    marked[i][col] = true;
                    markedC[i][col] = colorDataX[col][block_index];
                }

                if (blockPositionsX[col][block_index][0] < pos2 - dataX[col][block_index]+1
                        && pos2 - dataX[col][block_index]+1 > 0
                        && pos2 - dataX[col][block_index]+1 < h)
                    blockPositionsX[col][block_index][0] = pos2 - dataX[col][block_index]+1;

                if (blockPositionsX[col][block_index][1] > pos1 + dataX[col][block_index]-1
                        && pos1 + dataX[col][block_index]-1 > 0
                        && pos1 + dataX[col][block_index]-1 < h)
                    blockPositionsX[col][block_index][1] = pos1 + dataX[col][block_index]-1;

            }

        }



        len = blockPositionsX[col][block_index][1] - blockPositionsX[col][block_index][0] + 1;

        int i = blockPositionsX[col][block_index][0] + len - dataX[col][block_index]-1;
        while (i >= 0 && marked[i][col] && markedC[i][col] == colorDataX[col][block_index])
        {
            i--;
            blockPositionsX[col][block_index][1] = i + dataX[col][block_index];
        }

        i = blockPositionsX[col][block_index][0] + dataX[col][block_index];
        while (i < h && marked[i][col] && markedC[i][col] == colorDataX[col][block_index])
        {
            i++;
            blockPositionsX[col][block_index][0] = i - dataX[col][block_index];
        }

        len = blockPositionsX[col][block_index][1] - blockPositionsX[col][block_index][0] + 1;
        if (len == dataX[col][block_index])
        {
            if (block_index > 0 && colorDataX[col][block_index - 1] == colorDataX[col][block_index])
            {
                marked[blockPositionsX[col][block_index][0]-1][col] = true;
                markedC[blockPositionsX[col][block_index][0]-1][col] = QColor(Qt::white);

            }
            if (block_index < h0-1 && colorDataX[col][block_index + 1] == colorDataX[col][block_index])
            {
                marked[blockPositionsX[col][block_index][1]+1][col] = true;
                markedC[blockPositionsX[col][block_index][1]+1][col] = QColor(Qt::white);

            }
        }
    }
}
bool Solver::isSolvedLine(const int direction, const int pos)
{//checks if the all blocks in a line or a column have been found.
    QVector<int> data;
    QVector<QColor> colorVector;
    QVector<QColor> vector;
    int count = 0;

    if (direction == 0)
        vector = markedC[pos];
    else
        for(int i = 0; i < h; i++)
            vector.append(markedC[i][pos]);
    QColor c = vector[0];

    while(!vector.isEmpty())
    {
        if(vector[0] == c)
        {
            count++;
            if(vector.size() == 1 && c != QColor(Qt::white))
            {
                data.append(count);
                colorVector.append(c);
            }

        }
        else
        {
            if(c != QColor(Qt::white))
            {
                data.append(count);
                colorVector.append(c);
            }
            c = vector[0];
            count = 1;
            if(vector.size() == 1 && c != QColor(Qt::white))
            {
                data.append(count);
                colorVector.append(c);
            }
        }
        vector.removeFirst();

    }

    QVector<int > data0;
    QVector<QColor> colorData0;
    if (direction == 0)
    {
        data0 = dataY[pos];
        colorData0 = colorDataY[pos];
    }
    else
    {
        data0 = dataX[pos];
        colorData0 = colorDataX[pos];
    }

    while (data0[0] == 0)
    {
        data0.removeFirst();
        colorData0.removeFirst();
    }


    if (colorData0 == colorVector && data0 == data)
        return true;
    else
        return false;
}

bool Solver::isSolvable() const
{
    for(int i = 0; i < h; i++)
        for(int j = 0; j < w; j++)
             if (!marked[i][j])
                 return false;
    return true;
}
