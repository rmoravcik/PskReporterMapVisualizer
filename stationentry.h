#ifndef STATIONENTRY_H
#define STATIONENTRY_H

#include <QWidget>
#include <QString>
#include <QPoint>

#include "adifparser.h"

class StationEntry
{
public:
    typedef enum {
        MY_STATION = 0,
        SWL_STATION,
        QSO_STATION
    } StationType;

    typedef enum {
        BAND_UNKNOWN = 0,
        BAND_160M,
        BAND_80M,
        BAND_60M,
        BAND_40M,
        BAND_30M,
        BAND_20M,
        BAND_17M,
        BAND_15M,
        BAND_12M,
        BAND_10M,
        BAND_2M,
    } BandType;

    StationEntry(StationType type, AdifEntry entry);
    StationEntry(StationType type, QString callsign, QString grid_square, unsigned int distance);
    virtual ~StationEntry();

    StationType getType();
    QString getCallsign();
    QString getGridLocator();
    float getDistance();
    QPoint getGridSquarePos();
    BandType getBand();
    QString getBandString();
    QString getMode();

private:
    StationType m_type;
    QString m_callsign;
    QString m_grid_square;
    QPoint m_grid_square_pos;
    float m_distance;
    BandType m_band;
    QString m_mode;

    BandType getBandFromFreq(float freq);
};

#endif // STATIONENTRY_H
