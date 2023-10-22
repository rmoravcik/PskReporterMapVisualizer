#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QCheckBox>
#include <QLabel>
#include <QListWidget>
#include <QMainWindow>
#include <QPainter>
#include <QMouseEvent>
#include <QSettings>
#include <QPainter>

#include "stationentry.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    virtual void paintEvent(QPaintEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);

private:
    Ui::MainWindow *m_ui;

    QLabel *m_StationEntryCallsignLabel;
    QLabel *m_StationEntryGridSquareLabel;
    QLabel *m_StationEntryDistanceLabel;
    QLabel *m_StationEntryModeLabel;

    QList<StationEntry*> m_stations;
    QSettings *m_settings;

    QCheckBox *m_showQsoStationsCheckBox;
    QCheckBox *m_showSwlStationsCheckBox;

    QListWidget *m_bandFilterListWidget;

    QString m_myCallsign;
    QString m_myGridSquare;
    bool m_showSwlStations;
    bool m_showQsoStations;

    void loadSettings();
    void saveSettings();

    void updateBandFilterList();

    void drawStation(QPainter *painter, StationEntry* station);

private slots:
    void showQsoStationsCheckBoxStateChanged(int state);
    void showSwlStationsCheckBoxStateChanged(int state);
};
#endif // MAINWINDOW_H
