#ifndef GRIDSQUARE_H
#define GRIDSQUARE_H

#include <QPoint>
#include <QWidget>

#define MAP_WIDTH  1440.0f
#define MAP_HEIGHT 720.0f

#define FIELDS_COUNT    18
#define SQUERE_COUNT    10
#define SUBSQUARE_COUNT 24

class GridSquare
{
public:
    GridSquare(QString grid_square);
    virtual ~GridSquare();

    QPoint calcSquareCoordinates(void);

private:
    QString m_grid_square;
};

#endif // GRIDSQUARE_H
