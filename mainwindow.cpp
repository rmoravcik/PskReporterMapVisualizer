#include <QCheckBox>
#include <QGroupBox>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "adifparser.h"
#include "gridsquare.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_ui(new Ui::MainWindow)
    , m_settings(new QSettings("Roman Moravcik", "PskReporterMapVisualizer", this))
{
    m_ui->setupUi(this);

    AdifParser adifSwlStations("OK8RM.adif");
    adifSwlStations.parse();

    QList<AdifEntry> entries = adifSwlStations.getEntries();
    for (QList<AdifEntry>::ConstIterator it = entries.constBegin(); it != entries.constEnd(); ++it) {
        const AdifEntry& entry = *it;
        m_stations.append(new StationEntry(StationEntry::SWL_STATION, entry));
    }

    AdifParser adifQsoStations("All_log.adi");
    adifQsoStations.parse();

    entries = adifQsoStations.getEntries();
    for (QList<AdifEntry>::ConstIterator it = entries.constBegin(); it != entries.constEnd(); ++it) {
        const AdifEntry& entry = *it;
        m_stations.append(new StationEntry(StationEntry::QSO_STATION, entry));
    }

    QHBoxLayout *mainWindowLayout = new QHBoxLayout(this);

    QWidget *mapWidget = new QWidget();
    mapWidget->setFixedWidth(MAP_WIDTH);
    mainWindowLayout->addWidget(mapWidget);

    QVBoxLayout *sidePanelLayout = new QVBoxLayout();
    sidePanelLayout->setSpacing(12);
    mainWindowLayout->addItem(sidePanelLayout);

    QGroupBox *displaySettings = new QGroupBox("Display settings");
    sidePanelLayout->addWidget(displaySettings);

    QVBoxLayout *displaySettingsLayout = new QVBoxLayout();
    displaySettings->setLayout(displaySettingsLayout);

    m_showQsoStationsCheckBox = new QCheckBox("Show QSO stations");
    displaySettingsLayout->insertWidget(0, m_showQsoStationsCheckBox);
    connect(m_showQsoStationsCheckBox, &QCheckBox::stateChanged, this, &MainWindow::showQsoStationsCheckBoxStateChanged);

    m_showSwlStationsCheckBox = new QCheckBox("Show SWL stations");
    displaySettingsLayout->insertWidget(1, m_showSwlStationsCheckBox);
    connect(m_showSwlStationsCheckBox, &QCheckBox::stateChanged, this, &MainWindow::showSwlStationsCheckBoxStateChanged);

    QGroupBox *bandFilter = new QGroupBox("Band");
    sidePanelLayout->addWidget(bandFilter);

    QVBoxLayout *bandFilterLayout = new QVBoxLayout();
    bandFilter->setLayout(bandFilterLayout);

    m_bandFilterListWidget = new QListWidget();
    m_bandFilterListWidget->setSelectionMode(QAbstractItemView::MultiSelection);
    m_bandFilterListWidget->setSortingEnabled(true);
    m_bandFilterListWidget->sortItems(Qt::DescendingOrder);
    bandFilterLayout->insertWidget(0, m_bandFilterListWidget);

    QGroupBox *stationEntryBox = new QGroupBox("Station");
    sidePanelLayout->addWidget(stationEntryBox);

    QGridLayout *stationEntryLayout = new QGridLayout(this);
    stationEntryLayout->setAlignment(Qt::AlignLeft);
    stationEntryLayout->setVerticalSpacing(12);
    stationEntryBox->setLayout(stationEntryLayout);

    QLabel *operatorLabel = new QLabel(this);
    operatorLabel->setText("Operator:");
    stationEntryLayout->addWidget(operatorLabel, 0, 0);

    m_StationEntryCallsignLabel = new QLabel(this);
    m_StationEntryCallsignLabel->setText("N/A");
    stationEntryLayout->addWidget(m_StationEntryCallsignLabel, 0, 1);

    QLabel *locatorLabel = new QLabel(this);
    locatorLabel->setText("Gridsquare:");
    stationEntryLayout->addWidget(locatorLabel, 1, 0);

    m_StationEntryGridSquareLabel = new QLabel(this);
    m_StationEntryGridSquareLabel->setText("N/A");
    stationEntryLayout->addWidget(m_StationEntryGridSquareLabel, 1, 1);

    QLabel *distanceLabel = new QLabel(this);
    distanceLabel->setText("Distance:");
    stationEntryLayout->addWidget(distanceLabel, 2, 0);

    m_StationEntryDistanceLabel = new QLabel(this);
    m_StationEntryDistanceLabel->setText("N/A");
    stationEntryLayout->addWidget(m_StationEntryDistanceLabel, 2, 1);

    QLabel *modeLabel = new QLabel(this);
    modeLabel->setText("Mode:");
    stationEntryLayout->addWidget(modeLabel, 3, 0);

    m_StationEntryModeLabel = new QLabel(this);
    m_StationEntryModeLabel->setText("N/A");
    stationEntryLayout->addWidget(m_StationEntryModeLabel, 3, 1);

    sidePanelLayout->addStretch(0);

    QPushButton *qso_log_button = new QPushButton("Import QSO Log");
    sidePanelLayout->addWidget(qso_log_button);

    QPushButton *pskreporter_log_button = new QPushButton("Import PskReporter Log");
    sidePanelLayout->addWidget(pskreporter_log_button);

    QWidget *window = new QWidget();
    window->setLayout(mainWindowLayout);

    setCentralWidget(window);

    loadSettings();

    updateBandFilterList();
}

MainWindow::~MainWindow()
{
    qDeleteAll(m_stations.begin(), m_stations.end());
    delete m_ui;
}

void MainWindow::loadSettings()
{
    m_myCallsign = m_settings->value("my_callsign", "OK8RM").toString();
    m_myGridSquare = m_settings->value("my_grid_locator", "JO70").toString();
    m_showQsoStationsCheckBox->setChecked(m_settings->value("show_qso_stations", true).toBool());
    m_showSwlStationsCheckBox->setChecked(m_settings->value("show_swl_stations", true).toBool());

    m_stations.append(new StationEntry(StationEntry::MY_STATION, m_myCallsign, m_myGridSquare, 0));
}

void MainWindow::saveSettings()
{
    m_settings->setValue("my_callsign", m_myCallsign);
    m_settings->setValue("my_grid_locator", m_myGridSquare);
    m_settings->setValue("show_qso_stations", m_showQsoStationsCheckBox->isChecked());
    m_settings->setValue("show_swl_stations", m_showSwlStationsCheckBox->isChecked());

    m_stations.append(new StationEntry(StationEntry::MY_STATION, m_myCallsign, m_myGridSquare, 0));
}

void MainWindow::updateBandFilterList()
{
    m_bandFilterListWidget->clear();

    for (QList<StationEntry*>::ConstIterator it = m_stations.constBegin(); it != m_stations.constEnd(); ++it) {
        StationEntry* station = *it;

        QList<QListWidgetItem *> tmp = m_bandFilterListWidget->findItems(station->getBandString(), Qt::MatchExactly);
        if (tmp.empty())
        {
            m_bandFilterListWidget->addItem(station->getBandString());
        }
    }
}

void MainWindow::showSwlStationsCheckBoxStateChanged(int state)
{
    if (state)
        m_showSwlStations = true;
    else
        m_showSwlStations = false;

    repaint();
}

void MainWindow::showQsoStationsCheckBoxStateChanged(int state)
{
    if (state)
        m_showQsoStations = true;
    else
        m_showQsoStations = false;

    repaint();
}

void MainWindow::drawStation(QPainter *painter, StationEntry* station)
{
    QBrush brush;
    QPen pen;

    brush.setStyle(Qt::SolidPattern);
    pen.setWidth(1);

    switch (station->getType())
    {
        case StationEntry::MY_STATION:
            brush.setColor(Qt::blue);
            pen.setColor(Qt::blue);
            break;
        case StationEntry::SWL_STATION:
            brush.setColor(Qt::red);
            pen.setColor(Qt::red);
            break;
        case StationEntry::QSO_STATION:
            brush.setColor(Qt::black);
            pen.setColor(Qt::black);
            break;
        default:
            break;
    }

    painter->setBrush(brush);
    painter->setPen(pen);
    painter->drawRect(QRect(station->getGridSquarePos(), QSize(2, 2)));
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    painter.drawPixmap(QPoint(0,0), QPixmap("world-map.jpg"));

    for (QList<StationEntry*>::ConstIterator it = m_stations.constBegin(); it != m_stations.constEnd(); ++it) {
        StationEntry* station = *it;

        if ((station->getType() == StationEntry::SWL_STATION) && (m_showSwlStations == false))
        {
            continue;
        }

        if ((station->getType() == StationEntry::QSO_STATION) && (m_showQsoStations == false))
        {
            continue;
        }

        drawStation(&painter, station);
    }
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    for (QList<StationEntry*>::ConstIterator it = m_stations.constBegin(); it != m_stations.constEnd(); ++it) {
        StationEntry* station = *it;
        if (QRect(station->getGridSquarePos(), QSize(2, 2)).contains(event->pos()))
        {
            m_StationEntryCallsignLabel->setText(station->getCallsign());
            m_StationEntryGridSquareLabel->setText(station->getGridLocator());
            m_StationEntryDistanceLabel->setText(QString::number(station->getDistance()) + " km");
            m_StationEntryModeLabel->setText(station->getMode());
            break;
        }
    }
}
