#include <headers/aradstylejsongenerator.h>

#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QTextStream>
#include <QStringList>

#include <stdexcept>
#include <memory>
#include <functional>

void Arad::JsonGeneratorNP::AradStyleJsonGenerator::operator()(QVector<QHash<QString, QString>> const& container,
                                                               QString const& filePath,
                                                               QVector<QString> const& orderedKeys)
{
    std::unique_ptr<QFile, std::function<void(QFile*)>> file(new QFile(filePath), [](QFile* file) -> void {
        file->close();
    });
    if (!file->open(QFile::WriteOnly))
        throw std::runtime_error("couldn't open the file for writing the json information");

    bool anyOrder = orderedKeys.empty() ? true : false;
    std::unique_ptr<QJsonObject> objectRecord;
    std::unique_ptr<QJsonArray> arrayRecord = std::make_unique<QJsonArray>();
    bool stringListPermitted = true;
    if (!anyOrder)
    {
        size_t index = 0;
        while (1)
        {
            objectRecord = std::make_unique<QJsonObject>();
            for (auto const& key : orderedKeys)
            {
                if (container[index][key] == "")
                    continue;

                if ("string list" == container[index]["type"])
                {
                    QString value = container[index]["string list values"];
                    QStringList listOfValues = value.split(' ', Qt::KeepEmptyParts);

                    QJsonArray tempArray = QJsonArray::fromStringList(listOfValues);
                    objectRecord->insert(key, tempArray);

                    stringListPermitted = false;

                    continue;
                }
                else if ("string list values" != key)
                    objectRecord->insert(key, container[index][key]);
            }
            arrayRecord->push_back(std::move(*objectRecord));
            stringListPermitted = true;

            ++index;
            if (container.size() == index)
                break;
        }
    }
    else // if (anyOrder)
    {
        for (auto const& innerMap : container)
        {
            QStringList listOfKeys = innerMap.keys();
            objectRecord = std::make_unique<QJsonObject>();
            for (auto const& key : listOfKeys)
            {
                if ("string list" == innerMap["type"] and
                        stringListPermitted and
                        "string list values" == key)
                {
                    QString value = innerMap["string list values"];
                    QStringList listOfValues = value.split(' ', Qt::KeepEmptyParts);

                    QJsonArray tempArray = QJsonArray::fromStringList(listOfValues);
                    objectRecord->insert(key, tempArray);

                    stringListPermitted = false;
                }
                else if ("string list values" != key)
                    objectRecord->insert(key, innerMap[key]);
            }
            arrayRecord->push_back(std::move(*objectRecord));
            stringListPermitted = true;
        }
    }

    std::unique_ptr<QJsonDocument> jsonDocument = std::make_unique<QJsonDocument>();
    jsonDocument->setArray(std::move(*arrayRecord));

    QTextStream out(file.get());
    out << jsonDocument->toJson();
}
