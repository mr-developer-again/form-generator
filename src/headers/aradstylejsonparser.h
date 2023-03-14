#ifndef ARAD_PARSER_ARADSTYLEJSONPARSER_ARADSTYLEJSONPARSER_H
#define ARAD_PARSER_ARADSTYLEJSONPARSER_ARADSTYLEJSONPARSER_H

#include <headers/jsonparser.h>

#include <QVector>
#include <QMap>
#include <QStringList>

namespace Arad
{

    namespace Parser
    {

        class AradStyleJsonParser : public Arad::Parser::JsonParser
        {
        public:
            explicit AradStyleJsonParser(QString const& filePath,
                                        QStringList const& validKeys =
                                        {"name", "default value", "string list values", "description", "type", "readonly"});

            QVector<QMap<QString, QString>> parseJson() override;

        private:
            /// this method checks correctness of the json file
            bool jsonIsCorrect(QMap<QString, QString> const& inputMap, QString& error);
        };

    } // Parser namespace

} // Arad namespace

#endif // ARAD_PARSER_ARADSTYLEJSONPARSER_ARADSTYLEJSONPARSER_H
