#include <headers/jsongenerator.h>

#include <QJsonArray>
#include <QJsonObject>
#include <QFileDialog>

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

        class AradStyleJsonGenerator final : public Arad::JsonGeneratorNP::JsonGenerator
        {
        public:
            AradStyleJsonGenerator() = default;
            ~AradStyleJsonGenerator() = default;

            void operator()(QVector<QMap<QString, QString>> const& container,
                            QVector<QString> const& orderedKeys = QVector<QString>(),
                            QString const& filePath = "") override;
        };

    } // namespace JsonGeneratorNP

} // namespace Arad
