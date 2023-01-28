#ifndef ARAD_GENERATINGFORM_FORMGENERATOR_FORMGENERATOR_H
#define ARAD_GENERATINGFORM_FORMGENERATOR_FORMGENERATOR_H

#include <headers/jsonparser.h>

#include <QMainWindow>
#include <QWidget>

namespace Arad
{
    namespace GeneratingForm
    {

        class FormGenerator : public QMainWindow
        {
            Q_OBJECT

        public:
            explicit FormGenerator(QWidget *parent = nullptr);
            virtual ~FormGenerator();

        private:
            Arad::Parser::JsonParser *_jsonParser = nullptr;
        };

    } // GeneratingForm namespace

} // Arad namespace


#endif // ARAD_GENERATINGFORM_FORMGENERATOR_FORMGENERATOR_H
