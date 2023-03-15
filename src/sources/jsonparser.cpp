#include <headers/jsonparser.h>

#include <stdexcept>
#include <QFile>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonArray>
#include <QJsonObject>

Arad::Parser::JsonParser::JsonParser(QString const& filePath, QVector<QString> const& validKeys)
    : _filePath(filePath),
      _validKeys(validKeys)
{ /* constructor body */ }

bool Arad::Parser::JsonParser::fileCanBeOpened(QString const& filePath) noexcept
{
    QFile file(filePath);

    if (!file.open(QIODevice::ReadOnly))
        return false;

    file.close();
    return true;
}

void Arad::Parser::JsonParser::setFilePath(QString const& filePath)
{ this->_filePath = filePath; }

QString Arad::Parser::JsonParser::getFilePath() const
{ return this->_filePath; }

void Arad::Parser::JsonParser::setValidKeys(QVector<QString> const& validKeys)
{ this->_validKeys = validKeys; }

QVector<QString> Arad::Parser::JsonParser::getValidKeys() const
{ return this->_validKeys; }
