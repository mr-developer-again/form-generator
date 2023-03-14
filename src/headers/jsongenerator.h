#ifndef ARAD_JSONGENERATOR_H
#define ARAD_JSONGENERATOR_H

#include <QVector>
#include <QMap>
#include <QString>

namespace Arad
{

    namespace Parser
    {

        class AradStyleJsonParser;

    } // Parser namespace

    namespace JsonGeneratorNP
    {

        class JsonGenerator
        {
        public:
            JsonGenerator() = default;
            virtual ~JsonGenerator() = default;

            virtual void operator()(QVector<QMap<QString, QString>> const& container,
                                    QVector<QString> const& orderedKeys,
                                    QString const& filePath) = 0;
        };

    }

} // namespace Arad

#endif  // ARAD_JSONGENERATOR_H
