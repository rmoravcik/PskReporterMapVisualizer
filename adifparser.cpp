#include <QFile>
#include <QMessageBox>

#include "adifparser.h"

AdifParser::AdifParser(QString filename)
{
    m_filename = filename;
}

int AdifParser::getCount()
{
    return m_list.count();
}

QList<AdifEntry> AdifParser::getEntries()
{
    return m_list;
}

void AdifParser::parse()
{
    QFile file(m_filename);

    if (!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::information(0, "error", file.errorString());
    }

    QTextStream in(&file);

    while(!in.atEnd())
    {
        QString line = in.readLine();
        if (line.startsWith('<'))
        {
            AdifEntry entry;
            QStringList fields = line.split("<");

            for (const auto& field : fields)
            {
                if ((field.length() == 0) || field.toUpper() == "EOR>")
                {
                    continue;
                }

                QStringList tmp = field.split(">");
                QString field_name = tmp[0].split(":")[0].toUpper();
                QString field_data = tmp[1].toUpper();

                entry[field_name] = field_data;
            }
            m_list.append(entry);
        }
    }

    file.close();
}
