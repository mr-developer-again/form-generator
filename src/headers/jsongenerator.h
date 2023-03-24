#ifndef ARAD_JSONGENERATORNP_JSONGENERATOR_H
#define ARAD_JSONGENERATORNP_JSONGENERATOR_H

#include <QVector>
#include <QHash>
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

            virtual void operator()(QVector<QHash<QString, QString>> const& container,
                                    QString const& filePath,
                                    QVector<QString> const& orderedKeys = QVector<QString> {}) = 0;
        };

    }

} // namespace Arad

#endif  // ARAD_JSONGENERATORNP_JSONGENERATOR_H
