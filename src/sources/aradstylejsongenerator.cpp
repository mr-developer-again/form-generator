#include <headers/aradstylejsongenerator.h>

#include <QFile>

#include <stdexcept>

void Arad::JsonGeneratorNP::AradStyleJsonGenerator::operator()(QVector<QMap<QString, QString>> const& container,
                                                               QVector<QString> const& orderedKeys,
                                                               QString const& filePath)
{
    QFile file(filePath);
    if (!file.open(QFile::WriteOnly))
        throw std::runtime_error("couldn't open the file for writing the json information");

    bool anyOrder = orderedKeys.empty() ? true : false;
    QJsonObject *objectRecord;
    QVector<QJsonObject*> objects;
    if (anyOrder)
    {
        objectRecord = new QJsonObject;
        for (auto const& key : orderedKeys)
        {
            objectRecord->insert(key, container[]);
        }
    }
    else
    {
        ;
    }
}
