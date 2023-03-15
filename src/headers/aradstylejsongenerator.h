#include <headers/jsongenerator.h>

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

        class AradStyleJsonGenerator final : public Arad::JsonGeneratorNP::JsonGenerator
        {
        public:
            AradStyleJsonGenerator() = default;
            ~AradStyleJsonGenerator() = default;

            void operator()(QVector<QHash<QString, QString>> const& container,
                            QString const& filePath,
                            QVector<QString> const& orderedKeys = QVector<QString>{}) override;
        };

    } // namespace JsonGeneratorNP

} // namespace Arad
