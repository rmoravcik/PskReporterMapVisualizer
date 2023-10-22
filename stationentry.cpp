#include <QDebug>

#include "gridsquare.h"
#include "stationentry.h"

StationEntry::StationEntry(StationType type, AdifEntry entry)
{
    m_type = type;
    if (m_type == SWL_STATION)
    {
        m_callsign = entry["OPERATOR"].toUpper();
        m_grid_square = entry["MY_GRIDSQUARE"].toUpper();
        m_distance = entry["DISTANCE"].toFloat();
        m_band = getBandFromFreq(entry["FREQ"].toFloat());
        m_mode = entry["MODE"].toUpper();
    }
    else
    {
        m_callsign = entry["CALL"].toUpper();
        m_grid_square = entry["GRIDSQUARE"].toUpper();
        m_distance = entry["DISTANCE"].toFloat();
        m_band = getBandFromFreq(entry["FREQ"].toFloat());
        m_mode = entry["MODE"].toUpper();

        if (m_distance == 0.0)
        {
            // Try to extract distance from comment of FT8CN application log
            if (entry["COMMENT"].contains("DISTANCE: "))
            {
                QString tmp = entry["COMMENT"].split(':')[1];
                m_distance = tmp.split("KM")[0].toFloat();
            }
        }
    }

    GridSquare gs(m_grid_square);
    m_grid_square_pos = gs.calcSquareCoordinates();
}
StationEntry::StationEntry(StationType type, QString callsign, QString grid_square, unsigned int distance)
{
    m_type = type;
    m_callsign = callsign;
    m_grid_square = grid_square;
    m_distance = distance;
    m_band = BAND_UNKNOWN;
    m_mode = "UNKNOWN";

    GridSquare gs(m_grid_square);
    m_grid_square_pos = gs.calcSquareCoordinates();
}

StationEntry::~StationEntry()
{
}

StationEntry::StationType StationEntry::getType()
{
    return m_type;
}

QString StationEntry::getCallsign()
{
    return m_callsign;
}

QString StationEntry::getGridLocator()
{
    return m_grid_square;
}

float StationEntry::getDistance()
{
   return m_distance;
}

QPoint StationEntry::getGridSquarePos()
{
    return m_grid_square_pos;
}

StationEntry::BandType StationEntry::getBand()
{
    return m_band;
}

StationEntry::BandType StationEntry::getBandFromFreq(float freq)
{
    if ((freq >= 1.8) && (freq <= 2.0))
    {
        return BAND_160M;
    }
    else if ((freq >= 3.5) && (freq <= 4.0))
    {
        return BAND_80M;
    }
    else if ((freq >= 5.0) && (freq <= 6.0))
    {
        return BAND_60M;
    }
    else if ((freq >= 7.0) && (freq <= 7.3))
    {
        return BAND_40M;
    }
    else if ((freq >= 10.1) && (freq <= 10.15))
    {
        return BAND_30M;
    }
    else if ((freq >= 14.0) && (freq <= 14.35))
    {
        return BAND_20M;
    }
    else if ((freq >= 18.068) && (freq <= 18.168))
    {
        return BAND_17M;
    }
    else if ((freq >= 21.0) && (freq <= 21.45))
    {
        return BAND_15M;
    }
    else if ((freq >= 24.89) && (freq <= 24.99))
    {
        return BAND_12M;
    }
    else if ((freq >= 28.0) && (freq <= 29.7))
    {
        return BAND_10M;
    }
    else if ((freq >= 144) && (freq <= 148))
    {
        return BAND_2M;
    }
    else if (freq == 0.0)
    {
        return BAND_UNKNOWN;
    }
    else
    {
        qDebug() << "Unsupported frequency" << freq << "MHz";
    }

    return BAND_UNKNOWN;
}

QString StationEntry::getBandString()
{
    switch (m_band)
    {
        case BAND_160M: return "160m";
        case BAND_80M:  return " 80m";
        case BAND_60M:  return " 60m";
        case BAND_40M:  return " 40m";
        case BAND_30M:  return " 30m";
        case BAND_20M:  return " 20m";
        case BAND_17M:  return " 17m";
        case BAND_15M:  return " 15m";
        case BAND_12M:  return " 12m";
        case BAND_10M:  return " 10m";
        case BAND_2M:   return "  2m";
        default:        return "Unknown";
    }
}

QString StationEntry::getMode()
{
    return m_mode;
}
