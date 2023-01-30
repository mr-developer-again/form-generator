#ifndef ARAD_GENERATINGFORM_FORMGENERATOR_FORMGENERATOR_H
#define ARAD_GENERATINGFORM_FORMGENERATOR_FORMGENERATOR_H

#include <headers/jsonparser.h>

#include <QMainWindow>
#include <QWidget>
#include <QString>

namespace Arad
{
    namespace GeneratingForm
    {

        class FormGenerator : public QMainWindow
        {
            Q_OBJECT

        public:
            explicit FormGenerator(QString const& filePath, QWidget *parent = nullptr);
            virtual ~FormGenerator();

            virtual void setupForm() = 0;

        protected:
            void setFilePath(QString const& filePath);
            QString getFilePath() const noexcept;

            Arad::Parser::JsonParser *_jsonParser = nullptr;

        private:
            QString _filePath = "";
        };

    } // GeneratingForm namespace

} // Arad namespace


#endif // ARAD_GENERATINGFORM_FORMGENERATOR_FORMGENERATOR_H
