#include <QDebug>

#include "gridsquare.h"

GridSquare::GridSquare(QString grid_square)
{
    m_grid_square = grid_square.toUpper();
}

GridSquare::~GridSquare()
{
}

QPoint GridSquare::calcSquareCoordinates(void)
{
    float x = 0, y = 0, width = 0, height = 0;

    if (m_grid_square.length() >= 2)
    {
        width = MAP_WIDTH / FIELDS_COUNT;
        height = MAP_HEIGHT / FIELDS_COUNT;
        x = (m_grid_square[0].unicode() - 'A') * width;
        y = ('R' - m_grid_square[1].unicode()) * height;
    }

    if (m_grid_square.length() >= 4)
    {
        width = MAP_WIDTH / (FIELDS_COUNT *  SQUERE_COUNT);
        height = MAP_HEIGHT / (FIELDS_COUNT * SQUERE_COUNT);
        x += (m_grid_square[2].digitValue()) * width;
        y += (9 - m_grid_square[3].digitValue()) * height;
    }

    if (m_grid_square.length() == 4)
    {
        x += width / 2.0f;
        y += height / 2.0f;
    }

    else if (m_grid_square.length() >= 6)
    {
        width = MAP_WIDTH / (FIELDS_COUNT *  SQUERE_COUNT * SUBSQUARE_COUNT);
        height = MAP_HEIGHT / (FIELDS_COUNT * SQUERE_COUNT * SUBSQUARE_COUNT);
        x += (m_grid_square[4].unicode() - 'A') * width;
        y += ('X' - m_grid_square[5].unicode()) * height;
    }

    return QPoint((int)x, (int)y);
}
