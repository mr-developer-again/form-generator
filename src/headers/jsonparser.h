#ifndef ARAD_PARSER_JSONPARSER_JSONPARSER_H
#define ARAD_PARSER_JSONPARSER_JSONPARSER_H

#include <QVector>
#include <QHash>

namespace Arad
{

    namespace Parser
    {

        class JsonParser
        {
        public:
            JsonParser(QString const& filePath, QVector<QString> const& validKeys);
            virtual ~JsonParser() = default;

            virtual QVector<QHash<QString, QString>> parseJson() = 0;

            static bool fileCanBeOpened(QString const& filePath) noexcept;

            void setValidKeys(QVector<QString> const& validKeys);
            QVector<QString> getValidKeys() const;

        protected:
            void setFilePath(QString const& filePath);
            QString getFilePath() const;

        private:
            QString _filePath;
            QVector<QString> _validKeys;

        };

    } // Parser namespace

} // Arad namespace

#endif // ARAD_PARSER_JSONPARSER_JSONPARSER_H
