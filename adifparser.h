#ifndef ADIFPARSER_H
#define ADIFPARSER_H

#include <QString>
#include <QList>
#include <QMap>

typedef QMap<QString, QString> AdifEntry;

class AdifParser
{
public:
    AdifParser(QString filename);

    void parse();
    int getCount();
    QList<AdifEntry> getEntries();

private:
    QString m_filename;
    QList<AdifEntry> m_list;
};

#endif // ADIFPARSER_H
