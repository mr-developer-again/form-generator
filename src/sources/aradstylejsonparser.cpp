#include <headers/aradstylejsonparser.h>

#include <QFile>
#include <QByteArray>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonArray>
#include <QJsonObject>

Arad::Parser::AradStyleJsonParser::AradStyleJsonParser(QString const& filePath, QStringList const& validKeys)
    : Arad::Parser::JsonParser(filePath, validKeys)
{
    if (!this->fileCanBeOpened(filePath))
        throw std::runtime_error("the file couldn't be open");
}

QVector<QMap<QString, QString>> Arad::Parser::AradStyleJsonParser::parseJson()
{
    QString filePath = this->getFilePath();
    QStringList validKeys = this->getValidKeys();

    QMap<QString, QString> tempMap;
    QVector<QMap<QString, QString>> result;

    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly))
        throw std::runtime_error("couldn't open the file");

    QByteArray bytes = file.readAll();

    QJsonParseError jsonParseError;
    QJsonDocument document = QJsonDocument::fromJson(bytes, &jsonParseError);

    if (jsonParseError.error != QJsonParseError::NoError)
        throw std::runtime_error(jsonParseError.errorString().toStdString());

    if (document.isArray())
    {
        QJsonArray array = document.array();
        for (auto const& item : array)
        {
            if (item.isObject())
            {
                QJsonObject jsonObject = item.toObject();

                QStringList objectKeys = jsonObject.keys();

                for (auto const& key : objectKeys)
                {
                    if (validKeys.contains(key.toLower()))
                    {
                        QJsonValue value = jsonObject.take(key);

                        if (value.isArray())
                        {
                            QJsonArray valueArray = value.toArray();
                            QString resultValue = "";
                            for (auto const& valueFromArray : valueArray)
                            {
                                resultValue += (valueFromArray.toString() + " ");
//                                resultValue += ;
                            }

                            resultValue = resultValue.trimmed();
                            tempMap.insert(key, resultValue);
                        }
                        else
                            tempMap.insert(key, value.toString());
                    }
                }
            }

            QString error;
            if (!this->jsonIsCorrect(tempMap, error))
                throw std::runtime_error(error.toStdString());

            result.push_back(tempMap);
            tempMap.clear();
            result.shrink_to_fit();
        }
    }

    result.shrink_to_fit();
    return result;
}

void Arad::Parser::AradStyleJsonParser::extractedMapBalancer(QMap<QString, QString>& inputMap)
{
    QStringList validKeys = this->getValidKeys();

    for (auto const& validKey : validKeys)
    {
        if (!inputMap.contains(validKey))
            if (validKey == "readonly")
                inputMap.insert(validKey, "false");
    }
}

bool Arad::Parser::AradStyleJsonParser::jsonIsCorrect(QMap<QString, QString> const& inputMap, QString &error)
{
    if (inputMap["type"] == "string" or inputMap["type"] == "file")
    {
        if (!inputMap["default value"].isEmpty())
        {
            error = "you can't set default value for \"string\" and \"file\" types";
            return false;
        }
    }
    else /// if the type is other than "string" and "file"
    {
        if (!inputMap["description"].isEmpty())
        {
            error = "you can't set description for non-\"string/file\" types";
            return false;
        }
    }

    if (inputMap["name"] == "" or inputMap["type"] == "")
    {
        error = "you must set \"name\" and \"type\" keys and also fill value of them";
        return false;
    }

    return true;
}
