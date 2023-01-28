#ifndef ARAD_PARSER_JSONPARSER_JSONPARSER_H
#define ARAD_PARSER_JSONPARSER_JSONPARSER_H

#include <QStringList>
#include <QString>
#include <QVector>
#include <QMap>

namespace Arad
{

    namespace Parser
    {

        class JsonParser
        {
        public:
            JsonParser(QString const& filePath, QStringList const& validKeys);
            virtual ~JsonParser() = default;

            virtual QVector<QMap<QString, QString>> parseJson() = 0;

        protected:
            void setFilePath(QString const& filePath);
            QString getFilePath() const;

            void setValidKeys(QStringList const& validKeys);
            QStringList getValidKeys() const;

            bool fileCanBeOpened(QString const& filePath) const;

        private:
            QString _filePath;
            QStringList _validKeys;

        };

    } // Parser namespace

} // Arad namespace

#endif // ARAD_PARSER_JSONPARSER_JSONPARSER_H
